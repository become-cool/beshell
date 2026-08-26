#pragma once
#include "../NativeModule.hpp"

namespace be {
    class Sleep: public be::NativeModule {
    public:
        Sleep(JSContext * ctx, const char * name, uint8_t flagGlobal=0) ;

        static JSValue light(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue deep(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue modem(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;

        static JSValue enableTimerWakeup(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue enableExt0Wakeup(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue enableExt1Wakeup(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue enableLightGPIOWakeup(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue enableLightUARTWakeup(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue enableLightWiFiWakeup(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue enableLightBTWakeup(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue enableDeepGPIOWakeup(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue enableTouchWakeup(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue enableULPWakeup(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue disableWakeupSource(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;

        static JSValue wakeupCause(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue ext1WakeupPins(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue isValidWakeupGPIO(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
    } ;
}
