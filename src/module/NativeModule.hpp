#pragma once

#include "deps/quickjs/quickjs-libc.h"
#include <vector>
#include <string>

namespace be {
    class NativeModule {
    private:
        std::vector<JSCFunctionListEntry> funcs ;

        static int importModule(JSContext *ctx, JSModuleDef *m) ;

    public:
        std::string name ;
        bool replGlobal = false ;
        
        NativeModule(const char * name) ;

        void exportFunction(const char * funcName, JSCFunction * func, int length=0) ;

        JSModuleDef * createModule(JSContext *) ;
        virtual void load(JSContext * ctx) ;

        inline static NativeModule * fromJSModuleDef(JSModuleDef *) ;
    } ;
}