#pragma once

#include <NativeClass.hpp>

namespace be {

    class I2C: public be::NativeClass {
        DECLARE_NCLASS_META
        static std::vector<JSCFunctionListEntry> methods ;
        // static std::vector<JSCFunctionListEntry> staticMethods ;

    private:
        uint8_t busnum ;

    public:
        I2C(JSContext * ctx, JSValue jsobj=JS_NULL, uint8_t busnum=0) ;

        static JSValue constructor(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue jsMethod(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;

    } ;

}