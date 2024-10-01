#include "PCA9557.hpp"
#include <iostream>

#ifndef BIT
#define BIT(nr)                 (1UL << (nr))
#endif
#define AT581X_STATUS_Calibration_Enable    (3)             /* 1 --Calibration enable; 0 --Calibration disable */
#define AT581X_STATUS_CRC_FLAG              (4)
#define AT581X_STATUS_BUSY_INDICATION       (7)
using namespace std;
using namespace be;

namespace be::driver::io {
    DEFINE_NCLASS_META(PCA9557, I2CDevice)

    std::vector<JSCFunctionListEntry> PCA9557::methods = {
            JS_CFUNC_DEF("read", 0, PCA9557::read),
            JS_CFUNC_DEF("write", 0, PCA9557::write),
            JS_CFUNC_DEF("readMode", 0, PCA9557::readMode),
            JS_CFUNC_DEF("writeMode", 0, PCA9557::writeMode),
            JS_CFUNC_DEF("getPinMode", 0, PCA9557::getPinMode),
            JS_CFUNC_DEF("setPinMode", 0, PCA9557::setPinMode),
    };

    PCA9557::PCA9557(JSContext *ctx, JSValue _jsobj)
            : I2CDevice(ctx, build(ctx, _jsobj))
    {
        regVal_Mode = 0xFF ;
        regVal_Value = 0xFF ;
    }

    JSValue PCA9557::constructor(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        auto obj = new PCA9557(ctx);
        return obj->jsobj;
    }
    
    int PCA9557::setup() {
        if(!i2c) {
            return -1 ;
        }
        if(!readMode()) {
            return  -2 ;
        }
        return 0 ;
    }

    bool PCA9557::read(uint8_t pin, uint8_t & val) {
        if(!i2c || !addr) {
            return false ;
        }
        if(pin>7) {
            return false ;
        }
        if(!i2c->read<uint8_t,uint8_t>(addr, 0, regVal_Value)){
            return false ;
        }
        val = (regVal_Value >> pin) & 1 ;
        return true ;
    }

    bool PCA9557::write(uint8_t pin, uint8_t val) {
        if(!i2c || !addr) {
            return false ;
        }
        if(pin>7) {
            return false ;
        }
        if(val) {
            regVal_Value|= 1<<pin ;
        } else {
            regVal_Value&= ~(1<<pin) ;
        }
        i2c->write(addr, 1, regVal_Value) ;
        return true ;
    }
    
    uint8_t PCA9557::getMode() const {
        return regVal_Mode ;
    }

    bool PCA9557::readMode() {
        if(!i2c || !addr) {
            return false ;
        }
        if(!i2c->read<uint8_t,uint8_t>(addr, 3, regVal_Mode)){
            return false ;
        }
        return true ;
    }

    bool PCA9557::writeMode(uint8_t mode) {
        if(!i2c || !addr) {
            return false ;
        }
        regVal_Mode = mode ;
        i2c->write(addr, 3, regVal_Mode) ;
        return true ;
    }
    bool PCA9557::getPinMode(uint8_t pin, Mode & mode) {
        if(!i2c || !addr) {
            return false ;
        }
        if(pin>7) {
            return false ;
        }
        if(!i2c->read<uint8_t,uint8_t>(addr, 3, regVal_Mode)){
            return false ;
        }
        mode = (regVal_Mode & (1<<pin))? Mode::input: Mode::output ;
        return true ;
    }
    bool PCA9557::setPinMode(uint8_t pin, Mode mode) {
        if(!i2c || !addr) {
            return false ;
        }
        if(pin>7) {
            return false ;
        }
        if(mode==Mode::input) {
            regVal_Mode|= (1<<pin) ;
        } else {
            regVal_Mode&= ~(1<<pin) ;
        }
        return i2c->write(addr, 3, regVal_Mode) ;
    }

    JSValue PCA9557::read(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        return JS_UNDEFINED ;
    }
    JSValue PCA9557::write(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        return JS_UNDEFINED ;
    }
    JSValue PCA9557::readMode(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        THIS_NCLASS(PCA9557, thisobj)
        if(!thisobj->readMode()) {
            JSTHROW("%s.%s() failed", "PCA9557", "readMode")
        }
        return JS_NewUint32(ctx, thisobj->getMode()) ;
    }
    JSValue PCA9557::writeMode(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        CHECK_ARGC(1)
        THIS_NCLASS(PCA9557, thisobj)
        ARGV_TO_UINT8(0, mode)
        if(!thisobj->writeMode(mode)) {
            JSTHROW("%s.%s() failed", "PCA9557", "writeMode")
        }
        return JS_UNDEFINED ;
    }
    JSValue PCA9557::getPinMode(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        return JS_UNDEFINED ;
    }
    JSValue PCA9557::setPinMode(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        return JS_UNDEFINED ;
    }
}