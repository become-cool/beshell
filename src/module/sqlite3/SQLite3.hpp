#pragma once

#include "NativeModule.hpp"

namespace be::sqlite{
    class SQLite3: public be::NativeModule {
    public:
        static char const * const name ;
        SQLite3(JSContext * ctx, const char * name) ;

        // void exports(JSContext *ctx) ;
        static JSValue setup(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue open(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
    } ;
}
