#include "I2CDevice.hpp"

using namespace std ;

namespace be::driver {
    DEFINE_NCLASS_META(I2CDevice, NativeClass)

    I2CDevice::I2CDevice(JSContext * ctx, JSValue _jsobj)
        : NativeClass(ctx,build(ctx,_jsobj))
    {}

    int I2CDevice::begin(I2C * _i2c, uint8_t _addr) {
        if(!_i2c) {
            return -1 ;
        }
        i2c = _i2c ;
        addr = _addr ;
        return 0 ;
    }

    JSValue I2CDevice::begin(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        THIS_NCLASS(I2CDevice, thisobj)
        CHECK_ARGC(1)
        ARGV_TO_UINT8(0, busnum)
        ARGV_TO_UINT8(1, addr)
        I2C * i2c = be::I2C::flyweight(ctx, (i2c_port_t)busnum) ;
        if(!i2c) {
            JSTHROW("invalid i2c port number:%d", busnum)
        }
        int ret = thisobj->begin(i2c, addr) ;
        if( ret!=0 ){
            JSTHROW("%s.%s() failed, error: %d", "I2CDevice", "begin", ret)
        }
        return JS_UNDEFINED ;
    }

}