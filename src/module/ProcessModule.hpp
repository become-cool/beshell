#pragma once
#include "NativeModule.hpp"

namespace be {
    class ProcessModule: public NativeModule {
    public:
        ProcessModule(JSContext * ctx, const char * name,uint8_t flagGlobal=1) ;

        void import(JSContext *ctx) ;
        
        static JSValue reboot(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue top(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue usage(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue setTime(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
    } ;
}