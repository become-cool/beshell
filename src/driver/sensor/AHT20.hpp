#pragma once

#include "driver/I2CDevice.hpp"

namespace be::driver::sensor {
    class AHT20: public I2CDevice {
    DECLARE_NCLASS_META
    private:
        static std::vector<JSCFunctionListEntry> methods ;

    public:
        AHT20(JSContext * ctx, JSValue _jsobj=JS_NULL) ;
        static JSValue constructor(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;

        bool measure() ;
        int read(float *humidity,float *temperature);
        static uint8_t aht20_calc_crc(uint8_t *data, uint8_t len);

        static JSValue read(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue measure(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
    } ;
}