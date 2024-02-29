#pragma once

#include "quickjs/quickjs-libc.h"
#include "telnet/Telnet.hpp"
#include "JSTimer.hpp"
#include "ModuleLoader.hpp"
#include "basic/Console.hpp"


namespace be {
    class BeShell ;
    class JSEngine {
    private:

        Console * console = nullptr ;

        static JSContext * SetupContext(JSRuntime *rt) ;

    public:
        BeShell * beshell ;
        JSRuntime *rt = nullptr;
        JSContext *ctx = nullptr;

        JSTimer timer ;
        ModuleLoader mloader ;
        
        JSEngine(BeShell *) ;

        void setup() ;
        void loop() ;

        void print(JSValue content, int pkgId=-1, uint8_t cmd=OUTPUT, TelnetChannel * ch=nullptr) ;
        void dumpError(int pkgId=-1, TelnetChannel * ch=nullptr) ;
        std::string getExceptionStr() ;

        static void setGlobalValue(JSContext * ctx, const char * name, JSValue) ;
        static JSValue getGlobalValue(const char * name) ;

        JSValue eval(const char * code, int code_len=-1, const char * filename="eval", int flags=JS_EVAL_TYPE_GLOBAL) ;
        JSValue evalScript(const char * filepath, int flags=JS_EVAL_TYPE_MODULE) ;
        void runScript(const char * filepath, int flags=JS_EVAL_TYPE_MODULE) ;

        static JSEngine * fromJSContext(JSContext *) ;
        static JSEngine * fromJSRuntime(JSRuntime *) ;
    } ;
}