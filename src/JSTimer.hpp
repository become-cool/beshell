#pragma once

#include "deps/quickjs/quickjs-libc.h"
#include <vector>
#include <initializer_list>

namespace be {
    class JSTimerEvent ;

    class JSTimer {
    private:
        std::vector<JSTimerEvent*> events ;
        uint32_t lastTimerId = 0 ;

    public:
        void setup(JSContext * ctx) ;
        void loop(JSContext * ctx) ;
        
        static JSValue jsSetTimeout(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue jsSetInterval(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue jsSetImmediate(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue jsClearTimeout(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        
        JSTimerEvent * setTimer(JSContext *ctx, JSValue func, int interval, bool repeat=false, JSValue thisobj=JS_UNDEFINED, int argc=0, JSValueConst *argv=nullptr) ;
        // JSTimerEvent * setTimer(JSContext *ctx, JSValue func, int interval, bool repeat=false, JSValue thisobj=JS_UNDEFINED, std::initializer_list<JSValue> argv={}) ;

        JSTimerEvent * setImmediate(JSContext *ctx, JSValue func, JSValue thisobj, int argc, JSValueConst *argv) ;
        
        void removeTimer(JSContext *ctx, JSTimerEvent *) ;
        void removeTimer(JSContext *ctx, uint32_t id) ;
        JSTimerEvent * findWithId(uint32_t id) ;
    } ;
}