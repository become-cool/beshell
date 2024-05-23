#pragma once

#include <NativeClass.hpp>
#include "module/serial/I2C.hpp"


namespace be::driver {
    class I2CDevice: public be::NativeClass {
        DECLARE_NCLASS_META
    private:
        static std::vector<JSCFunctionListEntry> methods ;
    protected:
        be::I2C * i2c = nullptr ;
        uint8_t addr = 0 ;

    public:
        I2CDevice(JSContext * ctx, JSValue _jsobj=JS_NULL, uint8_t addr=0) ;

        virtual int setup() ;
        static JSValue setup(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;

    } ;
}