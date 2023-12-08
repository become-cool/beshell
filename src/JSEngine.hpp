#pragma once

#include "deps/quickjs/quickjs-libc.h"
#include "telnet/Telnet.hpp"
#include "module/ModuleLoader.hpp"


#define BESHELL_VERSION "0.3.0"
#define ESPIDF_VERSION IDF_VER
#define QUICKJS_VERSION "2021-03-27"


namespace be {
    class BeShell ;
    class JSEngine {
    private:

    public:
        JSRuntime *rt = nullptr;
        JSContext *ctx = nullptr;
        Telnet * telnet = nullptr ;

        ModuleLoader mloader ;
        
        JSEngine(Telnet * telnet) ;

        void setup(BeShell *) ;
        void loop() ;

        void print(JSValue content, bool pack=false, int pkgId=-1) ;
        void dumpError(bool pack=false) ;

        JSValue eval(const char * code, int code_len=-1, const char * filename="eval", int flags=JS_EVAL_TYPE_GLOBAL) ;
        void evalScript(const char * filepath, int flags=JS_EVAL_TYPE_GLOBAL) ;

        static JSContext * InitContext(JSRuntime *rt) ;
        static JSEngine * fromJSContext(JSContext *) ;
        static JSEngine * fromJSRuntime(JSRuntime *) ;
    } ;
}