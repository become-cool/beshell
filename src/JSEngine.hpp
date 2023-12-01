#pragma once

#include "quickjs-libc.h"

namespace beshell {
    class JSEngine {
    private:

    public:
        JSRuntime *rt = nullptr;
        JSContext *ctx = nullptr;
        JSEngine() ;
        

        void initRuntime() ;
        JSValue evalSync(const char * code, const char * filename="eval", int flags=JS_EVAL_TYPE_GLOBAL) ;
    } ;
}