#include "I2CDevice.hpp"

using namespace std ;

namespace be::driver {
    DEFINE_NCLASS_META(I2CDevice, NativeClass)
    std::vector<JSCFunctionListEntry> I2CDevice::methods = {
        JS_CFUNC_DEF("setup", 2, I2CDevice::setup),
    } ;

    I2CDevice::I2CDevice(JSContext * ctx, JSValue _jsobj, uint8_t addr)
        : NativeClass(ctx,build(ctx,_jsobj))
        , addr(addr)
    {}

    int I2CDevice::setup() {
        return 0 ;
    }

    JSValue I2CDevice::setup(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        THIS_NCLASS(I2CDevice, that)
        ASSERT_ARGC(1)
        int GET_INT32_PROP(argv[0], "i2c", busnum, )
        int GET_INT32_PROP_OPT(argv[0], "addr", addr, -1)
        I2C * i2c = be::I2C::flyweight(ctx, (i2c_port_t)busnum) ;
        if(!i2c) {
            JSTHROW("invalid i2c port number:%d", busnum)
        }
        
        that->i2c = i2c ;
        if(addr>-1) {
            that->addr = addr ;
        }

        int ret = that->setup() ;
        if( ret!=0 ){
            JSTHROW("%s.%s() failed, error: %d", "I2CDevice", "setup", ret)
        }
        return JS_UNDEFINED ;
    }

}