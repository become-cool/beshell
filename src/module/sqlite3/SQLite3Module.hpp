#pragma once

#include "BeShell.hpp"
#include "NativeModule.hpp"

namespace be::sqlite{
    class SQLite3Module: public be::NativeModule {
    public:
        SQLite3Module(JSContext * ctx, const char * name) ;

        // void import(JSContext *ctx) ;

        inline static void use(be::BeShell & beshell) {
            beshell.useModule<SQLite3Module>("sqlite3") ;
        }

        static JSValue setup(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue open(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
    } ;
}
