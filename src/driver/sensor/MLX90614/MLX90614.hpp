#pragma once

#include "driver/I2CDevice.hpp"

class DFRobot_MLX90614_I2C ;

namespace be::driver::sensor {
    class MLX90614: public I2CDevice {
        DECLARE_NCLASS_META
    public:

        MLX90614(JSContext * ctx, JSValue _jsobj=JS_NULL) ;
        ~MLX90614() ;
        static JSValue constructor(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;

        int setup() ;
        
        static JSValue setFilter(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue readObjectTemp(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue readAmbientTemp(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;

        DFRobot_MLX90614_I2C * driver() const ;

    private:
        static std::vector<JSCFunctionListEntry> methods ;
        DFRobot_MLX90614_I2C * _driver = nullptr ;
    } ;
}