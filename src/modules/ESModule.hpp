#pragma once

#include "quickjs-libc.h"
#include <vector>
#include <string>

namespace beshell {
    class ESModule {
    private:
        std::vector<JSCFunctionListEntry> funcs ;

    public:
        std::string name ;
        ESModule(const char * name) ;

        void exportFunction(const char * funcName, JSCFunction * func, int length=0) ;

        JSModuleDef * createModule(JSContext *) ;
        virtual void load(JSContext * ctx) ;
        
        // std::vector<JSCFunctionListEntry> & allExportedFunctions() ;
    } ;
}