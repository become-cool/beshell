#include "NativeModule.hpp"
#include <string.h>
#include <cassert>
#include <functional>
#include "debug.h"


namespace be {

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

    int NativeModule::importModule(JSContext *ctx, JSModuleDef *m) {
        NativeModule * nmodule = (NativeModule *) JS_GetModuleDefOpaque(m) ;
        assert(nmodule) ;

        int funcCnt = nmodule->funcs.size() ;
        JSCFunctionListEntry func_list[funcCnt] ;

        for(int i=0;i<funcCnt;i++) {
            func_list[i] = nmodule->funcs[i] ;
        }
        return JS_SetModuleExportList(ctx, m, func_list, nmodule->funcs.size());
    }

    JSModuleDef * NativeModule::createModule(JSContext * ctx) {

        JSModuleDef * m = JS_NewCModule(ctx, name.c_str(), importModule);
        JS_SetModuleDefOpaque(m,this) ;

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
    
    void NativeModule::load(JSContext * ctx) {}
}