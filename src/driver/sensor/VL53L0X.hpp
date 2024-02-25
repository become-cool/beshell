#pragma once

#include "driver/DriverModule.hpp"
#include <NativeClass.hpp>
#include "module/serial/I2C.hpp"


namespace be::driver {
    class VL53L0X: public be::NativeClass {
        DECLARE_NCLASS_META
    private:
        static std::vector<JSCFunctionListEntry> methods ;
        // static std::vector<JSCFunctionListEntry> staticMethods ;

    public:
        VL53L0X(JSContext * ctx, JSValue _jsobj=JS_NULL) ;

        static JSValue constructor(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue jsMethod(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
    
        static void provider(DriverModule * dm) ;
        static void use() ;
    } ;
}