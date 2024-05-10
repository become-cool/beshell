#pragma once

#include "deps/quickjs/quickjs-libc.h"
#include <vector>
#include <initializer_list>

#if defined(ESP_PLATFORM)
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "freertos/semphr.h"
#endif

namespace be {
    class JSTimerEvent ;

    class JSTimer {
    private:
        std::vector<JSTimerEvent*> events ;
        uint32_t lastTimerId = 0 ;

#if defined(ESP_PLATFORM)
        QueueHandle_t timer_queue = nullptr ;
        SemaphoreHandle_t xMutex = nullptr ;
#endif
        inline bool take(bool fromISR=false, bool createWhenFirst=false) ;
        inline void give(bool fromISR=false) ;

    public:
        void setup(JSContext * ctx) ;
        void loop(JSContext * ctx) ;
        
        static JSValue jsSetTimeout(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue jsSetInterval(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue jsSetImmediate(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue jsClearTimeout(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        
        JSTimerEvent * setTimer(JSContext *ctx, JSValue func, int interval, bool repeat=false, JSValue thisobj=JS_UNDEFINED, int argc=0, JSValueConst *argv=nullptr) ;
        JSTimerEvent * setTimerAsync(JSContext *ctx, JSValue func, int interval, bool repeat=false, JSValue thisobj=JS_UNDEFINED, int argc=0, JSValueConst *argv=nullptr, bool fromISR=false) ;
        // JSTimerEvent * setTimer(JSContext *ctx, JSValue func, int interval, bool repeat=false, JSValue thisobj=JS_UNDEFINED, std::initializer_list<JSValue> argv={}) ;

        JSTimerEvent * setImmediate(JSContext *ctx, JSValue func, JSValue thisobj, int argc, JSValueConst *argv) ;
        
#if defined(ESP_PLATFORM)
        JSTimerEvent * setImmediateAsync(JSContext *ctx, JSValue func, JSValue thisobj, int argc, JSValueConst *argv) ;
#endif
        
        void removeTimer(JSContext *ctx, JSTimerEvent *) ;
        void removeTimer(JSContext *ctx, uint32_t id) ;
        JSTimerEvent * findWithId(uint32_t id) ;
    } ;
}