#include "module_bt.h"
#include "utils.h"
#include <string.h>

#ifdef CONFIG_BT_ENABLED

#include "esp_bt.h"

#ifdef CONFIG_BLUEDROID_ENABLED
#include "esp_bt_main.h"
#include "esp_gap_bt_api.h"
#endif

// #include "esp_gap_ble_api.h"
// #include "nimble/nimble_npl.h"
// #include "host/ble_hs.h"

// #define DEVICE_NAME "My Bluetooth Device"
// #define SVC_UUID    BLE_UUID16_DECLARE(0x180F)

// static void ble_app_advertise(void)
// {
//     struct ble_hs_adv_fields fields;
//     memset(&fields, 0, sizeof(fields));
//     fields.flags = BLE_HS_ADV_F_DISC_GEN | BLE_HS_ADV_F_BREDR_UNSUP;

//     uint8_t svc_uuid[16];
//     ble_uuid_to_flat(&SVC_UUID.u, svc_uuid);
//     fields.uuids128 = svc_uuid;
//     fields.num_uuids128 = 1;

//     fields.name = (uint8_t *) DEVICE_NAME;
//     fields.name_len = strlen(DEVICE_NAME);
//     fields.name_is_complete = 1;

//     ble_hs_adv_set_fields(&fields);

//     struct ble_gap_adv_params adv_params;
//     memset(&adv_params, 0, sizeof(adv_params));
//     adv_params.conn_mode = BLE_GAP_CONN_MODE_NON;
//     ble_gap_adv_start(BLE_OWN_ADDR_PUBLIC, NULL, BLE_HS_FOREVER, &adv_params, NULL, NULL);
// }

// static int ble_app_gap_event(struct ble_gap_event *event, void *arg)
// {
//     switch (event->type) {
//         case BLE_GAP_EVENT_CONNECT:
//             printf("Connected to remote device\n");
//             break;

//         case BLE_GAP_EVENT_DISCONNECT:
//             printf("Disconnected from remote device\n");
//             ble_app_advertise();
//             break;

//         case BLE_GAP_EVENT_DISC:
//             printf("Received discovery result event\n");
//             break;

//         case BLE_GAP_EVENT_ADV_COMPLETE:
//             printf("Advertising stopped\n");
//             ble_app_advertise();
//             break;

//         case BLE_GAP_EVENT_SUBSCRIBE:
//             printf("Received subscribe event\n");
//             break;

//         case BLE_GAP_EVENT_MTU:
//             printf("Received MTU exchange complete event\n");
//             break;

//         default:
//             printf("Unknown GAP event type: %d\n", event->type);
//             break;
//     }

//     return 0;
// }

// static void ble_app_on_sync(void)
// {
//     printf("Bluetooth initialized\n");

//     ble_hs_cfg.sync_cb = ble_app_gap_event;

//     ble_app_advertise();
// }

// int main()
// {
//     esp_bt_controller_config_t bt_cfg = BT_CONTROLLER_INIT_CONFIG_DEFAULT();
//     esp_bt_controller_init(&bt_cfg);
//     esp_bt_controller_enable(ESP_BT_MODE_BTDM);

//     nimble_port_init();

//     ble_hs_cfg.sync_cb = ble_app_on_sync;

//     nimble_port_freertos_init(ble_hs_thread_ev, NULL);

//     vTaskStartScheduler();

//     return 0;
// }


#define ARGV_BT_MODE(i,var)     \
    ARGV_TO_UINT8(i, var)       \
    if(var!=ESP_BT_MODE_BLE && var!=ESP_BT_MODE_CLASSIC_BT) {         \
        THROW_EXCEPTION("argv mode must be 1 (ble), 2 (class bt)")  \
    }

#define CALL_IDF_API(api, failed_msg, ...)              \
        esp_err_t err = api(__VA_ARGS__) ;            \
        if (err != ESP_OK) {                                    \
            THROW_EXCEPTION(failed_msg, esp_err_to_name(err));  \
        }                                                       \
        

#define API_IDF2JS_NO_ARGV(jsfunc, api, failed_msg, ...)                    \
    static JSValue jsfunc(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) { \
        CALL_IDF_API(api, failed_msg, ##__VA_ARGS__)                   \
        return JS_TRUE ;                                   \
    }


static JSValue js_bt_controller_init_as_default(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    CHECK_ARGC(1)
    ARGV_BT_MODE(0, mode)
    esp_bt_controller_config_t bt_cfg = BT_CONTROLLER_INIT_CONFIG_DEFAULT();
#ifdef CONFIG_IDF_TARGET_ESP32S3
    bt_cfg.bluetooth_mode = mode ;
#else
    bt_cfg.mode = mode ;
#endif
    CALL_IDF_API(esp_bt_controller_init, "bt controller init failed: %s", &bt_cfg)
    return JS_TRUE ;
}

API_IDF2JS_NO_ARGV(js_bt_controller_deinit, esp_bt_controller_deinit, "bt controller deinit failed: %s")

static JSValue js_bt_controller_mem_release(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    CHECK_ARGC(1)
    ARGV_TO_UINT8(0, mode)
    CALL_IDF_API(esp_bt_controller_mem_release, "controller mem release failed: %s", mode)
    return JS_TRUE ;
}

static JSValue js_bt_controller_enable(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    CHECK_ARGC(1)
    ARGV_BT_MODE(0, mode)
    CALL_IDF_API(esp_bt_controller_enable, "controller enable failed: %s", mode)
    return JS_TRUE ;
}

static JSValue js_bt_controller_get_status(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    esp_bt_controller_status_t status = esp_bt_controller_get_status() ;
    return JS_NewInt32(ctx, status) ;
}

API_IDF2JS_NO_ARGV(js_bt_controller_disable, esp_bt_controller_disable, "controller disable failed: %s")

#ifdef CONFIG_BLUEDROID_ENABLED
API_IDF2JS_NO_ARGV(js_bt_bluedroid_init, esp_bluedroid_init, "bluedroid init failed: %s")
API_IDF2JS_NO_ARGV(js_bt_bluedroid_deinit, esp_bluedroid_deinit, "bluedroid deinit failed: %s")
API_IDF2JS_NO_ARGV(js_bt_bluedroid_enable, esp_bluedroid_enable, "bluedroid enable failed: %s")
API_IDF2JS_NO_ARGV(js_bt_bluedroid_disable, esp_bluedroid_disable, "bluedroid disable failed: %s")

static JSValue js_bt_bluedroid_get_status(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    esp_bluedroid_status_t status = esp_bluedroid_get_status() ;
    return JS_NewInt32(ctx, status) ;
}
#endif


// static JSValue js_bt_set_pin(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
//     CHECK_ARGC(1)
//     ARGV_TO_STRING_LEN_E(0, pin, pinlen, "argv len must be a string")

//     if(pinlen!=4) {
//         JS_FreeCString(ctx, pin) ;
//         THROW_EXCEPTION("pin length must be 4")
//     }

//     esp_bt_pin_type_t pin_type = ESP_BT_PIN_TYPE_FIXED;
//     esp_bt_pin_code_t pin_code;
//     memcpy(pin_code, pin, 4) ;
//     esp_bt_gap_set_pin(pin_type, 4, pin_code);

//     JS_FreeCString(ctx, pin) ;
//     return JS_UNDEFINED ;
// }

void be_module_bt_init() {
}

void be_module_bt_require(JSContext *ctx) {

    JSValue beapi = js_get_glob_prop(ctx, 1, "beapi") ;
    JSValue bt = JS_NewObject(ctx);
    JS_SetPropertyStr(ctx, beapi, "bt", bt);

    JS_SetPropertyStr(ctx, bt, "controllerMemRelease", JS_NewCFunction(ctx, js_bt_controller_mem_release, "controllerMemRelease", 1));
    JS_SetPropertyStr(ctx, bt, "controllerInitAsDefault", JS_NewCFunction(ctx, js_bt_controller_init_as_default, "controllerInitAsDefault", 1));
    JS_SetPropertyStr(ctx, bt, "controllerDeinit", JS_NewCFunction(ctx, js_bt_controller_deinit, "controllerDeinit", 1));
    JS_SetPropertyStr(ctx, bt, "controllerEnable", JS_NewCFunction(ctx, js_bt_controller_enable, "controllerEnable", 1));
    JS_SetPropertyStr(ctx, bt, "controllerDisable", JS_NewCFunction(ctx, js_bt_controller_disable, "controllerDisable", 1));
    JS_SetPropertyStr(ctx, bt, "controllerStatus", JS_NewCFunction(ctx, js_bt_controller_get_status, "controllerStatus", 1));
    
#ifdef CONFIG_NIMBLE_ENABLED

#endif

#ifdef CONFIG_BLUEDROID_ENABLED
    JS_SetPropertyStr(ctx, bt, "bluedroidInit", JS_NewCFunction(ctx, js_bt_bluedroid_init, "bluedroidInit", 1));
    JS_SetPropertyStr(ctx, bt, "bluedroidDeinit", JS_NewCFunction(ctx, js_bt_bluedroid_deinit, "bluedroidDeinit", 1));
    JS_SetPropertyStr(ctx, bt, "bluedroidEnable", JS_NewCFunction(ctx, js_bt_bluedroid_enable, "bluedroidEnable", 1));
    JS_SetPropertyStr(ctx, bt, "bluedroidDisable", JS_NewCFunction(ctx, js_bt_bluedroid_disable, "bluedroidDisable", 1));
    JS_SetPropertyStr(ctx, bt, "bluedroidStatus", JS_NewCFunction(ctx, js_bt_bluedroid_get_status, "bluedroidStatus", 1));
#endif

    JS_FreeValue(ctx, beapi);
}

void be_module_bt_loop(JSContext *ctx) {
}

void be_module_bt_reset(JSContext *ctx) {
}









#endif