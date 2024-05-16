#pragma once

#include "driver/DriverModule.hpp"
#include <NativeClass.hpp>
#include "module/serial/I2C.hpp"

namespace be::driver::io {
    class PCA9557: public be::NativeClass {
    DECLARE_NCLASS_META
    private:
        static std::vector<JSCFunctionListEntry> methods ;
        
        be::I2C * i2c = nullptr ;
        uint8_t addr = 0 ;
        uint8_t regVal_Mode = 0 ;
        uint8_t regVal_Value = 0 ;
    public:
        enum class Mode {
            output,
            input
        };
        PCA9557(JSContext * ctx, JSValue _jsobj=JS_NULL) ;
        static JSValue constructor(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;

        int setup(be::I2C *, uint8_t addr) ;
        bool read(uint8_t pin, uint8_t & val);
        bool write(uint8_t pin, uint8_t val) ;
        bool getMode(uint8_t & mode);
        bool setMode(uint8_t mode);
        bool getPinMode(uint8_t pin, Mode & mode);
        bool setPinMode(uint8_t pin, Mode mode);

        static JSValue setup(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue read(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue write(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue getMode(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue setMode(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue getPinMode(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue setPinMode(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
    } ;
}