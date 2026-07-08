#include "BT.hpp"
#include "debug.h"

#if CONFIG_BT_BLUEDROID_ENABLED

#include "esp_bt.h"
#include "esp_bt_defs.h"
#include "esp_bt_main.h"
#include "esp_err.h"
#include "esp_gatt_defs.h"
#include "esp_gatts_api.h"
#include "esp_gap_ble_api.h"
#include "esp_gatt_common_api.h"
#include "esp_log.h"
#include "esp_bt_device.h"
#include "qjs_utils.h"
#include "quickjs/quickjs.h"
#include "../../js/bt/central.c"
#include "../../js/bt/peripheral.c"
#include "freertos/FreeRTOS.h"
#include "freertos/semphr.h"

#define GATTS_TAG "GATT_SERVICE"


namespace be {

    static esp_gatt_if_t gatts_if_global = ESP_GATT_IF_NONE;
    static uint16_t last_service_handle = 0xFFFF;
    static uint16_t last_char_handle = 0xFFFF;
    static uint16_t last_descr_handle = 0xFFFF;
    static uint16_t last_conn_id = 0xFFFF;
    static SemaphoreHandle_t gatts_sem = NULL;
    static bool gatts_sem_success = false;
    static uint16_t peripheral_mtu = 23 ;

    void BT::gatts_event_handler(esp_gatts_cb_event_t event, esp_gatt_if_t gatts_if, esp_ble_gatts_cb_param_t *param) {
        if(gattsHandler && gattsHandler(event, gatts_if, param)){
            return;
        }
        bt_event event_msg = {
            .event = static_cast<uint16_t>(200 + event),
            .gatt_if = gatts_if,
            .gatts = *param,
            .data = {NULL,0}
        };
        switch (event) {
            case ESP_GATTS_REG_EVT:
                ESP_LOGI(GATTS_TAG, "ESP_GATTS_REG_EVT, status %d, app_id %d", param->reg.status, param->reg.app_id);
                gatts_if_global = gatts_if;
                gatts_sem_success = (param->reg.status == ESP_GATT_OK);
                printf("ESP_GATTS_REG_EVT   gatts_if_global=%d\n", gatts_if_global);
                if (gatts_sem != NULL) {
                    xSemaphoreGive(gatts_sem);
                }
                break;

            case ESP_GATTS_CREATE_EVT:
                gatts_sem_success = (param->create.status == ESP_GATT_OK);
                if(gatts_sem_success) {
                    last_service_handle = param->create.service_handle;
                }
                
                if (gatts_sem != NULL) {
                    xSemaphoreGive(gatts_sem);
                }
                break;
                
            case ESP_GATTS_START_EVT:
                gatts_sem_success = (param->start.status == ESP_GATT_OK);
                if (gatts_sem != NULL) {
                    xSemaphoreGive(gatts_sem);
                }
                break;
            case ESP_GATTS_STOP_EVT:
                break;

            case ESP_GATTS_ADD_CHAR_EVT:
                gatts_sem_success = (param->add_char.status == ESP_GATT_OK);
                if(gatts_sem_success) {
                    last_char_handle = param->add_char.attr_handle;
                }
                if (gatts_sem != NULL) {
                    xSemaphoreGive(gatts_sem);
                }
                break ;

            case ESP_GATTS_ADD_CHAR_DESCR_EVT:
                gatts_sem_success = (param->add_char_descr.status == ESP_GATT_OK);
                if(gatts_sem_success) {
                    last_descr_handle = param->add_char_descr.attr_handle;
                }
                if (gatts_sem != NULL) {
                    xSemaphoreGive(gatts_sem);
                }
                break ;

            case ESP_GATTS_READ_EVT:
            
                break;

            case ESP_GATTS_WRITE_EVT:
                if (param->write.len > 0) {
                    event_msg.data.len = param->write.len;
                    event_msg.data.ptr = malloc(param->write.len);
                    if (event_msg.data.ptr) {
                        memcpy(event_msg.data.ptr, param->write.value, param->write.len);
                    } else {
                        printf("malloc failed for write event data\n");
                    }
                }
                
                if (param->write.need_rsp){
                    esp_ble_gatts_send_response(gatts_if, param->write.conn_id, param->write.trans_id, ESP_GATT_OK, NULL);
                }
                break;

            case ESP_GATTS_EXEC_WRITE_EVT:
                esp_ble_gatts_send_response(gatts_if, param->exec_write.conn_id, param->exec_write.trans_id, ESP_GATT_OK, NULL);
                break;

            case ESP_GATTS_MTU_EVT:
                // printf("gatts_if: %d, MTU size: %d\n", gatts_if, param->mtu.mtu);
                peripheral_mtu = param->mtu.mtu;
                break;
            case ESP_GATTS_UNREG_EVT:
                break;
                
            case ESP_GATTS_CONNECT_EVT: {
                // 忽略 central 触发的连接事件
                if(connect_type == 1) {
                    return ;
                }
                esp_ble_conn_update_params_t conn_params = {0};
                memcpy(conn_params.bda, param->connect.remote_bda, sizeof(esp_bd_addr_t));
                conn_params.latency = 0;
                conn_params.max_int = 0x20;
                conn_params.min_int = 0x10;
                conn_params.timeout = 400;
                esp_ble_gap_update_conn_params(&conn_params);

                last_conn_id = param->connect.conn_id;
                break;
            }
            case ESP_GATTS_DISCONNECT_EVT:
                // 忽略 central 触发的断开连接事件
                if(disconnect_type == 1) {
                    return ;
                }
                last_conn_id = 0xFFFF ;
                break;

            case ESP_GATTS_OPEN_EVT:
            case ESP_GATTS_CANCEL_OPEN_EVT:
            case ESP_GATTS_CLOSE_EVT:
            case ESP_GATTS_LISTEN_EVT:
            case ESP_GATTS_CONGEST_EVT:
            default:
                break;
        }

        if(BT::singleton){
            if( !BT::singleton->emitNativeEvent(&event_msg) ){
                printf("bt queue full\n") ;
                if(event_msg.data.ptr) {
                    free(event_msg.data.ptr) ;
                    event_msg.data.ptr = NULL ;
                    event_msg.data.len = 0 ;
                }
            }
        }
    }

    bool BT::initPeripheral() {
        static bool inited = false;
        if(inited) {
            return true;
        }
        
        gatts_sem_success = false;
        
        if (gatts_sem == NULL) {
            gatts_sem = xSemaphoreCreateBinary();
            if (gatts_sem == NULL) {
                ESP_LOGE(GATTS_TAG, "Failed to create gatts registration semaphore");
                return false;
            }
        }
        
        init();

        esp_err_t err = esp_ble_gatts_register_callback(gatts_event_handler);
        if(err != ESP_OK) {
            ESP_LOGE(GATTS_TAG, "esp_ble_gatts_register_callback failed, err = %d", err);
            return false;
        }
        
        err = esp_ble_gatts_app_register(2);
        if (err != ESP_OK) {
            ESP_LOGE(GATTS_TAG, "esp_ble_gatts_app_register failed, err = %d", err);
            return false;
        }
        
        if (xSemaphoreTake(gatts_sem, pdMS_TO_TICKS(1000)) != pdTRUE) {
            ESP_LOGE(GATTS_TAG, "BT peripheral registration timed out after 1000ms");
            return false;
        }
        
        if (gatts_sem_success) {
            inited = true;
            return true;
        } else {
            ESP_LOGE(GATTS_TAG, "BT peripheral registration failed");
            return false;
        }
    }
    
    JSValue BT::initPeripheral(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        if(!initPeripheral()) {
            JSTHROW("bt peripheral init failed")
        }
        return JS_UNDEFINED ;
    }
    
    #define MAC_TO_STR(str,mac)  \
                char str [18] ;  \
                sprintf(addr, "%02X:%02X:%02X:%02X:%02X:%02X", mac[0], mac[1], mac[2], mac[3], mac[4], mac[5] ) ;
    void BT::onNativePeripheralEvent(JSContext *ctx, bt_event * event) {
        esp_gatts_cb_event_t gatts_event = static_cast<esp_gatts_cb_event_t>(event->event - 200);
        
        switch (gatts_event) {
            case ESP_GATTS_REG_EVT:
                emitSyncFree("register", {
                    JS_NewInt32(ctx, event->gatts.reg.status),
                    JS_NewInt32(ctx, event->gatts.reg.app_id)
                });
                break;
                
            case ESP_GATTS_CREATE_EVT:
                emitSyncFree("create", {
                    JS_NewInt32(ctx, event->gatts.create.status),
                    JS_NewInt32(ctx, event->gatts.create.service_handle)
                });
                break;
                
            case ESP_GATTS_START_EVT:
                emitSyncFree("start", {
                    JS_NewInt32(ctx, event->gatts.start.status),
                    JS_NewInt32(ctx, event->gatts.start.service_handle)
                });
                break;
                
            case ESP_GATTS_ADD_CHAR_EVT:
                emitSyncFree("add-char", {
                    JS_NewInt32(ctx, event->gatts.add_char.status),
                    JS_NewInt32(ctx, event->gatts.add_char.attr_handle),
                    JS_NewInt32(ctx, event->gatts.add_char.service_handle)
                });
                break;
                
            case ESP_GATTS_ADD_CHAR_DESCR_EVT:
                emitSyncFree("add-descr", {
                    JS_NewInt32(ctx, event->gatts.add_char_descr.status),
                    JS_NewInt32(ctx, event->gatts.add_char_descr.attr_handle),
                    JS_NewInt32(ctx, event->gatts.add_char_descr.service_handle)
                });
                break;
                
            case ESP_GATTS_READ_EVT: {
                emitSyncFree("read", {
                    JS_NewInt32(ctx, event->gatts.read.handle),
                    JS_NewInt32(ctx, event->gatts.read.conn_id),
                    JS_NewBool(ctx, event->gatts.read.is_long),
                    JS_NewBool(ctx, event->gatts.read.need_rsp),
                });
                break;
            }

            case ESP_GATTS_WRITE_EVT: {
                emitSyncFree("WRITE", {
                    JS_NewInt32(ctx, event->gatts.write.handle),
                    JS_NewArrayBufferCopy(ctx, (const uint8_t*)event->data.ptr, event->data.len),
                    JS_NewInt32(ctx, event->gatts.write.conn_id),
                    JS_NewBool(ctx, event->gatts.write.is_prep),
                    JS_NewBool(ctx, event->gatts.write.need_rsp)
                });
                break;
            }
                
            case ESP_GATTS_CONNECT_EVT: {
                MAC_TO_STR(addr, event->gatts.connect.remote_bda)
                emitSyncFree("periph.connect", {
                    JS_NewInt32(ctx, event->gatts.connect.conn_id),
                    JS_NewString(ctx, addr),
                    JS_NewInt32(ctx, event->gatts.connect.conn_params.interval),
                    JS_NewInt32(ctx, event->gatts.connect.conn_params.latency),
                    JS_NewInt32(ctx, event->gatts.connect.conn_params.timeout)
                });
                break;
            }
                
            case ESP_GATTS_DISCONNECT_EVT: {
                MAC_TO_STR(addr, event->gatts.disconnect.remote_bda)
                emitSyncFree("periph.disconnect", {
                    JS_NewInt32(ctx, event->gatts.disconnect.conn_id),
                    JS_NewString(ctx, addr),
                    JS_NewInt32(ctx, event->gatts.disconnect.reason)
                });
                break;
            }
                
            case ESP_GATTS_OPEN_EVT: {
                emitSyncFree("periph.open", {
                    JS_NewUint32(ctx, event->gattc.open.status) ,
                });
                break;
            }
            case ESP_GATTS_CLOSE_EVT: {
                emitSyncFree("periph.close", {
                    JS_NewUint32(ctx, event->gattc.open.status) ,
                });
                break;
            }
                
            case ESP_GATTS_MTU_EVT:
                emitSyncFree("mtu", {
                    JS_NewInt32(ctx, event->gatts.mtu.conn_id),
                    JS_NewInt32(ctx, event->gatts.mtu.mtu)
                });
                break;
                
            case ESP_GATTS_EXEC_WRITE_EVT: {
                emitSyncFree("exec-write", {
                    JS_NewInt32(ctx, event->gatts.exec_write.conn_id),
                    JS_NewInt32(ctx, event->gatts.exec_write.exec_write_flag)
                });
                break;
            }
                
            case ESP_GATTS_CONGEST_EVT:
                emitSyncFree("congest", {
                    JS_NewInt32(ctx, event->gatts.congest.conn_id),
                    JS_NewBool(ctx, event->gatts.congest.congested)
                });
                break;
                
            default:
                break;
        }
        
        if (event->data.ptr) {
            free(event->data.ptr);
            event->data.ptr = NULL;
            event->data.len = 0;
        }
    }
    
    #define CHECK_GATTS_IF                          \
        if(gatts_if_global==ESP_GATT_IF_NONE) {     \
            JSTHROW("call bt.init() first")         \
        }

    JSValue BT::setAdvName(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        CHECK_GATTS_IF
        CHECK_ARGC(1)

        ARGV_TO_CSTRING(0, name)
        
        esp_err_t err = esp_ble_gap_set_device_name(name);
        JS_FreeCString(ctx, name) ;

        if(err!=ESP_OK) {
            JSTHROW("esp_ble_gap_set_device_name failed, err = %d", err)
        }

        return JS_UNDEFINED ;
    }

    JSValue BT::setAdvData(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        CHECK_GATTS_IF
        CHECK_ARGC(1)
        ARGV_TO_ARRAYBUFFER(0, data, datalen)
#if (BLE_42_FEATURE_SUPPORT == TRUE)
        esp_err_t err = esp_ble_gap_config_adv_data_raw(data, datalen);
#elif CONFIG_BT_BLE_50_FEATURES_SUPPORTED
        esp_err_t err = esp_ble_gap_config_ext_adv_data_raw(0, datalen, data);
#else
        JSTHROW("BLE is not supported")
#endif
        if(err!=ESP_OK) {
            JSTHROW("esp_ble_gap_config_adv_data_raw failed, err = %d", err)
        }
        return JS_UNDEFINED ;
    }

    JSValue BT::startAdv(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        CHECK_GATTS_IF

        // 公共参数
        int32_t own_addr_type     = BLE_ADDR_TYPE_PUBLIC;
        int32_t channel_map       = ADV_CHNL_ALL;
        int32_t adv_filter_policy = ADV_FILTER_ALLOW_SCAN_ANY_CON_ANY;
        uint16_t adv_int_min      = 0xA0;
        uint16_t adv_int_max      = 0xB0;

        if(argc > 0) {
            GET_UINT16_PROP_OPT( argv[0], "min",                adv_int_min,       0xA0 )
            GET_UINT16_PROP_OPT( argv[0], "max",                adv_int_max,       0xB0 )
            GET_INT32_PROP_OPT( argv[0], "own_addr_type",      own_addr_type,     BLE_ADDR_TYPE_PUBLIC )
            GET_INT32_PROP_OPT( argv[0], "channel_map",        channel_map,       ADV_CHNL_ALL )
            GET_INT32_PROP_OPT( argv[0], "adv_filter_policy",  adv_filter_policy, ADV_FILTER_ALLOW_SCAN_ANY_CON_ANY )
        }

#if (BLE_42_FEATURE_SUPPORT == TRUE)
        esp_ble_adv_params_t adv_params = {
            .adv_int_min       = adv_int_min,
            .adv_int_max       = adv_int_max,
            .adv_type          = ADV_TYPE_IND,
            .own_addr_type     = static_cast<esp_ble_addr_type_t>(own_addr_type),
            .channel_map       = static_cast<esp_ble_adv_channel_t>(channel_map),
            .adv_filter_policy = static_cast<esp_ble_adv_filter_t>(adv_filter_policy),
        };
        if(argc > 0) {
            int32_t adv_type = ADV_TYPE_IND;
            GET_INT32_PROP_OPT( argv[0], "type", adv_type, ADV_TYPE_IND )
            adv_params.adv_type = static_cast<esp_ble_adv_type_t>(adv_type);
        }
        esp_err_t err = esp_ble_gap_start_advertising(&adv_params);
        if(err!=ESP_OK) {
            JSTHROW("esp_ble_gap_start_advertising failed, err = %d", err)
        }
#elif CONFIG_BT_BLE_50_FEATURES_SUPPORTED
        int32_t adv_type      = ADV_TYPE_IND;
        int32_t primary_phy   = ESP_BLE_GAP_PHY_1M;
        int32_t secondary_phy = ESP_BLE_GAP_PHY_1M;
        int32_t tx_power      = EXT_ADV_TX_PWR_NO_PREFERENCE;
        if(argc > 0) {
            GET_INT32_PROP_OPT( argv[0], "type",          adv_type,      ADV_TYPE_IND )
            GET_INT32_PROP_OPT( argv[0], "primary_phy",   primary_phy,   ESP_BLE_GAP_PHY_1M )
            GET_INT32_PROP_OPT( argv[0], "secondary_phy", secondary_phy, ESP_BLE_GAP_PHY_1M )
            GET_INT32_PROP_OPT( argv[0], "tx_power",      tx_power,      EXT_ADV_TX_PWR_NO_PREFERENCE )
        }

        esp_ble_gap_ext_adv_params_t adv_params = {};
        adv_params.interval_min      = adv_int_min;
        adv_params.interval_max      = adv_int_max;
        adv_params.own_addr_type     = static_cast<esp_ble_addr_type_t>(own_addr_type);
        adv_params.channel_map       = static_cast<esp_ble_adv_channel_t>(channel_map);
        adv_params.filter_policy     = static_cast<esp_ble_adv_filter_t>(adv_filter_policy);
        adv_params.primary_phy       = static_cast<esp_ble_gap_phy_t>(primary_phy);
        adv_params.secondary_phy     = static_cast<esp_ble_gap_phy_t>(secondary_phy);
        adv_params.tx_power          = static_cast<int8_t>(tx_power);
        adv_params.peer_addr_type    = BLE_ADDR_TYPE_PUBLIC;
        adv_params.max_skip          = 0;
        adv_params.sid               = 0;
        adv_params.scan_req_notif    = false;

        // type 映射：0-4 legacy（兼容 4.2），5=125K NONCONN（值为 0），其他直接作为 bitmask
        switch(adv_type) {
            case 0:  adv_params.type = ESP_BLE_GAP_SET_EXT_ADV_PROP_LEGACY_IND; break;
            case 1:  adv_params.type = ESP_BLE_GAP_SET_EXT_ADV_PROP_LEGACY_HD_DIR; break;
            case 2:  adv_params.type = ESP_BLE_GAP_SET_EXT_ADV_PROP_LEGACY_SCAN; break;
            case 3:  adv_params.type = ESP_BLE_GAP_SET_EXT_ADV_PROP_LEGACY_NONCONN; break;
            case 4:  adv_params.type = ESP_BLE_GAP_SET_EXT_ADV_PROP_LEGACY_LD_DIR; break;
            case 5:  adv_params.type = ESP_BLE_GAP_SET_EXT_ADV_PROP_NONCONN_NONSCANNABLE_UNDIRECTED; break;
            default: adv_params.type = static_cast<uint16_t>(adv_type); break;
        }

        esp_err_t err = esp_ble_gap_ext_adv_set_params(0, &adv_params);
        if(err!=ESP_OK) {
            JSTHROW("esp_ble_gap_ext_adv_set_params failed, err = %d", err)
        }
        esp_ble_gap_ext_adv_t ext_adv = {
            .instance   = 0,
            .duration   = 0,
            .max_events = 0,
        };
        err = esp_ble_gap_ext_adv_start(1, &ext_adv);
        if(err!=ESP_OK) {
            JSTHROW("esp_ble_gap_ext_adv_start failed, err = %d", err)
        }
#else
        JSTHROW("BLE is not supported")
#endif
        return JS_UNDEFINED ;
    }
    
    JSValue BT::stopAdv(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        CHECK_GATTS_IF
#if (BLE_42_FEATURE_SUPPORT == TRUE)
        esp_err_t err = esp_ble_gap_stop_advertising();
#elif CONFIG_BT_BLE_50_FEATURES_SUPPORTED
        uint8_t adv_inst = 0;
        esp_err_t err = esp_ble_gap_ext_adv_stop(1, &adv_inst);
#else
        JSTHROW("BLE is not supported")
#endif
        if(err!=ESP_OK) {
            JSTHROW("esp_ble_gap_stop_advertising failed, err = %d", err)
        }
        return JS_UNDEFINED ;
    }

    JSValue BT::addService(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        CHECK_GATTS_IF
        CHECK_ARGC(2)
        
        ARGV_TO_CSTRING(0, uuid_str)
        
        int32_t num_handle;
        if (JS_ToInt32(ctx, &num_handle, argv[1]) < 0 || num_handle <= 0) {
            JS_FreeCString(ctx, uuid_str);
            JSTHROW("Invalid number of handles")
        }
        
        esp_bt_uuid_t uuid = bt_string_to_uuid((char *)uuid_str, strlen(uuid_str));
        if (uuid.len == ESP_UUID_LEN_16 && uuid.uuid.uuid16 == 0) {
            JS_FreeCString(ctx, uuid_str);
            JSTHROW("Invalid UUID format")
        }
        
        // Reset status flags
        gatts_sem_success = false;
        last_service_handle = 0xFFFF;
        
        esp_gatt_srvc_id_t srvc_id = {
            .id = {
                .uuid = uuid,
                .inst_id = 0
            },
            .is_primary = true
        };
        
        esp_err_t err = esp_ble_gatts_create_service(gatts_if_global, &srvc_id, num_handle);
        
        if (err != ESP_OK) {
            JS_FreeCString(ctx, uuid_str);
            JSTHROW("esp_ble_gatts_create_service failed, err = %d", err)
        }
        
        // Wait for the create event
        if (xSemaphoreTake(gatts_sem, pdMS_TO_TICKS(1000)) != pdTRUE) {
            JS_FreeCString(ctx, uuid_str);
            JSTHROW("Service creation timed out after 1000ms")
        }
        
        if (!gatts_sem_success) {
            JS_FreeCString(ctx, uuid_str);
            JSTHROW("Service creation failed")
        }
        
        JS_FreeCString(ctx, uuid_str);
        
        err = esp_ble_gatts_start_service(last_service_handle);
        if(err!=ESP_OK) {
            JSTHROW("esp_ble_gatts_start_service failed, err = %d", err)
        }
        
        // Wait for the create event
        if (xSemaphoreTake(gatts_sem, pdMS_TO_TICKS(1000)) != pdTRUE) {
            JS_FreeCString(ctx, uuid_str);
            JSTHROW("Service start timed out after 1000ms")
        }

        return JS_NewUint32(ctx, last_service_handle);
    }

    JSValue BT::addChar(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        CHECK_GATTS_IF
        CHECK_ARGC(3)
        
        uint16_t service_handle;
        if (JS_ToUint32(ctx, (uint32_t*)&service_handle, argv[0]) < 0) {
            JSTHROW("Invalid service handle")
        }

        ARGV_TO_CSTRING(1, uuid_str)
        
        esp_bt_uuid_t uuid = bt_string_to_uuid((char *)uuid_str, strlen(uuid_str));
        if (uuid.len == ESP_UUID_LEN_16 && uuid.uuid.uuid16 == 0) {
            JS_FreeCString(ctx, uuid_str);
            JSTHROW("Invalid UUID format")
        }

        uint8_t property = 0;
        uint8_t perm = 0;
        
        esp_attr_control_t control = {
            .auto_rsp = ESP_GATT_AUTO_RSP,
        };
        esp_attr_value_t attr_value = {
            .attr_max_len = 512,
            .attr_len = 0,
            .attr_value = NULL
        };

        if (JS_IsString(argv[2])) {
            ARGV_TO_CSTRING(2, perm_str)
            if (strstr(perm_str, "read")) {
                property |= ESP_GATT_CHAR_PROP_BIT_READ;
                perm |= ESP_GATT_PERM_READ;
            }
            if (strstr(perm_str, "write")) {
                property |= ESP_GATT_CHAR_PROP_BIT_WRITE;
                perm |= ESP_GATT_PERM_WRITE;
            }
            if (strstr(perm_str, "writeNR")) {
                property |= ESP_GATT_CHAR_PROP_BIT_WRITE_NR;
                perm |= ESP_GATT_PERM_WRITE;
            }
            if (strstr(perm_str, "notify")) {
                property |= ESP_GATT_CHAR_PROP_BIT_NOTIFY;
            }
            if (strstr(perm_str, "indicate")) {
                property |= ESP_GATT_CHAR_PROP_BIT_INDICATE;
            }
            JS_FreeCString(ctx, perm_str);
        }
        else if (JS_IsArray(ctx, argv[2])) {
            JSValue length = JS_GetPropertyStr(ctx, argv[2], "length");
            int32_t arr_len;
            JS_ToInt32(ctx, &arr_len, length);
            JS_FreeValue(ctx, length);

            for (int i = 0; i < arr_len; i++) {
                JSValue item = JS_GetPropertyUint32(ctx, argv[2], i);
                if (JS_IsString(item)) {
                    const char* perm_item = JS_ToCString(ctx, item);
                    ds(perm_item)
                    if (strcmp(perm_item, "read") == 0) {
                        property |= ESP_GATT_CHAR_PROP_BIT_READ;
                        perm |= ESP_GATT_PERM_READ;
                    }
                    else if (strcmp(perm_item, "write") == 0) {
                        property |= ESP_GATT_CHAR_PROP_BIT_WRITE;
                        perm |= ESP_GATT_PERM_WRITE;
                    }
                    else if (strcmp(perm_item, "writeNR") == 0) {
                        property |= ESP_GATT_CHAR_PROP_BIT_WRITE_NR;
                        perm |= ESP_GATT_PERM_WRITE;
                    }
                    else if (strcmp(perm_item, "notify") == 0) {
                        property |= ESP_GATT_CHAR_PROP_BIT_NOTIFY;
                        perm |= ESP_GATT_PERM_WRITE;
                    }
                    else if (strcmp(perm_item, "indicate") == 0) {
                        property |= ESP_GATT_CHAR_PROP_BIT_INDICATE;
                        perm |= ESP_GATT_PERM_WRITE;
                    }
                    JS_FreeCString(ctx, perm_item);
                }
                JS_FreeValue(ctx, item);
            }
        }
        else {
            JS_FreeCString(ctx, uuid_str);
            JSTHROW("Permission parameter must be a string or array")
        }

        last_char_handle = 0xFFFF;
        esp_err_t err = esp_ble_gatts_add_char(
            service_handle,
            &uuid,
            perm,
            property,
            // NULL, NULL
            & attr_value,
            & control
        );

        JS_FreeCString(ctx, uuid_str);
        
        if (err != ESP_OK) {
            JSTHROW("esp_ble_gatts_add_char failed, err = %d", err)
        }
        
        // Wait for the create event
        if (xSemaphoreTake(gatts_sem, pdMS_TO_TICKS(1000)) != pdTRUE) {
            JS_FreeCString(ctx, uuid_str);
            JSTHROW("esp_ble_gatts_add_char() timed out after 1000ms")
        }
        
        if (property & (ESP_GATT_CHAR_PROP_BIT_NOTIFY | ESP_GATT_CHAR_PROP_BIT_INDICATE)) {
            esp_bt_uuid_t desc_uuid = {
                .len = ESP_UUID_LEN_16,
                .uuid = {
                    .uuid16 = ESP_GATT_UUID_CHAR_CLIENT_CONFIG,
                }
            };

            esp_err_t err = esp_ble_gatts_add_char_descr(
                service_handle,
                &desc_uuid,
                ESP_GATT_PERM_READ | ESP_GATT_PERM_WRITE,
                NULL,
                NULL
            );

            if (err != ESP_OK) {
                printf("Failed to add CCCD descriptor, err = %d\n", err);
            }
            else {
                // Wait for the create event
                if (xSemaphoreTake(gatts_sem, pdMS_TO_TICKS(1000)) != pdTRUE) {
                    JS_FreeCString(ctx, uuid_str);
                    JSTHROW("esp_ble_gatts_add_char_descr() timed out after 1000ms")
                }
            }
        }

        return JS_NewUint32(ctx, last_char_handle);
    }
    
    JSValue BT::setCharValue(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        CHECK_GATTS_IF
        CHECK_ARGC(2)

        uint16_t char_handle;
        if (JS_ToUint32(ctx, (uint32_t*)&char_handle, argv[0]) < 0) {
            JSTHROW("Invalid characteristic handle")
        }

        uint8_t* value_data = NULL;
        uint16_t value_len = 0;
        bool need_free = false;

        // Handle different value types
        if (JS_IsString(argv[1])) {
            // String input
            const char* str = JS_ToCString(ctx, argv[1]);
            if (!str) {
                JSTHROW("Failed to convert string")
            }
            value_len = strlen(str);
            value_data = (uint8_t*)malloc(value_len);
            if (!value_data) {
                JS_FreeCString(ctx, str);
                JSTHROW("Failed to allocate memory for string value")
            }
            memcpy(value_data, str, value_len);
            JS_FreeCString(ctx, str);
            need_free = true;
        }
        else if (JS_IsArrayBuffer(argv[1])) {
            // ArrayBuffer input
            size_t buffer_len;
            uint8_t* buffer_data = (uint8_t*)JS_GetArrayBuffer(ctx, &buffer_len, argv[1]);
            if (!buffer_data) {
                JSTHROW("Failed to get ArrayBuffer data")
            }
            value_len = (uint16_t)buffer_len;
            value_data = (uint8_t*)malloc(value_len);
            if (!value_data) {
                JSTHROW("Failed to allocate memory for ArrayBuffer value")
            }
            memcpy(value_data, buffer_data, value_len);
            need_free = true;
        }
        else if (JS_IsNumber(argv[1])) {
            // Integer input - convert to 4-byte little-endian
            int32_t int_value;
            if (JS_ToInt32(ctx, &int_value, argv[1]) < 0) {
                JSTHROW("Failed to convert number")
            }
            value_len = 4;
            value_data = (uint8_t*)malloc(value_len);
            if (!value_data) {
                JSTHROW("Failed to allocate memory for integer value")
            }
            // Store as little-endian
            value_data[0] = (uint8_t)(int_value & 0xFF);
            value_data[1] = (uint8_t)((int_value >> 8) & 0xFF);
            value_data[2] = (uint8_t)((int_value >> 16) & 0xFF);
            value_data[3] = (uint8_t)((int_value >> 24) & 0xFF);
            need_free = true;
        }
        else {
            JSTHROW("Value must be string, ArrayBuffer, or number")
        }

        // Set the characteristic value
        esp_err_t err = esp_ble_gatts_set_attr_value(char_handle, value_len, value_data);
        // Free allocated memory for new value
        if (need_free && value_data) {
            free(value_data);
        }

        if (err != ESP_OK) {
            JSTHROW("esp_ble_gatts_set_attr_value failed, err = %d", err)
        }

        return JS_UNDEFINED;
    }

    JSValue BT::sendNotify(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        CHECK_GATTS_IF
        CHECK_ARGC(2)

        uint32_t char_handle = 0xFFFFFFFF;
        if (JS_ToUint32(ctx, (uint32_t*)&char_handle, argv[0]) < 0) {
            JSTHROW("Invalid characteristic handle")
        }

        uint16_t conn_id = last_conn_id;
        if (argc > 3 && !JS_IsUndefined(argv[3])) {
            if (JS_ToUint32(ctx, (uint32_t*)&conn_id, argv[3]) < 0) {
                JSTHROW("Invalid connection ID")
            }
        }

        if (conn_id == 0xFFFF) {
            JSTHROW("No device connected")
        }

        bool is_indication = false;
        if (argc > 2 && !JS_IsUndefined(argv[2])) {
            if (!JS_IsBool(argv[2])) {
                JSTHROW("Third parameter must be boolean")
            }
            is_indication = JS_ToBool(ctx, argv[2]);
        }

        uint8_t* data = NULL;
        size_t data_len = 0;
        bool is_string = false;

        if (JS_IsString(argv[1])) {
            const char* str = JS_ToCString(ctx, argv[1]);
            if (!str) {
                JSTHROW("Failed to convert string")
            }
            data_len = strlen(str);
            data = (uint8_t*)str;
            is_string = true;
        }
        else if (JS_IsArrayBuffer(argv[1])) {
            size_t offset;
            data = (uint8_t*)JS_GetArrayBuffer(ctx, &data_len, argv[1]);
            if (!data) {
                JSTHROW("Failed to get ArrayBuffer data")
            }
        }
        else {
            JSTHROW("Data must be string or ArrayBuffer")
        }

        // dn5(gatts_if_global, conn_id, char_handle, is_indication, data_len)

        // Calculate effective MTU size for payload (3 bytes are used for ATT header)
        // ATT_MTU = 3 + ATT_PAYLOAD_SIZE
        size_t max_chunk_size = peripheral_mtu > 3 ? peripheral_mtu - 3 : 20;
        esp_err_t err = ESP_OK;

        if (data_len <= max_chunk_size) {
            // If data fits in a single packet, send it directly
            err = esp_ble_gatts_send_indicate(
                gatts_if_global,
                conn_id,
                (uint16_t)char_handle,
                data_len,
                data,
                is_indication
            );
        } else {
            // Split data into multiple chunks and send each chunk
            size_t remaining = data_len;
            size_t offset = 0;
            
            while (remaining > 0 && err == ESP_OK) {
                size_t chunk_size = (remaining > max_chunk_size) ? max_chunk_size : remaining;
                
                err = esp_ble_gatts_send_indicate(
                    gatts_if_global,
                    conn_id,
                    (uint16_t)char_handle,
                    chunk_size,
                    data + offset,
                    is_indication
                );
                
                if (err != ESP_OK) {
                    break;
                }
                
                // Add a small delay between chunks to avoid overwhelming the BLE stack
                vTaskDelay(pdMS_TO_TICKS(10));
                
                offset += chunk_size;
                remaining -= chunk_size;
            }
        }

        if (is_string) {
            JS_FreeCString(ctx, (const char*)data);
        }

        if (err != ESP_OK) {
            JSTHROW("Failed to send notification/indication, err = %d", err)
        }

        return JS_UNDEFINED;
    }
    JSValue BT::setMTU(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        CHECK_ARGC(1)
        ARGV_TO_UINT16(0, mtu)
        if ((mtu < ESP_GATT_DEF_BLE_MTU_SIZE) || (mtu > ESP_GATT_MAX_MTU_SIZE)) {
            JSTHROW("Invalid MTU size, must be between %d and %d", ESP_GATT_DEF_BLE_MTU_SIZE, ESP_GATT_MAX_MTU_SIZE)
        }
        esp_err_t res = esp_ble_gatt_set_local_mtu(mtu) ;
        peripheral_mtu = mtu ;
        if(res!=ESP_OK) {
            JSTHROW("esp_ble_gatt_set_local_mtu failed, err = %d", res)
        }
        return JS_UNDEFINED ;
    }
    JSValue BT::getMTU(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        return JS_NewUint32(ctx, peripheral_mtu) ;
    }
}
#endif