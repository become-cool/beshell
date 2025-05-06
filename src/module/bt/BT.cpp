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



namespace be{

    char const * const BT::name = "bt" ;
    BT * BT::singleton = nullptr ;
    bool BT::bScanning = false ;
    
    gap_handler_t   BT::gapHandler = nullptr ;
    gattc_handler_t BT::gattcHandler = nullptr ;
    gatts_handler_t BT::gattsHandler = nullptr ;

    BT::BT(JSContext * ctx, const char * name)
        : EventModule(ctx, name, 0)
    {
        if(!singleton) {
            singleton = this ;
        }
        
        exportFunction("_initCentral", initCentral, 0) ;
        exportFunction("_initPeripheral",initPeripheral, 0) ;

        // central
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

        EXPORT_FUNCTION(setPower)
        EXPORT_FUNCTION(power)

        // peripheral
        EXPORT_FUNCTION(setAdvName)
        EXPORT_FUNCTION(setAdvData)
        EXPORT_FUNCTION(startAdv)
        EXPORT_FUNCTION(stopAdv)
        EXPORT_FUNCTION(addService)
        EXPORT_FUNCTION(addChar)
        EXPORT_FUNCTION(sendNotify)

        exportName("central") ;
        exportName("peripheral") ;
        exportName("parseAdv") ;
        exportName("waitScanning") ;
        exportName("cent") ;
        exportName("periph") ;
        exportName("pher") ;
        
        enableNativeEvent(ctx, sizeof(struct bt_event), 128) ;
    }

    void BT::use(be::BeShell * beshell) {
        beshell->use<BT>() ;
    }
    
    void BT::exports(JSContext *ctx) {
        EventModule::exports(ctx) ;

        JSEngineEvalEmbeded(ctx, central)
        JSEngineEvalEmbeded(ctx, peripheral)
    }
    
    char * BT::bt_uuid_to_string(const esp_bt_uuid_t *uuid, char *str, size_t str_size) {
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

    esp_bt_uuid_t BT::bt_string_to_uuid(char *str, size_t str_size) {
        esp_bt_uuid_t uuid;
        memset(&uuid, 0, sizeof(esp_bt_uuid_t));

        if (str == NULL || str_size == 0) {
            uuid.len = ESP_UUID_LEN_16;  // Set to invalid length
            return uuid;
        }

        // Remove "0x" prefix if present
        if (strncmp(str, "0x", 2) == 0) {
            str += 2;
            str_size -= 2;
        }

        // Determine UUID format based on string length
        size_t len = strlen(str);
        if (len <= 4) { // 16-bit UUID
            uuid.len = ESP_UUID_LEN_16;
            sscanf(str, "%04x", &uuid.uuid.uuid16);
        }
        else if (len <= 8) { // 32-bit UUID
            uuid.len = ESP_UUID_LEN_32;
            sscanf(str, "%08x", &uuid.uuid.uuid32);
        }
        else if (len == 36) { // 128-bit UUID (8-4-4-4-12 format with hyphens)
            uuid.len = ESP_UUID_LEN_128;
            
            // Remove hyphens and convert to bytes
            char clean_uuid[33];
            int clean_idx = 0;
            
            for (size_t i = 0; i < len; i++) {
                if (str[i] != '-') {
                    clean_uuid[clean_idx++] = str[i];
                }
            }
            clean_uuid[32] = '\0';

            // Convert hex string to bytes (reverse order for BLE)
            for (int i = 0; i < 16; i++) {
                char hex[3] = {clean_uuid[30-i*2], clean_uuid[31-i*2], '\0'};
                sscanf(hex, "%02hhx", &uuid.uuid.uuid128[i]);
            }
        }

        return uuid;
    }

    void BT::gap_event_handler(esp_gap_ble_cb_event_t event, esp_ble_gap_cb_param_t *param) {

        if( gapHandler && gapHandler(event, param) ){
            return ;
        }

        switch(event){
            // event queue 可能会丢失或不及时
            case ESP_GAP_SEARCH_INQ_CMPL_EVT: {
                BT::bScanning = false ;
                break;
            }
            // 广播
            case ESP_GAP_BLE_ADV_DATA_SET_COMPLETE_EVT:
                // printf("start adv\n");
                break;
            case ESP_GAP_BLE_ADV_START_COMPLETE_EVT:
                // printf("start completed: %d\n",param->adv_start_cmpl.status);
                // if (param->adv_start_cmpl.status != ESP_BT_STATUS_SUCCESS) {
                //     printf("Advertising start failed\n");
                // }
                break;
        }
        if(BT::singleton){
            bt_event event_msg = {
                .event = event ,
                .gatt_if = 0,
                .gap = *param ,
            } ;
            if( !BT::singleton->emitNativeEvent(&event_msg) ){
                printf("bt queue full (ev:%d)\n", event) ;
            }
        }
    }

    void BT::onNativeEvent(JSContext *ctx, void * param) {
        bt_event * msg = (bt_event*) param ;
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
                            sprintf((char *)addr, "%02X:%02X:%02X:%02X:%02X:%02X", 
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
                            JS_SetPropertyStr(ctx, obj, "addr", JS_NewString(ctx, (const char *)addr)) ;
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
            onNativeCentralEvent(ctx, msg) ;
        }
        else if(msg->event<300) {
            onNativePeripheralEvent(ctx, msg) ;
        }
    }

    bool BT::init() {

        static bool inited = false ;
        if(inited) {
            return true ;
        }
        inited = true ;

        esp_err_t ret;
        esp_bt_controller_config_t bt_cfg = BT_CONTROLLER_INIT_CONFIG_DEFAULT();

        // 初始化BLE控制器
        ret = esp_bt_controller_init(&bt_cfg);
        if (ret) {
            printf("Bluetooth controller initialize failed: %s\n", esp_err_to_name(ret));
            return false ;
        }

        ret = esp_bt_controller_enable(ESP_BT_MODE_BLE);
        if (ret) {
            printf("Bluetooth controller enable failed: %s\n", esp_err_to_name(ret));
            return false ;
        }
        
        // Initialize Bluedroid
        ret = esp_bluedroid_init();
        if (ret) {
            printf("Failed to initialize Bluedroid: %s\n", esp_err_to_name(ret));
            return false ;
        }

        // Enable Bluedroid
        ret = esp_bluedroid_enable();
        if (ret) {
            printf("Failed to enable Bluedroid: %s\n", esp_err_to_name(ret));
            return false ;
        }

        // initCentral() ;
        // initPeripheral() ;
        
        // 注册GAP回调函数
        ret = esp_ble_gap_register_callback(gap_event_handler);
        if (ret) {
            printf("GAP callback registration failed\n");\
            return false ;
        }
        
        return true ;
    }

    
    void BT::setGapHandler(gap_handler_t handler) {
        gapHandler = handler ;
    }
    void BT::setGattcHandler(gattc_handler_t handler) {
        gattcHandler = handler ;
    }
    void BT::setGattsHandler(gatts_handler_t handler) {
        gattsHandler = handler ;
    }

    JSValue BT::setPower(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        CHECK_ARGC(1)
        ARGV_TO_UINT32(0, level)
        if(level<ESP_PWR_LVL_N24 || level>ESP_PWR_LVL_P21) {
            JSTHROW("Invalid power level")
        }
        esp_ble_power_type_t type = ESP_BLE_PWR_TYPE_DEFAULT ;
        if(argc>1) {
            if( JS_ToUint32(ctx, (uint32_t*)&type, argv[1])!=0 ) {
                JSTHROW("Invalid power type")
            }
            if(type<0 || type>=ESP_BLE_PWR_TYPE_NUM) {
                JSTHROW("Invalid power type")
            }
        }
        esp_err_t ret = esp_ble_tx_power_set(type, (esp_power_level_t)level) ;
        return JS_NewInt32(ctx, ret) ;
    }
    JSValue BT::power(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        return JS_NewInt32(ctx, esp_ble_tx_power_get(ESP_BLE_PWR_TYPE_DEFAULT)) ;
    }
}

#endif