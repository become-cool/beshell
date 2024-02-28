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

namespace be::driver {
    DEFINE_NCLASS_META(PCA9557, NativeClass)

    std::vector<JSCFunctionListEntry> PCA9557::methods = {
            JS_CFUNC_DEF("begin", 0, PCA9557::begin),
            JS_CFUNC_DEF("read", 0, PCA9557::read),
            JS_CFUNC_DEF("write", 0, PCA9557::write),
            JS_CFUNC_DEF("getMode", 0, PCA9557::getMode),
            JS_CFUNC_DEF("setMode", 0, PCA9557::setMode),
            JS_CFUNC_DEF("getPinMode", 0, PCA9557::getPinMode),
            JS_CFUNC_DEF("setPinMode", 0, PCA9557::setPinMode),
    };

    PCA9557::PCA9557(JSContext *ctx, JSValue _jsobj)
            : NativeClass(ctx, build(ctx, _jsobj)) {
    }

    JSValue PCA9557::constructor(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        auto obj = new PCA9557(ctx);
        return obj->jsobj;
    }
    
    int PCA9557::begin(I2C * _i2c, uint8_t _addr) {
        if(!_i2c) {
            return -1 ;
        }
        i2c = _i2c ;
        addr = _addr ;
        regVal_Mode = 0xFF ;
        regVal_Value = 0xFF ;
        i2c->write(addr, 3, regVal_Mode) ;
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

    bool PCA9557::getMode(uint8_t & mode) {
        if(!i2c || !addr) {
            return false ;
        }
        return i2c->read<uint8_t,uint8_t>(addr, 3, regVal_Mode) ;
    }

    bool PCA9557::setMode(uint8_t mode) {
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
    
    JSValue PCA9557::begin(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        THIS_NCLASS(PCA9557, thisobj)
        CHECK_ARGC(1)
        ARGV_TO_UINT8(0, busnum)
        ARGV_TO_UINT8_OPT(1, addr, 0x38)
        I2C * i2c = be::I2C::flyweight(ctx, (i2c_port_t)busnum) ;
        if(!i2c) {
            JSTHROW("invalid i2c port number:%d", busnum)
        }
        int ret = thisobj->begin(i2c, addr) ;
        if( ret!=0 ){
            JSTHROW("%s.%s() failed, error: %d", "PCA9557", "begin", ret)
        }
        return JS_UNDEFINED ;
    }

    JSValue PCA9557::read(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        return JS_UNDEFINED ;
    }
    JSValue PCA9557::write(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        return JS_UNDEFINED ;
    }
    JSValue PCA9557::getMode(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        return JS_UNDEFINED ;
    }
    JSValue PCA9557::setMode(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        return JS_UNDEFINED ;
    }
    JSValue PCA9557::getPinMode(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        return JS_UNDEFINED ;
    }
    JSValue PCA9557::setPinMode(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        return JS_UNDEFINED ;
    }
}