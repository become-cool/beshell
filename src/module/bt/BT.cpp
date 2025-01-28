#if CONFIG_BT_BLUEDROID_ENABLED

#include "BT.hpp"
#include "esp_bt.h"
#include "esp_bt_defs.h"
#include "esp_bt_main.h"
#include "esp_gatt_defs.h"
#include "esp_gattc_api.h"
#include "esp_gap_ble_api.h"
#include "esp_log.h"
#include "esp_bt_device.h"
#include "qjs_utils.h"

static const char *TAG = "bt";


// 全局变量，用于记录连接相关信息
static esp_gatt_if_t      gattc_if_global     = 0;
static uint16_t           conn_id_global      = 0;
static bool               get_service         = false;
static esp_gatt_srvc_id_t remote_filter_srvc_id;
static uint16_t           char_handle         = 0; // 发现特征后会存储

// 写入数据示例
static uint8_t write_data[5] = {0x01, 0x02, 0x03, 0x04, 0x05};


#define SERVICE_UUID          "a6ed0401-d344-460a-8075-b9e8ec90d71b"
static const uint8_t remote_service_uuid[16] = {0xa6, 0xed, 0x04, 0x01, 0xd3, 0x44, 0x46, 0x0a, 0x80, 0x75, 0xb9, 0xe8, 0xec, 0x90, 0xd7, 0x1b};

#define CHARACTERISTIC_UUID   "a6ed0403-d344-460a-8075-b9e8ec90d71b"
static const uint8_t remote_char_uuid[16] = {0xa6, 0xed, 0x04, 0x03, 0xd3, 0x44, 0x46, 0x0a, 0x80, 0x75, 0xb9, 0xe8, 0xec, 0x90, 0xd7, 0x1b};


namespace be{
    const char * const BT::name = "bt" ;
    BT::BT(JSContext * ctx, const char * name)
        : EventModule(ctx, name, 0)
    {
        exportFunction("init",init,0) ;
        exportFunction("scan",scan,0) ;
        exportFunction("setMTU",setMTU,0) ;
        exportFunction("connect",connect,0) ;
        exportFunction("disconnect",disconnect,0) ;
        exportFunction("search",search,0) ;
        exportFunction("read",read,0) ;
        exportFunction("write",write,0) ;
        exportFunction("subscribe",subscribe,0) ;
        
        // enableNativeEvent(ctx, sizeof(struct ble_gap_event), 10) ;
    }

    void BT::use(be::BeShell * beshell) {
        beshell->use<BT>() ;
    }

    /**
     * @brief 将 esp_bt_uuid_t 转换为字符串形式
     *
     * @param[in]  uuid      指向 esp_bt_uuid_t 结构体的指针
     * @param[out] str       用于输出字符串的缓冲区
     * @param[in]  str_size  缓冲区大小
     * @return     返回传入的 str 指针，以方便链式调用或直接使用
     *
     * @note       - 对于 16 位和 32 位，直接输出为 0xXXXX / 0xXXXXXXXX 格式；
     *             - 对于 128 位，输出为 XXXXXXXX-XXXX-XXXX-XXXX-XXXXXXXXXXXX 的标准 UUID 格式。
     *             - 如果需要遵循 BLE/GATT 的字节序，请自行调整 uuid128 的输出顺序。
     */
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
        dn(event)
        switch (event) {
            case ESP_GAP_BLE_SCAN_PARAM_SET_COMPLETE_EVT:
            dd
                // if (param->scan_param_cmpl.status == ESP_BT_STATUS_SUCCESS) {
                //     printf("Scan parameters set, starting scan...\n");
                //     esp_ble_gap_start_scanning(10);  // Scan for 10 seconds
                // } else {
                //     printf("Failed to set scan parameters\n");
                // }
                break;

            case ESP_GAP_BLE_SCAN_RESULT_EVT:
            dd
                if (param->scan_rst.search_evt == ESP_GAP_SEARCH_INQ_RES_EVT) {
                    // Get the device name
                    // char device_name[64];
                    // get_device_name(param, device_name, sizeof(device_name));

                    // Log the device name and RSSI
                    if(param->scan_rst.bda[0]==0xd1 && param->scan_rst.bda[5]==0xa0) {
                        printf("Device found: %02x:%02x:%02x:%02x:%02x:%02x, RSSI %d\n", 
                            param->scan_rst.bda[0],param->scan_rst.bda[1],param->scan_rst.bda[2],param->scan_rst.bda[3],param->scan_rst.bda[4],param->scan_rst.bda[5],
                            param->scan_rst.rssi);
                        
                        print_block(param->scan_rst.ble_adv,31,16) ;

                        esp_ble_gap_stop_scanning() ;

                        esp_err_t err = esp_ble_gattc_open(
                            gattc_if_global,  // gattc_if，如果只有一个 GATT client，通常是 app_id=0 对应的 gattc_if
                            param->scan_rst.bda,   // 目标设备的 MAC 地址
                            BLE_ADDR_TYPE_RANDOM,        // 若设备是 Public Address，则用此类型
                            true // 是否开启重连特性（若连接断开，是否自动重连）
                        );
                        if (err != ESP_OK) {
                            ESP_LOGE(TAG, "esp_ble_gattc_open failed, err = %d", err);
                        }
                    }
                }
                break;

            case ESP_GAP_BLE_SCAN_STOP_COMPLETE_EVT:
                printf("Scan complete\n");
                break;

            default:
                break;
        }
    }

    // 处理GATT事件
    static void gattc_event_handler(esp_gattc_cb_event_t event,
                                    esp_gatt_if_t gattc_if,
                                    esp_ble_gattc_cb_param_t *param)
    {
        dn(event)
        switch (event) {
        case ESP_GATTC_REG_EVT:
            gattc_if_global = gattc_if ;
            ESP_LOGI(TAG, "ESP_GATTC_REG_EVT");
            // 开始扫描
            // esp_ble_gap_set_scan_params(&ble_scan_params);
            break;

        case ESP_GATTC_CONNECT_EVT: {
            ESP_LOGI(TAG, "ESP_GATTC_CONNECT_EVT, conn_id %d, if %d", param->connect.conn_id, gattc_if);
            // gattc_if_global = gattc_if;
            conn_id_global  = param->connect.conn_id;
            // 保存连接的蓝牙地址等信息，以便后续使用
            break;
        }

        case ESP_GATTC_OPEN_EVT:
            dn(param->open.conn_id)
            if (param->open.status == ESP_GATT_OK) {
                ESP_LOGI(TAG, "ESP_GATTC_OPEN_EVT conn_id %d, status %d", param->open.conn_id, param->open.status);
            } else {
                ESP_LOGE(TAG, "Open failed, status %d", param->open.status);
            }
            break;

        case ESP_GATTC_SEARCH_RES_EVT: {
            // 搜索到服务
            // esp_gatt_srvc_id_t *srvc_id = &param->search_res.srvc_id;
            printf("ESP_GATTC_SEARCH_RES_EVT\n") ;
            
            uint16_t count  = 0;
            uint16_t offset = 0;
            esp_gatt_status_t ret_status = esp_ble_gattc_get_attr_count(gattc_if,
                                    param->search_res.conn_id,
                                    ESP_GATT_DB_CHARACTERISTIC,
                                    param->search_res.start_handle,
                                    param->search_res.end_handle,
                                    NULL ,
                                    &count);
            if (ret_status != ESP_GATT_OK) {
                ESP_LOGE(TAG, "esp_ble_gattc_get_attr_count error, %d", __LINE__);
                break;
            }
                dn(count)
            if (count <= 0) {
                break ;
            }

            char_elem_result = (esp_gattc_char_elem_t *)malloc(sizeof(esp_gattc_char_elem_t) * count);
            if (!char_elem_result) {
                ESP_LOGE(TAG, "gattc no mem");
                break;
            } else {
                memset(char_elem_result, 0xff, sizeof(esp_gattc_char_elem_t) * count);
                ret_status = esp_ble_gattc_get_all_char(gattc_if,
                                    param->search_res.conn_id,
                                    param->search_res.start_handle,
                                    param->search_res.end_handle,
                                    char_elem_result,
                                    &count,
                                    offset);
                if (ret_status != ESP_GATT_OK) {
                    ESP_LOGE(TAG, "esp_ble_gattc_get_all_char error, %d", __LINE__);
                    free(char_elem_result);
                    char_elem_result = NULL;
                    break;
                }
                dn(count)
                if (count > 0) {

                    for (int i = 0; i < count; i ++) {
                        char uuid_str[37] ;
                        bt_uuid_to_string(&char_elem_result[i].uuid,uuid_str,sizeof(uuid_str)) ;

                        printf("%s, handle:%d, properties:%d\n",uuid_str,char_elem_result[i].char_handle, char_elem_result[i].properties) ;
                        
                        /*  Every service have only one char in our 'ESP_GATTS_DEMO' demo, so we used first 'char_elem_result' */
                        if (count > 0 && (char_elem_result[0].properties & ESP_GATT_CHAR_PROP_BIT_NOTIFY)){
                            // esp_ble_gattc_register_for_notify (gattc_if, gl_profile_tab[PROFILE_A_APP_ID].remote_bda, char_elem_result[0].char_handle);
                        }
                    }
                }

                free(char_elem_result);
                char_elem_result = NULL;
            }


            break;
        }

        case ESP_GATTC_SEARCH_CMPL_EVT: {
            ESP_LOGI(TAG, "ESP_GATTC_SEARCH_CMPL_EVT");
            if (get_service) {
                // 如果找到目标服务，则发起特征发现
                // esp_ble_gattc_get_char_by_uuid
                // esp_ble_gattc_get_characteristic(gattc_if,
                //                                 param->search_cmpl.conn_id,
                //                                 &remote_filter_srvc_id,
                //                                 NULL);
            }
            break;
        }

        case ESP_GATTC_WRITE_CHAR_EVT: {
            if (param->write.status == ESP_GATT_OK) {
                ESP_LOGI(TAG, "写特征成功, handle = %d", param->write.handle);
            } else {
                ESP_LOGE(TAG, "写特征失败, error status = %d", param->write.status);
            }
            break;
        }

        default:
            break;
        }
    }


    JSValue BT::init(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
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
        dd
        
        // 注册GAP回调函数
        ret = esp_ble_gap_register_callback(gap_event_handler);
        if (ret) {
            JSTHROW("GAP callback registration failed");
        }

        // esp_ble_auth_req_t auth_req = ESP_LE_AUTH_REQ_SC_MITM_BOND;
        // esp_ble_gap_set_security_param(ESP_BLE_SM_AUTHEN_REQ_MODE, &auth_req, sizeof(auth_req));

        return JS_UNDEFINED ;
    }
    
    JSValue BT::scan(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
#if (BLE_42_FEATURE_SUPPORT == TRUE)
        esp_ble_scan_params_t ble_scan_params = {
            .scan_type = BLE_SCAN_TYPE_ACTIVE, //BLE_SCAN_TYPE_PASSIVE,
            .own_addr_type = BLE_ADDR_TYPE_RANDOM, //BLE_ADDR_TYPE_PUBLIC, //BLE_ADDR_TYPE_RANDOM,
            .scan_filter_policy = BLE_SCAN_FILTER_ALLOW_ALL,
            .scan_interval = 0x50,
            .scan_window = 0x40,
            .scan_duplicate = BLE_SCAN_DUPLICATE_DISABLE,
        };

        // 开始扫描BLE设备
        esp_err_t ret = esp_ble_gap_set_scan_params(&ble_scan_params);
        if (ret != ESP_OK) {
            JSTHROW("Failed to set scan params: %s", esp_err_to_name(ret));
        }

        ret = esp_ble_gap_start_scanning(5) ;
        if (ret != ESP_OK) {
            JSTHROW("Failed to start scanning: %s", esp_err_to_name(ret));
        }
#endif
        return JS_UNDEFINED ;
    }

    JSValue BT::connect(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {

        if(gattc_if_global<=0) {
            JSTHROW("call bt.init() first")
        }

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
    JSValue BT::setMTU(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        return JS_UNDEFINED ;
    }
    JSValue BT::disconnect(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        return JS_UNDEFINED ;
    }
    JSValue BT::search(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        CHECK_ARGC(1)
        ARGV_TO_UINT16(0, conn_id)
        esp_ble_gattc_search_service(gattc_if_global, conn_id, NULL);
        return JS_UNDEFINED ;
    }
    JSValue BT::read(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        return JS_UNDEFINED ;
    }
    JSValue BT::write(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        CHECK_ARGC(3)
        ARGV_TO_UINT16(0, conn_id)
        ARGV_TO_UINT16(1, handle)
        size_t size ;
        char * buff = (char *)JS_GetArrayBuffer(ctx, &size, argv[2]) ;
        if(!buff || !size) {
            JSTHROW("argv is not a valid ArrayBuffer")
        }

        esp_err_t err = esp_ble_gattc_write_char( gattc_if_global, conn_id, handle, size, buff, ESP_GATT_WRITE_TYPE_NO_RSP, ESP_GATT_AUTH_REQ_NONE);
        if (err != ESP_OK) {
            JSTHROW("esp_ble_gattc_write_char() failed, error = %x")
        }
        
        return JS_UNDEFINED ;
    }
    JSValue BT::subscribe(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        return JS_UNDEFINED ;
    }

    void BT::onNativeEvent(JSContext *ctx, void * param) {
// esp_ble_gattc_search_service
    }
}

#endif