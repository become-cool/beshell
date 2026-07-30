/**
 * > WiFi 模块的例子请参考：[用 JavaScript 连接 WiFi](../guide/wifi.md)
 * 
 * 系统在初次 `import` WiFi 模块时，会自动初始化所有相关的硬件资源。
 * 
 * @module wifi
 */

#include "WiFi.hpp"
#include "BeShell.class.hpp"
#include <lwip/apps/sntp.h>
#include <esp_err.h>
#include <esp_event.h>
#include <esp_log.h>
#include <esp_wifi.h>
#include <esp_netif.h>
#include <lwip/dns.h>
#include <lwip/sockets.h>
#include "../js/wifi.c"
#include "./NVS.hpp"
#include "NativeModule.hpp"
#include "esp_interface.h"
#include "platform.hpp"
// #include "esp_eap_client.h"


using namespace be ;

// extern const uint8_t src_js_wifi_js_start[] asm("_binary_src_js_wifi_js_start");
// extern const uint8_t src_js_wifi_js_end[] asm("_binary_src_js_wifi_js_end");


#define DEFAULT_SCAN_LIST_SIZE 16

#define MAX_SSID_CHARLEN        31
#define MAX_PASSWORD_CHARLEN    63

typedef struct {
    esp_event_base_t event_base ;
    int32_t event_id ;
    int reason ;
} wifi_nevent_t ;

#ifdef CONFIG_ESP_WIFI_SOFTAP_SUPPORT
static esp_netif_t * netif_ap = NULL ;
esp_netif_t * get_netif_ap() {
    return netif_ap ;
}
#endif

static esp_netif_t * netif_sta = NULL ;
esp_netif_t * get_netif_sta() {
    return netif_sta ;
}

static esp_event_handler_instance_t instance_any_id;
static esp_event_handler_instance_t instance_got_ip;
static esp_event_handler_instance_t instance_lost_ip;

static bool wifi_inited = false ;
bool WiFi::hasInited() {
    return wifi_inited ;
}


#define CHECK_WIFI_INITED                                       \
    if(!wifi_inited) {                                          \
        JSTHROW("wifi not init, please call wifi.start() first")\
    }

#ifndef CONFIG_ESP_WIFI_SOFTAP_SUPPORT
#define CHECK_SOFTAP_SUPPORT()                                  \
    JSTHROW("SoftAP is not supported, please enable CONFIG_ESP_WIFI_SOFTAP_SUPPORT in sdkconfig")
#else
#define CHECK_SOFTAP_SUPPORT()
#endif

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
    
    if(event_base==WIFI_EVENT) {
        if(event_id == WIFI_EVENT_WIFI_READY) {
            // printf("WIFI_EVENT_WIFI_READY\n") ;
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
    }
    else {
    }
    
    if(WiFi::singleton) {
        wifi_nevent_t nevent = {
            .event_base = event_base ,
            .event_id = event_id ,
            .reason = 0 ,
        } ;
        if(event_base==WIFI_EVENT && event_id == WIFI_EVENT_STA_DISCONNECTED) {
            nevent.reason = REASON(event_data) ;
        }
        WiFi::singleton->emitNativeEvent(&nevent) ;
    }
    // if( __event_handle_ctx!=NULL && JS_IsFunction(__event_handle_ctx, __event_handle) ) {
    //     MAKE_ARGV3(argv, JS_NewInt32(__event_handle_ctx, eventType), JS_NewInt32(__event_handle_ctx, event_id), JS_UNDEFINED)
    //     // dis reason
    //     if(event_base==WIFI_EVENT && event_id == WIFI_EVENT_STA_DISCONNECTED) {
    //         argv[2] = JS_NewInt32(__event_handle_ctx, REASON(event_data)) ;
    //     }
    //     dd
    //     JSEngine::fromJSContext(__event_handle_ctx)->timer.setTimer(__event_handle_ctx, __event_handle, 0, false, JS_UNDEFINED, 3, argv) ;
    //     free(argv) ;
    // }
}


// // event type
// const EVENT_WIFI = 1
// const EVENT_IP = 2

// // WiFi events
// const WIFI_READY = 0
// const WIFI_SCAN_DONE = 1
// const WIFI_STA_START = 2
// const WIFI_STA_STOP = 3
// const WIFI_STA_CONNECTED = 4
// const WIFI_STA_DISCONNECTED = 5
// // const WIFI_STA_AUTHMODE_CHANGE = 6
// // const WIFI_STA_WPS_ER_SUCCESS = 7
// // const WIFI_STA_WPS_ER_FAILED = 8
// // const WIFI_STA_WPS_ER_TIMEOUT = 9
// // const WIFI_STA_WPS_ER_PIN = 10
// // const WIFI_STA_WPS_ER_PBC_OVERLAP = 11
// const WIFI_AP_START = 12
// const WIFI_AP_STOP = 13
// const WIFI_AP_STACONNECTED = 14
// const WIFI_AP_STADISCONNECTED = 15
// // const WIFI_AP_PROBEREQRECVED = 16
// // const WIFI_FTM_REPORT = 17
// // const WIFI_STA_BSS_RSSI_LOW = 18
// // const WIFI_ACTION_TX_STATUS = 19
// // const WIFI_ROC_DONE = 21
// // const WIFI_STA_BEACON_TIMEOUT = 21
// const WIFI_STA_CONNECTING = 101

// let evtNames = {}
// evtNames[WIFI_READY] = "ready"
// evtNames[WIFI_SCAN_DONE] = "scan.done"
// evtNames[WIFI_STA_START] = "sta.start"
// evtNames[WIFI_STA_STOP] = "sta.stop"
// evtNames[WIFI_STA_CONNECTED] = "sta.connected"
// evtNames[WIFI_STA_DISCONNECTED] = "sta.disconnected"
// evtNames[WIFI_AP_START] = "ap.start"
// evtNames[WIFI_AP_STOP] = "ap.stop"
// evtNames[WIFI_AP_STACONNECTED] = "ap.sta.connected"
// evtNames[WIFI_AP_STADISCONNECTED] = "ap.sta.disconnected"
// evtNames[WIFI_STA_CONNECTING] = "sta.connecting"
// evtNames[102] = "sta.disconnecting"

// // IP events
// const STA_GOT_IP = 0
// const STA_LOST_IP = 1
// // const AP_STAIPASSIGNED = 2
// // const GOT_IP6 = 3
// // const ETH_GOT_IP = 4
// // const PPP_GOT_IP = 5
// // const PPP_LOST_IP = 6

// let ipEvtNames = {}
// ipEvtNames[STA_GOT_IP] = "ip.got"
// ipEvtNames[STA_LOST_IP] = "ip.lost"

// // power safe
// const PS_NONE = 0
// // const PS_MIN_MODEM = 1
// // const PS_MAX_MODEM = 2 

#define MAP_EVENT(event_id, event_name)         \
        case event_id :{                        \
            name = (char *)event_name ;         \
            break;                              \
        }


void be::WiFi::onNativeEvent(JSContext *ctx, void * param) {
    wifi_nevent_t * nevent = (wifi_nevent_t*) param ;
    char * name = nullptr ;
    if(nevent->event_base==WIFI_EVENT) {
        switch(nevent->event_id) {
            MAP_EVENT(WIFI_EVENT_WIFI_READY, "ready")
            MAP_EVENT(WIFI_EVENT_SCAN_DONE, "scan.done")
            MAP_EVENT(WIFI_EVENT_STA_START, "sta.start")
            MAP_EVENT(WIFI_EVENT_STA_STOP, "sta.stop")
            MAP_EVENT(WIFI_EVENT_STA_CONNECTED, "sta.connected")
            MAP_EVENT(WIFI_EVENT_STA_DISCONNECTED, "sta.disconnected")
            MAP_EVENT(WIFI_EVENT_AP_START, "ap.start")
            MAP_EVENT(WIFI_EVENT_AP_STOP, "ap.stop")
            MAP_EVENT(WIFI_EVENT_AP_STACONNECTED, "ap.sta.connected")
            MAP_EVENT(WIFI_EVENT_AP_STADISCONNECTED, "ap.sta.disconnected")
            MAP_EVENT(WIFI_EVENT_STA_CONNECTING, "sta.connecting")
            MAP_EVENT(WIFI_EVENT_STA_DISCONNECTING, "sta.disconnecting")
            MAP_EVENT(WIFI_EVENT_HOME_CHANNEL_CHANGE, "home-channel-chage")
            MAP_EVENT(WIFI_EVENT_STA_NEIGHBOR_REP, "sta.neighbor-report")
            default: break;
        }
    }
    else if(nevent->event_base==IP_EVENT) {
        switch(nevent->event_id) {
            MAP_EVENT(IP_EVENT_STA_GOT_IP, "IP.GOT")
            MAP_EVENT(IP_EVENT_STA_LOST_IP, "ip.lost")
            default: break;
        }
    }
    if(!name){
        static char unknow_name[20] ;
        snprintf(unknow_name,sizeof(unknow_name),"event:%d",nevent->event_id) ;
        name = unknow_name ;
    }
    emitSyncFree(name, {}) ;
}

namespace be {

    char const * const WiFi::name = "wifi" ;
    WiFi * WiFi::singleton = nullptr ;
    
    WiFi::WiFi(JSContext * ctx, const char * name)
        : EventModule(ctx, name, 0)
    {
        if(!singleton) {
            singleton = this ;
        }
        
        exportName("start") ;
        exportName("isReady") ;
        exportName("stop") ;
        exportName("startAP") ;
        exportName("stopAP") ;
        exportName("connect") ;
        exportName("disconnect") ;
        exportName("isConnecting") ;
        exportName("status") ;
        exportName("scan") ;
        exportName("waitIP") ;
        exportName("startStaDeamon") ;
        exportName("stopStaDeamon") ;
        exportName("isStaDeamonRunning") ;

        exportFunction("peripheralStart",start,0) ;
        exportFunction("peripheralStop",stop,0) ;
        exportFunction("setPS",setPS,0) ;
        exportFunction("setMode",setMode,0) ;
        exportFunction("mode",mode,0) ;
        exportFunction("setAPConfig",setAPConfig,0) ;
        exportFunction("setStaConfig",setStaConfig,0) ;
        exportFunction("dhcpStaStart",dhcpStaStart,0) ;
        exportFunction("dhcpStaStop",dhcpStaStop,0) ;
        exportFunction("setStaIP",setStaIP,0) ;
        exportFunction("config",config,0) ;
        exportFunction("peripheralConnect",peripheralConnect,0) ;
        exportFunction("peripheralDisconnect",peripheralDisconnect,0) ;
        exportFunction("getIpInfo",getIpInfo,0) ;
        exportFunction("setHostname",setHostname,0) ;
        exportFunction("allSta",allSta,0) ;
        exportFunction("scanStart",scanStart,0) ;
        exportFunction("scanStop",scanStop,0) ;
        exportFunction("isScanning",isScanning,0) ;
        exportFunction("scanRecords",scanRecords,0) ;
        exportFunction("staStarted",staStarted,0) ;
        exportFunction("staConnected",staConnected,0) ;
        exportFunction("apStarted",apStarted,0) ;
        exportFunction("setMAC",setMAC,0) ;
        exportFunction("getMAC",getMAC,0) ;
        exportFunction("isSoftAPSupported",isSoftAPSupported,0) ;

        enableNativeEvent(ctx, sizeof(wifi_nevent_t), 5) ;
    }

    void WiFi::exports(JSContext *ctx) {

        EventModule::exports(ctx) ;

        init() ;
        JSEngineEvalEmbeded(ctx, wifi)
    }

    #define ESP_API(code)       \
            res = code ;        \
            if(res!=ESP_OK) {   \
                printf("%s failed, return error code:%d\n", #code, res) ; \
            }

    void WiFi::init() {
        if(wifi_inited) {
            return ;
        }

        be::platform::networkInit() ;

        esp_err_t res ;

        netif_sta = esp_netif_create_default_wifi_sta();
#ifdef CONFIG_ESP_WIFI_SOFTAP_SUPPORT
        netif_ap = esp_netif_create_default_wifi_ap();
#endif

        wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
        ESP_API(esp_wifi_init(&cfg))

        ESP_API(esp_event_handler_instance_register(WIFI_EVENT, ESP_EVENT_ANY_ID, &esp32_wifi_eventHandler, (void *)NULL, &instance_any_id))
        ESP_API(esp_event_handler_instance_register(IP_EVENT, ESP_EVENT_ANY_ID, &esp32_wifi_eventHandler, (void *)NULL, &instance_got_ip))

        res = esp_wifi_set_mode(WIFI_MODE_NULL) ;
        if(res!=ESP_OK) {
            printf("esp_wifi_set_mode failed, return error code:%d\n", res) ;
        }

        wifi_inited = true ;
    }

    void WiFi::use(be::BeShell * beshell) {
        beshell->use<be::NVS>() ;
    }

    /**
     * 启动 WiFi 硬件
     *
     * 所有 WiFi 模块里的 API 都需要在 `start()` 之后才能调用。
     * `startAP` 和 `connect` 会自动调用 `start()`，因此一般情况下可以直接使用。
     *
     * @module wifi
     * @function start
     * @return number 返回 0 表示成功，非 0 表示错误代码
     * @throws WiFi 初始化失败
     */
    JSValue WiFi::start(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        init() ;
        esp_err_t err = esp_wifi_start() ;
        _started = err == ESP_OK ;
        return JS_NewInt32(ctx, err);
    }

    /**
     * 停止 WiFi 硬件
     *
     * 返回 0 表示成功，非 0 表示错误代码
     *
     * @module wifi
     * @function stop
     * @return number 返回 0 表示成功，非 0 表示错误代码
     * @throws WiFi 未初始化
     */
    JSValue WiFi::stop(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        CHECK_WIFI_INITED
        esp_err_t err = esp_wifi_stop() ;
        _started = ! (err == ESP_OK) ;
        return JS_NewInt32(ctx, err);
    }
    
    /**
     * 返回是否已经调用过 `start()` 启动 WiFi
     *
     * @module wifi
     * @function isReady
     * @return bool 返回 true 表示 WiFi 已启动，false 表示未启动
     */

    /**
     * 启动 WiFi 热点
     *
     * > 该函数会自动调用 `start()` 启动 WiFi 模块。
     *
     * 异步返回 Promise\<bool\> 表示成功或者失败
     *
     * 示例：
     * ```javascript
     * import { startAP } from "wifi"
     *
     * await startAP("MyAP", "password123")
     * console.log("AP started!")
     * ```
     *
     * @module wifi
     * @function startAP
     * @param ssid:string WiFi 热点名称
     * @param password:string WiFi 热点密码
     *
     * @return Promise\<bool\>
     */

    /**
     * 停止 WiFi 热点
     *
     * 异步返回 Promise\<bool\> 表示成功或者失败
     *
     * @module wifi
     * @function stopAP
     * @return Promise\<bool\>
     */

    /**
     * 做为 WiFi STA 连接到热点
     *
     * > 该函数会自动调用 `start()` 启动 WiFi 模块。
     *
     * 异步返回 Promise\<bool\> 表示成功或者失败
     *
     * 示例：
     * ```javascript
     * import { connect, waitIP } from "wifi"
     *
     * const connected = await connect("MyWiFi", "password123")
     * if (connected) {
     *     const ipInfo = await waitIP()
     *     console.log("Connected! IP:", ipInfo.ip)
     * }
     * ```
     *
     * @module wifi
     * @function connect
     * @param ssid:string WiFi 热点名称
     * @param password:string WiFi 热点密码
     * @param retry:number=3 连接失败重试次数
     * @param retryDuration:number=2000 连接重试间隔时间，单位 ms
     *
     * @return Promise\<bool\>
     */

    /**
     * 断开 WiFi STA 的连接
     *
     * 异步返回 Promise\<bool\> 表示成功或者失败
     *
     * 未连接状态下，直接返回 Promise\<true\>
     *
     * @module wifi
     * @function disconnect
     * @return Promise\<bool\>
     */

    /**
     * 返回是否正在连接中
     *
     * @module wifi
     * @function isConnecting
     * @return bool 返回 true 表示正在连接中，false 表示未连接
     */


    /**
     * 返回 AP/STA 详细状态
     *
     * 返回对象的格式：
     *
     * ::: code-tabs
     *
     * \@tab apsta
     *
     * ```javascript
     * {
     *     "ap": {
     *         "ssid": string,
     *         "password": string,
     *         "started": bool,
     *         "ip":string,
     *         "netmask":string,
     *         "gw":string
     *      } ,
     *     "sta": {
     *         "ssid": string,
     *         "password": string,
     *         "started": bool,
     *         "connected": bool,
     *         "authmode":number,
     *         "ip":string,
     *         "netmask":string,
     *         "gw":string
     *      } ,
     * }
     * ```
     *
     * \@tab ap
     *
     * ```javascript
     * "ap": {
     *     "ssid": string,
     *     "password": string,
     *     "started": bool,
     *     "ip":string,
     *     "netmask":string,
     *     "gw":string
     * } ,
     * ```
     *
     * \@tab sta
     *
     * ```javascript
     * "sta": {
     *     "ssid": string,
     *     "password": string,
     *     "started": bool,
     *     "connected": bool,
     *     "authmode":number,
     *     "ip":string,
     *     "netmask":string,
     *     "gw":string
     * } ,
     * ```
     * :::
     *
     * 示例：
     * ```javascript
     * import { status } from "wifi"
     *
     * const info = status("sta")
     * console.log("Connected:", info.sta.connected)
     * console.log("IP:", info.sta.ip)
     * ```
     *
     * @module wifi
     * @function status
     * @param type:string="apsta" 可选值："apsta"、"ap"、"sta"
     * @return object
     */


    /**
     * 等待从 DHCP 获取 IP 地址
     *
     * 异步返回 Promise\<object|false\> ，如果失败返回 false，成功则返回包含 ip 的 sta status 对象。
     *
     * 参考 `status()` 函数的返回值。
     *
     * @module wifi
     * @function waitIP
     * @return Promise\<object|false\>
     */

    /**
     * 启动 STA 守护进程
     *
     * 当 WiFi 连接断开时，守护进程会自动尝试重新连接。
     * 通常在 `connect()` 函数中设置 retry 为负数时会自动启动守护进程。
     *
     * @module wifi
     * @function startStaDeamon
     * @param dur:number=10000 检查间隔时间，单位毫秒，默认 10000ms
     */

    /**
     * 停止 STA 守护进程
     *
     * 停止自动重连功能。`disconnect()` 函数会自动调用此函数。
     *
     * @module wifi
     * @function stopStaDeamon
     */

    /**
     * 返回 STA 守护进程是否正在运行
     *
     * @module wifi
     * @function isStaDeamonRunning
     * @return boolean 返回 true 表示守护进程正在运行，false 表示未运行
     */


    /**
     * 设置 WiFi 的节能模式 (Power Save)
     *
     * 参数 mode：
     * - 0: 关闭节能模式
     * - 1: 最小节能模式
     * - 2: 最大节能模式
     *
     * 关闭节能模式可以避免 WiFi 休眠，提高通信的效率和稳定性，但会增加功耗。
     *
     * @module wifi
     * @function setPS
     * @param mode:number 节能模式，取值 0|1|2
     * @return number 返回 0 表示成功，非 0 表示错误代码
     * @throws WiFi 未初始化
     * @throws 参数数量不足
     */
    JSValue WiFi::setPS(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        CHECK_WIFI_INITED
        ASSERT_ARGC(1)
        ARGV_TO_UINT32(0, ps_mode)
        return JS_NewInt32(ctx, esp_wifi_set_ps((wifi_ps_type_t)ps_mode)) ;
    }
    /**
     * 设置 WiFi 的工作模式
     *
     * `startAP` 和 `connect` 会自动设置工作模式，因此通常不需要直接使用这个 API。
     *
     * 参数 mode：
     * - 0: 未启动
     * - 1: STA 模式
     * - 2: AP 模式
     * - 3: STA + AP 模式
     *
     * @module wifi
     * @function setMode
     * @param mode:number 工作模式，取值 0|1|2|3
     * @return number 返回 0 表示成功，非 0 表示错误代码
     * @throws WiFi 未初始化
     */
    JSValue WiFi::setMode(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        CHECK_WIFI_INITED
        ARGV_TO_INT8(0, mode)
        if(mode==WIFI_MODE_AP || mode==WIFI_MODE_APSTA) {
            CHECK_SOFTAP_SUPPORT()
        }
        return JS_NewInt32(ctx, esp_wifi_set_mode((wifi_mode_t)mode));
    }

    /**
     * 返回 WiFi 的工作模式
     *
     * 返回值：
     * - 0: 未启动
     * - 1: STA 模式
     * - 2: AP 模式
     * - 3: STA + AP 模式
     *
     * @module wifi
     * @function getMode
     * @return number 当前工作模式
     * @throws WiFi 未初始化
     */
    JSValue WiFi::mode(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
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

    /**
     * 设置 WiFi AP 模式的参数
     *
     * `startAP` 会自动配置 AP 参数，需要对设备更详细的设置时，可以使用这个 API。
     *
     * 参数 config：
     * ```javascript
     * {
     *     ssid: string,              // 热点名称
     *     password?: string,         // 热点密码
     *     "threshold.authmode"?: number,  // 认证模式 0-8
     *     channel?: number,          // 信道 1-13
     *     max_connection?: number,   // 最大客户端连接数，默认 4
     *     ssid_hidden?: boolean,     // 是否隐藏热点，默认 false
     *     beacon_interval?: number,  // 信标间隔
     *     pairwise_cipher?: number,  // 加密方式
     *     ftm_responder?: boolean,   // 启用 FTM Responder 模式
     * }
     * ```
     *
     * 如果 `password` 为空，则 `threshold.authmode` 自动设置为 `WIFI_AUTH_OPEN`。
     *
     * @module wifi
     * @function setAPConfig
     * @param config:object AP 配置对象
     * @return number 返回 0 表示成功，非 0 表示错误代码
     * @throws WiFi 未初始化
     * @throws 参数数量不足
     * @throws 参数类型错误
     */
    JSValue WiFi::setAPConfig(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        CHECK_WIFI_INITED
        CHECK_SOFTAP_SUPPORT()
        ASSERT_ARGC(1)
        if(!JS_IsObject(argv[0])) {
            JSTHROW("missing options arg")
        }
        
        wifi_config_t wifi_config ;
        memset(&wifi_config, 0, sizeof(wifi_config_t)) ;

        wifi_config.ap.ssid_len = 0;
        wifi_config.ap.channel = 1;
        wifi_config.ap.max_connection = 4;
        wifi_config.ap.authmode = WIFI_AUTH_WPA_WPA2_PSK;
        wifi_config.ap.ssid_hidden = 0;

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

        return JS_NewInt32(ctx, esp_wifi_set_config(WIFI_IF_AP, &wifi_config) );
    }
    
    /**
     * 设置 WiFi STA 模式的连接参数
     *
     * `connect` 会自动配置连接参数，需要使用更详细的连接参数时使用此 API。
     *
     * 参数 config：
     * ```javascript
     * {
     *     ssid: string,              // 目标 AP 的 SSID
     *     password?: string,         // 目标 AP 的密码
     *     "threshold.authmode"?: number,  // 认证模式 0-8
     *     scan_method?: number,      // 扫描方法 0|1
     *     channel?: number,          // 目标信道 1-13
     *     listen_interval?: number,  // 监听间隔，默认 3
     *     sort_method?: number,      // 排序方法 0|1，默认 0
     * }
     * ```
     *
     * 如果 `password` 为空，则 `threshold.authmode` 自动设置为 `WIFI_AUTH_OPEN`。
     *
     * @module wifi
     * @function setStaConfig
     * @param config:object STA 配置对象
     * @return number 返回 0 表示成功，非 0 表示错误代码
     * @throws WiFi 未初始化
     * @throws 参数数量不足
     * @throws 参数类型错误
     */
    JSValue WiFi::setStaConfig(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        CHECK_WIFI_INITED
        ASSERT_ARGC(1)
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

        return JS_NewInt32(ctx, esp_wifi_set_config(WIFI_IF_STA, &wifi_config) );
    }
    
    /**
     * 返回 WiFi 的工作参数
     *
     * 返回的对象可参考 `setStaConfig()` 和 `setAPConfig()` 的参数说明。
     *
     * @module wifi
     * @function getConfig
     * @param mode:number 1 代表 STA，2 代表 AP
     * @return object 配置对象
     * @throws WiFi 未初始化
     * @throws 参数数量不足
     * @throws 获取配置失败
     * @throws 未知的网络接口类型
     */
    JSValue WiFi::config(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        CHECK_WIFI_INITED
        ASSERT_ARGC(1)
        ARGV_TO_UINT8(0,netif)

        wifi_config_t conf ;

        JSValue jsconf = JS_NewObject(ctx) ;
        if( netif==WIFI_MODE_STA ) {
            esp_err_t err = esp_wifi_get_config(WIFI_IF_STA, &conf) ;
            if(err!=ESP_OK) {
                JSTHROW("esp_wifi_get_config() failed: %d", err)
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
            CHECK_SOFTAP_SUPPORT()
            esp_err_t err = esp_wifi_get_config(WIFI_IF_AP, &conf) ;
            if(err!=ESP_OK) {
                JSTHROW("esp_wifi_get_config() failed: %d", err)
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
            JSTHROW("unknow netif: %d", netif)
        }

        return jsconf ;
    }

    /**
     * 启动 STA 模式的 DHCP 客户端
     *
     * 使用 `setStaIP()` 设置静态 IP 后，如需重新使用 DHCP，可调用此函数。
     *
     * @module wifi
     * @function dhcpStaStart
     * @return undefined
     */
    JSValue WiFi::dhcpStaStart(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        esp_netif_dhcpc_start(netif_sta);
        return JS_UNDEFINED ;
    }

    /**
     * 停止 STA 模式的 DHCP 客户端
     *
     * 停止 DHCP 后，可以使用 `setStaIP()` 设置静态 IP 地址。
     *
     * @module wifi
     * @function dhcpStaStop
     * @return undefined
     */
    JSValue WiFi::dhcpStaStop(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        esp_netif_dhcpc_stop(netif_sta);
        return JS_UNDEFINED ;
    }

    /**
     * 设置 WiFi STA 的静态 IP 地址
     *
     * 会先停止 DHCP 客户端，如果需要重新使用 DHCP，可调用 `dhcpStaStart()` 函数。
     *
     * @module wifi
     * @function setStaIP
     * @param ip:string IP 地址
     * @param netmask:string 子网掩码
     * @param gw:string 网关地址
     * @return undefined
     * @throws WiFi 未初始化
     * @throws 参数数量不足
     * @throws 设置 IP 失败
     */
    JSValue WiFi::setStaIP(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        CHECK_WIFI_INITED
        ASSERT_ARGC(3)

        std::string ARGV_TO_STRING(0, ip)
        std::string ARGV_TO_STRING(1, netmask)
        std::string ARGV_TO_STRING(2, gw)

        esp_netif_ip_info_t ip_info;
        memset(&ip_info, 0, sizeof(esp_netif_ip_info_t));
        ip_info.ip.addr = ipaddr_addr(ip.c_str());
        ip_info.netmask.addr = ipaddr_addr(netmask.c_str());
        ip_info.gw.addr = ipaddr_addr(gw.c_str());

        esp_netif_dhcpc_stop(netif_sta); // 必须先停止DHCP客户端
        esp_err_t err = esp_netif_set_ip_info(netif_sta, &ip_info) ;
        if(err != ESP_OK) {
            JSTHROW("esp_netif_set_ip_info() failed: %d", err)
        }

        return JS_UNDEFINED ;
    }
    
    /**
     * 执行 WiFi STA 连接操作
     *
     * 这是底层连接函数，通常在 JS 侧被 `connect()` 调用。
     *
     * @module wifi
     * @function peripheralConnect
     * @return number 返回 0 表示成功，非 0 表示错误代码
     * @throws WiFi 未初始化
     */
    JSValue WiFi::peripheralConnect(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        CHECK_WIFI_INITED
        if(singleton){
            singleton->emitSync("sta.connecting", {}) ;
        }
        return JS_NewInt32(ctx, esp_wifi_connect()) ;
    }
    
    
    JSValue WiFi::connectEAP(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        // CHECK_WIFI_INITED
        // ASSERT_ARGC(3)
        // std::string ARGV_TO_STRING(0, ssid)
        // std::string ARGV_TO_STRING(1, username)
        // std::string ARGV_TO_STRING(2, password)
        
        
        // wifi_config_t wifi_config ;
        // memset(&wifi_config, 0, sizeof(wifi_config_t)) ;
        
        // // SET_MEMBER_STRING(ap, ssid, MAX_SSID_CHARLEN)
        // // wifi_config_t wifi_config = {
        // //     .sta = {
        // //         .password = "",  // 企业认证通常不需要此字段
        // //         .threshold = {
        // //             .authmode = WIFI_AUTH_WPA2_ENTERPRISE,
        // //         },
        // //     },
        // // };
        // strncpy((char *)wifi_config.sta.ssid, ssid.c_str(), 32) ;
        // wifi_config.sta.threshold.authmode = WIFI_AUTH_WPA2_ENTERPRISE ;

        // ds(wifi_config.sta.ssid)

        // esp_wifi_set_config(WIFI_IF_STA, &wifi_config) ;

        // // 使用新的 EAP 客户端 API 配置企业认证
        // esp_eap_client_set_identity((const uint8_t *)ssid.c_str(), ssid.length());
        // esp_eap_client_set_username((const uint8_t *)username.c_str(), username.length());
        // esp_eap_client_set_password((const uint8_t *)password.c_str(), password.length());

        // dstr(ssid)
        // dstr(username)
        // dstr(password)
        
        // // 启用企业认证
        // esp_wifi_sta_enterprise_enable());

        return JS_UNDEFINED ;
    }

    /**
     * WiFi STA 断开连接
     *
     * 这是底层断开函数，通常在 JS 侧被 `disconnect()` 调用。
     *
     * @module wifi
     * @function peripheralDisconnect
     * @return number 返回 0 表示成功，非 0 表示错误代码
     * @throws WiFi 未初始化
     */
    JSValue WiFi::peripheralDisconnect(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        CHECK_WIFI_INITED
        return JS_NewInt32(ctx, esp_wifi_disconnect()) ;
    }

    /**
     * 返回 AP/STA 的 IP 信息
     *
     * 返回一个对象，包含 ip、netmask、gw 信息。
     * ```javascript
     * {
     *     ip: string,
     *     netmask: string,
     *     gw: string
     * }
     * ```
     *
     * @module wifi
     * @function getIpInfo
     * @param type:number 1 代表 STA，2 代表 AP
     * @return object IP 信息对象
     * @throws WiFi 未初始化
     * @throws 参数数量不足
     * @throws 未知的网络接口类型
     */
    JSValue WiFi::getIpInfo(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        CHECK_WIFI_INITED
        ASSERT_ARGC(1)
        ARGV_TO_UINT8(0,type)

        esp_netif_ip_info_t ipinfo;
        bzero(&ipinfo, sizeof(esp_netif_ip_info_t));

        if( type==WIFI_MODE_STA ) {
            esp_netif_get_ip_info(netif_sta, &ipinfo);
        }
#ifdef CONFIG_ESP_WIFI_SOFTAP_SUPPORT
        else if(type==WIFI_MODE_AP) {
            esp_netif_get_ip_info(netif_ap, &ipinfo);
        }
#else
        else if(type==WIFI_MODE_AP) {
            JSTHROW("SoftAP is not supported, please enable CONFIG_ESP_WIFI_SOFTAP_SUPPORT in sdkconfig")
        }
#endif
        else{
            JSTHROW("unknow netif type: %d", type)
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
     * 设置 WiFi 在局域网中显示的主机名
     *
     * @module wifi
     * @function setHostname
     * @param name:string 主机名
     * @return undefined
     * @throws WiFi 未初始化
     * @throws 参数数量不足
     */
    JSValue WiFi::setHostname(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        CHECK_WIFI_INITED
        ASSERT_ARGC(1)

        const char * hostname = JS_ToCString(ctx, argv[0]) ;
        esp_netif_set_hostname(netif_sta, hostname);
#ifdef CONFIG_ESP_WIFI_SOFTAP_SUPPORT
        esp_netif_set_hostname(netif_ap, hostname);
#endif
        JS_FreeCString(ctx, hostname) ;

        return JS_UNDEFINED ;
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
     * 返回所有连接到本机 AP 的客户端
     *
     * 返回对象数组，每个对象包含 mac 和 rssi 信息。
     * ```javascript
     * [
     *     {mac: "xx:xx:xx:xx:xx:xx", rssi: -30},
     * ]
     * ```
     *
     * `rssi` 值范围 -100 ~ 0，单位为 dBm，表示信号强度，越大（绝对值越小）表示信号越强。
     *
     * @module wifi
     * @function allSta
     * @return object[] 客户端列表，每个对象包含 mac 和 rssi 属性
     * @throws WiFi 未初始化
     * @throws 获取客户端列表失败
     */
    JSValue WiFi::allSta(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        CHECK_WIFI_INITED
        CHECK_SOFTAP_SUPPORT()
        wifi_sta_list_t clients;
        if(esp_wifi_ap_get_sta_list(&clients) != ESP_OK) {
            JSTHROW("esp_wifi_ap_get_sta_list() failed")
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
     * 扫描周围的 WiFi 热点，必须在 STA 模式下运行
     *
     * 异步返回 Promise\<object[]\>
     *
     * 返回对象数组的格式：
     *
     * ```javascript
     * [
     *     {
     *         bssid:string ,
     *         ssid:string ,
     *         channel:number ,
     *         rssi:number ,
     *         authmode:number ,
     *     } ,
     *     ...
     * ]
     * ```
     *
     * 示例：
     * ```javascript
     * import { scan } from "wifi"
     *
     * const aps = await scan()
     * aps.forEach(ap => {
     *     console.log(ap.ssid, ap.rssi, "dBm")
     * })
     * ```
     *
     * @module wifi
     * @function scan
     * @return Promise\<object[]\>
     */

    /**
     * 开始扫描附近的 AP
     *
     * WiFi STA 模式必须已启动。
     *
     * @module wifi
     * @function scanStart
     * @return boolean 返回 true 表示开始扫描成功，false 表示失败
     * @throws WiFi 未初始化
     */
    JSValue WiFi::scanStart(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
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
     * 停止 AP 扫描
     *
     * @module wifi
     * @function scanStop
     * @return boolean 返回 true 表示停止成功，false 表示失败
     * @throws WiFi 未初始化
     */
    JSValue WiFi::scanStop(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        CHECK_WIFI_INITED
        return esp_wifi_scan_stop() == ESP_OK? JS_TRUE : JS_FALSE;
    }
    
    /**
     * 返回 AP 扫描是否正在进行
     *
     * @module wifi
     * @function isScanning
     * @return boolean 返回 true 表示正在扫描中，false 表示未扫描
     * @throws WiFi 未初始化
     */
    JSValue WiFi::isScanning(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
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
    static const char * authmode_names(wifi_auth_mode_t auto_mode) {
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
     * 返回的数组格式：
     * ```javascript
     * [
     *     {
     *         bssid: string,
     *         ssid: string,
     *         channel: number,
     *         rssi: number,
     *         authmode: number,
     *     }
     * ]
     * ```
     *
     * @module wifi
     * @function scanRecords
     * @return object[] AP 列表
     * @throws WiFi 未初始化
     * @throws 内存分配失败
     */
    JSValue WiFi::scanRecords(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
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
            JSTHROW("out of memory?")
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

    /**
     * 返回 WiFi STA 模式是否已启动
     *
     * @module wifi
     * @function staStarted
     * @return boolean 返回 true 表示 STA 已启动，false 表示未启动
     */
    JSValue WiFi::staStarted(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        if(!wifi_inited)
            return JS_FALSE;
        return JS_NewBool(ctx, _sta_started) ;
    }
    /**
     * 返回 WiFi STA 是否已经连接到 AP
     *
     * @module wifi
     * @function staConnected
     * @return boolean 返回 true 表示已连接，false 表示未连接
     */
    JSValue WiFi::staConnected(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        if(!wifi_inited)
            return JS_FALSE;
        return JS_NewBool(ctx, _sta_connected) ;
    }
    /**
     * 返回 WiFi AP 模式是否已启用
     *
     * @module wifi
     * @function apStarted
     * @return boolean 返回 true 表示 AP 已启用，false 表示未启用
     */
    JSValue WiFi::apStarted(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        if(!wifi_inited)
            return JS_FALSE;
        return JS_NewBool(ctx, _ap_started) ;
    }

    /**
     * 检查是否支持 SoftAP (WiFi 热点) 功能
     *
     * 返回 `true` 表示支持 SoftAP，`false` 表示不支持。
     * 如果返回 `false`，则所有 AP 相关的 API（如 `startAP`、`setAPConfig`、`allSta` 等）都会抛出异常。
     *
     * @module wifi
     * @function isSoftAPSupported
     * @return boolean 返回 true 表示支持 SoftAP，false 表示不支持
     */
    JSValue WiFi::isSoftAPSupported(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
#ifdef CONFIG_ESP_WIFI_SOFTAP_SUPPORT
        return JS_TRUE ;
#else
        return JS_FALSE ;
#endif
    }

    /**
     * 设置 WiFi 接口的 MAC 地址
     *
     * 必须在 WiFi 启动前设置 MAC 地址。
     *
     * @module wifi
     * @function setMAC
     * @param ifname:string 接口名称，可选值为 "sta"、"ap"、"nan"
     * @param mac:string MAC 地址，格式为 "xx:xx:xx:xx:xx:xx"
     * @return undefined
     * @throws WiFi 已启动，无法设置 MAC 地址
     * @throws 参数数量不足
     * @throws 接口名称无效
     * @throws MAC 地址格式错误
     * @throws 设置 MAC 地址失败
     */
    JSValue WiFi::setMAC(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        if(_started) {
            JSTHROW("WiFi is already started, cannot set MAC address")
        }
        CHECK_ARGC(2)

        std::string ARGV_TO_STRING(0, ifname)
        std::string ARGV_TO_STRING(1, mac)

        wifi_interface_t ifac ;
        if(ifname=="sta") {
            ifac = WIFI_IF_STA ;
        }
        else if(ifname=="ap") {
            CHECK_SOFTAP_SUPPORT()
            ifac = WIFI_IF_AP ;
        }
        else if(ifname=="nan") {
            ifac = WIFI_IF_NAN ;
        }
        else {
            JSTHROW("ifname must be 'ap', 'sta' or 'nan'") ;
        }

        if(mac.length()<17) {
            JSTHROW("mac address must be a string like '12:34:56:78:9a:bc'") ;
        }

        uint8_t custom_mac[7] = {0xcc,0x1b,0xe0,0xe3,0xc0,0xfc};

        if (sscanf(mac.c_str(), "%02x:%02x:%02x:%02x:%02x:%02x",
            &custom_mac[0], &custom_mac[1], &custom_mac[2],
            &custom_mac[3], &custom_mac[4], &custom_mac[5]) != 6)
        {
            JSTHROW("mac address must be a string like '12:34:56:78:9a:bc'") ;
        }

        esp_err_t err = esp_wifi_set_mac(ifac, custom_mac) ;
        if(err != ESP_OK) {
            JSTHROW("esp_wifi_set_mac() failed: %d", err)
        }

        return JS_UNDEFINED ;
    }
    /**
     * 获取 WiFi 接口的 MAC 地址
     *
     * @module wifi
     * @function getMAC
     * @param ifname:string 接口名称，可选值为 "sta"、"ap"、"nan"
     * @return string MAC 地址，格式为 "xx:xx:xx:xx:xx:xx" (字母小写)
     * @throws 参数数量不足
     * @throws 接口名称无效
     * @throws 获取 MAC 地址失败
     */
    JSValue WiFi::getMAC(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        CHECK_ARGC(1)
        std::string ARGV_TO_STRING(0, ifname)

        wifi_interface_t ifac ;
        if(ifname=="sta") {
            ifac = WIFI_IF_STA ;
        }
        else if(ifname=="ap") {
            CHECK_SOFTAP_SUPPORT()
            ifac = WIFI_IF_AP ;
        }
        else if(ifname=="nan") {
            ifac = WIFI_IF_NAN ;
        }
        else {
            JSTHROW("ifname must be 'ap', 'sta' or 'nan'") ;
        }

        uint8_t mac[6] = {0,0,0,0,0,0} ;
        esp_err_t err = esp_wifi_get_mac(ifac, mac) ;
        if(err != ESP_OK) {
            JSTHROW("esp_wifi_get_mac() failed: %d", err)
        }

        char mac_str[18] = {0} ;
        snprintf(mac_str, sizeof(mac_str), "%02x:%02x:%02x:%02x:%02x:%02x", mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]) ;

        return JS_NewString(ctx, (const char *)mac_str) ;

    }
}

// ============================================================================
// 以下函数在 js/wifi.js 中实现，通过 exportValue 导出到 WiFi 模块
// ============================================================================

/**
 * 返回 WiFi 是否已准备好
 * 
 * 检查 WiFi 是否已经启动并处于可用状态。
 * 
 * 示例：
 * ```javascript
 * import * as wifi from "wifi"
 * 
 * if (wifi.isReady()) {
 *     console.log("WiFi is ready")
 * }
 * ```
 *
 * @module wifi
 * @function isReady
 * @return boolean 返回 true 表示 WiFi 已准备好，false 表示未准备好
 */

/**
 * 启动 WiFi
 * 
 * 异步启动 WiFi 硬件，返回 Promise。
 * `startAP` 和 `connect` 会自动调用此函数。
 * 
 * 示例：
 * ```javascript
 * import * as wifi from "wifi"
 * 
 * await wifi.start()
 * console.log("WiFi started!")
 * ```
 *
 * @module wifi
 * @function start
 * @return Promise\<undefined\>
 */

/**
 * 停止 WiFi
 * 
 * 异步停止 WiFi 硬件，返回 Promise。
 * 
 * 示例：
 * ```javascript
 * import * as wifi from "wifi"
 * 
 * await wifi.stop()
 * console.log("WiFi stopped!")
 * ```
 *
 * @module wifi
 * @function stop
 * @return Promise\<undefined\>
 */

/**
 * 连接到 WiFi 热点
 * 
 * 作为 STA 模式连接到指定的 WiFi 热点。
 * 如果 retry 为负数，则会启动守护进程自动重连。
 * 
 * 示例：
 * ```javascript
 * import * as wifi from "wifi"
 * 
 * // 基本连接
 * const connected = await wifi.connect("MyWiFi", "password123")
 * if (connected) {
 *     console.log("Connected!")
 * }
 * 
 * // 使用配置对象
 * await wifi.connect({
 *     ssid: "MyWiFi",
 *     password: "password123",
 *     retry: 3,
 *     retryDur: 5000
 * })
 * 
 * // 启动守护进程（retry < 0）
 * await wifi.connect("MyWiFi", "password123", -1)
 * ```
 *
 * @module wifi
 * @function connect
 * @param ssid:string|object WiFi 热点名称或配置对象
 * @param password:string WiFi 热点密码（当 ssid 为字符串时）
 * @param retry:number=3 连接失败重试次数，负数表示启动守护进程
 * @param retryDur:number=5000 连接重试间隔时间，单位毫秒
 * @return Promise\<boolean\> 返回 true 表示连接成功，false 表示失败
 */

/**
 * 断开 WiFi 连接
 * 
 * 断开 STA 模式的 WiFi 连接，并停止守护进程（如果正在运行）。
 * 
 * 示例：
 * ```javascript
 * import * as wifi from "wifi"
 * 
 * await wifi.disconnect()
 * console.log("Disconnected!")
 * ```
 *
 * @module wifi
 * @function disconnect
 * @param dontStopDeamon:boolean=false 如果为 true，则不停止守护进程
 * @return Promise\<undefined\>
 */

/**
 * 返回是否正在连接中
 * 
 * @module wifi
 * @function isConnecting
 * @return boolean 返回 true 表示正在连接中，false 表示未连接
 */

/**
 * 启动 WiFi 热点（AP 模式）
 * 
 * 启动一个 WiFi 热点，供其他设备连接。
 * 
 * 示例：
 * ```javascript
 * import * as wifi from "wifi"
 * 
 * const started = await wifi.startAP("MyAP", "password123")
 * if (started) {
 *     console.log("AP started!")
 * }
 * ```
 *
 * @module wifi
 * @function startAP
 * @param ssid:string 热点名称
 * @param password:string 热点密码
 * @return Promise\<boolean\> 返回 true 表示启动成功，false 表示失败
 */

/**
 * 停止 WiFi 热点
 * 
 * 停止 AP 模式的 WiFi 热点。
 * 
 * 示例：
 * ```javascript
 * import * as wifi from "wifi"
 * 
 * const stopped = await wifi.stopAP()
 * if (stopped) {
 *     console.log("AP stopped!")
 * }
 * ```
 *
 * @module wifi
 * @function stopAP
 * @return Promise\<boolean\> 返回 true 表示停止成功，false 表示失败
 */

/**
 * 返回 WiFi 状态
 * 
 * 返回 AP 和/或 STA 的详细状态信息。
 * 
 * 示例：
 * ```javascript
 * import * as wifi from "wifi"
 * 
 * // 获取所有状态
 * const status = wifi.status()
 * console.log(status.sta)
 * console.log(status.ap)
 * 
 * // 仅获取 STA 状态
 * const staStatus = wifi.status("sta")
 * console.log(staStatus.ip)
 * ```
 *
 * @module wifi
 * @function status
 * @param netif:string="apsta" 网络接口类型，可选值为 "apsta"、"sta"、"ap"
 * @return object 状态对象
 */

/**
 * 启动 STA 守护进程
 * 
 * 当 WiFi 连接断开时，守护进程会自动尝试重新连接。
 * 通常在 `connect()` 函数中设置 retry 为负数时会自动启动守护进程。
 * 
 * @module wifi
 * @function startStaDeamon
 * @param dur:number=10000 检查间隔时间，单位毫秒，默认 10000ms
 */

/**
 * 停止 STA 守护进程
 * 
 * 停止自动重连功能。`disconnect()` 函数会自动调用此函数。
 * 
 * @module wifi
 * @function stopStaDeamon
 */

/**
 * 返回 STA 守护进程是否正在运行
 * 
 * @module wifi
 * @function isStaDeamonRunning
 * @return boolean 返回 true 表示守护进程正在运行，false 表示未运行
 */

/**
 * 扫描周围的 WiFi 热点
 * 
 * 扫描并返回周围可用的 WiFi 热点列表。
 * 
 * 示例：
 * ```javascript
 * import * as wifi from "wifi"
 * 
 * const aps = await wifi.scan()
 * aps.forEach(ap => {
 *     console.log(ap.ssid, ap.rssi, "dBm")
 * })
 * ```
 *
 * @module wifi
 * @function scan
 * @return Promise\<object[]\> AP 列表，每个对象包含 ssid、rssi、authmode 等属性
 */

/**
 * 等待获取 IP 地址
 * 
 * 等待分配 IP 地址，返回包含 IP 信息的状态对象，对象格式参考 `wifi.status("apsta")`
 * 
 * 如果连接断开，返回 false。
 * 
 * 如果已经获得IP，则直接返回，不需要等待。
 * 
 * > 连接到 WiFi 以后，还需要分配一个 IP 地址才能通讯。通常在 `await connect()` 之后 `await waitIP()`。
 * 
 * 示例：
 * ```javascript
 * import * as wifi from "wifi"
 * 
 * await wifi.connect("MyWiFi", "password123")
 * const ipInfo = await wifi.waitIP()
 * if (ipInfo) {
 *     console.log("IP:", ipInfo.ip)
 * }
 * 
 * // @todo 
 * // 现在可以请求服务器了
 * // ... ...
 * 
 * ```
 *
 * @module wifi
 * @function waitIP
 * @return Promise\<object|false\> 成功返回状态对象，失败返回 false
 */
