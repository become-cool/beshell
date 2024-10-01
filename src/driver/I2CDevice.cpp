#include "I2CDevice.hpp"

using namespace std ;

namespace be::driver {
    DEFINE_NCLASS_META(I2CDevice, NativeClass)
    std::vector<JSCFunctionListEntry> I2CDevice::methods = {
        JS_CFUNC_DEF("setup", 2, I2CDevice::setup),
        JS_CFUNC_DEF("readReg", 2, I2CDevice::readReg),
        JS_CFUNC_DEF("writeReg", 2, I2CDevice::writeReg),
    } ;

    I2CDevice::I2CDevice(JSContext * ctx, JSValue jsobj, uint8_t regSize, uint8_t regAddrSize, uint8_t addr)
        : NativeClass(ctx,build(ctx,jsobj))
        , addr(addr)
        , regSize(regSize)
        , regAddrSize(regAddrSize)
    {}
    
    JSValue I2CDevice::constructor(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {

        ARGV_TO_UINT8_OPT(0, regSize, 1)
        ARGV_TO_UINT8_OPT(1, regAddrSize, 1)
        ARGV_TO_UINT8_OPT(2, addr, 0)

        if(regSize!=1 && regSize!=2 && regSize!=4) {
            JSTHROW("register size must be 1, 2 or 4 bytes")
        }
        if(regAddrSize!=1 && regAddrSize!=2 && regAddrSize!=4) {
            JSTHROW("register address size must be 1, 2 or 4 bytes")
        }

        auto obj = new I2CDevice(ctx,this_val,regSize,regAddrSize,addr) ;
        
        obj->shared() ;
        return obj->jsobj;
    }

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
        if(addr>0) {
            that->addr = addr ;
        }

        int ret = that->setup() ;
        if( ret!=0 ){
            JSTHROW("%s.%s() failed, error: %d", "I2CDevice", "setup", ret)
        }
        return JS_UNDEFINED ;
    }

    JSValue I2CDevice::readReg(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        THIS_NCLASS(I2CDevice, that)
        if(!that->i2c) {
            JSTHROW("i2c device not initialized")
        }

        ASSERT_ARGC(1)
        ARGV_TO_UINT32(argv[0], regAddr)
        ARGV_TO_UINT8_OPT(argv[1], regCnt, 1)
        bool isSigned = JS_ToBool(ctx, argv[2]) ;

        uint8_t bufferSize = regCnt*that->regSize ;
        uint8_t * buffer = new uint8_t[bufferSize]() ;

        #define READ_REG(ADDRTYPE, REGTYPE)                                             \
            if(!that->i2c->read<ADDRTYPE>(that->addr, regAddr, buffer, bufferSize)) {   \
                delete [] buffer ;                                                      \
                JSTHROW("i2c read failed")                                              \
            }                                                                           \
            if(isSigned) {                                                              \
                JS_NewArrayWithInt(arr, ((REGTYPE *)buffer), regCnt)                    \
            } else {                                                                    \
                JS_NewArrayWithUint(arr, ((REGTYPE *)buffer), regCnt)                   \
            }

        #define READ_REG_TYPES(ADDRTYPE)            \
            switch (that->regSize)                  \
            {                                       \
            case 1:                                 \
                READ_REG(ADDRTYPE, uint8_t)         \
                break;                              \
            case 2:                                 \
                READ_REG(ADDRTYPE, uint16_t)        \
                break;                              \
            case 4:                                 \
                READ_REG(ADDRTYPE, uint32_t)        \
                break;                              \
            }


        JSValue arr = JS_NULL ;
        switch (that->regAddrSize)
        {
        case 1:
            READ_REG_TYPES(uint8_t)
            break;
        case 2:
            READ_REG_TYPES(uint16_t)
            break;
        case 4:
            READ_REG_TYPES(uint32_t)
            break;
        }

        
        delete [] buffer ;
        
        return arr ;
    }

    JSValue I2CDevice::writeReg(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        THIS_NCLASS(I2CDevice, that)
        ASSERT_ARGC(1)

        return JS_UNDEFINED ;
    }
}