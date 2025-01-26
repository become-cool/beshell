#pragma once

#include "NativeModule.hpp"

namespace be {

    struct js_nimble_event {
        int type ;
        union {
            struct ble_gap_event * raw_event ;
            struct peer * peer ;
        } ;
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
        static JSValue discSvc(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue discChar(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;

        static JSValue read(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue write(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue subscribe(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
    } ;
}
