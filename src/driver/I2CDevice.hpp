#pragma once

#include "EventEmitter.hpp"
#include "module/serial/I2C.hpp"


namespace be::driver {
    class I2CDevice: public be::EventEmitter {
        DECLARE_NCLASS_META
    private:
        static std::vector<JSCFunctionListEntry> methods ;
    protected:
        be::I2C * i2c = nullptr ;
        uint8_t addr = 0 ;

        uint8_t regSize = 1 ;
        uint8_t regAddrSize = 1 ;

    public:
        I2CDevice(JSContext * ctx, JSValue jsobj=JS_NULL, uint8_t regSize=1, uint8_t regAddrSize=1, uint8_t addr=0) ;
        static JSValue constructor(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;

        virtual int setup() ;
        static JSValue setup(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        
        static JSValue readReg(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue writeReg(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        
    } ;
}