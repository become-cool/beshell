#pragma once

#include "driver/DriverModule.hpp"
#include <NativeClass.hpp>
#include "module/serial/I2C.hpp"

namespace be::driver {

    class BMP280: public be::NativeClass {
        DECLARE_NCLASS_META
    private:
        static std::vector<JSCFunctionListEntry> methods ;

        be::I2C * i2c = nullptr ;
        uint8_t addr = 0x76 ;

        uint16_t dig_T1 = 0;
        uint16_t dig_T2 = 0;
        uint16_t dig_T3 = 0;

        uint16_t dig_P1 = 0;
        uint16_t dig_P2 = 0;
        uint16_t dig_P3 = 0;
        uint16_t dig_P4 = 0;
        uint16_t dig_P5 = 0;
        uint16_t dig_P6 = 0;
        uint16_t dig_P7 = 0;
        uint16_t dig_P8 = 0;
        uint16_t dig_P9 = 0;

    public:
        BMP280(JSContext * ctx, JSValue _jsobj=JS_NULL) ;
        static JSValue constructor(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;

        int begin(be::I2C *, uint8_t addr) ;
        int readTemperature(double & value) ;
        int readPressure(double & value) ;

        static JSValue begin(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue readTemperature(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue readPressure(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue readAltitude(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;

        static void provider(DriverModule * dm) ;
        static void use() ;
    } ;

}