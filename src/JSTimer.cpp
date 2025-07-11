#include "JSTimer.hpp"
#include "BeShell.hpp"
#include "JSEngine.hpp"
#include "qjs_utils.h"
#include <cassert>

using namespace std;

#define _MUTEX(fromISR, createWhenFirst, critical)     \
    {                                                  \
        bool toke = take(fromISR, createWhenFirst) ;   \
        critical                                       \
        if(toke) {                                     \
            give(fromISR) ;                            \
        }                                              \
    }

#define MUTEX(critical)        _MUTEX(false, false, critical)
#define MUTEX_ISR(critical)    _MUTEX(true, true,  critical)


namespace be {

    class JSTimerEvent {
    public :
        JSValue func ;
        JSValue thisobj ;
        int argc ;
        JSValueConst * argv ;

        uint64_t deadline ;
        uint64_t interval ;
        bool repeat:1 ;
        bool requestAnimationFrame:1 ;
        bool debug:1 ;
        uint32_t id ;
    
        void destroy (JSContext * ctx) {
            JS_FreeValue(ctx, func) ;
            JS_FreeValue(ctx, thisobj) ;
            if(argc>0 && argv) {
                for(int i=0;i<argc; i++) {
                    JS_FreeValue(ctx, argv[i]) ;
                }
                delete[] argv ;
                argv = nullptr ;
                argc = 0 ;
            }
        }
        
        friend class JSTimer ;
    } ;

    void JSTimer::setup(JSContext * ctx) {
        JSValue global = JS_GetGlobalObject(ctx);

        JS_SetPropertyStr(ctx, global, "setTimeout", JS_NewCFunction(ctx, jsSetTimeout, "setTimeout", 2));
        JS_SetPropertyStr(ctx, global, "setInterval", JS_NewCFunction(ctx, jsSetInterval, "setInterval", 2));
        JS_SetPropertyStr(ctx, global, "setImmediate", JS_NewCFunction(ctx, jsSetImmediate, "setImmediate", 1));
        JS_SetPropertyStr(ctx, global, "clearTimeout", JS_NewCFunction(ctx, jsClearTimeout, "clearTimeout", 1));
        JS_SetPropertyStr(ctx, global, "clearInterval", JS_NewCFunction(ctx, jsClearTimeout, "clearInterval", 1));
        JS_SetPropertyStr(ctx, global, "clearImmediate", JS_NewCFunction(ctx, jsClearTimeout, "clearImmediate", 1));

        JS_FreeValue(ctx,global) ;
    }

    void JSTimer::loop(JSContext * ctx) {
        take(false) ;
        if(events.size()<1){
            give(false) ;
            return ;
        }
        uint64_t now = gettime() ;

        for(auto event: events) {
            if(event->deadline <= now) {
                
                JSValue ret = JS_Call(ctx, event->func, event->thisobj, event->argc, event->argv) ;
                if( JS_IsException(ret) ) {
                    js_std_dump_error(ctx) ;
                }
                JS_FreeValue(ctx, ret) ;

                // 一次性任务
                if(!event->repeat) {
                    removeTimer(ctx, event) ;
                    continue ;
                }

                // 连续重复任务
                if(event->interval==0) {
                    event->deadline = now ;
                }
                // 间隔重复
                else {
                    event->deadline+= event->interval ;
                }

            }
        }
        
        give(false) ;
    }

    #define CHECK_ENGINE                                     \
        JSEngine * engine = JSEngine::fromJSContext(ctx) ;  \
        assert(engine) ;

    static inline JSValue __js_set_timeout(JSContext *ctx, int argc, JSValueConst *argv, bool repeat){
        CHECK_ENGINE
        ASSERT_ARGC(2)

        if(!JS_IsFunction(ctx, argv[0])) {
            JSTHROW("arg callback is not a function")
        }
        uint32_t interval ;
        if(JS_ToUint32(ctx, &interval, argv[1]) ) {
            JSTHROW("Invalid param type")
        }

        JSTimerEvent * event = engine->timer.setTimer(
            ctx, (JSValue)argv[0], interval, repeat
        ) ;

        return JS_NewInt32(ctx, event->id) ;
    }

    JSValue JSTimer::jsSetTimeout(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv){
        return __js_set_timeout(ctx, argc, argv, false) ;
    }

    JSValue JSTimer::jsSetInterval(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv){
        return __js_set_timeout(ctx, argc, argv, true) ;
    }

    JSValue JSTimer::jsSetImmediate(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv){
        CHECK_ENGINE
        ASSERT_ARGC(1)
        if(!JS_IsFunction(ctx, argv[0])) {
            JSTHROW("arg callback is not a function")
        }

        JSTimerEvent * event = engine->timer.setTimer(
            ctx, (JSValue)argv[0], 0, false
        ) ;

        return JS_NewInt32(ctx, event->id) ;
    }
    JSValue JSTimer::jsClearTimeout(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv){
        CHECK_ENGINE
        ASSERT_ARGC(1)
        ARGV_TO_UINT32(0, id)
        engine->timer.removeTimer(ctx, id) ;
        return JS_UNDEFINED ;
    }
    
    
    void JSTimer::updateTime(int64_t ms) {
        
        uint64_t now = gettime() ;
        ms-= now ;

        for(auto event: events) {
            event->deadline += ms ;
        }
    }
    
    bool JSTimer::take(bool fromISR, bool createWhenFirst) {
#ifdef ESP_PLATFORM
        bool toke = false ; 
        if(!xMutex) {
            if(createWhenFirst) {
                xMutex = xSemaphoreCreateMutex();
            }else{
                return false ;
            }
        }
        if(xMutex) {
            if(fromISR) {
                if(xSemaphoreTakeFromISR(xMutex, 0) == pdTRUE) {
                    toke = true ;
                }
            } else {
                if(xSemaphoreTake(xMutex, portMAX_DELAY) == pdTRUE) {
                    toke = true ;
                }
            }
        }
        return toke ;
#else
        return true ;
#endif
    }
    void JSTimer::give(bool fromISR) {
#ifdef ESP_PLATFORM
        if(!xMutex) {
            return ;
        }
        if(fromISR) {
            xSemaphoreGiveFromISR(xMutex, nullptr);
        } else {
            xSemaphoreGive(xMutex);
        }
#endif
    }
    
    JSTimerEvent * JSTimer::setTimer(JSContext *ctx, JSValue func, int interval, bool repeat, JSValue thisobj, int argc, JSValueConst *argv) {
        JSTimerEvent * event = new JSTimerEvent ;
        memset(event,0,sizeof(JSTimerEvent)) ;

        event->func = JS_DupValue(ctx, func) ;
        event->thisobj = JS_DupValue(ctx, thisobj) ;
        event->interval = interval ;
        event->repeat = repeat ;
        event->requestAnimationFrame = false ;
        event->deadline = gettime() + interval ;
        event->debug = 0 ;

        // @todo 避免重复
        lastTimerId ++ ;
        if(lastTimerId==0) {
            lastTimerId = 1 ;
        }
        event->id = lastTimerId ;

        if(argc>0 && argv) {
            event->argc = argc ;
            event->argv = new JSValue[argc] ;
            for(int i=0;i<argc; i++) {
                event->argv[i] = JS_DupValue(ctx, argv[i]) ;
            }
        }

        MUTEX({
            events.push_back(event) ;
        })

        return event ;
    }

    JSTimerEvent * JSTimer::setTimerAsync(JSContext *ctx, JSValue func, int interval, bool repeat, JSValue thisobj, int argc, JSValueConst *argv, bool fromISR) {
        JSTimerEvent * event = nullptr ;
        MUTEX_ISR({
            event = setTimer(ctx, func, interval, repeat,  thisobj, argc, argv) ;
        })
        return event ;
    }
    
    void JSTimer::removeTimer(JSContext *ctx, JSTimerEvent * event) {
        MUTEX({
            auto it = std::find(events.begin(),events.end(),event) ;
            if(it==events.end()) {
                give(false) ;
                return ;
            }
            events.erase(it);
        })
        event->destroy(ctx) ;
        delete event ;
    }
    
    void JSTimer::removeTimer(JSContext *ctx, uint32_t id) {
        JSTimerEvent * event = findWithId(id) ;
        if(event) {
            removeTimer(ctx, event) ;
        }
    }

    JSTimerEvent * JSTimer::findWithId(uint32_t id) {
        MUTEX({
            for(auto event: events) {
                if(event->id == id) {
                    give(false) ;
                    return event ;
                }
            }
        })
        return nullptr ;
    }

    
#if defined(ESP_PLATFORM)
    #define TIMER_QUEUE_LEN 10
    JSTimerEvent * JSTimer::setImmediateAsync(JSContext *ctx, JSValue func, JSValue thisobj, int argc, JSValueConst *argv) {
        if(timer_queue==nullptr) {
            // timer_queue = xQueueCreate(TIMER_QUEUE_LEN, sizeof(JSTimerEvent *));
        }
        // @todo
        return nullptr ;
    }
#endif
}