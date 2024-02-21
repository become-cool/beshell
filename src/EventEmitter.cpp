#include "EventEmitter.hpp"

using namespace std ;

namespace be {

    DEFINE_NCLASS_META_STATIC(EventEmitter)
    DEFINE_NCLASS_META_BUILD(EventEmitter)

    std::vector<JSCFunctionListEntry> EventEmitter::methods = {
    } ;

    EventEmitter::EventEmitter(JSContext * ctx, JSValue _jsobj)
        : NativeClass(ctx,build(ctx,_jsobj))
    {
        JS_SetPropertyStr(ctx,jsobj,"_handlers",JS_NewObject(ctx)) ;
    }

    JSValue EventEmitter::constructor(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        auto obj = new EventEmitter(ctx) ;
        obj->self = std::shared_ptr<EventEmitter> (obj) ;
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
    if(typeof(event)=="string") {
        if(!this._handlers[event]){
            this._handlers[event] = []
            if(event!="#EVENT.ADD#"&&event!="#EVENT.CLEAR#")
                this.emit("#EVENT.ADD#",event)
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
    return this
})", "EventEmitter.js", {})

    JSValue DEF_JS_FUNC(jsOnce, R"(function(eventName, handle, norepeat) {
    var wrapper =  (...args) => {
        this.off(eventName, wrapper.__origin)
        handle.apply(this, args)
    }
    wrapper.__origin = this.originHanlde(handle)
    this.on(eventName, wrapper, norepeat)
    return this
})", "EventEmitter.js", {})

    JSValue DEF_JS_FUNC(jsRace, R"(function(events, callback) {
    let h = (evt, ...args) => {
        if(!events.includes(evt)) {
            return
        }
        this.off("*", h)
        callback(evt, ...args)
    }
    this.on("*", h)
    return this
})", "EventEmitter.js", {})

    JSValue DEF_JS_FUNC(jsOff, R"(function(eventName, handle, all) {
    if(!this._handlers[eventName]) {
        return
    }
    for(let h=this._handlers[eventName].length-1; h>=0; h--) {
        let func = this.originHanlde(this._handlers[eventName][h])
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
        if(eventName!="#EVENT.ADD#"&&eventName!="#EVENT.CLEAR#")
            this.emit("#EVENT.CLEAR#",eventName)
    }
    return this
})", "EventEmitter.js", {})

    JSValue DEF_JS_FUNC(jsOriginHanlde, R"(function(handle) {
    for(var h=handle; h.__origin; h=h.__origin){}
    return h
})", "EventEmitter.js", {})

    JSValue DEF_JS_FUNC(jsIsListening, R"(function(event, handle) {
    if(!this._handlers[event])
        return false
    for(let cb of this._handlers[event]) {
        if( this.originHanlde(cb)==handle )
            return true
    }
    return false
})", "EventEmitter.js", {})

    JSValue DEF_JS_FUNC(jsEmit, R"((eventName, ...args) {
    if(eventName!='*'&&this._handlers&&this._handlers[eventName]) {
        for(let handle of this._handlers[eventName]) {
            handle.apply(this, args)
        }
    }
    if(this._handlers["*"]) {
        for(let handle of this._handlers["*"]) {
            handle.apply(this, [eventName, ...args])
        }
    }
    return this
})", "EventEmitter.js", {})

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
})", "EventEmitter.js", {})

        JSValue proto = NativeClass::mapCtxClassID2Proto[ctx][EventEmitter::classID] ;
        JS_SetPropertyStr(ctx, proto, "on", jsOn) ;
        JS_SetPropertyStr(ctx, proto, "once", jsOnce) ;
        JS_SetPropertyStr(ctx, proto, "race", jsRace) ;
        JS_SetPropertyStr(ctx, proto, "off", jsOff) ;
        JS_SetPropertyStr(ctx, proto, "originHanle", jsOriginHanlde) ;
        JS_SetPropertyStr(ctx, proto, "isListening", jsIsListening) ;
        JS_SetPropertyStr(ctx, proto, "emit", jsEmit) ;
        JS_SetPropertyStr(ctx, proto, "destroy", jsDestroy) ;

        return jscotr ;
    }
}