#pragma once

#include "quickjs-libc.h"
#include "Telnet.hpp"

namespace beshell {
    class JSEngine {
    private:

    public:
        JSRuntime *rt = nullptr;
        JSContext *ctx = nullptr;
        Telnet * telnet = nullptr ;
        
        JSEngine(Telnet * telnet) ;

        void initRuntime() ;

        void print(const char * content) ;
        void print(JSValue content) ;
        void dumpError() ;

        void evalSync(const char * code, size_t code_len, const char * filename="eval", int flags=JS_EVAL_TYPE_GLOBAL) ;
    
    } ;
}