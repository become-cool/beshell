#pragma once

#include "deps/quickjs/quickjs-libc.h"
#include <vector>
#include <string>

namespace be {
    class NativeModule {
    private:
        std::vector<JSCFunctionListEntry> funcs ;

    public:
        std::string name ;
        bool replGlobal = false ;
        
        NativeModule(const char * name) ;

        void exportFunction(const char * funcName, JSCFunction * func, int length=0) ;
        JSValue createGlobalObject(JSContext * ctx, const char * name) ;

        JSModuleDef * createModule(JSContext *) ;
        virtual void load(JSContext * ctx) ;

        
        // std::vector<JSCFunctionListEntry> & allExportedFunctions() ;
    } ;
}