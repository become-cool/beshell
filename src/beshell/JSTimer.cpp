#include "JSTimer.hpp"
#include "BeShell.class.hpp"
#include "JSEngine.hpp"
#include "qjs_utils.h"
#include "quickjs.h"
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
        uint64_t deadline ;
        uint64_t interval ;
        uint32_t id ;

        JSValue func ;
        JSValue thisobj ;
        int argc ;
        JSValueConst * argv ;

        bool repeat:1 ;
        bool requestAnimationFrame:1 ;
        bool debug:1 ;
        bool removing:1 ;
    
        void destroy (JSContext * ctx) {
            JS_FreeValue(ctx, func) ;
            func = JS_NULL ;
            JS_FreeValue(ctx, thisobj) ;
            thisobj = JS_NULL ;
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
        JS_SetPropertyStr(ctx, global, "resetTimeout", JS_NewCFunction(ctx, jsResetTimeout, "resetTimeout", 2));
        JS_SetPropertyStr(ctx, global, "peekTimeout", JS_NewCFunction(ctx, jsPeekTimeout, "peekTimeout", 2));

        JS_FreeValue(ctx,global) ;
    }

    void JSTimer::loop(JSContext * ctx) {
        take(false) ;
        if(events.size()<1){
            give(false) ;
            return ;
        }
        uint64_t now = gettime() ;

        // 先清理标记为删除的任务, 并收集本轮到期的任务
        // 注意: 回调中可能调用 setTimeout/setInterval 向 events push_back (导致 vector 重新分配内存),
        // 所以不能边迭代 events 边执行回调。这里只收集事件指针, 事件对象在回调中只会被标记 removing,
        // 不会被释放 (jsClearTimeout 仅标记), 因此指针在回调执行期间保持有效。
        std::vector<JSTimerEvent*> due ;
        for(size_t i=0; i<events.size();) {
            JSTimerEvent * event = events[i] ;
            if(!event||event->removing) {
                events.erase(events.begin()+i);
                if(event) {
                    event->destroy(ctx) ;
                    delete event ;
                }
                continue ;
            }
            if(event->deadline <= now) {
                due.push_back(event) ;
            }
            i++ ;
        }

        give(false) ;

        for(auto event: due) {
            // 之前的回调可能已经把这个任务标记为删除
            if(event->removing) {
                continue ;
            }

            if(!JS_IsFunction(ctx, event->func)) {
                printf("timer callback is not a function, event:%p, total event: %d\n", event, (int)events.size()) ;
                removeTimer(ctx, event) ;
                continue ;
            }

            JSValue ret = JS_Call(ctx, event->func, event->thisobj, event->argc, event->argv) ;
            if( JS_IsException(ret) ) {
                JSEngine::getExceptionStr(ctx, ret) ;
            }
            JS_FreeValue(ctx, ret) ;

            // 回调中调用了 clearTimeout/clearInterval 清除自己, 留待下次 loop 清理
            if(event->removing) {
                continue ;
            }

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
        ARGV_TO_INT32(0, id)
        if(id<0) {
            JSTHROW("Invalid timer id") ;
        }

        JSTimerEvent * event = engine->timer.findWithId(id) ;
        if( !event ){
            printf("clearTimeout(): timer id %d not found\n", id) ;
            return JS_UNDEFINED ;
        }

        // clearTimeout 可能在 event loop 中调用，所以不直接从事件队列里删除，等到下一次 loop 时再删除
        // 通过设置 removing 标志来标记为删除状态
        event->removing = true ;

        return JS_UNDEFINED ;
    }
    
    JSValue JSTimer::jsPeekTimeout(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv){
        CHECK_ENGINE
        ASSERT_ARGC(1)
        ARGV_TO_UINT32(0, id)
        JSTimerEvent * event = engine->timer.findWithId(id) ;
        if(!event) {
            JSTHROW("timer event not found")
        }
        return JS_NewInt64(ctx, event->deadline) ;
    }

    JSValue JSTimer::jsResetTimeout(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv){
        CHECK_ENGINE
        ASSERT_ARGC(2)
        ARGV_TO_UINT32(0, id)
        JSTimerEvent * event = engine->timer.findWithId(id) ;
        if(!event) {
            JSTHROW("timer event not found")
        }
        ARGV_TO_UINT32(1, interval)
        event->deadline = gettime() + interval ;
        return JS_UNDEFINED ;
    }
    
    void JSTimer::updateTime(int64_t ms) {
        
        uint64_t now = gettime() ;
        ms-= now ;

        for(auto event: events) {
            event->deadline += ms ;
        }
    }
    
    void JSTimer::setTime(int64_t ms) {

        updateTime(ms) ;

        struct timeval tv;
        tv.tv_sec = ms/1000;  // epoch time (seconds)
        tv.tv_usec = (ms%1000)*1000;    // microseconds

        //printf("%lu.%lu\n",tv.tv_sec,tv.tv_usec);

#ifdef ESP_PLATFORM
        settimeofday(&tv, NULL);
#endif

    }
    int64_t JSTimer::getTime() {
        return gettime() ;
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
        event->debug = false ;
        event->removing = false ;

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
    
    bool JSTimer::removeTimer(JSContext *ctx, JSTimerEvent * event) {
        MUTEX({
            auto it = std::find(events.begin(),events.end(),event) ;
            if(it==events.end()) {
                printf("removing timer event not found\n") ;
                give(false) ;
                return false ;
            }
            events.erase(it);
        })
        event->destroy(ctx) ;
        delete event ;
        return true ;
    }
    
    bool JSTimer::removeTimer(JSContext *ctx, uint32_t id) {
        JSTimerEvent * event = findWithId(id) ;
        if(event) {
            return removeTimer(ctx, event) ;
        }
        return false ;
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

    JSValue JSTimer::getTimerCount(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        CHECK_ENGINE
        if(argc>0) {
            JSTHROW("too many arguments")
        }
        return JS_NewInt32(ctx, engine->timer.events.size()) ;
    }
    JSValue JSTimer::getTimerCallback(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        CHECK_ENGINE
        ASSERT_ARGC(1)
        ARGV_TO_UINT32(0, idx)
        if(idx>=engine->timer.events.size()) {
            JSTHROW("timer event not found")
        }
        JSTimerEvent * event = engine->timer.events[idx] ;
        if(!event) {
            JSTHROW("timer event not found")
        }
        return JS_DupValue(ctx, event->func) ;
    }


}