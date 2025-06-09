#pragma once

#include "BeShell.hpp"
#include "NativeModule.hpp"
#include "driver/gpio.h"

namespace be {
 
    class GPIO: public be::NativeModule {
    public:
        GPIO(JSContext * ctx, const char * name) ;
        static void loop(JSContext *, void * arg) ;

        void exports(JSContext *ctx) ;
        
        static bool installISR(int flag=0) ;
        static void uninstallISR() ;

        static JSValue mode(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue setMode(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue pull(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;

        static JSValue write(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue read(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;

        static JSValue apiConfigPWM(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue apiWritePWM(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue apiUpdatePWM(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue apiStopPWM(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue pwmMaxSpeedMode(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;

        static JSValue watch(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue unwatch(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;

        static JSValue adcUnitInit(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue adcChannelInit(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue adcRead(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue adcInfo(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        
        static JSValue resetPin(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;

        static JSValue test(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
    
    private:
        static bool isr_installed ;
    } ;
}


