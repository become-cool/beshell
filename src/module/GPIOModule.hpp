#pragma once

#include "BeShell.hpp"
#include "NativeModule.hpp"

namespace be {


    class GPIOModule: public be::NativeModule {
    public:
        GPIOModule(JSContext * ctx, const char * name) ;

        void import(JSContext *ctx) ;

        static JSValue mode(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue setMode(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue pull(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;

        static JSValue write(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue read(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;

        static JSValue readAnalog(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue writeAnalog(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue adcSetBits(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue adcSetChannelAtten(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;

        static JSValue writePWM(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue readPWM(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;

        static JSValue watch(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue unwatch(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
    } ;
}


