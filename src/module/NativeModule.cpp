#include "NativeModule.hpp"
#include <string.h>
#include <cassert>
#include <functional>
#include <iostream>
#include "debug.h"

using namespace std ;

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
    
    NativeModule * NativeModule::fromJSModuleDef(JSModuleDef *m) {
        return (NativeModule *) JS_GetModuleDefOpaque(m) ;
    }

    int NativeModule::importModule(JSContext *ctx, JSModuleDef *m) {
        NativeModule * nmodule = fromJSModuleDef(m) ;
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
    
    void NativeModule::init(JSRuntime * rt) {}
    void NativeModule::setup(JSContext * ctx) {}
}