#pragma once

#include <NativeClass.hpp>
#include <iostream>

#include "deps/sqlite3/sqlite3.h"

namespace be::sqlite{
    class DB: public be::NativeClass {
        DECLARE_NCLASS_META
    private:
        static std::vector<JSCFunctionListEntry> methods ;
        
        sqlite3 * db = nullptr ;

    public:
        DB(JSContext * ctx, sqlite3 *) ;
        ~DB() ;

        static JSValue execSync(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue close(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
    
    } ;
}