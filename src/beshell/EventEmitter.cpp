#include "EventEmitter.hpp"

using namespace std ;

namespace be {

    DEFINE_NCLASS_META_STATIC(EventEmitter)
    DEFINE_NCLASS_META_BUILD(EventEmitter)


    std::vector<JSCFunctionListEntry> EventEmitter::methods = {
        JS_CFUNC_DEF("eventAdded", 1, EventEmitter::eventAdded),
        JS_CFUNC_DEF("eventRemoved", 1, EventEmitter::eventRemoved),
    } ;

    EventEmitter::EventEmitter(JSContext * ctx, JSValue _jsobj)
        : NativeClass(ctx,build(ctx,_jsobj))
    {
        JS_SetPropertyStr(ctx,jsobj,"_handlers",JS_NewObject(ctx)) ;
    }

    EventEmitter::~EventEmitter() {

        if(nevent_queue) {
            JSEngine::fromJSContext(ctx)->removeLooping((EngineLoopFunction)nativeEventLoop, this) ;
            vQueueDelete(nevent_queue) ;
        }
        if(native_param) {
            free(native_param) ;
            native_param = nullptr ;
        }
    }

    JSValue EventEmitter::constructor(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        auto obj = new EventEmitter(ctx, this_val) ;
        obj->shared() ;
        return obj->jsobj ;
    }
    
    JSValue EventEmitter::defineClass(JSContext * ctx) {
        if(NativeClass::classID==0) {
            NativeClass::defineClass(ctx) ;
        }
        JSValue jscotr = NativeClass::defineClass(
            ctx,EventEmitter::classID,EventEmitter::className
            , EventEmitter::methods, EventEmitter::staticMethods
            , EventEmitter::constructor
            , EventEmitter::finalizer
            , NativeClass::classID
        ) ;
        
    JSValue DEF_JS_FUNC(jsOn, R"(function(event, handle, norepeat) {
    if(typeof(handle)!="function") {
        throw new Error("handle is not a function")
    }
    if(typeof(event)=="string") {
        if(!this._handlers[event]){
            this._handlers[event] = []
            if(event!="#EVENT.ADD#"&&event!="#EVENT.REMOVE#") {
                this.emit("#EVENT.ADD#",event)
                this.eventAdded && this.eventAdded(event)
            }
        }
        if(!norepeat || !this.isListening(event, handle)) {
            this._handlers[event].push(handle)
        }

    }
    else if(event instanceof Array) {
        for(var eventName of event) {
            this.on(eventName, handle, norepeat)
        }
    }
})", "on()", {})

    JSValue DEF_JS_FUNC(jsOnce, R"(function(eventName, handle, norepeat) {
    var wrapper =  (...args) => {
        this.off(eventName, wrapper.__origin)
        handle.apply(this, args)
    }
    wrapper.__origin = this.originHandle(handle)
    this.on(eventName, wrapper, norepeat)
})", "once()", {})

    JSValue DEF_JS_FUNC(jsRace, R"(function(events, callback) {
    let h = (evt, ...args) => {
        if(!events.includes(evt)) {
            return
        }
        this.off("*", h)
        callback(evt, ...args)
    }
    this.on("*", h)
})", "race()", {})

    JSValue DEF_JS_FUNC(jsOff, R"(function(eventName, handle, all) {
    if(!this._handlers[eventName]) {
        return
    }
    for(let h=this._handlers[eventName].length-1; h>=0; h--) {
        let func = this.originHandle(this._handlers[eventName][h])
        if(!handle || handle==func) {
            this._handlers[eventName][h].__origin = null
            this._handlers[eventName].splice(h,1)
            if(handle && !all) {
                break
            }
        }
    }
    if(!this._handlers[eventName].length) {
        delete this._handlers[eventName]
        if(eventName!="#EVENT.ADD#"&&eventName!="#EVENT.REMOVE#") {
            this.emit("#EVENT.REMOVE#",eventName)
            this.eventRemoved && this.eventRemoved(eventName)
        }
    }
})", "off()", {})

    JSValue DEF_JS_FUNC(jsOriginHandle, R"(function(handle) {
    for(var h=handle; h.__origin; h=h.__origin){}
    return h
})", "originHandle()", {})

    JSValue DEF_JS_FUNC(jsIsListening, R"(function(event, handle) {
    if(!this._handlers[event])
        return false
    for(let cb of this._handlers[event]) {
        if( this.originHandle(cb)==handle )
            return true
    }
    return false
})", "isListening()", {})

    JSValue DEF_JS_FUNC(jsEmit, R"(function(eventName, ...args){
    if(eventName!='*'&&this._handlers[eventName]) {
        for(let handle of this._handlers[eventName]) {
            handle.apply(this, args)
        }
    }
    if(this._handlers["*"]) {
        for(let handle of this._handlers["*"]) {
            handle.apply(this, [eventName, ...args])
        }
    }
})", "emit()", {})

    JSValue DEF_JS_FUNC(jsDestroy, R"(function() {
    for(let eventName in this._handlers) {
        this._handlers[eventName].forEach(cb=>{
            if(cb.__origin) {
                delete cb.__origin
            }
        })
        this._handlers[eventName].splice(0)
        delete this._handlers[eventName]
    }
})", "destroy", {})

        JSValue proto = JS_GetPropertyStr(ctx,jscotr,"prototype") ;
        JS_SetPropertyStr(ctx, proto, "on", jsOn) ;
        JS_SetPropertyStr(ctx, proto, "once", jsOnce) ;
        JS_SetPropertyStr(ctx, proto, "race", jsRace) ;
        JS_SetPropertyStr(ctx, proto, "off", jsOff) ;
        JS_SetPropertyStr(ctx, proto, "originHandle", jsOriginHandle) ;
        JS_SetPropertyStr(ctx, proto, "isListening", jsIsListening) ;
        JS_SetPropertyStr(ctx, proto, "emit", jsEmit) ;
        JS_SetPropertyStr(ctx, proto, "destroy", jsDestroy) ;

        return jscotr ;
    }

    void EventEmitter::emitSyncFree(const char * eventName, std::initializer_list<JSValue> args) {
        emitSync(eventName, args) ;
        for(auto arg : args) {
            JS_FreeValue(ctx, arg) ;
        }
    }
    void EventEmitter::emitSync(const char * eventName, std::initializer_list<JSValue> args) {
        JSValue name = JS_NewString(ctx, eventName) ;
        emitSync(name, args) ;
        JS_FreeValue(ctx, name) ;
    }
    void EventEmitter::emitSync(const JSValue & eventName, std::initializer_list<JSValue> args) {
        int arglen = args.size() + 1;
        JSValue * jsargv = new JSValue[arglen] ;
        jsargv[0] = eventName ;
        int i = 0 ;
        for(auto arg : args) {
            jsargv[i+1] = arg ;
            ++ i ;
        }
        JSValue func_emit = js_get_prop(ctx, jsobj, 1, "emit") ;
        JSValue ret = JS_Call(ctx, func_emit, jsobj, arglen, jsargv) ;
        if(JS_IsException(ret)) {
            JSEngine::fromJSContext(ctx)->dumpError() ;
        }
        JS_FreeValue(ctx, ret) ;
        JS_FreeValue(ctx, func_emit) ;

        delete[] jsargv ;
    }
    void EventEmitter::emitSync(const char * eventName) {
        JSValue jsargv = JS_NewString(ctx, eventName) ;
        JSValue func_emit = js_get_prop(ctx, jsobj, 1, "emit") ;
        JSValue ret = JS_Call(ctx, func_emit, jsobj, 1, &jsargv) ;
        if(JS_IsException(ret)) {
            JSEngine::fromJSContext(ctx)->dumpError() ;
        }
        JS_FreeValue(ctx, ret) ;
        JS_FreeValue(ctx, jsargv) ;
        JS_FreeValue(ctx, func_emit) ;
    }

    void EventEmitter::eventAdded(const char * eventName) {}
    void EventEmitter::eventRemoved(const char * eventName) {}
    
    JSValue EventEmitter::eventAdded(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        THIS_NCLASS(EventEmitter, that)
        const char * event_name = JS_ToCString(ctx, argv[0]) ;
        that->eventAdded(event_name) ;
        JS_FreeCString(ctx, event_name) ;
        return JS_NULL ;
    }

    JSValue EventEmitter::eventRemoved(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        THIS_NCLASS(EventEmitter, that)
        const char * event_name = JS_ToCString(ctx, argv[0]) ;
        that->eventRemoved(event_name) ;
        JS_FreeCString(ctx, event_name) ;
        return JS_NULL ;
    }

    void EventEmitter::enableNativeEvent(JSContext *ctx, size_t param_size, size_t queue_size) {
        if(nevent_queue) {
            return ;
        }
        native_param = malloc(param_size) ;
        nevent_queue = xQueueCreate(queue_size, param_size);
        JSEngine::fromJSContext(ctx)->addLoopFunction((EngineLoopFunction)nativeEventLoop, this, true) ;
    }

    // 注意，该函数可能返回 false ，
    // 消息没有实际进入到队列中，因为队列满了或队列尚未创建
    // 如果消息中有由接收方负责释放的指针，此时需要由发送方负责释放回收
    bool IRAM_ATTR EventEmitter::emitNativeEvent(void * param, bool fromISR) {
        if(!nevent_queue) {
            return false ;
        }
        if(fromISR) {
            BaseType_t higherPriorityTaskWoken = pdFALSE;
            BaseType_t result = xQueueSendFromISR(nevent_queue, param, &higherPriorityTaskWoken);
            if(result == pdTRUE) {
                portYIELD_FROM_ISR(higherPriorityTaskWoken);
                return true;
            }
            return false;
        } else {
            return xQueueSend(nevent_queue, param, 0) == pdTRUE ;
        }
    }
    
    void EventEmitter::nativeEventLoop(JSContext * ctx, EventEmitter * ee) {
        while(xQueueReceive(ee->nevent_queue, ee->native_param, 0)==pdTRUE) {
            ee->onNativeEvent(ctx, ee->native_param) ;
        }
    }

    void EventEmitter::onNativeEvent(JSContext *ctx, void * param) {}

}
// ============================================================================
// 以下函数在 js/EventEmitter.js 中实现，通过 defineClass 导出到 EventEmitter 类
// ============================================================================

/**
 * EventEmitter 类，事件发布/订阅基类
 * 
 * EventEmitter 是一个基础类，提供了事件发布/订阅（Pub/Sub）模式的实现。
 * 其他类可以继承 EventEmitter 来获得事件处理能力。
 * 
 * EventEmitter 支持以下特性：
 * - 监听事件：`on()`、`once()`
 * - 取消监听：`off()`
 * - 触发事件：`emit()`
 * - 监听所有事件：使用通配符 `*`，监听器会收到事件名作为第一个参数
 * - 竞争监听：`race()` - 多个事件中只响应第一个触发的事件
 * 
 * 示例：
 * ```javascript
 * import { EventEmitter } from 'beshell'
 * 
 * // 创建 EventEmitter 实例
 * const emitter = new EventEmitter()
 * 
 * // 监听特定事件
 * emitter.on('data', (data) => {
 *   console.log('收到数据:', data)
 * })
 * 
 * // 触发事件
 * emitter.emit('data', 'hello world')
 * 
 * // 只监听一次
 * emitter.once('connect', () => {
 *   console.log('已连接')
 * })
 * 
 * // 使用通配符 * 监听所有事件
 * // 监听器第一个参数是事件名，后续参数是触发事件时传递的参数
 * emitter.on('*', (eventName, ...args) => {
 *   console.log('事件:', eventName, '参数:', args)
 * })
 * 
 * // 触发 'data' 事件时，通配符监听器收到: 'data', 'hello world'
 * emitter.emit('data', 'hello world')
 * ```
 *
 * @module global
 * @class EventEmitter
 */

/**
 * 监听事件
 * 
 * 为指定事件添加一个监听器。当事件被触发时，监听器函数会被调用。
 * 可以多次调用 `on()` 为同一个事件添加多个监听器，它们会按添加顺序执行。
 * 
 * 使用通配符 `*` 可以监听所有事件，监听器会收到事件名作为第一个参数。
 * 
 * 示例：
 * ```javascript
 * emitter.on('message', (msg) => {
 *   console.log('收到消息:', msg)
 * })
 * 
 * // 监听多个事件
 * emitter.on(['connect', 'disconnect'], (event) => {
 *   console.log('状态变化:', event)
 * })
 * 
 * // 使用通配符监听所有事件
 * emitter.on('*', (eventName, ...args) => {
 *   console.log('事件:', eventName, '参数:', args)
 * })
 * 
 * // 不重复添加相同的监听器
 * emitter.on('update', handler, true)  // norepeat=true
 * ```
 *
 * @module global
 * @class EventEmitter
 * @method on
 * @param event:string|Array\<string\> 事件名称、事件名称数组，或通配符 `*`
 * @param handle:function 事件处理函数
 * @param norepeat:boolean=false 如果为 true，不重复添加相同的处理函数
 * @return undefined
 */

/**
 * 监听事件（只触发一次）
 * 
 * 为指定事件添加一个只触发一次的监听器。事件触发后，监听器会自动移除。
 * 
 * 示例：
 * ```javascript
 * emitter.once('ready', () => {
 *   console.log('准备就绪，只触发一次')
 * })
 * ```
 *
 * @module global
 * @class EventEmitter
 * @method once
 * @param eventName:string 事件名称
 * @param handle:function 事件处理函数
 * @param norepeat:boolean=false 如果为 true，不重复添加相同的处理函数
 * @return undefined
 */

/**
 * 竞争监听
 * 
 * 同时监听多个事件，只响应第一个触发的事件，然后自动取消监听。
 * 适用于等待多个异步事件中任意一个完成的场景。
 * 
 * 示例：
 * ```javascript
 * // 等待 connect 或 error 事件，只处理第一个触发的
 * emitter.race(['connect', 'error'], (eventName, ...args) => {
 *   console.log('最先触发的事件:', eventName)
 * })
 * ```
 *
 * @module global
 * @class EventEmitter
 * @method race
 * @param events:Array\<string\> 事件名称数组
 * @param callback:function 回调函数，参数为 (eventName, ...args)
 * @return undefined
 */

/**
 * 取消监听事件
 * 
 * 移除指定事件的监听器。可以移除特定处理函数，或移除所有监听器。
 * 
 * 示例：
 * ```javascript
 * const handler = (data) => console.log(data)
 * 
 * // 添加监听
 * emitter.on('data', handler)
 * 
 * // 移除特定监听器
 * emitter.off('data', handler)
 * 
 * // 移除所有监听器
 * emitter.off('data')
 * 
 * // 移除所有匹配的监听器（all=true）
 * emitter.off('data', handler, true)
 * ```
 *
 * @module global
 * @class EventEmitter
 * @method off
 * @param eventName:string 事件名称
 * @param handle:function=undefined 要移除的处理函数，不传则移除所有
 * @param all:boolean=false 如果为 true，移除所有匹配的处理函数
 * @return undefined
 */

/**
 * 触发事件
 * 
 * 触发指定事件，调用所有已注册的监听器。可以传递任意数量的参数给监听器。
 * 使用 `*` 可以触发通配符监听器。
 * 
 * 示例：
 * ```javascript
 * // 触发事件并传递参数
 * emitter.emit('data', 'hello', 123)
 * 
 * // 通配符监听器会收到事件名作为第一个参数
 * emitter.on('*', (eventName, ...args) => {
 *   console.log('事件:', eventName, '参数:', args)
 * })
 * emitter.emit('data', 'hello')  // 通配符监听器收到: 'data', 'hello'
 * ```
 *
 * @module global
 * @class EventEmitter
 * @method emit
 * @param eventName:string 事件名称
 * @param ...args:any 传递给监听器的参数
 * @return undefined
 */

/**
 * 获取原始处理函数
 * 
 * 内部方法，用于获取被包装的处理函数的原始函数。
 * 主要用于 `once()` 等内部包装场景。
 *
 * @module global
 * @class EventEmitter
 * @method originHandle
 * @param handle:function 处理函数
 * @return function 原始处理函数
 */

/**
 * 检查是否正在监听事件
 * 
 * 检查指定事件是否有特定的处理函数在监听。
 * 
 * 示例：
 * ```javascript
 * const handler = () => {}
 * emitter.on('data', handler)
 * 
 * console.log(emitter.isListening('data', handler))  // true
 * console.log(emitter.isListening('data', () => {})) // false
 * ```
 *
 * @module global
 * @class EventEmitter
 * @method isListening
 * @param event:string 事件名称
 * @param handle:function 处理函数
 * @return boolean 是否正在监听
 */

/**
 * 销毁 EventEmitter
 * 
 * 清理所有事件监听器，释放资源。
 * 销毁后，EventEmitter 实例不再可用。
 *
 * @module global
 * @class EventEmitter
 * @method destroy
 * @return undefined
 */

