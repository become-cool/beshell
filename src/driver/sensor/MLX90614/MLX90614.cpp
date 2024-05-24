#include "MLX90614.hpp"
#include "DFRobot_MLX90614.hpp"
#include <iostream>

using namespace std ;

namespace be::driver::sensor {
    DEFINE_NCLASS_META(MLX90614, I2CDevice)

    std::vector<JSCFunctionListEntry> MLX90614::methods = {
            JS_CFUNC_DEF("setFilter", 0, MLX90614::setFilter),
            JS_CFUNC_DEF("readObjectTemp", 0, MLX90614::readObjectTemp),
            JS_CFUNC_DEF("readAmbientTemp", 0, MLX90614::readAmbientTemp),
    };

    MLX90614::MLX90614(JSContext *ctx, JSValue _jsobj)
            : I2CDevice(ctx, build(ctx, _jsobj))
    {}

    MLX90614::~MLX90614() {
        if(_driver) {
            delete _driver ;
        }
    }
    
    int MLX90614::setup() {
        if(!i2c) {
            return -1 ;
        }
        if(!_driver) {
            _driver = new DFRobot_MLX90614_I2C(addr?addr:0x5A, i2c, (int)i2c->sda(), (int)i2c->scl()) ;
        }
        _driver->begin() ;
        return 0 ;
    }

    JSValue MLX90614::constructor(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        auto obj = new MLX90614(ctx);
        obj->shared() ;
        return obj->jsobj;
    }
    
    JSValue MLX90614::setFilter(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        CHECK_ARGC(2)
        ARGV_TO_UINT8(0, iir)
        ARGV_TO_UINT8(1, fir)
        if(iir>DFRobot_MLX90614::eIIR57) {
            JSTHROW("Invalid IIR filter value") ;
        }
        if(fir>DFRobot_MLX90614::eFIR1024) {
            JSTHROW("Invalid FIR filter value") ;
        }
        THIS_NCLASS(MLX90614, that)
        that->_driver->setMeasuredParameters((DFRobot_MLX90614::eIIRMode_t)iir, (DFRobot_MLX90614::eFIRMode_t)fir) ;
        return JS_UNDEFINED ;
    }
    JSValue MLX90614::readObjectTemp(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        THIS_NCLASS(MLX90614, that)
        if(!that->_driver) {
            JSTHROW("Device not initialized") ;
        }
        float temp = that->_driver->getObjectTempCelsius() ;
        return JS_NewFloat64(ctx, temp) ;
    }
    JSValue MLX90614::readAmbientTemp(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        THIS_NCLASS(MLX90614, that)
        if(!that->_driver) {
            JSTHROW("Device not initialized") ;
        }
        float temp = that->_driver->getAmbientTempCelsius() ;
        return JS_NewFloat64(ctx, temp) ;
    }

    
    DFRobot_MLX90614_I2C * MLX90614::driver() const {
        return _driver ;
    }

}