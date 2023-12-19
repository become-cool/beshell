#pragma once

#include "deps/quickjs/quickjs-libc.h"
#include <vector>
#include <string>
#include <set>
#include <map>

namespace be {
    class NativeModule {
    private:
        std::map<std::string,JSValue> exportValues ;
        std::vector<JSCFunctionListEntry> exportFuncs ;

        static int importModule(JSContext *ctx, JSModuleDef *m) ;

    protected:
        JSModuleDef * m = nullptr ;
        JSContext * ctx = nullptr ;
        virtual void defineExports() ;

    public:
        std::string name ;

        bool isGlobal = false ;
        bool isReplGlobal = false ;
        
        NativeModule(const char * name) ;

        void exportValue(const char * name, JSValue value) ;
        void exportFunction(const char * funcName, JSCFunction * func, int length=0) ;

        JSModuleDef * createModule(JSContext *) ;

        virtual void init(JSRuntime * ctx) ;
        virtual void setup(JSContext * ctx) ;

        inline static NativeModule * fromJSModuleDef(JSModuleDef *) ;
    } ;
}