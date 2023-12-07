#pragma once

#include "quickjs-libc.h"
#include "telnet/Telnet.hpp"
#include "modules/ModuleLoader.hpp"

namespace beshell {
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
    } ;
}