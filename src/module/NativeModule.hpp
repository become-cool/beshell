#pragma once

#include "deps/quickjs/quickjs-libc.h"
#include <vector>
#include <string>
#include <set>
#include <map>

namespace be {

	typedef JSValue (*ModuleExportor)(JSContext *) ;

    class NativeModule {
    private:
        std::map<std::string,JSValue> exportValues ;
        std::map<std::string,ModuleExportor> exportors ;
        std::vector<JSCFunctionListEntry> exportFuncs ;

        static int importModule(JSContext *ctx, JSModuleDef *m) ;

    protected:
        JSModuleDef * m = nullptr ;
        JSContext * ctx = nullptr ;
        virtual void defineExports() ;

    public:
        std::string name ;

        uint8_t flagGlobal = 0 ;
        
        NativeModule(JSContext * ctx, const char * name, uint8_t flagGlobal=0) ;

        void exportValue(const char * name, JSValue value) ;
        void exportFunction(const char * funcName, JSCFunction * func, int length=0) ;
        void exportWithCallback(const char * name, ModuleExportor func) ;

        JSModuleDef * createModule(JSContext *) ;

        virtual void init(JSRuntime * ctx) ;
        virtual void setup(JSContext * ctx) ;

        inline static NativeModule * fromJSModuleDef(JSModuleDef *) ;
        
        friend class ModuleLoader ;
    } ;
}