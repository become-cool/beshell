#include "module_wifi.h"
#include "logging.h"
#include "eventloop.h"
#include "js_main_loop.h"
#include "utils.h"

#include <lwip/apps/sntp.h>
#include <esp_err.h>
#include <esp_event_loop.h>
#include <esp_log.h>
#include <esp_wifi.h>
#include <lwip/dns.h>
#include <lwip/sockets.h>

LOG_TAG("wifi");

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

static bool _started = false ;
static bool _sta_started = false ;
static bool _sta_connected = false ;
static bool _ap_started = false ;
static bool _scanning = false ;

#define WIFI_EVENT_STA_CONNECTING 101
// #define WIFI_EVENT_STA_DISCONNECTING 102

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
        eventType = event_base ;
        return ;
    }

    if( __event_handle_ctx!=NULL && JS_IsFunction(__event_handle_ctx, __event_handle) ) {
        MAKE_ARGV3(argv, JS_NewInt32(__event_handle_ctx, eventType), JS_NewInt32(__event_handle_ctx, event_id), JS_UNDEFINED)
        // dis reason
        if(event_base==WIFI_EVENT && event_id == WIFI_EVENT_STA_DISCONNECTED) {
            argv[2] = JS_NewInt32(__event_handle_ctx, REASON(event_data)) ;
        }

        eventloop_push_with_argv(__event_handle_ctx, __event_handle, JS_UNDEFINED, 3, argv) ;
    }
}

/**
 * 返回 WiFi STA 模式是否启动
 * 
 * @beapi beapi.wifi.staStarted
 * @return bool
 */
static JSValue js_wifi_sta_started(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv){
    if(!wifi_inited)
        return JS_FALSE;
    return JS_NewBool(ctx, _sta_started) ;
}

/**
 * 返回 WiFi STA 是否已经连接
 * 
 * @beapi beapi.wifi.staConnected
 * @return bool
 */
static JSValue js_wifi_sta_connected(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv){
    if(!wifi_inited)
        return JS_FALSE;
    return JS_NewBool(ctx, _sta_connected) ;
}

/**
 * 返回 WiFi AP 模式是否启用
 * 
 * @beapi beapi.wifi.apStarted
 * @return bool
 */
static JSValue js_wifi_ap_started(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv){
    if(!wifi_inited)
        return JS_FALSE;
    return JS_NewBool(ctx, _ap_started) ;
}


/**
 * WiFi STA 断开连接
 * 
 * 返回 0 表示成功; 非 0 代表对应的错误
 * 
 * @beapi beapi.wifi.disconnect
 * @return number
 */
static JSValue js_wifi_disconnect(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    CHECK_WIFI_INITED
    return JS_NewInt32(ctx, esp_wifi_disconnect()) ;
}

/**
 * WiFi STA 连接到热点
 * 
 * ssid, password 等参数需要通过 beapi.wifi.setStaConfig() 设置
 * 
 * 返回 0 仅表示api函数调用成功; 非 0 代表对应的错误
 * 
 * 连接成功或失败回触发回调函数, 回调函数由 beapi.wifi.registerEventHandle() 设置
 * 
 * @beapi beapi.wifi.connect
 * @return number
 */
static JSValue js_wifi_connect(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    CHECK_WIFI_INITED
    if( JS_IsFunction(ctx, __event_handle) ) {
        MAKE_ARGV2(argv, JS_NewInt32(__event_handle_ctx, 1), JS_NewInt32(__event_handle_ctx, WIFI_EVENT_STA_CONNECTING))
        JS_Call(ctx, __event_handle, JS_UNDEFINED, 2, argv) ;
        free(argv) ;
    }
    return JS_NewInt32(ctx, esp_wifi_connect()) ;
}

/**
 * 返回 AP/STA 的 IP 
 * 
 * @beapi beapi.wifi.getIpInfo
 * @param type:number 1代表 sta, 2代表 ap
 * @return {ip:string,netmask:string,gw:string} 
 */
static JSValue js_wifi_get_ip_info(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    CHECK_WIFI_INITED
    CHECK_ARGC(1)
    ARGV_TO_UINT8(0,type)

    esp_netif_ip_info_t ipinfo;
    bzero(&ipinfo, sizeof(esp_netif_ip_info_t));

    if( type==WIFI_MODE_STA ) {
        esp_netif_get_ip_info(netif_sta, &ipinfo);
    }
    else if(type==WIFI_MODE_AP) {
        esp_netif_get_ip_info(netif_ap, &ipinfo);
    }
    else{
        THROW_EXCEPTION("unknow netif type: %d", type)
    }

    JSValue status = JS_NewObject(ctx) ;

    char ip[16] ;
    sprintf(ip,IPSTR, IP2STR((&ipinfo.ip))) ;
    JS_SetPropertyStr(ctx, status, "ip", JS_NewString(ctx, ip)) ;

    sprintf(ip,IPSTR, IP2STR((&ipinfo.netmask))) ;
    JS_SetPropertyStr(ctx, status, "netmask", JS_NewString(ctx, ip)) ;

    sprintf(ip,IPSTR, IP2STR((&ipinfo.gw))) ;
    JS_SetPropertyStr(ctx, status, "gw", JS_NewString(ctx, ip)) ;

    return status ;
}

/**
 * 返回设置 wifi 的在局域网中可被显示的主机名
 * 
 * @beapi beapi.wifi.setHostname
 * @param nane:string
 */
static JSValue js_wifi_set_hostname(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    CHECK_WIFI_INITED
    CHECK_ARGC(1)

    char * hostname = JS_ToCString(ctx, argv[0]) ;
    esp_netif_set_hostname(netif_sta, hostname);
    esp_netif_set_hostname(netif_ap, hostname);

    JS_FreeCString(ctx, hostname) ;

    return JS_UNDEFINED ;
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
static JSValue js_wifi_get_mode(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    CHECK_WIFI_INITED
    wifi_mode_t mode_ori;
    esp_wifi_get_mode(&mode_ori) ;
    return JS_NewInt32(ctx, mode_ori);
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
static JSValue js_wifi_set_mode(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    CHECK_WIFI_INITED
    ARGV_TO_INT8(0, mode)
    return JS_NewInt32(ctx, esp_wifi_set_mode(mode));
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
static JSValue js_wifi_set_ps(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    CHECK_WIFI_INITED
    CHECK_ARGC(1)
    ARGV_TO_UINT32(0, ps_mode)
    return JS_NewInt32(ctx, esp_wifi_set_ps(ps_mode)) ;
}





#define SET_MEMBER_STRING(mode, name, max)                          \
    size_t name##_len = 0 ;                                         \
    {                                                               \
        JSValue val = JS_GetPropertyStr(ctx, argv[0], #name) ;      \
        if( !JS_IsUndefined(val) ) {                                \
            char * str = JS_ToCStringLen(ctx, & name##_len, val) ;  \
            if(name##_len>max) {                                    \
                name##_len = max ;                                  \
                str[name##_len] = 0 ;                               \
            }                                                       \
            strcpy((char *)wifi_config.mode.name, str) ;            \
            JS_FreeValue(ctx,val) ;                                 \
            JS_FreeCString(ctx,str) ;                               \
        }                                                           \
    }

#define SET_MEMBER_INT_(mode, name, jspropname)                     \
    {                                                               \
        JSValue val = JS_GetPropertyStr(ctx, argv[0], #jspropname) ;\
        if( !JS_IsUndefined(val) ) {                                \
            JS_ToInt32(ctx, &wifi_config.mode.name, val) ;          \
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
static JSValue js_wifi_set_sta_config(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv){
    CHECK_WIFI_INITED
    CHECK_ARGC(1)
    if(!JS_IsObject(argv[0])) {
        THROW_EXCEPTION("missing options arg")
    }

    wifi_config_t wifi_config = {
        .sta = {
            /* Setting a password implies station will connect to all security modes including WEP/WPA.
             * However these modes are deprecated and not advisable to be used. Incase your Access point
             * doesn't support WPA2, these mode can be enabled by commenting below line */
	     	.threshold.authmode = WIFI_AUTH_WPA2_PSK,
            .pmf_cfg = {
                .capable = true,
                .required = false
            },
        },
    };

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

    return JS_NewInt32(ctx, esp_wifi_set_config(ESP_IF_WIFI_STA, &wifi_config) );
}

// typedef struct {
//     uint8_t ssid[32];           /**< SSID of ESP32 soft-AP. If ssid_len field is 0, this must be a Null terminated string. Otherwise, length is set according to ssid_len. */
//     uint8_t password[64];       /**< Password of ESP32 soft-AP. */
//     uint8_t ssid_len;           /**< Optional length of SSID field. */
//     uint8_t channel;            /**< Channel of ESP32 soft-AP */
//     wifi_auth_mode_t authmode;  /**< Auth mode of ESP32 soft-AP. Do not support AUTH_WEP in soft-AP mode */
//     uint8_t ssid_hidden;        /**< Broadcast SSID or not, default 0, broadcast the SSID */
//     uint8_t max_connection;     /**< Max number of stations allowed to connect in, default 4, max 10 */
//     uint16_t beacon_interval;   /**< Beacon interval which should be multiples of 100. Unit: TU(time unit, 1 TU = 1024 us). Range: 100 ~ 60000. Default value: 100 */
//     wifi_cipher_type_t pairwise_cipher;   /**< pairwise cipher of SoftAP, group cipher will be derived using this. cipher values are valid starting from WIFI_CIPHER_TYPE_TKIP, enum values before that will be considered as invalid and default cipher suites(TKIP+CCMP) will be used. Valid cipher suites in softAP mode are WIFI_CIPHER_TYPE_TKIP, WIFI_CIPHER_TYPE_CCMP and WIFI_CIPHER_TYPE_TKIP_CCMP. */
//     bool ftm_responder;         /**< Enable FTM Responder mode */
// } wifi_ap_config_t;


/**
 * 设置 wifi AP 模式的参数
 * 
 * 参数 mode :
 * ```
 * {
 *     ssid: string ,
 *     password?: string ,
 *     "threshold.authmode"?: 0-8 ,
 *     channel?: 1-13 ,
 *     max_connection?: number = 4 , // 最大客户连接数
 *     ssid_hidden:bool = false ,    // 热点不会被扫描到
 *     beacon_interval?: number ,
 *     pairwise_cipher?: number ,
 *     ftm_responder?: bool ,        // Enable FTM Responder mode
 * }
 * ```
 * 
 * 如果 `password` 为空, 则 `threshold.authmode` 自动设置为 `WIFI_AUTH_OPEN`
 * 
 * 返回 0 表示 api 调用成功, 返回非 0 表示错误代码
 * 
 * @beapi beapi.wifi.setAPConfig
 * @param config:object
 * @return number
 */
static JSValue js_wifi_set_ap_config(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    CHECK_WIFI_INITED
    CHECK_ARGC(1)
    if(!JS_IsObject(argv[0])) {
        THROW_EXCEPTION("missing options arg")
    }

    wifi_config_t wifi_config = {
        .ap = {
            .ssid_len = 0,
            .channel = 1,
            .max_connection = 4,
            .authmode = WIFI_AUTH_WPA_WPA2_PSK ,
            .ssid_hidden = 0
        },
    };

    SET_MEMBER_STRING(ap, ssid, MAX_SSID_CHARLEN)
    SET_MEMBER_STRING(ap, password, MAX_PASSWORD_CHARLEN)
    if(password_len==0) {
        wifi_config.ap.authmode = WIFI_AUTH_OPEN ;
    }

    SET_MEMBER_INT(ap, channel)
    SET_MEMBER_INT(ap, max_connection)
    SET_MEMBER_INT(ap, ssid_hidden)
    SET_MEMBER_INT(ap, beacon_interval)
    SET_MEMBER_INT(ap, pairwise_cipher)
    SET_MEMBER_BOOL(ap, ftm_responder)

    return JS_NewInt32(ctx, esp_wifi_set_config(ESP_IF_WIFI_AP, &wifi_config) );
}


/**
 * 返回 wifi 的工作参数
 * 
 * 返回的对象可参考 `setStaConfig()` 和 `setAPConfig()` 的 `mode` 参数
 * 
 * @beapi beapi.wifi.getConfig
 * @param mode:1|2  1代表 sta , 2代表 ap
 * @return object
 */
static JSValue js_wifi_get_config(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    CHECK_WIFI_INITED
    CHECK_ARGC(1)
    ARGV_TO_UINT8(0,netif)

    wifi_config_t conf ;

    JSValue jsconf = JS_NewObject(ctx) ;
    if( netif==WIFI_MODE_STA ) {
        esp_err_t err = esp_wifi_get_config(ESP_IF_WIFI_STA, &conf) ;
        if(err!=ESP_OK) {
            THROW_EXCEPTION("esp_wifi_get_config() failed: %d", err)
        }
        JS_SetPropertyStr(ctx, jsconf, "ssid", JS_NewString(ctx, (const char *)conf.sta.ssid)) ;
        JS_SetPropertyStr(ctx, jsconf, "password", JS_NewString(ctx, (const char *)conf.sta.password)) ;
        JS_SetPropertyStr(ctx, jsconf, "authmode", JS_NewInt32(ctx, conf.sta.threshold.authmode)) ;
        JS_SetPropertyStr(ctx, jsconf, "channel", JS_NewInt32(ctx, conf.sta.channel)) ;
        JS_SetPropertyStr(ctx, jsconf, "scan_method", JS_NewInt32(ctx, conf.sta.scan_method)) ;
        JS_SetPropertyStr(ctx, jsconf, "listen_interval", JS_NewInt32(ctx, conf.sta.listen_interval)) ;
        JS_SetPropertyStr(ctx, jsconf, "sort_method", JS_NewInt32(ctx, conf.sta.sort_method)) ;
        JS_SetPropertyStr(ctx, jsconf, "rm_enabled", JS_NewInt32(ctx, conf.sta.rm_enabled)) ;
        JS_SetPropertyStr(ctx, jsconf, "btm_enabled", JS_NewInt32(ctx, conf.sta.btm_enabled)) ;
    }
    else if(netif==WIFI_MODE_AP) {
        esp_err_t err = esp_wifi_get_config(ESP_IF_WIFI_AP, &conf) ;
        if(err!=ESP_OK) {
            THROW_EXCEPTION("esp_wifi_get_config() failed: %d", err)
        }
        JS_SetPropertyStr(ctx, jsconf, "ssid", JS_NewString(ctx, (const char *)conf.ap.ssid)) ;
        JS_SetPropertyStr(ctx, jsconf, "password", JS_NewString(ctx, (const char *)conf.ap.password)) ;
        JS_SetPropertyStr(ctx, jsconf, "channel", JS_NewInt32(ctx, conf.ap.channel)) ;
        JS_SetPropertyStr(ctx, jsconf, "authmode", JS_NewInt32(ctx, conf.ap.authmode)) ;
        JS_SetPropertyStr(ctx, jsconf, "ssid_hidden", JS_NewInt32(ctx, conf.ap.ssid_hidden)) ;
        JS_SetPropertyStr(ctx, jsconf, "max_connection", JS_NewInt32(ctx, conf.ap.max_connection)) ;
        JS_SetPropertyStr(ctx, jsconf, "beacon_interval", JS_NewInt32(ctx, conf.ap.beacon_interval)) ;
        JS_SetPropertyStr(ctx, jsconf, "ftm_responder", JS_NewInt32(ctx, conf.ap.ftm_responder)) ;
    }
    else{
        THROW_EXCEPTION("unknow netif: %d", netif)
    }

    return jsconf ;
}


/**
 * 设置一个用于接收 wifi 事件的回调函数
 * 
 * > 只能设置一个回调函数, 如果已经存在, 则会替换掉原来的
 * 
 * @beapi beapi.wifi.registerEventHandle
 * @param callback:(eventType:number,eventId:number,data:any)=>void
 */
static JSValue js_wifi_register_event_handle(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    CHECK_ARGC(1)
    if( !JS_IsFunction(ctx, argv[0]) ){
        THROW_EXCEPTION("wifi event handle must be a function")
    }
    JS_DupValue(ctx, argv[0]) ;
    __event_handle = argv[0] ;
    __event_handle_ctx = ctx ;
    return JS_UNDEFINED ;
}

/**
 * 启动 wifi
 * 
 * 返回 0 表示 api 调用成功, 返回非 0 表示错误代码
 * 
 * @beapi beapi.wifi.start
 * @return number
 */
static JSValue js_wifi_start(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    CHECK_WIFI_INITED
    esp_err_t err = esp_wifi_start() ;
    _started = err == ESP_OK ;
    return JS_NewInt32(ctx, err);
}

/**
 * 停止 wifi
 * 
 * 返回 0 表示 api 调用成功, 返回非 0 表示错误代码
 * 
 * @beapi beapi.wifi.stop
 * @return number
 */
static JSValue js_wifi_stop(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    CHECK_WIFI_INITED
    esp_err_t err = esp_wifi_stop() ;
    _started = ! (err == ESP_OK) ;
    return JS_NewInt32(ctx, err);
}

// typedef struct {
//     uint8_t mac[6];  /**< mac address */
//     int8_t  rssi;    /**< current average rssi of sta connected */
//     uint32_t phy_11b:1;      /**< bit: 0 flag to identify if 11b mode is enabled or not */
//     uint32_t phy_11g:1;      /**< bit: 1 flag to identify if 11g mode is enabled or not */
//     uint32_t phy_11n:1;      /**< bit: 2 flag to identify if 11n mode is enabled or not */
//     uint32_t phy_lr:1;       /**< bit: 3 flag to identify if low rate is enabled or not */
//     uint32_t reserved:28;    /**< bit: 4..31 reserved */
// } wifi_sta_info_t;

// #define ESP_WIFI_MAX_CONN_NUM  (10)       /**< max number of stations which can connect to ESP32 soft-AP */

// /** @brief List of stations associated with the ESP32 Soft-AP */
// typedef struct {
//     wifi_sta_info_t sta[ESP_WIFI_MAX_CONN_NUM]; /**< station list */
//     int       num; /**< number of stations in the list (other entries are invalid) */
// } wifi_sta_list_t;

/**
 * 返回所有连接到本机AP的客户机
 * 
 * @beapi beapi.wifi.allSta
 * @return {mac:string,rssi:string}[]
 */
static JSValue js_wifi_ap_all_sta(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    CHECK_WIFI_INITED
    wifi_sta_list_t clients;
    if(esp_wifi_ap_get_sta_list(&clients) != ESP_OK) {
        THROW_EXCEPTION("esp_wifi_ap_get_sta_list() failed")
    }

    JSValue arr = JS_NewArray(ctx) ;
    for(int i=0; i<clients.num; i++) {
        JSValue obj = JS_NewObject(ctx) ;
        JS_SetPropertyStr(ctx, obj, "mac", JS_NewString(ctx, "unimplemented")) ;
        JS_SetPropertyStr(ctx, obj, "rssi", JS_NewInt32(ctx, clients.sta[i].rssi)) ;
        JS_SetPropertyUint32(ctx, arr, i, obj) ;
    }

    return arr ;
}

/**
 * 开始扫描附近的AP
 * 
 * > wifi STA 模式必须启动
 * 
 * @beapi beapi.wifi.scanStart
 * @return bool
 */
static JSValue js_wifi_scan_start(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    CHECK_WIFI_INITED
    wifi_scan_config_t scanConf = {
        .ssid = NULL,
        .bssid = NULL,
        .channel = 0,
        .show_hidden = true
    };

    esp_err_t ret = esp_wifi_scan_start(&scanConf, false) ;
    if( ret == ESP_OK) {
        _scanning = true ;
        return JS_TRUE ;
    }
    else{
        printf("esp_wifi_scan_start() failed:(%d) %s\n",ret, esp_err_to_name(ret)) ;
        _scanning = false ;
        return JS_FALSE ;
    }
}

/**
 * 停止AP扫描
 * 
 * @beapi beapi.wifi.scanStop
 * @return bool
 */
static JSValue js_wifi_scan_stop(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    CHECK_WIFI_INITED
    return esp_wifi_scan_stop() == ESP_OK? JS_TRUE : JS_FALSE;
}
/**
 * 返回 AP 扫描是否正在进行
 * 
 * @beapi beapi.wifi.isScanning
 * @return bool
 */
static JSValue js_wifi_is_scanning(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    CHECK_WIFI_INITED
    return _scanning? JS_TRUE : JS_FALSE;
}

// typedef enum {
//     WIFI_AUTH_OPEN = 0,         /**< authenticate mode : open */
//     WIFI_AUTH_WEP,              /**< authenticate mode : WEP */
//     WIFI_AUTH_WPA_PSK,          /**< authenticate mode : WPA_PSK */
//     WIFI_AUTH_WPA2_PSK,         /**< authenticate mode : WPA2_PSK */
//     WIFI_AUTH_WPA_WPA2_PSK,     /**< authenticate mode : WPA_WPA2_PSK */
//     WIFI_AUTH_WPA2_ENTERPRISE,  /**< authenticate mode : WPA2_ENTERPRISE */
//     WIFI_AUTH_WPA3_PSK,         /**< authenticate mode : WPA3_PSK */
//     WIFI_AUTH_WPA2_WPA3_PSK,    /**< authenticate mode : WPA2_WPA3_PSK */
//     WIFI_AUTH_WAPI_PSK,         /**< authenticate mode : WAPI_PSK */
//     WIFI_AUTH_OWE,              /**< authenticate mode : OWE */
//     WIFI_AUTH_MAX
// } wifi_auth_mode_t;
static char * authmode_names(wifi_auth_mode_t auto_mode) {
    switch(auto_mode) {
        case WIFI_AUTH_OPEN: return "OPEN" ;
        case WIFI_AUTH_WEP: return "WEP" ;
        case WIFI_AUTH_WPA_PSK: return "WPA_PSK" ;
        case WIFI_AUTH_WPA2_PSK: return "WPA2_PSK" ;
        case WIFI_AUTH_WPA_WPA2_PSK: return "WPA_WPA2_PSK" ;
        case WIFI_AUTH_WPA2_ENTERPRISE: return "WPA_WPA2_PSK" ;
        case WIFI_AUTH_WPA3_PSK: return "WPA3_PSK" ;
        case WIFI_AUTH_WPA2_WPA3_PSK: return "WPA2_WPA3_PSK" ;
        case WIFI_AUTH_WAPI_PSK: return "WAPI_PSK" ;
        // case WIFI_AUTH_OWE: return "OWE" ;
        default:
            return "unknow" ;
    }
}

/**
 * 取回 AP 扫描的结果
 * 
 * AP对象的格式:
 * ```
 * {
 *     bssid:string ,
 *     ssid:string ,
 *     channel:number ,
 *     rssi:number ,
 *     authmode:number ,
 * }
 * ```
 * 
 * @beapi beapi.wifi.scanRecords
 * @return object[]
 */
static JSValue js_wifi_scan_records(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    CHECK_WIFI_INITED
    JSValue array = JS_NewArray(ctx) ;

    uint16_t apCount = 0;
    esp_wifi_scan_get_ap_num(&apCount);

    // printf("Number of access points found: %d\n",event->event_info.scan_done.number);
    if (apCount == 0) {
        return array ;
    }

    wifi_ap_record_t *list = (wifi_ap_record_t *)malloc(sizeof(wifi_ap_record_t) * apCount);
    if(!list) {
        THROW_EXCEPTION("out of memory?")
    }

    esp_wifi_scan_get_ap_records(&apCount, list) ;

    for (int i=0; i<apCount; i++) {        
        JSValue apobj = JS_NewObject(ctx) ;
        JS_SetPropertyStr(ctx, apobj, "bssid", JS_NewString(ctx, (char*)list[i].bssid)) ;
        JS_SetPropertyStr(ctx, apobj, "ssid", JS_NewString(ctx, (char*)list[i].ssid)) ;
        JS_SetPropertyStr(ctx, apobj, "channel", JS_NewInt32(ctx, list[i].primary)) ;
        JS_SetPropertyStr(ctx, apobj, "rssi", JS_NewInt32(ctx, list[i].rssi)) ;
        JS_SetPropertyStr(ctx, apobj, "authmode", JS_NewInt32(ctx, list[i].authmode)) ;
        JS_SetPropertyUint32(ctx, array, i, apobj) ;
    }

    free(list) ;

    return array ;
}


bool inline wifi_has_inited() {
    return wifi_inited ;
}
/**
 * 返回 wifi 是否已经初始化
 * 
 * beshell 启动时回自动初始化 wifi (除非在 nvs 中禁用 wifi)
 * 
 * @beapi beapi.wifi.hasInited
 * @return bool
 */
static JSValue js_wifi_has_inited(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    return wifi_inited? JS_TRUE: JS_FALSE ;
}

void be_module_wifi_init() {

    ESP_ERROR_CHECK(esp_event_handler_instance_register(WIFI_EVENT, ESP_EVENT_ANY_ID, &esp32_wifi_eventHandler, NULL, &instance_any_id));
    ESP_ERROR_CHECK(esp_event_handler_instance_register(IP_EVENT, IP_EVENT_STA_GOT_IP, &esp32_wifi_eventHandler, NULL, &instance_got_ip));

    ESP_ERROR_CHECK(esp_netif_init());

    netif_sta = esp_netif_create_default_wifi_sta();
    netif_ap = esp_netif_create_default_wifi_ap();

    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
    ESP_ERROR_CHECK(esp_wifi_init(&cfg));

    wifi_inited = true ;
}

void be_module_wifi_require(JSContext *ctx) {

    JSValue global = JS_GetGlobalObject(ctx);
    JSValue beapi = JS_GetPropertyStr(ctx, global, "beapi") ;

    JSValue wifi = JS_NewObject(ctx);
    JS_SetPropertyStr(ctx, beapi, "wifi", wifi);

    JS_SetPropertyStr(ctx, wifi, "hasInited", JS_NewCFunction(ctx, js_wifi_has_inited, "hasInited", 1));
    JS_SetPropertyStr(ctx, wifi, "start", JS_NewCFunction(ctx, js_wifi_start, "start", 1));
    JS_SetPropertyStr(ctx, wifi, "stop", JS_NewCFunction(ctx, js_wifi_stop, "stop", 1));
    JS_SetPropertyStr(ctx, wifi, "setPS", JS_NewCFunction(ctx, js_wifi_set_ps, "setPS", 1));
    JS_SetPropertyStr(ctx, wifi, "setMode", JS_NewCFunction(ctx, js_wifi_set_mode, "setMode", 1));
    JS_SetPropertyStr(ctx, wifi, "getMode", JS_NewCFunction(ctx, js_wifi_get_mode, "getMode", 1));
    JS_SetPropertyStr(ctx, wifi, "setStaConfig", JS_NewCFunction(ctx, js_wifi_set_sta_config, "setStaConfig", 1));
    JS_SetPropertyStr(ctx, wifi, "setAPConfig", JS_NewCFunction(ctx, js_wifi_set_ap_config, "setAPConfig", 1));
    JS_SetPropertyStr(ctx, wifi, "getConfig", JS_NewCFunction(ctx, js_wifi_get_config, "getConfig", 1));
    JS_SetPropertyStr(ctx, wifi, "connect", JS_NewCFunction(ctx, js_wifi_connect, "connect", 1));
    JS_SetPropertyStr(ctx, wifi, "disconnect", JS_NewCFunction(ctx, js_wifi_disconnect, "disconnect", 1));
    JS_SetPropertyStr(ctx, wifi, "getIpInfo", JS_NewCFunction(ctx, js_wifi_get_ip_info, "getIpInfo", 1));
    JS_SetPropertyStr(ctx, wifi, "setHostname", JS_NewCFunction(ctx, js_wifi_set_hostname, "setHostname", 1));
    JS_SetPropertyStr(ctx, wifi, "allSta", JS_NewCFunction(ctx, js_wifi_ap_all_sta, "allSta", 1));
    JS_SetPropertyStr(ctx, wifi, "registerEventHandle", JS_NewCFunction(ctx, js_wifi_register_event_handle, "registerEventHandle", 1));

    JS_SetPropertyStr(ctx, wifi, "scanStart", JS_NewCFunction(ctx, js_wifi_scan_start, "scanStart", 1));
    JS_SetPropertyStr(ctx, wifi, "scanStop", JS_NewCFunction(ctx, js_wifi_scan_stop, "scanStop", 1));
    JS_SetPropertyStr(ctx, wifi, "isScanning", JS_NewCFunction(ctx, js_wifi_is_scanning, "isScanning", 1));
    JS_SetPropertyStr(ctx, wifi, "scanRecords", JS_NewCFunction(ctx, js_wifi_scan_records, "scanRecords", 1));
    
    JS_SetPropertyStr(ctx, wifi, "staStarted", JS_NewCFunction(ctx, js_wifi_sta_started, "staStarted", 1));
    JS_SetPropertyStr(ctx, wifi, "staConnected", JS_NewCFunction(ctx, js_wifi_sta_connected, "staConnected", 1));
    JS_SetPropertyStr(ctx, wifi, "apStarted", JS_NewCFunction(ctx, js_wifi_ap_started, "apStarted", 1));

    JS_FreeValue(ctx, global);
    JS_FreeValue(ctx, beapi);
}



void be_module_wifi_reset(JSContext *ctx) {
    JS_FreeValue(ctx, __event_handle);
    __event_handle  = JS_UNDEFINED ;
    __event_handle_ctx = NULL ;
}
