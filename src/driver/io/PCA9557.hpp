#pragma once

#include "driver/DriverModule.hpp"
#include "../I2CDevice.hpp"

namespace be::driver::io {
    class PCA9557: public I2CDevice {
    DECLARE_NCLASS_META
    private:
        static std::vector<JSCFunctionListEntry> methods ;
        
        uint8_t regVal_Mode = 0 ;
        uint8_t regVal_Value = 0 ;
    public:
        enum class Mode {
            output,
            input
        };
        PCA9557(JSContext * ctx, JSValue _jsobj=JS_NULL) ;
        static JSValue constructor(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;

        int setup() ;
        bool read(uint8_t pin, uint8_t & val);
        bool write(uint8_t pin, uint8_t val) ;
        bool readMode();
        uint8_t getMode() const;
        bool writeMode(uint8_t mode);
        bool getPinMode(uint8_t pin, Mode & mode);
        bool setPinMode(uint8_t pin, Mode mode);

        static JSValue read(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue write(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue readMode(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue writeMode(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue getPinMode(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue setPinMode(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
    } ;
}