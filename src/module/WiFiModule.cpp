#include "WiFiModule.hpp"

#include <lwip/apps/sntp.h>
#include <esp_err.h>
#include <esp_event_loop.h>
#include <esp_log.h>
#include <esp_wifi.h>
#include <lwip/dns.h>
#include <lwip/sockets.h>
#include <esp_netif.h>

using namespace be ;

#define DEFAULT_SCAN_LIST_SIZE 16

#define MAX_SSID_CHARLEN        31
#define MAX_PASSWORD_CHARLEN    63



static JSValue __event_handle = JS_UNDEFINED ;
static JSContext * __event_handle_ctx = NULL ;

static esp_netif_t * netif_ap = NULL ;
esp_netif_t * get_netif_ap() {
    return netif_ap ;
}

static esp_netif_t * netif_sta = NULL ;
esp_netif_t * get_netif_sta() {
    return netif_sta ;
}

static esp_event_handler_instance_t instance_any_id;
static esp_event_handler_instance_t instance_got_ip;

static bool wifi_inited = false ;

#define CHECK_WIFI_INITED                                       \
    if(!wifi_inited) {                                          \
        JSTHROW("wifi not init, please call WiFi.start() first")\
    }

static bool _started = false ;
static bool _sta_started = false ;
static bool _sta_connected = false ;
static bool _ap_started = false ;
static bool _scanning = false ;

#define WIFI_EVENT_STA_CONNECTING 101
#define WIFI_EVENT_STA_DISCONNECTING 102

// typedef enum {
//     WIFI_EVENT_WIFI_READY = 0,           /**< ESP32 WiFi ready */
//     WIFI_EVENT_SCAN_DONE = 1,                /**< ESP32 finish scanning AP */
//     WIFI_EVENT_STA_START = 2,                /**< ESP32 station start */
//     WIFI_EVENT_STA_STOP = 3,                 /**< ESP32 station stop */
//     WIFI_EVENT_STA_CONNECTED = 4,            /**< ESP32 station connected to AP */
//     WIFI_EVENT_STA_DISCONNECTED = 5,         /**< ESP32 station disconnected from AP */
//     WIFI_EVENT_STA_AUTHMODE_CHANGE=6,      /**< the auth mode of AP connected by ESP32 station changed */

//     WIFI_EVENT_STA_WPS_ER_SUCCESS=7,       /**< ESP32 station wps succeeds in enrollee mode */
//     WIFI_EVENT_STA_WPS_ER_FAILED=8,        /**< ESP32 station wps fails in enrollee mode */
//     WIFI_EVENT_STA_WPS_ER_TIMEOUT=9,       /**< ESP32 station wps timeout in enrollee mode */
//     WIFI_EVENT_STA_WPS_ER_PIN=10,           /**< ESP32 station wps pin code in enrollee mode */
//     WIFI_EVENT_STA_WPS_ER_PBC_OVERLAP=11,   /**< ESP32 station wps overlap in enrollee mode */

//     WIFI_EVENT_AP_START = 12,                 /**< ESP32 soft-AP start */
//     WIFI_EVENT_AP_STOP = 13,                  /**< ESP32 soft-AP stop */
//     WIFI_EVENT_AP_STACONNECTED = 14,          /**< a station connected to ESP32 soft-AP */
//     WIFI_EVENT_AP_STADISCONNECTED = 15,       /**< a station disconnected from ESP32 soft-AP */
//     WIFI_EVENT_AP_PROBEREQRECVED=16,        /**< Receive probe request packet in soft-AP interface */

//     WIFI_EVENT_FTM_REPORT=17,               /**< Receive report of FTM procedure */

//     /* Add next events after this only */
//     WIFI_EVENT_STA_BSS_RSSI_LOW=18,         /**< AP's RSSI crossed configured threshold */
//     WIFI_EVENT_ACTION_TX_STATUS=19,         /**< Status indication of Action Tx operation */
//     WIFI_EVENT_ROC_DONE=20,                 /**< Remain-on-Channel operation complete */

//     WIFI_EVENT_STA_BEACON_TIMEOUT=21,       /**< ESP32 station beacon timeout */

//     WIFI_EVENT_MAX,                      /**< Invalid WiFi event ID */
// } wifi_event_t;

/*
错误原因
WIFI_REASON_UNSPECIFIED = 1
WIFI_REASON_AUTH_EXPIRE = 2
WIFI_REASON_AUTH_LEAVE = 3
WIFI_REASON_ASSOC_EXPIRE = 4
WIFI_REASON_ASSOC_TOOMANY = 5
WIFI_REASON_NOT_AUTHED = 6
WIFI_REASON_NOT_ASSOCED = 7
WIFI_REASON_ASSOC_LEAVE = 8
WIFI_REASON_ASSOC_NOT_AUTHED = 9
WIFI_REASON_DISASSOC_PWRCAP_BAD = 10
WIFI_REASON_DISASSOC_SUPCHAN_BAD = 11
WIFI_REASON_BSS_TRANSITION_DISASSOC = 12
WIFI_REASON_IE_INVALID = 13
WIFI_REASON_MIC_FAILURE = 14
WIFI_REASON_4WAY_HANDSHAKE_TIMEOUT = 15
WIFI_REASON_GROUP_KEY_UPDATE_TIMEOUT = 16
WIFI_REASON_IE_IN_4WAY_DIFFERS = 17
WIFI_REASON_GROUP_CIPHER_INVALID = 18
WIFI_REASON_PAIRWISE_CIPHER_INVALID = 19
WIFI_REASON_AKMP_INVALID = 20
WIFI_REASON_UNSUPP_RSN_IE_VERSION = 21
WIFI_REASON_INVALID_RSN_IE_CAP = 22
WIFI_REASON_802_1X_AUTH_FAILED = 23
WIFI_REASON_CIPHER_SUITE_REJECTED = 24
WIFI_REASON_INVALID_PMKID = 53
WIFI_REASON_BEACON_TIMEOUT = 200
WIFI_REASON_NO_AP_FOUND = 201
WIFI_REASON_AUTH_FAIL = 202
WIFI_REASON_ASSOC_FAIL = 203
WIFI_REASON_HANDSHAKE_TIMEOUT = 204
WIFI_REASON_CONNECTION_FAIL = 205
WIFI_REASON_AP_TSF_RESET = 206
WIFI_REASON_ROAMING = 207
*/
#define REASON(event_data) ((wifi_event_sta_disconnected_t*) event_data)->reason
static void esp32_wifi_eventHandler(void* arg, esp_event_base_t event_base, int32_t event_id, void* event_data) {
	// printf("event_base: %d, event_id: %d\n", (int32_t)event_base, event_id);
    // dn2( ((int32_t)event_base), event_id)
    
    int eventType = -1 ;
    if(event_base==WIFI_EVENT) {
        eventType = 1 ;
        
        if(event_id == WIFI_EVENT_WIFI_READY) {
            printf("WIFI_EVENT_WIFI_READY\n") ;
        }
        else if(event_id == WIFI_EVENT_SCAN_DONE) {
            // printf("WIFI_EVENT_SCAN_DONE\n") ;
            _scanning = false ;
        }
        else if(event_id == WIFI_EVENT_STA_START) {
            _sta_started = true ;
        }
        else if(event_id == WIFI_EVENT_STA_STOP) {
            _sta_started = false ;
        }
        else if(event_id == WIFI_EVENT_STA_CONNECTED) {
            _sta_connected = true ;
        }
        else if(event_id == WIFI_EVENT_STA_DISCONNECTED) {
            _sta_connected = false ;
        }
        else if(event_id == WIFI_EVENT_AP_START) {
            _ap_started = true ;
        }
        else if(event_id == WIFI_EVENT_AP_STOP) {
            _ap_started = false ;
        }
    }
    else if(event_base==IP_EVENT) {
        eventType = 2 ;
    }
    else {
        eventType = (int)event_base ;
        return ;
    }

    if( arg && __event_handle_ctx!=NULL && JS_IsFunction(__event_handle_ctx, __event_handle) ) {
        MAKE_ARGV3(argv, JS_NewInt32(__event_handle_ctx, eventType), JS_NewInt32(__event_handle_ctx, event_id), JS_UNDEFINED)
        // dis reason
        if(event_base==WIFI_EVENT && event_id == WIFI_EVENT_STA_DISCONNECTED) {
            argv[2] = JS_NewInt32(__event_handle_ctx, REASON(event_data)) ;
        }

        ((BeShell*)arg)->engine->timer.setTimer(__event_handle_ctx, __event_handle, 0, false, JS_UNDEFINED, 3, argv) ;
        // eventloop_push_with_argv(__event_handle_ctx, __event_handle, JS_UNDEFINED, 3, argv) ;
    }
}

namespace be {
    
    WiFiModule::WiFiModule(JSContext * ctx, const char * name)
        : NativeModule(ctx, name, 0)
    {
        // exportClass<ClassName>() ;
        exportFunction("start",start,0) ;
        exportFunction("stop",stop,0) ;

        exportFunction("setPS",setPS,0) ;
        exportFunction("setMode",setMode,0) ;
        exportFunction("mode",mode,0) ;
        exportFunction("setAPConfig",setAPConfig,0) ;
        exportFunction("setStaConfig",setStaConfig,0) ;
        exportFunction("config",config,0) ;
        exportFunction("connect",connect,0) ;
        exportFunction("disconnect",disconnect,0) ;
        exportFunction("getIpInfo",getIpInfo,0) ;
        exportFunction("setHostname",setHostname,0) ;
        exportFunction("allSta",allSta,0) ;
        exportFunction("registerEventHandle",registerEventHandle,0) ;
        exportFunction("scanStart",scanStart,0) ;
        exportFunction("scanStop",scanStop,0) ;
        exportFunction("isScanning",isScanning,0) ;
        exportFunction("scanRecords",scanRecords,0) ;
        exportFunction("staStarted",staStarted,0) ;
        exportFunction("apStarted",apStarted,0) ;
    }

    void WiFiModule::use(be::BeShell & beshell) {
        beshell.addModule<WiFiModule>("wifi") ;
        
        ESP_ERROR_CHECK(esp_event_handler_instance_register(WIFI_EVENT, ESP_EVENT_ANY_ID, &esp32_wifi_eventHandler, (void *)&beshell, &instance_any_id));
        ESP_ERROR_CHECK(esp_event_handler_instance_register(IP_EVENT, IP_EVENT_STA_GOT_IP, &esp32_wifi_eventHandler, (void *)&beshell, &instance_got_ip));

        ESP_ERROR_CHECK(esp_netif_init());

        netif_sta = esp_netif_create_default_wifi_sta();
        netif_ap = esp_netif_create_default_wifi_ap();

        wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
        ESP_ERROR_CHECK(esp_wifi_init(&cfg));

        wifi_inited = true ;
    }

    JSValue WiFiModule::start(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        CHECK_WIFI_INITED
        esp_err_t err = esp_wifi_start() ;
        _started = err == ESP_OK ;
        return JS_NewInt32(ctx, err);
    }
    JSValue WiFiModule::stop(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        CHECK_WIFI_INITED
        esp_err_t err = esp_wifi_stop() ;
        _started = ! (err == ESP_OK) ;
        return JS_NewInt32(ctx, err);
    }
    
    /**
     * 设置 wifi 的节能模式 (PowerSafe)
     * 
     * 参数 mode :
     * * 0 关闭节能模式
     * * 1 最小
     * * 2 最大
     * 
     * @beapi beapi.wifi.setPS
     * @param mode:0|1|2
     * @return number
     */
    JSValue WiFiModule::setPS(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        CHECK_WIFI_INITED
        CHECK_ARGC(1)
        ARGV_TO_UINT32(0, ps_mode)
        return JS_NewInt32(ctx, esp_wifi_set_ps((wifi_ps_type_t)ps_mode)) ;
    }
    /**
     * 设置 wifi 的工作模式
     * 
     * 参数 mode :
     * * 0 未启动
     * * 1 STA
     * * 2 AP
     * * 3 STA + AP
     * 
     * 返回 0 表示 api 调用成功, 返回非 0 表示错误代码
     * 
     * @beapi beapi.wifi.setMode
     * @param mode:0|1|2|3
     * @return number
     */
    JSValue WiFiModule::setMode(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        CHECK_WIFI_INITED
        ARGV_TO_INT8(0, mode)
        return JS_NewInt32(ctx, esp_wifi_set_mode((wifi_mode_t)mode));
    }

    /**
     * 返回设置 wifi 的工作模式
     * 
     * * 0 未启动
     * * 1 STA
     * * 2 AP
     * * 3 STA + AP
     * 
     * @beapi beapi.wifi.getMode
     * @return number
     */
    JSValue WiFiModule::mode(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        CHECK_WIFI_INITED
        wifi_mode_t mode_ori;
        esp_wifi_get_mode(&mode_ori) ;
        return JS_NewInt32(ctx, mode_ori);
    }


    #define SET_MEMBER_STRING(mode, name, max)                          \
        size_t name##_len = 0 ;                                         \
        {                                                               \
            JSValue val = JS_GetPropertyStr(ctx, argv[0], #name) ;      \
            if( !JS_IsUndefined(val) ) {                                \
                const char * str = (char *)JS_ToCStringLen(ctx, &name##_len, val) ;  \
                if(name##_len>max) {                                    \
                    name##_len = max ;                                  \
                }                                                       \
                strncpy((char *)wifi_config.mode.name, str, name##_len) ;            \
                JS_FreeValue(ctx,val) ;                                 \
                JS_FreeCString(ctx,str) ;                               \
            }                                                           \
        }

    #define SET_MEMBER_INT_(mode, name, jspropname)                     \
        {                                                               \
            JSValue val = JS_GetPropertyStr(ctx, argv[0], #jspropname) ;\
            if( !JS_IsUndefined(val) ) {                                \
                JS_ToUint32(ctx, (uint32_t*)&wifi_config.mode.name, val) ;\
            }                                                           \
            JS_FreeValue(ctx,val) ;                                     \
        }
    #define SET_MEMBER_INT(mode, name)                                  \
        SET_MEMBER_INT_(mode, name, name)
    #define SET_MEMBER_BOOL(mode, name)                                 \
        {                                                               \
            JSValue val = JS_GetPropertyStr(ctx, argv[0], #name) ;      \
            if( !JS_IsUndefined(val) ) {                                \
                wifi_config.mode.name = JS_ToBool(ctx, val) ;           \
            }                                                           \
            JS_FreeValue(ctx,val) ;                                     \
        }

    // /** @brief STA configuration settings for the ESP32 */
    // typedef struct {
    //     uint8_t ssid[32];      /**< SSID of target AP. */
    //     uint8_t password[64];  /**< Password of target AP. */
    //     wifi_scan_method_t scan_method;    /**< do all channel scan or fast scan */
    //     bool bssid_set;        /**< whether set MAC address of target AP or not. Generally, station_config.bssid_set needs to be 0; and it needs to be 1 only when users need to check the MAC address of the AP.*/
    //     uint8_t bssid[6];     /**< MAC address of target AP*/
    //     uint8_t channel;       /**< channel of target AP. Set to 1~13 to scan starting from the specified channel before connecting to AP. If the channel of AP is unknown, set it to 0.*/
    //     uint16_t listen_interval;   /**< Listen interval for ESP32 station to receive beacon when WIFI_PS_MAX_MODEM is set. Units: AP beacon intervals. Defaults to 3 if set to 0. */
    //     wifi_sort_method_t sort_method;    /**< sort the connect AP in the list by rssi or security mode */
    //     wifi_scan_threshold_t  threshold;     /**< When sort_method is set, only APs which have an auth mode that is more secure than the selected auth mode and a signal stronger than the minimum RSSI will be used. */
    //     wifi_pmf_config_t pmf_cfg;    /**< Configuration for Protected Management Frame. Will be advertized in RSN Capabilities in RSN IE. */
    //     uint32_t rm_enabled:1;        /**< Whether Radio Measurements are enabled for the connection */
    //     uint32_t btm_enabled:1;       /**< Whether BSS Transition Management is enabled for the connection */
    //     uint32_t reserved:30;         /**< Reserved for future feature set */
    // } wifi_sta_config_t;
    // typedef enum {
    //     WIFI_AUTH_OPEN = 0,         /**< authenticate mode : open */
    //     WIFI_AUTH_WEP = 1,              /**< authenticate mode : WEP */
    //     WIFI_AUTH_WPA_PSK = 2,          /**< authenticate mode : WPA_PSK */
    //     WIFI_AUTH_WPA2_PSK = 3,         /**< authenticate mode : WPA2_PSK */
    //     WIFI_AUTH_WPA_WPA2_PSK = 4,     /**< authenticate mode : WPA_WPA2_PSK */
    //     WIFI_AUTH_WPA2_ENTERPRISE = 5,  /**< authenticate mode : WPA2_ENTERPRISE */
    //     WIFI_AUTH_WPA3_PSK = 6,         /**< authenticate mode : WPA3_PSK */
    //     WIFI_AUTH_WPA2_WPA3_PSK = 7,    /**< authenticate mode : WPA2_WPA3_PSK */
    //     WIFI_AUTH_WAPI_PSK = 8,         /**< authenticate mode : WAPI_PSK */
    //     WIFI_AUTH_MAX
    // } wifi_auth_mode_t;

    JSValue WiFiModule::setAPConfig(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        JSTHROW("not implements")
        return JS_UNDEFINED ;
    }

    
    /**
     * 设置 wifi STA 模式的参数
     * 
     * 参数 mode :
     * ```
     * {
     *     ssid: string ,
     *     password?: string ,
     *     "threshold.authmode"?: 0-8 ,
     *     scan_method?: 0|1 ,
     *     channel?: 1-13 ,
     *     listen_interval?:number = 3 ,
     *     sort_method?: 0|1 = 0 ,       // sort the connect AP in the list by rssi or security mode
     * }
     * ```
     * 
     * 如果 `password` 为空, 则 `threshold.authmode` 自动设置为 `WIFI_AUTH_OPEN`
     * 
     * 返回 0 表示 api 调用成功, 返回非 0 表示错误代码
     * 
     * @beapi beapi.wifi.setStaConfig
     * @param config:object
     * @return number
     */
    JSValue WiFiModule::setStaConfig(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        CHECK_WIFI_INITED
        CHECK_ARGC(1)
        if(!JS_IsObject(argv[0])) {
            JSTHROW("missing options arg")
        }

        wifi_config_t wifi_config ;
        memset(&wifi_config, 0, sizeof(wifi_config_t)) ;
        wifi_config.sta.threshold.authmode = WIFI_AUTH_WPA2_PSK ;
        wifi_config.sta.pmf_cfg.capable = true ;
        wifi_config.sta.pmf_cfg.required = false ;

        SET_MEMBER_STRING(sta, ssid, MAX_SSID_CHARLEN)
        SET_MEMBER_STRING(sta, password, MAX_PASSWORD_CHARLEN)
        SET_MEMBER_INT_(sta, threshold.authmode, authmode)
        if(password_len==0) {
            wifi_config.sta.threshold.authmode = WIFI_AUTH_OPEN ;
        }

        SET_MEMBER_INT(sta, scan_method)
        SET_MEMBER_INT(sta, channel)
        SET_MEMBER_INT(sta, listen_interval)
        SET_MEMBER_INT(sta, sort_method)
        // SET_MEMBER_INT(sta, rm_enabled)
        // SET_MEMBER_INT(sta, btm_enabled)

        // dn(wifi_config.sta.threshold.authmode)
        // ds(wifi_config.sta.ssid)
        // ds(wifi_config.sta.password)

        return JS_NewInt32(ctx, esp_wifi_set_config(WIFI_IF_STA, &wifi_config) );
    }
    JSValue WiFiModule::config(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        JSTHROW("not implements")
        return JS_UNDEFINED ;
    }
    JSValue WiFiModule::connect(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        JSTHROW("not implements")
        return JS_UNDEFINED ;
    }
    JSValue WiFiModule::disconnect(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        JSTHROW("not implements")
        return JS_UNDEFINED ;
    }
    JSValue WiFiModule::getIpInfo(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        JSTHROW("not implements")
        return JS_UNDEFINED ;
    }
    JSValue WiFiModule::setHostname(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        JSTHROW("not implements")
        return JS_UNDEFINED ;
    }
    JSValue WiFiModule::allSta(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        JSTHROW("not implements")
        return JS_UNDEFINED ;
    }
    JSValue WiFiModule::registerEventHandle(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        JSTHROW("not implements")
        return JS_UNDEFINED ;
    }

    JSValue WiFiModule::scanStart(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        JSTHROW("not implements")
        return JS_UNDEFINED ;
    }
    JSValue WiFiModule::scanStop(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        JSTHROW("not implements")
        return JS_UNDEFINED ;
    }
    JSValue WiFiModule::isScanning(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        JSTHROW("not implements")
        return JS_UNDEFINED ;
    }
    JSValue WiFiModule::scanRecords(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        JSTHROW("not implements")
        return JS_UNDEFINED ;
    }

    JSValue WiFiModule::staStarted(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        JSTHROW("not implements")
        return JS_UNDEFINED ;
    }
    JSValue WiFiModule::staConnected(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        JSTHROW("not implements")
        return JS_UNDEFINED ;
    }
    JSValue WiFiModule::apStarted(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        JSTHROW("not implements")
        return JS_UNDEFINED ;
    }

}
