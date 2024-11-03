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
        auto obj = new EventEmitter(ctx) ;
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
            js_std_dump_error(ctx) ;
        }
        JS_FreeValue(ctx, ret) ;
        JS_FreeValue(ctx, func_emit) ;

        delete[] jsargv ;
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

    void EventEmitter::emitNativeEvent(void * param) {
        if(!nevent_queue) {
            return ;
        }
        xQueueSend(nevent_queue, param, 0) ;
    }
    
    void EventEmitter::nativeEventLoop(JSContext * ctx, EventEmitter * ee) {
        while(xQueueReceive(ee->nevent_queue, ee->native_param, 0)==pdTRUE) {
            ee->onNativeEvent(ctx, ee->native_param) ;
        }
    }

    void EventEmitter::onNativeEvent(JSContext *ctx, void * param) {}

}