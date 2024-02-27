#pragma once

#include "driver/DriverModule.hpp"
#include <NativeClass.hpp>
#include "module/serial/I2C.hpp"

namespace be::driver {
    class AHT20: public be::NativeClass {
    DECLARE_NCLASS_META
    private:
        static std::vector<JSCFunctionListEntry> methods ;
        
        be::I2C * i2c = nullptr ;
        uint8_t addr = 0x38 ;
    public:
        AHT20(JSContext * ctx, JSValue _jsobj=JS_NULL) ;
        static JSValue constructor(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;

        int begin(be::I2C *, uint8_t addr) ;
        bool measure() ;
        int read(float *humidity,float *temperature);
        static uint8_t aht20_calc_crc(uint8_t *data, uint8_t len);

        static JSValue begin(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue read(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue measure(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;

        static void provider(DriverModule * dm) ;
        static void use();
    } ;
}