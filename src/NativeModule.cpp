#include "NativeModule.hpp"
#include <string.h>
#include <functional>
#include <iostream>
#include "EventEmitter.hpp"
#include "debug.h"
#include "deps/quickjs/quickjs.h"
#include "quickjs/quickjs.h"

#ifdef ESP_PLATFORM
#include "freertos/queue.h"
#endif

using namespace std ;

namespace be {

    const char * NativeModule::name = nullptr ;

    NativeModule::NativeModule(JSContext * _ctx, const char * name, uint8_t _flagGlobal)
        : ctx(_ctx)
        , flagGlobal(_flagGlobal)
    {
        m = JS_NewCModule(ctx, name, importModule);
        JS_SetModuleDefOpaque(m,this) ;
    }

    NativeModule::~NativeModule(){
        // @todo
        // 可能需要将 ns 中的 opaque 清除

#ifdef ESP_PLATFORM
        if(nevent_queue) {
            JSEngine::fromJSContext(ctx)->removeLooping((EngineLoopFunction)nativeEventLoop, this) ;
            vQueueDelete((QueueHandle_t)nevent_queue) ;
        }
#endif
    }
    
    void NativeModule::exportFunction(const char * name, JSCFunction * func, int length) {
        assert(ctx) ;

        JSCFunctionListEntry fe ;
        memset(&fe,0,sizeof(JSCFunctionListEntry)) ;
        fe.name = name ;
        fe.prop_flags = JS_PROP_WRITABLE | JS_PROP_CONFIGURABLE ;
        fe.def_type = JS_DEF_CFUNC ;
        fe.magic = 0 ;
        fe.u.func.length = length ;
        fe.u.func.cproto = JS_CFUNC_generic ;
        fe.u.func.cfunc.generic = func ;
        exportFuncs.push_back(fe) ;

        JS_AddModuleExport(ctx, m, name) ;
    }
    void NativeModule::exportName(const char * name) {
        assert(ctx) ;
        assert(m) ;
        JS_AddModuleExport(ctx, m, name) ;
    }

    void NativeModule::exportValue(const char * name, JSValue value) {
        assert(ctx) ;
        assert(m) ;
        JS_SetModuleExport(ctx, m, name, value) ;
    }

    
    NativeModule * NativeModule::fromJSModuleDef(JSModuleDef *m) {
        assert(m) ;
        return (NativeModule *) JS_GetModuleDefOpaque(m) ;
    }

    int NativeModule::importModule(JSContext *ctx, JSModuleDef *m) {
        NativeModule * nmodule = fromJSModuleDef(m) ;
        assert(nmodule) ;

        JS_SetModuleExportList(ctx, m, nmodule->exportFuncs.data(), nmodule->exportFuncs.size());

        for(auto factory : nmodule->classFactories) {
            factory(ctx, m) ;
        }

        nmodule->exports(ctx) ;

        return 0 ;
    }

    JSValue NativeModule::nsObject() {
        return js_get_module_ns(ctx, m ) ;
    }

    void NativeModule::exports(JSContext *ctx) {}

    void NativeModule::use(BeShell * beshell) {}


#ifdef ESP_PLATFORM
    void NativeModule::enableNativeEvent(JSContext *ctx, size_t param_size, size_t queue_size) {
        if(nevent_queue) {
            return ;
        }
        native_param = malloc(param_size) ;
        nevent_queue = (void *)xQueueCreate(queue_size, param_size);
        JSEngine::fromJSContext(ctx)->addLoopFunction((EngineLoopFunction)nativeEventLoop, this, true) ;
    }

    // 注意，该函数可能返回 false ，
    // 消息没有实际进入到队列中，因为队列满了或队列尚未创建
    // 如果消息中有由接收方负责释放的指针，此时需要由发送方负责释放回收
    bool NativeModule::emitNativeEvent(void * param) {
        if(!nevent_queue) {
            return false ;
        }
        return xQueueSend((QueueHandle_t)nevent_queue, param, 0)==pdTRUE ;
    }
    
    void NativeModule::nativeEventLoop(JSContext * ctx, NativeModule * ee) {
        while(xQueueReceive((QueueHandle_t)ee->nevent_queue, ee->native_param, 0)==pdTRUE) {
            ee->onNativeEvent(ctx, ee->native_param) ;
        }
    }

    void NativeModule::onNativeEvent(JSContext *ctx, void * param) {}
#endif


    EventModule::EventModule(JSContext * ctx, const char * name, uint8_t flagGlobal)
        : NativeModule(ctx, name, flagGlobal)
    {
        exportName("_handlers") ;
        exportName("on") ;
        exportName("once") ;
        exportName("race") ;
        exportName("off") ;
        exportName("emit") ;
        exportName("originHandle") ;
    }
    
    void EventModule::exports(JSContext *ctx) {
        
        exportValue("_handlers", JS_NewObject(ctx)) ;

        JSValue proto = JS_GetClassProto(ctx, EventEmitter::classID) ;

        exportValue("on", JS_GetPropertyStr(ctx, proto, "on"))  ;
        exportValue("once", JS_GetPropertyStr(ctx, proto, "once"))  ;
        exportValue("race", JS_GetPropertyStr(ctx, proto, "race"))  ;
        exportValue("off", JS_GetPropertyStr(ctx, proto, "off"))  ;
        exportValue("emit", JS_GetPropertyStr(ctx, proto, "emit"))  ;
        exportValue("originHandle", JS_GetPropertyStr(ctx, proto, "originHandle"))  ;

        JS_FreeValue(ctx, proto) ;
    }

#ifdef ESP_PLATFORM

    void EventModule::emitSync(const JSValue & eventName, std::initializer_list<JSValue> args) {
        
        assert(ctx) ;
        assert(m) ;

        int arglen = args.size() + 1;
        JSValue * jsargv = new JSValue[arglen] ;
        jsargv[0] = eventName ;
        int i = 0 ;
        for(auto arg : args) {
            jsargv[i+1] = arg ;
            ++ i ;
        }

        JSValue jsobj = js_get_module_ns(ctx, m) ;

        JSValue proto = JS_GetClassProto(ctx, EventEmitter::classID) ;
        JSValue func_emit = JS_GetPropertyStr(ctx, proto, "emit") ;
        // dn(JS_IsFunction(ctx, func_emit))
        
        JSValue ret = JS_Call(ctx, func_emit, jsobj, arglen, jsargv) ;
        if(JS_IsException(ret)) {
            js_std_dump_error(ctx) ;
        }
        
        JS_FreeValue(ctx, ret) ;
        JS_FreeValue(ctx, func_emit) ;
        JS_FreeValue(ctx, jsobj) ;

        delete[] jsargv ;
    }
#endif



}


     