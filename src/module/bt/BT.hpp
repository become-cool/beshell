#pragma once

#include "BeShell.hpp"
#include "NativeModule.hpp"
#include "esp_gattc_api.h"
#include "esp_gap_ble_api.h"

namespace be{

    struct be_bt_event {
        uint16_t event ;
        esp_gatt_if_t gattc_if ;
        union {
            esp_ble_gap_cb_param_t gap ;
            esp_ble_gattc_cb_param_t gatt ;
        };
    } ;
    

    class BT: public be::EventModule {

    public:
        static const char * const name ;
        static BT * singleton ;
        static bool bScanning ;

        BT(JSContext * ctx, const char * name) ;

        void onNativeEvent(JSContext *ctx, void * param) ;
        
        static void use(be::BeShell * beshell) ;

        static JSValue init(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue startScan(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue stopScan(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue setScanParam(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue isScaning(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue setMTU(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue requestMTU(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue connect(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue disconnect(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue search(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue read(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue write(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue subscribe(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
    protected:
        virtual void exports(JSContext *ctx) ;
    private :
        static JSValue getCharacteristics(JSContext *ctx, esp_gatt_if_t gattc_if, uint16_t conn_id, uint16_t start_handle, uint16_t end_handle) ;
    } ;
}
