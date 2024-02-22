#pragma once

#include <NativeClass.hpp>
#include "driver/DriverModule.hpp"
#include "module/serial/I2C.hpp"

namespace be::driver {

    class GT911: public be::NativeClass {
        DECLARE_NCLASS_META
    private:
        static std::vector<JSCFunctionListEntry> methods ;
        
        uint8_t addr = 0x5D ;
        be::I2C * i2c = nullptr ;

    public:
        GT911(JSContext * ctx, be::I2C * i2c, uint8_t addr=0x5D) ;
        static JSValue constructor(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;

        bool reset() ;
        uint8_t readConfigVersion() ;
        
        bool readPos(uint8_t i, uint16_t &x, uint16_t &y) ;
        uint8_t readPointCount() ;

        static JSValue setup(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue reset(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue pid(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        
        static void provider(DriverModule * dm) ;
        static void use() ;
    } ;
}