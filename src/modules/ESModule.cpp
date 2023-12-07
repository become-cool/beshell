#include "ESModule.hpp"
#include <string.h>
#include <functional>
#include "debug.h"


namespace beshell {

    ESModule::ESModule(const char * _name): name(_name) {}
    
    void ESModule::exportFunction(const char * name, JSCFunction * func, int length) {
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

    JSModuleDef * ESModule::createModule(JSContext * ctx) {

        std::function<JSModuleInitFunc> import_module([this](JSContext *ctx, JSModuleDef *m)->int {

            ds(name.c_str())
            // return JS_SetModuleExportList(ctx, m, js_funcs, countof(js_funcs));
            return 0 ;
        }) ;

        JSModuleDef * m = JS_NewCModule(ctx, name.c_str(), import_module.target<JSModuleInitFunc>());
dd
        for(auto f: funcs) {
            JS_AddModuleExport(ctx, m, f.name) ;
        }
        return  m ;
    }
    
    void ESModule::load(JSContext * ctx) {
    }
}