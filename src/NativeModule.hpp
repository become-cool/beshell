#pragma once

#include "deps/quickjs/quickjs-libc.h"
#include "NativeClass.hpp"
#include "debug.h"
#include <vector>
#include <string>
#include <set>
#include <map>
#include <cassert>

#define EXPORT_FUNCTION(funcname)   exportFunction(#funcname, funcname, 0) ;

namespace be {

	typedef void (*NativeClassFactoryFunc)(JSContext * ctx, JSModuleDef * m) ;

    class BeShell ;
    class NativeModule {
    private:
        std::vector<JSCFunctionListEntry> exportFuncs ;
        std::vector<NativeClassFactoryFunc> classFactories ;

        static int importModule(JSContext *ctx, JSModuleDef *m) ;

        template <typename C>
        static void classFactory(JSContext * ctx, JSModuleDef * m) {
            JSValue jscotr = C::defineClass(ctx) ;
            JS_SetModuleExport(ctx, m, C::className, jscotr) ;
        }

    protected:
        JSContext * ctx = nullptr ;
        JSModuleDef * m = nullptr ;
        virtual void import(JSContext *ctx) ;

    public:
        static const char * name ;

        uint8_t flagGlobal = 0 ;
        
        NativeModule(JSContext * ctx, const char * name, uint8_t flagGlobal=0) ;

        static void use(BeShell * beshell) ;

        void exportName(const char * name) ;

        void exportValue(const char * name, JSValue value) ;

        void exportFunction(const char * funcName, JSCFunction * func, int length=0) ;

        template <typename C>
        void exportClass() {
            assert(ctx) ;
            assert(m) ;
            JS_AddModuleExport(ctx, m, C::className) ;
            classFactories.push_back(classFactory<C>) ;
        }

        inline static NativeModule * fromJSModuleDef(JSModuleDef *) ;
        
        friend class ModuleLoader ;
    } ;
}
