#include "NativeModule.hpp"
#include <string.h>
#include <functional>
#include "debug.h"


namespace beshell {

    NativeModule::NativeModule(const char * _name): name(_name) {}
    
    void NativeModule::exportFunction(const char * name, JSCFunction * func, int length) {
        JSCFunctionListEntry fe ;
        memset(&fe,0,sizeof(JSCFunctionListEntry)) ;
        fe.name = name ;
        fe.prop_flags = JS_PROP_WRITABLE | JS_PROP_CONFIGURABLE ;
        fe.def_type = JS_DEF_CFUNC ;
        fe.magic = 0 ;
        fe.u.func.length = length ;
        fe.u.func.cproto = JS_CFUNC_generic ;
        fe.u.func.cfunc.generic = func ;
        funcs.push_back(fe) ;
    }

    JSModuleDef * NativeModule::createModule(JSContext * ctx) {

        std::function<JSModuleInitFunc> import_module([this](JSContext *ctx, JSModuleDef *m)->int {
            // return JS_SetModuleExportList(ctx, m, js_funcs, countof(js_funcs));
            return 0 ;
        }) ;

        JSModuleDef * m = JS_NewCModule(ctx, name.c_str(), import_module.target<JSModuleInitFunc>());

        for(auto f: funcs) {
            JS_AddModuleExport(ctx, m, f.name) ;
        }
        return  m ;
    }

    JSValue NativeModule::createGlobalObject(JSContext * ctx, const char * name) {
        JSValue global = JS_GetGlobalObject(ctx) ;
        JSValue object = JS_NewObject(ctx) ;
        JS_SetPropertyStr(ctx, global, name, object);
        // JS_FreeValue(ctx,object) ;
        JS_FreeValue(ctx,global) ;
        return object ;
    }
    
    void NativeModule::load(JSContext * ctx) {
    }
}