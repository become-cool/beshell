#pragma once

#include "NativeModule.hpp"

namespace be {
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
    } ;
}
