#pragma once

#include "NativeModule.hpp"

namespace be {

    class BeShell ;

    class WiFi: public be::NativeModule {
    public:
        static const char * const name ;

        WiFi(JSContext * ctx, const char * name) ;

        static bool hasInited() ;
        void exports(JSContext *ctx) ;

        static void use(be::BeShell * beshell) ;

        static void init() ;

        static JSValue start(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue stop(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue setPS(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue setMode(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue mode(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue setAPConfig(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue setStaConfig(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue config(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue peripheralConnect(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue peripheralDisconnect(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue getIpInfo(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue setHostname(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue allSta(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue registerEventHandle(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;

        static JSValue scanStart(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue scanStop(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue isScanning(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue scanRecords(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;

        static JSValue staStarted(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue staConnected(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue apStarted(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;

    } ;
}