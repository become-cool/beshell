#include "BT.hpp"

#if CONFIG_BT_BLUEDROID_ENABLED

#include "esp_bt.h"
#include "esp_bt_defs.h"
#include "esp_bt_main.h"
#include "esp_err.h"
#include "esp_gatt_defs.h"
#include "esp_gattc_api.h"
#include "esp_gap_ble_api.h"
#include "esp_gatt_common_api.h"
#include "esp_log.h"
#include "esp_bt_device.h"
#include "qjs_utils.h"
#include "quickjs/quickjs.h"
#include "../../js/bt/central.c"
#include "../../js/bt/peripheral.c"

static const char *TAG = "bt";


// 全局变量，用于记录连接相关信息
static esp_gatt_if_t      gattc_if_global     = 0;

namespace be{
    const char * const BT::name = "bt" ;
    BT * BT::singleton = nullptr ;
    bool BT::bScanning = false ;

    BT::BT(JSContext * ctx, const char * name)
        : EventModule(ctx, name, 0)
    {
        if(!singleton) {
            singleton = this ;
        }
        EXPORT_FUNCTION(init)
        EXPORT_FUNCTION(setScanParam)
        EXPORT_FUNCTION(startScan)
        EXPORT_FUNCTION(stopScan)
        EXPORT_FUNCTION(isScanning)
        EXPORT_FUNCTION(setMTU)
        EXPORT_FUNCTION(requestMTU)
        EXPORT_FUNCTION(connect)
        EXPORT_FUNCTION(disconnect)
        EXPORT_FUNCTION(search)
        EXPORT_FUNCTION(read)
        EXPORT_FUNCTION(write)
        EXPORT_FUNCTION(subscribe)

        exportName("central") ;
        exportName("peripheral") ;
        exportName("parseAdv") ;
        exportName("waitScanning") ;
        
        enableNativeEvent(ctx, sizeof(struct be_bt_event), 128) ;
    }

    void BT::use(be::BeShell * beshell) {
        beshell->use<BT>() ;
    }
    
    void BT::exports(JSContext *ctx) {
        EventModule::exports(ctx) ;

        JSEngineEvalEmbeded(ctx, central)
    }
    
    static char * bt_uuid_to_string(const esp_bt_uuid_t *uuid, char *str, size_t str_size) {
        if (uuid == NULL || str == NULL || str_size == 0) {
            return NULL;
        }

        switch (uuid->len) {
        case ESP_UUID_LEN_16:
            snprintf(str, str_size, "0x%04X", uuid->uuid.uuid16);
            break;

        case ESP_UUID_LEN_32:
            snprintf(str, str_size, "0x%08X", uuid->uuid.uuid32);
            break;

        case ESP_UUID_LEN_128:
            /* 按照常见的 8-4-4-4-12 格式输出 128 位 UUID。
            * 如果需要 BLE 标准字节序，需要对 uuid128 数组做相应的 endian 转换。
            */
            snprintf(str, str_size,
                    "%02X%02X%02X%02X-%02X%02X-%02X%02X-%02X%02X-%02X%02X%02X%02X%02X%02X",
                    uuid->uuid.uuid128[15], uuid->uuid.uuid128[14],
                    uuid->uuid.uuid128[13], uuid->uuid.uuid128[12],
                    uuid->uuid.uuid128[11], uuid->uuid.uuid128[10],
                    uuid->uuid.uuid128[9],  uuid->uuid.uuid128[8],
                    uuid->uuid.uuid128[7],  uuid->uuid.uuid128[6],
                    uuid->uuid.uuid128[5],  uuid->uuid.uuid128[4],
                    uuid->uuid.uuid128[3],  uuid->uuid.uuid128[2],
                    uuid->uuid.uuid128[1],  uuid->uuid.uuid128[0]);
            break;

        default:
            snprintf(str, str_size, "UNKNOWN_UUID_LEN_%u", uuid->len);
            break;
        }

        return str;
    }

    // GATT客户端相关操作
    static esp_gattc_char_elem_t *char_elem_result = NULL;
    // static esp_gattc_cb_param_t *gattc_cb_param = NULL;

    static void gap_event_handler(esp_gap_ble_cb_event_t event, esp_ble_gap_cb_param_t *param) {

        switch(event){
            // event queue 可能会丢失或不及时
            case ESP_GAP_SEARCH_INQ_CMPL_EVT: {
                BT::bScanning = false ;
                break;
            }
        }
        if(BT::singleton){
            be_bt_event event_msg = {
                .event = event ,
                .gattc_if = 0,
                .gap = *param ,
            } ;
            if( !BT::singleton->emitNativeEvent(&event_msg) ){
                printf("bt queue full (ev:%d)\n", event) ;
            }
        }
    }

    // 处理GATT事件
    static void gattc_event_handler(esp_gattc_cb_event_t event,
                                    esp_gatt_if_t gattc_if,
                                    esp_ble_gattc_cb_param_t *param)
    {
        switch (event) {
        case ESP_GATTC_REG_EVT:
            gattc_if_global = gattc_if ;
            break;

        case ESP_GATTC_WRITE_CHAR_EVT: 
            // printf("ESP_GATTC_WRITE_CHAR_EVT\n") ;
            break;
        case ESP_GATTC_WRITE_DESCR_EVT: 
            // printf("ESP_GATTC_WRITE_DESCR_EVT\n") ;
            break;

        default:
            break;
        }

        if(BT::singleton){
            be_bt_event event_msg = {
                .event = static_cast<uint16_t>(100 + event) ,
                .gattc_if = gattc_if,
                .gatt = *param ,
            } ;
            if( !BT::singleton->emitNativeEvent(&event_msg) ){
                printf("bt queue full\n") ;
            }
        }
    }

    void BT::onNativeEvent(JSContext *ctx, void * param) {
        be_bt_event * msg = (be_bt_event*) param ;
        if(msg->event<100) {
            // dn(msg->event)
            switch(msg->event) {
                case ESP_GAP_BLE_SCAN_START_COMPLETE_EVT: {
                    // printf("scan start ...\n") ;
                    break ;
                }
                case ESP_GAP_BLE_SCAN_RESULT_EVT:{
                    switch(msg->gap.scan_rst.search_evt) {
                        case ESP_GAP_SEARCH_INQ_RES_EVT: {
                            uint8_t addr[18] ;
                            sprintf(addr, "%02X:%02X:%02X:%02X:%02X:%02X", 
                                msg->gap.scan_rst.bda[0] ,
                                msg->gap.scan_rst.bda[1] ,
                                msg->gap.scan_rst.bda[2] ,
                                msg->gap.scan_rst.bda[3] ,
                                msg->gap.scan_rst.bda[4] ,
                                msg->gap.scan_rst.bda[5]
                            );
                            JSValue obj = JS_NewObject(ctx) ;
                            JS_SetPropertyStr(ctx, obj, "searchEvt", JS_NewUint32(ctx, msg->gap.scan_rst.search_evt)) ;
                            JS_SetPropertyStr(ctx, obj, "evt", JS_NewUint32(ctx, msg->gap.scan_rst.ble_evt_type)) ;
                            JS_SetPropertyStr(ctx, obj, "devType", JS_NewUint32(ctx, msg->gap.scan_rst.dev_type)) ;
                            JS_SetPropertyStr(ctx, obj, "addrType", JS_NewUint32(ctx, msg->gap.scan_rst.ble_addr_type)) ;
                            JS_SetPropertyStr(ctx, obj, "addr", JS_NewString(ctx, addr)) ;
                            JS_SetPropertyStr(ctx, obj, "rssi", JS_NewInt32(ctx, msg->gap.scan_rst.rssi)) ;
                            JS_SetPropertyStr(ctx, obj, "flag", JS_NewInt32(ctx, msg->gap.scan_rst.flag)) ;
                            JS_SetPropertyStr(ctx, obj, "num_resps", JS_NewInt32(ctx, msg->gap.scan_rst.num_resps)) ;
                            JS_SetPropertyStr(ctx, obj, "scan_rsp_len", JS_NewUint32(ctx, msg->gap.scan_rst.scan_rsp_len)) ;
                            JS_SetPropertyStr(ctx, obj, "num_dis", JS_NewUint32(ctx, msg->gap.scan_rst.num_dis)) ;
                            JS_SetPropertyStr(ctx, obj, "adv_raw", JS_NewArrayBufferCopy(ctx, msg->gap.scan_rst.ble_adv, msg->gap.scan_rst.adv_data_len)) ;
                            emitSyncFree("scan-res", {obj}) ;
                            break ;
                        }
                        case ESP_GAP_SEARCH_INQ_CMPL_EVT: {
                            // bScanning = false ;
                            // printf("scan complete\n") ;
                            emitSyncFree("scan-cmpl", {}) ;
                            break ;
                        }
                        default:
                            // dn(msg->gap.scan_rst.search_evt)
                            break;
                    }
                    break ;
                }
                default:
                    // dn(msg->event)
                    break;
            }
        }
        else if(msg->event<200) {
            // dn(msg->event-100)
            switch(msg->event-100) {
            case ESP_GATTC_CONNECT_EVT:
                break;
            case ESP_GATTC_OPEN_EVT: {
                char addr [18] ;
                sprintf(addr, "%02X:%02X:%02X:%02X:%02X:%02X", 
                            msg->gatt.open.remote_bda[0],
                            msg->gatt.open.remote_bda[1],
                            msg->gatt.open.remote_bda[2],
                            msg->gatt.open.remote_bda[3],
                            msg->gatt.open.remote_bda[4],
                            msg->gatt.open.remote_bda[5]
                );
                emitSyncFree("open", {
                    JS_NewUint32(ctx, msg->gatt.open.status) ,
                    JS_NewUint32(ctx, msg->gatt.open.conn_id) ,
                    JS_NewString(ctx, (const char *)addr) ,
                    JS_NewUint32(ctx, msg->gatt.open.mtu) ,
                }) ;
                break ;
            }
            case ESP_GATTC_DIS_SRVC_CMPL_EVT:
                // dn(ESP_GATTC_DIS_SRVC_CMPL_EVT)
                emitSyncFree("dis-srvc-cmpl", {
                    JS_NewInt32(ctx, msg->gatt.dis_srvc_cmpl.status) ,
                    JS_NewInt32(ctx, msg->gatt.dis_srvc_cmpl.conn_id)
                }) ;
                break ;
                
            // 服务/特征 搜索
            case ESP_GATTC_SEARCH_RES_EVT: {
                // dn(ESP_GATTC_SEARCH_RES_EVT)
                char uuid_str[37] ;
                bt_uuid_to_string(&msg->gatt.search_res.srvc_id.uuid,uuid_str,sizeof(uuid_str)) ;

                JSValue lstChars = getCharacteristics(ctx, 
                    msg->gattc_if, 
                    msg->gatt.search_res.conn_id,
                    msg->gatt.search_res.start_handle,
                    msg->gatt.search_res.end_handle) ;

                JSValue objSvr = JS_NewObject(ctx) ;
                JS_SetPropertyStr(ctx, objSvr, "chars", lstChars) ;
                JS_SetPropertyStr(ctx, objSvr, "uuid", JS_NewString(ctx, uuid_str)) ;
                
                emitSyncFree("search-res", {JS_NewInt32(ctx, msg->gatt.search_res.conn_id), objSvr}) ;
                break;
            }
            case ESP_GATTC_SEARCH_CMPL_EVT:
                // dn(ESP_GATTC_SEARCH_CMPL_EVT)
                emitSyncFree("search-cmpl", {
                    JS_NewInt32(ctx, msg->gatt.dis_srvc_cmpl.status) ,
                    JS_NewInt32(ctx, msg->gatt.dis_srvc_cmpl.conn_id)
                }) ;
                break ;

            case ESP_GATTC_WRITE_CHAR_EVT: {
                // printf("ESP_GATTC_WRITE_CHAR_EVT2\n") ;
                emitSyncFree("write-char", {
                    JS_NewInt32(ctx, msg->gatt.write.status) ,
                    JS_NewInt32(ctx, msg->gatt.write.handle) ,
                    JS_NewInt32(ctx, msg->gatt.write.offset)
                }) ;
                break;
            }
            case ESP_GATTC_WRITE_DESCR_EVT: {
                printf("ESP_GATTC_WRITE_DESCR_EVT\n") ;
                emitSyncFree("write-desc", {
                    JS_NewInt32(ctx, msg->gatt.write.status) ,
                    JS_NewInt32(ctx, msg->gatt.write.handle) ,
                    JS_NewInt32(ctx, msg->gatt.write.offset)
                }) ;
                break;
            }

            case ESP_GATTC_NOTIFY_EVT: {
                printf("ESP_GATTC_NOTIFY_EVT\n"); 
                break;
            }

            case ESP_GATTC_CFG_MTU_EVT: {
                emitSyncFree("cfg-mtu", {
                    JS_NewInt32(ctx, msg->gatt.cfg_mtu.status) ,
                    JS_NewInt32(ctx, msg->gatt.cfg_mtu.conn_id) ,
                    JS_NewInt32(ctx, msg->gatt.cfg_mtu.mtu)
                }) ;

            }

            default:break ;
            }
        }
    }

    JSValue BT::getCharacteristics(JSContext *ctx, esp_gatt_if_t gattc_if, uint16_t conn_id, uint16_t start_handle, uint16_t end_handle) {
        uint16_t count  = 0;
        uint16_t offset = 0;
        esp_gatt_status_t ret_status = esp_ble_gattc_get_attr_count(
                                gattc_if,
                                conn_id,
                                ESP_GATT_DB_CHARACTERISTIC,
                                start_handle, end_handle, NULL, &count);
        if (ret_status != ESP_GATT_OK) {
            ESP_LOGE(TAG, "esp_ble_gattc_get_attr_count error, %d", __LINE__);
            return JS_NULL ;
        }
        if (count <= 0) {
            return JS_NewArray(ctx) ;
        }
        char_elem_result = (esp_gattc_char_elem_t *)malloc(sizeof(esp_gattc_char_elem_t) * count);
        if (!char_elem_result) {
            ESP_LOGE(TAG, "gattc no mem");
            return JS_NULL ;
        }

        memset(char_elem_result, 0xff, sizeof(esp_gattc_char_elem_t) * count);
        ret_status = esp_ble_gattc_get_all_char(
                            gattc_if,
                            conn_id,
                            start_handle,
                            end_handle,
                            char_elem_result,
                            &count,
                            offset);
        if (ret_status != ESP_GATT_OK) {
            ESP_LOGE(TAG, "esp_ble_gattc_get_all_char error, %d", __LINE__);
            free(char_elem_result);
            char_elem_result = NULL;
            return JS_NULL ;
        }

        JSValue lstChars = JS_NewArray(ctx) ;
        for (int i = 0; i < count; i ++) {
            JSValue objChar = JS_NewObject(ctx) ;
            JS_SetPropertyUint32(ctx, lstChars, i, objChar) ;

            char uuid_str[37] ;
            bt_uuid_to_string(&char_elem_result[i].uuid,uuid_str,sizeof(uuid_str)) ;

            // printf("%s, handle:%d, properties:%d\n",uuid_str,char_elem_result[i].char_handle, char_elem_result[i].properties) ;

            JS_SetPropertyStr(ctx, objChar, "addr", JS_NewString(ctx, uuid_str)) ;
            JS_SetPropertyStr(ctx, objChar, "handle", JS_NewUint32(ctx, char_elem_result[i].char_handle)) ;
            JS_SetPropertyStr(ctx, objChar, "props", JS_NewUint32(ctx, char_elem_result[i].properties)) ;

            /*  Every service have only one char in our 'ESP_GATTS_DEMO' demo, so we used first 'char_elem_result' */
            if (count > 0 && (char_elem_result[0].properties & ESP_GATT_CHAR_PROP_BIT_NOTIFY)){
                // esp_ble_gattc_register_for_notify (gattc_if, gl_profile_tab[PROFILE_A_APP_ID].remote_bda, char_elem_result[0].char_handle);
            
                uint16_t descr_count = 0;
                esp_err_t ret = esp_ble_gattc_get_attr_count(
                            gattc_if, conn_id, ESP_GATT_DB_DESCRIPTOR, start_handle, end_handle, char_elem_result[i].char_handle,&descr_count
                );
                if (ret != ESP_GATT_OK || count == 0) {
                    ESP_LOGW(TAG, "No descriptor found for this char (ret=%d, count=%d)", ret, count);
                    continue;
                }

                esp_gattc_descr_elem_t *descr_elems = (esp_gattc_descr_elem_t *)malloc(sizeof(esp_gattc_descr_elem_t) * descr_count);
                if (!descr_elems) {
                    ESP_LOGE(TAG, "No memory for descr_elems!");
                    continue;
                }

                ret = esp_ble_gattc_get_all_descr(gattc_if,
                                                conn_id,
                                                char_elem_result[i].char_handle,
                                                descr_elems,
                                                &descr_count,
                                                0);
                if (ret == ESP_GATT_OK && descr_count > 0) {
                    JSValue mapDescr = JS_NewObject(ctx) ;
                    for (int i = 0; i < descr_count; i ++) {
                        JSValue objDescr = JS_NewObject(ctx) ;

                        char uuid_str[37] ;
                        bt_uuid_to_string(&descr_elems[i].uuid,uuid_str,sizeof(uuid_str)) ;

                        JS_SetPropertyStr(ctx, objDescr, "uuid", JS_NewString(ctx, uuid_str)) ;
                        JS_SetPropertyStr(ctx, objDescr, "handle", JS_NewUint32(ctx, descr_elems[i].handle)) ;
                        JS_SetPropertyStr(ctx, mapDescr, uuid_str, objDescr) ;
                    }

                    JS_SetPropertyStr(ctx, objChar, "desc", mapDescr) ;
                }

                free(descr_elems);
            }


        }

        free(char_elem_result);
        char_elem_result = NULL;
    
        return lstChars ;
    }

    JSValue BT::init(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {

        static bool be_inited = false ;
        if(be_inited) {
            return JS_UNDEFINED ;
        }
        be_inited = true ;

        esp_err_t ret;
        esp_bt_controller_config_t bt_cfg = BT_CONTROLLER_INIT_CONFIG_DEFAULT();

        // 初始化BLE控制器
        ret = esp_bt_controller_init(&bt_cfg);
        if (ret) {
            JSTHROW("Bluetooth controller initialize failed: %s", esp_err_to_name(ret));
        }

        ret = esp_bt_controller_enable(ESP_BT_MODE_BLE);
        if (ret) {
            JSTHROW("Bluetooth controller enable failed: %s", esp_err_to_name(ret));
        }
        
        // Initialize Bluedroid
        ret = esp_bluedroid_init();
        if (ret) {
            JSTHROW("Failed to initialize Bluedroid: %s", esp_err_to_name(ret));
        }

        // Enable Bluedroid
        ret = esp_bluedroid_enable();
        if (ret) {
            JSTHROW("Failed to enable Bluedroid: %s", esp_err_to_name(ret));
        }

        esp_ble_gattc_register_callback(gattc_event_handler);
        esp_ble_gattc_app_register(0); // 注册应用ID
        
        // 注册GAP回调函数
        ret = esp_ble_gap_register_callback(gap_event_handler);
        if (ret) {
            JSTHROW("GAP callback registration failed");
        }
        // esp_ble_auth_req_t auth_req = ESP_LE_AUTH_REQ_SC_MITM_BOND;
        // esp_ble_gap_set_security_param(ESP_BLE_SM_AUTHEN_REQ_MODE, &auth_req, sizeof(auth_req));
        return JS_UNDEFINED ;
    }
    JSValue BT::setScanParam(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
#if (BLE_42_FEATURE_SUPPORT == TRUE)

        CHECK_ARGC(1)

        bool active = false ;
        int own_addr_type = BLE_ADDR_TYPE_RANDOM ;
        int scan_filter_policy = BLE_ADDR_TYPE_RANDOM ;

        esp_ble_scan_params_t ble_scan_params ;
        GET_INT32_PROP_OPT( argv[0], "scan_type",            ble_scan_params.scan_type,              BLE_SCAN_TYPE_PASSIVE )
        GET_INT32_PROP_OPT( argv[0], "own_addr_type",        ble_scan_params.own_addr_type,          BLE_ADDR_TYPE_PUBLIC )
        GET_INT32_PROP_OPT( argv[0], "scan_filter_policy",   ble_scan_params.scan_filter_policy,     BLE_SCAN_FILTER_ALLOW_ALL )
        GET_INT16_PROP_OPT( argv[0], "scan_interval",        ble_scan_params.scan_interval,          0x50 )
        GET_INT16_PROP_OPT( argv[0], "scan_window",          ble_scan_params.scan_window,            0x40 )
        GET_INT32_PROP_OPT( argv[0], "scan_duplicate",       ble_scan_params.scan_duplicate,         BLE_SCAN_DUPLICATE_DISABLE )

        // esp_ble_scan_params_t ble_scan_params = {
        //     .scan_type = active? BLE_SCAN_TYPE_ACTIVE: BLE_SCAN_TYPE_PASSIVE,
        //     .own_addr_type = BLE_ADDR_TYPE_RANDOM,  //BLE_ADDR_TYPE_PUBLIC, //BLE_ADDR_TYPE_RANDOM,
        //     .scan_filter_policy = BLE_SCAN_FILTER_ALLOW_ALL,
        //     .scan_interval = 0x50,
        //     .scan_window = 0x40,
        //     .scan_duplicate = BLE_SCAN_DUPLICATE_DISABLE,
        // };
        // dn(ble_scan_params.scan_type)
        // dn(ble_scan_params.own_addr_type)
        // dn(ble_scan_params.scan_filter_policy)
        // dn(ble_scan_params.scan_interval)
        // dn(ble_scan_params.scan_window)
        // dn(ble_scan_params.scan_duplicate)

        // 开始扫描BLE设备
        esp_err_t ret = esp_ble_gap_set_scan_params(&ble_scan_params);
        if (ret != ESP_OK) {
            JSTHROW("Failed to set scan params: %s", esp_err_to_name(ret));
        }

        return JS_UNDEFINED ;
#else
        JSTHROW("BLE 4.2 is not supported") ;
#endif
    }
    
    JSValue BT::startScan(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
#if (BLE_42_FEATURE_SUPPORT == TRUE)
        // if(bScanning) {
        //     JSTHROW("already scanning")
        // }

        ARGV_TO_UINT32_OPT(0,dur,UINT32_MAX)

        esp_err_t ret = esp_ble_gap_start_scanning(dur) ;
        if (ret != ESP_OK) {
            JSTHROW("Failed to start scanning: %s", esp_err_to_name(ret));
        }
        bScanning = true ;

        return JS_UNDEFINED ;
#else
        JSTHROW("BLE 4.2 is not supported") ;
#endif
    }

    JSValue BT::stopScan(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
#if (BLE_42_FEATURE_SUPPORT == TRUE)
        bScanning = false ;
        return esp_ble_gap_stop_scanning()==ESP_OK? JS_TRUE: JS_FALSE ;
#else
        JSTHROW("BLE 4.2 is not supported") ;
#endif
    }
    JSValue BT::isScanning(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        return bScanning? JS_TRUE: JS_FALSE ;
    }

#define CHECK_GATTC_IF                      \
        if(gattc_if_global<=0) {            \
            JSTHROW("call bt.init() first") \
        }

    JSValue BT::connect(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        CHECK_GATTC_IF
        esp_bd_addr_t addr = {0} ;

        CHECK_ARGC(1)
        ARGV_TO_CSTRING_LEN(0, caddr, caddr_len)
        if(caddr_len!=17) {
            JS_FreeCString(ctx, caddr) ;
            JSTHROW("not a valid address")
        }
        /* Convert string to address */
        sscanf(caddr, "%hhx:%hhx:%hhx:%hhx:%hhx:%hhx", (char *)addr+0, (char *)addr+1, (char *)addr+2, (char *)addr+3, (char *)addr+4, (char *)addr+5);
        JS_FreeCString(ctx, caddr) ;

        esp_err_t err = esp_ble_gattc_open(
            gattc_if_global,
            addr,                       // 目标设备的 MAC 地址
            BLE_ADDR_TYPE_RANDOM,       // 若设备是 Public Address，则用此类型
            true                        // 是否开启重连特性（若连接断开，是否自动重连）
        );
        if (err != ESP_OK) {
            JSTHROW("esp_ble_gattc_open failed, err = %d", err)
        }
        return JS_UNDEFINED ;
    }
    JSValue BT::disconnect(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        CHECK_GATTC_IF
        CHECK_ARGC(1)
        ARGV_TO_UINT16(0, conn_id)
        esp_err_t res = esp_ble_gattc_close(gattc_if_global,conn_id) ;
        if (res != ESP_OK) {
            JSTHROW("esp_ble_gattc_close failed, err = %d", res)
        }
        return JS_UNDEFINED ;
    }

    JSValue BT::setMTU(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        CHECK_ARGC(1)
        ARGV_TO_UINT16(0, mtu)
        esp_err_t res = esp_ble_gatt_set_local_mtu(mtu) ;
        if(res!=ESP_OK) {
            JSTHROW("esp_ble_gatt_set_local_mtu failed, err = %d", res)
        }
        return JS_UNDEFINED ;
    }
    JSValue BT::requestMTU(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        CHECK_GATTC_IF
        CHECK_ARGC(1)
        ARGV_TO_UINT16(0, conn_id)
        esp_ble_gattc_send_mtu_req(gattc_if_global,conn_id) ;
        return JS_UNDEFINED ;
    }
    JSValue BT::search(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        CHECK_GATTC_IF
        CHECK_ARGC(1)
        ARGV_TO_UINT16(0, conn_id)
        esp_ble_gattc_search_service(gattc_if_global, conn_id, NULL);
        return JS_UNDEFINED ;
    }
    JSValue BT::read(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        CHECK_GATTC_IF
        return JS_UNDEFINED ;
    }
    JSValue BT::write(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        CHECK_GATTC_IF
        CHECK_ARGC(3)
        ARGV_TO_UINT16(0, conn_id)
        ARGV_TO_UINT16(1, handle)
        size_t size ;
        char * buff = (char *)JS_GetArrayBuffer(ctx, &size, argv[2]) ;
        if(!buff || !size) {
            JSTHROW("argv is not a valid ArrayBuffer")
        }

        bool rsp = false ;
        if(argc>3) {
            rsp = JS_ToBool(ctx, argv[3]) ;
        }

        esp_err_t err = esp_ble_gattc_write_char(
            gattc_if_global, conn_id, handle, size, buff ,
            ( rsp? ESP_GATT_WRITE_TYPE_RSP: ESP_GATT_WRITE_TYPE_NO_RSP ) ,
            ESP_GATT_AUTH_REQ_NONE
        ) ;
        if (err != ESP_OK) {
            JSTHROW("esp_ble_gattc_write_char() failed, error = %x")
        }
        
        return JS_UNDEFINED ;
    }
    JSValue BT::subscribe(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        CHECK_GATTC_IF
        CHECK_ARGC(2)
        ARGV_TO_UINT16(0, conn_id)
        ARGV_TO_UINT16(1, handle)
        ARGV_TO_UINT8_OPT(2, type, 1)
        if(type!=1 && type!=2) {
            JSTHROW("type must be 1 or 2")
        }

        // 0x01,0x00 表示启用 Notification；0x02,0x00 可表示启用 Indication
        uint8_t notify_en[2] = {type, 0x00};

        esp_err_t err = esp_ble_gattc_write_char_descr(
            gattc_if_global,
            conn_id,
            handle,
            sizeof(notify_en),
            notify_en,
            ESP_GATT_WRITE_TYPE_RSP,
            ESP_GATT_AUTH_REQ_NONE
        );
        if(err) {
            JSTHROW("esp_ble_gattc_write_char_descr() failed, error = %d",err)
        }

        return JS_UNDEFINED ;
    }
}

#endif