#include "NativeModule.hpp"
#include <string.h>
#include <cassert>
#include <functional>
#include <iostream>
#include "debug.h"

using namespace std ;

namespace be {

    NativeModule::NativeModule(JSContext * _ctx, const char * _name, uint8_t _flagGlobal)
        : ctx(_ctx)
        , name(_name)
        , flagGlobal(_flagGlobal)
    {
        m = JS_NewCModule(ctx, name.c_str(), importModule);
        JS_SetModuleDefOpaque(m,this) ;
    }
    
    void NativeModule::exportFunction(const char * name, JSCFunction * func, int length) {
        assert(ctx) ;
        assert(m) ;

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
    void NativeModule::exportValue(const char * name, JSValue value) {
        assert(m) ;
        assert(ctx) ;

        exportValues[name] = value ;

        JS_AddModuleExport(ctx, m, name) ;
    }
    
    void NativeModule::exportWithCallback(const char * name, ModuleExportor func) {
        assert(m) ;
        assert(ctx) ;
        exportors[name] = func ;
        JS_AddModuleExport(ctx, m, name) ;
    }
    
    NativeModule * NativeModule::fromJSModuleDef(JSModuleDef *m) {
        return (NativeModule *) JS_GetModuleDefOpaque(m) ;
    }

    int NativeModule::importModule(JSContext *ctx, JSModuleDef *m) {
        NativeModule * nmodule = fromJSModuleDef(m) ;
        assert(nmodule) ;

        // funcs
        int funcCnt = nmodule->exportFuncs.size() ;
        JSCFunctionListEntry func_list[funcCnt] ;
        for(int i=0;i<funcCnt;i++) {
            func_list[i] = nmodule->exportFuncs[i] ;
        }
        JS_SetModuleExportList(ctx, m, func_list, nmodule->exportFuncs.size());

        // values
        for(auto pair: nmodule->exportValues) {
            JS_SetModuleExport(ctx, m, pair.first.c_str(), pair.second);
        }

        // exportors
        for(auto pair: nmodule->exportors) {
            JS_SetModuleExport(ctx, m, pair.first.c_str(), pair.second(ctx));
        }

        return 0 ;
    }

    void NativeModule::defineExports() {}

    // JSModuleDef * NativeModule::createModule(JSContext * _ctx) {
    //     ctx = _ctx ;
    //     m = JS_NewCModule(ctx, name.c_str(), importModule);
    //     JS_SetModuleDefOpaque(m,this) ;

    //     defineExports() ;

    //     return  m ;
    // }
    
    void NativeModule::init(JSRuntime * rt) {}
    void NativeModule::setup(JSContext * ctx) {}
}