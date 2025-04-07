#pragma once

#include "sdkconfig.h"
#if CONFIG_BT_BLUEDROID_ENABLED

#include "BeShell.hpp"
#include "NativeModule.hpp"
#include "esp_gattc_api.h"
#include "esp_gattS_api.h"
#include "esp_gap_ble_api.h"

namespace be{

    struct bt_event {
        uint16_t event ;
        esp_gatt_if_t gatt_if ;
        union {
            esp_ble_gap_cb_param_t gap ;
            esp_ble_gattc_cb_param_t gattc ;
            esp_ble_gatts_cb_param_t gatts ;
        };
        struct {
            void * ptr ;
            uint16_t len ;
        } data ;
    } ;
    
    typedef bool (* gap_handler_t)(esp_gap_ble_cb_event_t event, esp_ble_gap_cb_param_t *param);
    typedef bool (* gattc_handler_t)(esp_gattc_cb_event_t event, esp_gatt_if_t gattc_if, esp_ble_gattc_cb_param_t *param);
    typedef bool (* gatts_handler_t)(esp_gatts_cb_event_t event, esp_gatt_if_t gatts_if, esp_ble_gatts_cb_param_t *param);

    class BT: public be::EventModule {

    public:
        static char const * const name ;
        static BT * singleton ;
        static bool bScanning ;

        BT(JSContext * ctx, const char * name) ;

        void onNativeEvent(JSContext *ctx, void * param) ;
        
        static void use(be::BeShell * beshell) ;

        static JSValue initCentral(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue initPeripheral(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue startScan(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue stopScan(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue setScanParam(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue isScanning(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue waitScanning(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue setAdvName(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue setAdvData(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue startAdv(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue stopAdv(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue setMTU(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue requestMTU(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue connect(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue disconnect(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue search(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue read(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue write(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue subscribe(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;

        static JSValue addService(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue addChar(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue sendNotify(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        
        static JSValue setPower(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue power(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;

        static char * bt_uuid_to_string(const esp_bt_uuid_t *uuid, char *str, size_t str_size) ;
        static esp_bt_uuid_t bt_string_to_uuid(char *str, size_t str_size) ;

        static void setGapHandler(gap_handler_t handler) ;
        static void setGattcHandler(gattc_handler_t handler) ;
        static void setGattsHandler(gatts_handler_t handler) ;


    protected:
        virtual void exports(JSContext *ctx) ;
    private :
        static JSValue getCharacteristics(JSContext *ctx, esp_gatt_if_t gattc_if, uint16_t conn_id, uint16_t start_handle, uint16_t end_handle) ;
        
        static bool init() ;
        static bool initCentral() ;
        static bool initPeripheral() ;

        
        static void gap_event_handler(esp_gap_ble_cb_event_t event, esp_ble_gap_cb_param_t *param) ;
        static void gattc_event_handler(esp_gattc_cb_event_t event, esp_gatt_if_t gattc_if, esp_ble_gattc_cb_param_t *param) ;
        static void gatts_event_handler(esp_gatts_cb_event_t event, esp_gatt_if_t gatts_if, esp_ble_gatts_cb_param_t *param) ;

        void onNativeCentralEvent(JSContext *ctx, bt_event * event) ;
        void onNativePeripheralEvent(JSContext *ctx, bt_event * event) ;

        static gap_handler_t gapHandler ;
        static gattc_handler_t gattcHandler ;
        static gatts_handler_t gattsHandler ;
    } ;
}

#endif
