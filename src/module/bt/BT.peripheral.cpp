#include "BT.hpp"

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

#define GATTS_TAG "GATT_SERVICE"


namespace be {

    static esp_gatt_if_t gatts_if_global = ESP_GATT_IF_NONE ;
    static uint16_t last_service_handle = 0xFFFF ;
    static uint16_t last_char_handle = 0xFFFF ;
    static uint16_t last_descr_handle = 0xFFFF ;
    static uint16_t last_conn_id = 0xFFFF ;

    void BT::gatts_event_handler(esp_gatts_cb_event_t event, esp_gatt_if_t gatts_if, esp_ble_gatts_cb_param_t *param) {
        if(gattsHandler && gattsHandler(event, gatts_if, param)){
            return ;
        }
        bt_event event_msg = {
            .event = static_cast<uint16_t>(200 + event) ,
            .gatt_if = gatts_if,
            .gatts = *param ,
            .data = {NULL,0}
        } ;
        switch (event) {
            case ESP_GATTS_REG_EVT:
                // printf("ESP_GATTS_REG_EVT, status %d, app_id %d\n", param->reg.status, param->reg.app_id);
                gatts_if_global = gatts_if ;
                break;

            // 创建服务
            case ESP_GATTS_CREATE_EVT:
                // printf("SERVICE_CREATED, status %d, service_handle %d\n", param->create.status, param->create.service_handle);
                if(ESP_GATT_OK==param->create.status) {
                    last_service_handle = param->create.service_handle ;
                }
                break;
            // 添加特征
            case ESP_GATTS_ADD_CHAR_EVT:
                // printf("Characteristic add, status %d, attr_handle %d, service_handle %d\n", param->add_char.status, param->add_char.attr_handle, param->add_char.service_handle);
                if(ESP_GATT_OK==param->add_char.status) {
                    last_char_handle = param->add_char.attr_handle;
                }
                break ;
            // 添加描述符
            case ESP_GATTS_ADD_CHAR_DESCR_EVT:
                // printf("Descriptor add, status %d, attr_handle %d, service_handle %d\n", param->add_char_descr.status, param->add_char_descr.attr_handle, param->add_char_descr.service_handle);
                last_descr_handle = param->add_char_descr.attr_handle ;
                break ;
                
            case ESP_GATTS_START_EVT:
                // printf("Service start, status %d, service_handle %d\n",
                //         param->start.status, param->start.service_handle);
                break;
            case ESP_GATTS_STOP_EVT:
                break;

            // 读
            case ESP_GATTS_READ_EVT:
                // printf("ESP_GATTS_READ_EVT, handle %d\n", param->read.handle);
                break;

            // 写
            case ESP_GATTS_WRITE_EVT:
                // printf("ESP_GATTS_WRITE_EVT, handle %d, value len %d\n", param->write.handle, param->write.len);
                
                // Copy write data to event for event system to process
                if (param->write.len > 0) {
                    event_msg.data.len = param->write.len;
                    event_msg.data.ptr = malloc(param->write.len);
                    if (event_msg.data.ptr) {
                        memcpy(event_msg.data.ptr, param->write.value, param->write.len);
                    } else {
                        printf("malloc failed for write event data\n");
                    }

                    // dn2(((uint8_t*)event_msg.data.ptr)[0], ((uint8_t*)event_msg.data.ptr)[1]) ;
                }
                
                if (param->write.need_rsp){
                    esp_ble_gatts_send_response(gatts_if, param->write.conn_id, param->write.trans_id, ESP_GATT_OK, NULL);
                }
                break;

            case ESP_GATTS_EXEC_WRITE_EVT:
                // printf("Execute write, conn_id %d, trans_id %d, exec_write_flag %d\n", 
                //        param->exec_write.conn_id, param->exec_write.trans_id, param->exec_write.exec_write_flag);
                
                // Send response for the exec write request
                esp_ble_gatts_send_response(gatts_if, param->exec_write.conn_id, param->exec_write.trans_id, ESP_GATT_OK, NULL);
                break;

            case ESP_GATTS_MTU_EVT:
                // printf("MTU exchange, MTU %d\n", param->mtu.mtu);
                break;
            case ESP_GATTS_UNREG_EVT:
                break;
                
            case ESP_GATTS_CONNECT_EVT: {
                
                // printf("ESP_GATTS_CONNECT_EVT\n");
                esp_ble_conn_update_params_t conn_params = {0};
                memcpy(conn_params.bda, param->connect.remote_bda, sizeof(esp_bd_addr_t));
                /* For the IOS system, please reference the apple official documents about the ble connection parameters restrictions. */
                conn_params.latency = 0;
                conn_params.max_int = 0x20;    // max_int = 0x20*1.25ms = 40ms
                conn_params.min_int = 0x10;    // min_int = 0x10*1.25ms = 20ms
                conn_params.timeout = 400;    // timeout = 400*10ms = 4000ms
                esp_ble_gap_update_conn_params(&conn_params);

                last_conn_id = param->connect.conn_id;
                break;
            }
            case ESP_GATTS_DISCONNECT_EVT:
                // printf("ESP_GATTS_DISCONNECT_EVT\n");
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

        static bool inited = false ;
        if(inited) {
            return true ;
        }
        inited = true ;
        
        init() ;

        esp_err_t err = esp_ble_gatts_register_callback(gatts_event_handler);
        if(err!=ESP_OK) {
            printf("esp_ble_gatts_register_callback failed, err = %d\n", err);
            return false ;
        }
        err = esp_ble_gatts_app_register(0); // 注册应用ID
        if (err!=ESP_OK) {
            printf("esp_ble_gatts_app_register failed, err = %d\n", err);
            return false ;
        }

        return true ;
    }
    
    JSValue BT::initPeripheral(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        if(!initPeripheral()) {
            JSTHROW("bt peripheralinit failed")
        }
        return JS_UNDEFINED ;
    }
    
    void BT::onNativePeripheralEvent(JSContext *ctx, bt_event * event) {
        // Handle peripheral events (200-299 range)
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
                
            case ESP_GATTS_START_EVT:
                emitSyncFree("start", {
                    JS_NewInt32(ctx, event->gatts.start.status),
                    JS_NewInt32(ctx, event->gatts.start.service_handle)
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
                // Keep this one uppercase
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
                char addr[18];
                sprintf(addr, "%02X:%02X:%02X:%02X:%02X:%02X",
                    event->gatts.connect.remote_bda[0], event->gatts.connect.remote_bda[1],
                    event->gatts.connect.remote_bda[2], event->gatts.connect.remote_bda[3],
                    event->gatts.connect.remote_bda[4], event->gatts.connect.remote_bda[5]);
                    
                // Keep this one uppercase
                emitSyncFree("CONNECT", {
                    JS_NewInt32(ctx, event->gatts.connect.conn_id),
                    JS_NewString(ctx, addr),
                    JS_NewInt32(ctx, event->gatts.connect.conn_params.interval),
                    JS_NewInt32(ctx, event->gatts.connect.conn_params.latency),
                    JS_NewInt32(ctx, event->gatts.connect.conn_params.timeout)
                });
                break;
            }
                
            case ESP_GATTS_DISCONNECT_EVT: {
                char addr[18];
                sprintf(addr, "%02X:%02X:%02X:%02X:%02X:%02X",
                    event->gatts.disconnect.remote_bda[0], event->gatts.disconnect.remote_bda[1],
                    event->gatts.disconnect.remote_bda[2], event->gatts.disconnect.remote_bda[3],
                    event->gatts.disconnect.remote_bda[4], event->gatts.disconnect.remote_bda[5]);
                    
                // Keep this one uppercase
                emitSyncFree("DISCONNECT", {
                    JS_NewInt32(ctx, event->gatts.disconnect.conn_id),
                    JS_NewString(ctx, addr),
                    JS_NewInt32(ctx, event->gatts.disconnect.reason)
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
                // Handle other events if needed
                // emitSyncFree("unknown", {
                //     JS_NewInt32(ctx, event->event - 200)
                // });
                break;
        }
        
        // Free any allocated data
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
        esp_err_t err = esp_ble_gap_config_adv_data_raw(data, datalen);
        if(err!=ESP_OK) {
            JSTHROW("esp_ble_gap_config_adv_data_raw failed, err = %d", err)
        }
        return JS_UNDEFINED ;
    }

    JSValue BT::startAdv(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        CHECK_GATTS_IF
        esp_ble_adv_params_t adv_params = {
            .adv_int_min       = 0xA0,                              // 最小广播间隔 0xA0*0.625ms=250ms
            .adv_int_max       = 0xB0,                              // 最大广播间隔 0xB0*0.625ms=300ms
            .adv_type          = ADV_TYPE_IND,                      // 可连接的非定向广播
            .own_addr_type     = BLE_ADDR_TYPE_PUBLIC,              // 公共地址
            .channel_map       = ADV_CHNL_ALL,                      // 使用所有频道
            .adv_filter_policy = ADV_FILTER_ALLOW_SCAN_ANY_CON_ANY, // 允许扫描和连接
        };
        if(argc>0) {
            GET_INT16_PROP_OPT  ( argv[0], "min",                 adv_params.adv_int_min,         0xA0 )
            GET_INT16_PROP_OPT  ( argv[0], "max",                 adv_params.adv_int_max,         0xB0 )
            GET_INTEGER_PROP_OPT( argv[0], "type",                adv_params.adv_type,            esp_ble_adv_type_t,       int32_t, JS_ToInt32, ADV_TYPE_IND )
            GET_INTEGER_PROP_OPT( argv[0], "own_addr_type",       adv_params.own_addr_type,       esp_ble_addr_type_t,      int32_t, JS_ToInt32, BLE_ADDR_TYPE_PUBLIC )
            GET_INTEGER_PROP_OPT( argv[0], "channel_map",         adv_params.channel_map,         esp_ble_adv_channel_t,    int32_t, JS_ToInt32, ADV_CHNL_ALL )
            GET_INTEGER_PROP_OPT( argv[0], "adv_filter_policy",   adv_params.adv_filter_policy,   esp_ble_adv_filter_t,     int32_t, JS_ToInt32, ADV_FILTER_ALLOW_SCAN_ANY_CON_ANY )
        }

        // dn(adv_params.adv_int_min)
        // dn(adv_params.adv_int_max)
        // dn(adv_params.adv_type)
        // dn(adv_params.own_addr_type)
        // dn(adv_params.channel_map)
        // dn(adv_params.adv_filter_policy)

        esp_err_t err = esp_ble_gap_start_advertising(&adv_params);
        if(err!=ESP_OK) {
            JSTHROW("esp_ble_gap_start_advertising failed, err = %d", err)
        }
        return JS_UNDEFINED ;
    }
    
    JSValue BT::stopAdv(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        CHECK_GATTS_IF
        esp_err_t err = esp_ble_gap_stop_advertising();
        if(err!=ESP_OK) {
            JSTHROW("esp_ble_gap_start_advertising failed, err = %d", err)
        }
        return JS_UNDEFINED ;
    }

    JSValue BT::addService(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        CHECK_GATTS_IF
        CHECK_ARGC(2)
        
        // Get UUID string
        ARGV_TO_CSTRING(0, uuid_str)
        
        // Get number of handles
        int32_t num_handle;
        if (JS_ToInt32(ctx, &num_handle, argv[1]) < 0 || num_handle <= 0) {
            JS_FreeCString(ctx, uuid_str);
            JSTHROW("Invalid number of handles")
        }
        
        // Convert UUID string to esp_bt_uuid_t
        esp_bt_uuid_t uuid = bt_string_to_uuid((char *)uuid_str, strlen(uuid_str));
        if (uuid.len == ESP_UUID_LEN_16 && uuid.uuid.uuid16 == 0) {
            JS_FreeCString(ctx, uuid_str);
            JSTHROW("Invalid UUID format")
        }
        
        // Create service ID structure
        esp_gatt_srvc_id_t srvc_id = {
            .id = {
                .uuid = uuid,
                .inst_id = 0
            },
            .is_primary = true
        };
        
        // Create the service
        last_service_handle = 0xFFFF ;
        esp_err_t err = esp_ble_gatts_create_service(gatts_if_global, &srvc_id, num_handle);
        JS_FreeCString(ctx, uuid_str);
        if (err != ESP_OK) {
            JSTHROW("esp_ble_gatts_create_service failed, err = %d", err)
        }
        
        err = esp_ble_gatts_start_service(last_service_handle);
        if(err!=ESP_OK) {
            JSTHROW("esp_ble_gatts_start_service failed, err = %d", err)
        }
        
        return JS_NewUint32(ctx, last_service_handle);
    }

    JSValue BT::addChar(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        CHECK_GATTS_IF
        CHECK_ARGC(3)
        
        // Get service handle
        uint16_t service_handle;
        if (JS_ToUint32(ctx, (uint32_t*)&service_handle, argv[0]) < 0) {
            JSTHROW("Invalid service handle")
        }

        // Get UUID string
        ARGV_TO_CSTRING(1, uuid_str)
        
        // Convert UUID string to esp_bt_uuid_t
        esp_bt_uuid_t uuid = bt_string_to_uuid((char *)uuid_str, strlen(uuid_str));
        if (uuid.len == ESP_UUID_LEN_16 && uuid.uuid.uuid16 == 0) {
            JS_FreeCString(ctx, uuid_str);
            JSTHROW("Invalid UUID format")
        }

        // Set up property flags
        uint8_t property = 0;
        uint8_t perm = 0;

        if (JS_IsString(argv[2])) {
            // Handle string format
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
            // Handle array format
            JSValue length = JS_GetPropertyStr(ctx, argv[2], "length");
            int32_t arr_len;
            JS_ToInt32(ctx, &arr_len, length);
            JS_FreeValue(ctx, length);

            for (int i = 0; i < arr_len; i++) {
                JSValue item = JS_GetPropertyUint32(ctx, argv[2], i);
                if (JS_IsString(item)) {
                    const char* perm_item = JS_ToCString(ctx, item);
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
                    }
                    else if (strcmp(perm_item, "indicate") == 0) {
                        property |= ESP_GATT_CHAR_PROP_BIT_INDICATE;
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

        // Add characteristic
        last_char_handle = 0xFFFF;
        esp_err_t err = esp_ble_gatts_add_char(
            service_handle,
            &uuid,
            perm,
            property,
            NULL,    // char value
            NULL     // control
        );

        JS_FreeCString(ctx, uuid_str);
        
        if (err != ESP_OK) {
            JSTHROW("esp_ble_gatts_add_char failed, err = %d", err)
        }
        
        
        // Add CCCD if the characteristic has notify or indicate property
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
                NULL,    // value
                NULL     // control
            );

            if (err != ESP_OK) {
                printf("Failed to add CCCD descriptor, err = %d\n", err);
            }
        }

        return JS_NewUint32(ctx, last_char_handle);
    }

    JSValue BT::sendNotify(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        CHECK_GATTS_IF
        CHECK_ARGC(2)

        // Get characteristic handle
        uint32_t char_handle = 0xFFFFFFFF;
        if (JS_ToUint32(ctx, (uint32_t*)&char_handle, argv[0]) < 0) {
            JSTHROW("Invalid characteristic handle")
        }

        // Get connection ID (optional fourth parameter)
        uint16_t conn_id = last_conn_id;
        if (argc > 3 && !JS_IsUndefined(argv[3])) {
            if (JS_ToUint32(ctx, (uint32_t*)&conn_id, argv[3]) < 0) {
                JSTHROW("Invalid connection ID")
            }
        }

        // Check connection
        if (conn_id == 0xFFFF) {
            JSTHROW("No device connected")
        }

        // Get indicate flag (optional third parameter)
        bool is_indication = false;
        if (argc > 2 && !JS_IsUndefined(argv[2])) {
            if (!JS_IsBool(argv[2])) {
                JSTHROW("Third parameter must be boolean")
            }
            is_indication = JS_ToBool(ctx, argv[2]);
        }

        uint8_t* data = NULL;
        size_t data_len = 0;

        // Handle string input
        if (JS_IsString(argv[1])) {
            const char* str = JS_ToCString(ctx, argv[1]);
            if (!str) {
                JSTHROW("Failed to convert string")
            }
            data_len = strlen(str);
            data = (uint8_t*)str;
        }
        // Handle ArrayBuffer input
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

        // Send notification or indication
        esp_err_t err = esp_ble_gatts_send_indicate(
            gatts_if_global,
            conn_id,  // Now using the specified conn_id instead of hardcoded last_conn_id
            (uint16_t)char_handle,
            data_len,
            data,
            is_indication  // false for notification, true for indication
        );

        // Free string if we allocated one
        if (JS_IsString(argv[1])) {
            JS_FreeCString(ctx, (const char*)data);
        }

        if (err != ESP_OK) {
            JSTHROW("Failed to send notification/indication, err = %d", err)
        }

        return JS_UNDEFINED;
    }
}
#endif