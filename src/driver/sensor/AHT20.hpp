#pragma once

#include "driver/DriverModule.hpp"
#include <NativeClass.hpp>
#include "module/serial/I2C.hpp"

namespace be::driver {
    class AHT20: public be::NativeClass {
    DECLARE_NCLASS_META
    private:
        static std::vector<JSCFunctionListEntry> methods ;
        // static std::vector<JSCFunctionListEntry> staticMethods ;
        be::I2C * i2c = nullptr ;
        uint8_t addr = 0x38 ;
        uint8_t reg = 0xAC ;
    public:
        AHT20(JSContext * ctx, JSValue _jsobj=JS_NULL) ;

        int readTemperatureHumidity(uint32_t *temperature_raw, float *temperature,
                                    uint32_t *humidity_raw, float *humidity);

        static JSValue constructor(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue jsMethod(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;

        static void provider(DriverModule * dm) ;

        static void use();

        static uint8_t aht20_calc_crc(uint8_t *data, uint8_t len);

        static JSValue readTemperatureHumidity(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
    } ;
}