#pragma once

#include "driver/I2CDevice.hpp"

namespace be::driver::sensor {
    class WH4530A: public I2CDevice {
        DECLARE_NCLASS_META
    private:
        static std::vector<JSCFunctionListEntry> methods ;
    public:

        WH4530A(JSContext * ctx, JSValue _jsobj=JS_NULL) ;
        static JSValue constructor(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;

        int setup() ;
        int read() ;
        
        static JSValue read(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
    } ;
}