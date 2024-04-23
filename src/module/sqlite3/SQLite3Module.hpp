#pragma once

#include "BeShell.hpp"
#include "NativeModule.hpp"

namespace be::sqlite{
    class SQLite3Module: public be::NativeModule {
    public:
        static const char * name ;
        SQLite3Module(JSContext * ctx, const char * name) ;

        // void import(JSContext *ctx) ;
        static JSValue setup(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue open(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
    } ;
}
