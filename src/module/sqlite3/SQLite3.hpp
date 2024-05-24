#pragma once

#include "NativeModule.hpp"

namespace be::sqlite{
    class SQLite3: public be::NativeModule {
    public:
        static const char * name ;
        SQLite3(JSContext * ctx, const char * name) ;

        // void import(JSContext *ctx) ;
        static JSValue setup(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue open(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
    } ;
}
