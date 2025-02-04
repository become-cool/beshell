#pragma once

#include "NativeModule.hpp"

namespace be::nimble {

    // 从 100 开始，大于 BLE_GAP_EVENT_XX 事件
    #define JS_NIMBLE_EVENT_DISC_ALL 200  // 发现所有服务、特征、cccd

    struct js_nimble_event {
        uint8_t type ;
        union {
            struct {
                int status ;
                struct peer * peer ;
            } disc_all ;
        }  ;
    } ;

    class NimBLE: public be::EventModule {
    public:
        static const char * const name ;

        NimBLE(JSContext * ctx, const char * name) ;

        // void import(JSContext *ctx) ;

        static void use(be::BeShell * beshell) ;

        void onNativeEvent(JSContext *ctx, void * param) ;

        static JSValue init(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue setName(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue startScan(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue stopScan(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue isScaning(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;

        static JSValue connect(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue disconnect(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
    } ;
}
