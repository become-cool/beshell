#pragma once

#include "driver/I2CDevice.hpp"

namespace be::driver::sensor {
    class MLX90614: public I2CDevice {
        DECLARE_NCLASS_META
    private:
        static std::vector<JSCFunctionListEntry> methods ;
    public:

        MLX90614(JSContext * ctx, JSValue _jsobj=JS_NULL) ;
        static JSValue constructor(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;

        bool read(uint8_t & val) ;
        
        static JSValue read(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
    } ;
}