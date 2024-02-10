#pragma once

#include "NativeClass.hpp"

namespace be {

    class Console: public NativeClass {
        DECLARE_NCLASS_META
        static std::vector<JSCFunctionListEntry> methods ;
    public:
        Console(JSContext * ctx, JSValue jsobj=JS_NULL) ;
        static JSValue constructor(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;

        std::string stringify(JSContext * ctx, JSValue val) ;

        static JSValue jsWrite(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue jsLog(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;

    private:
        JSValue jsStringify = JS_UNDEFINED ;
    } ;

}