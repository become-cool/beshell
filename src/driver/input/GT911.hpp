#pragma once

#include <NativeClass.hpp>
#include "driver/DriverModule.hpp"

namespace be::driver {

    class GT911: public be::NativeClass {
        DECLARE_NCLASS_META
    private:
        static std::vector<JSCFunctionListEntry> methods ;
        // static std::vector<JSCFunctionListEntry> staticMethods ;

    public:
        GT911(JSContext * ctx, JSValue _jsobj=JS_NULL) ;

        static JSValue constructor(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue jsMethod(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;

        static void provider(DriverModule * dm) ;
        static void use() ;
    } ;
}