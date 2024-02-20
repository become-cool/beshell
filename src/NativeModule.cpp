#include "NativeModule.hpp"
#include <string.h>
#include <functional>
#include <iostream>
#include "debug.h"

using namespace std ;

namespace be {

    const char * NativeModule::name = "unname_module" ;

    NativeModule::NativeModule(JSContext * _ctx, const char * name, uint8_t _flagGlobal)
        : ctx(_ctx)
        , flagGlobal(_flagGlobal)
    {
        m = JS_NewCModule(ctx, name, importModule);
        JS_SetModuleDefOpaque(m,this) ;
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

        nmodule->import(ctx) ;

        return 0 ;
    }
    
    void NativeModule::import(JSContext *ctx) {}

}