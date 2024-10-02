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
        if(!that->i2c || !that->addr) {
            JSTHROW("i2c device not initialized")
        }

        ASSERT_ARGC(1)
        ARGV_TO_UINT32(0, regAddr)
        ARGV_TO_UINT8_OPT(1, regCnt, 1)
        bool isSigned = false ;
        if(argc>2) {
            isSigned = JS_ToBool(ctx, argv[2]) ;
        }
        uint8_t bufferSize = regCnt*that->regSize ;
        uint8_t * buffer = new uint8_t[bufferSize]() ;

        #define READ_REG(ADDRTYPE, REGTYPE)                                                     \
            if(!that->i2c->read<ADDRTYPE>(that->addr, regAddr, buffer, bufferSize)) {           \
                delete [] buffer ;                                                              \
                JSTHROW("i2c read failed")                                                      \
            }                                                                                   \
            for(int i=0;i<regCnt;i++) {                                                         \
                if(isSigned) {                                                                  \
                    JS_SetPropertyUint32(ctx, arr, i, JS_NewInt32(ctx,(REGTYPE)((REGTYPE*)buffer)[i])) ;        \
                } else {                                                                        \
                    JS_SetPropertyUint32(ctx, arr, i, JS_NewUint32(ctx,(u##REGTYPE)((u##REGTYPE*)buffer)[i])) ; \
                }                                                                               \
            }

        #define READ_REG_TYPES(ADDRTYPE)            \
            switch (that->regSize)                  \
            {                                       \
            case 1:                                 \
                READ_REG(ADDRTYPE, int8_t)          \
                break;                              \
            case 2:                                 \
                READ_REG(ADDRTYPE, int16_t)         \
                break;                              \
            case 4:                                 \
                READ_REG(ADDRTYPE, int32_t)         \
                break;                              \
            }


        JSValue arr = JS_NewArray(ctx) ;

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

    #define WRITE_REG(REGTYPE)                                                  \
        switch(that->regAddrSize) {                                             \
        case 1:                                                                 \
            if( !that->i2c->write<uint8_t,REGTYPE>(that->addr, regAddr, val) ){ \
                JSTHROW("i2c write failed")                                     \
            }                                                                   \
            break;                                                              \
        case 2:                                                                 \
            if( !that->i2c->write<uint16_t,REGTYPE>(that->addr, regAddr, val) ){\
                JSTHROW("i2c write failed")                                     \
            }                                                                   \
            break;                                                              \
        case 4:                                                                 \
            if( !that->i2c->write<uint32_t,REGTYPE>(that->addr, regAddr, val) ){\
                JSTHROW("i2c write failed")                                     \
            }                                                                   \
            break;                                                              \
        }
        

    JSValue I2CDevice::writeReg(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        THIS_NCLASS(I2CDevice, that)
        if(!that->i2c || !that->addr) {
            JSTHROW("i2c device not initialized")
        }

        ASSERT_ARGC(2)
        ARGV_TO_UINT32(0, regAddr)

        // size_t length = 0 ;
        // uint8_t * buff = JS_GetArrayBuffer(ctx, &length, argv[1]) ;  // <- 如果不是 arraybuffer 会抛出异常

        // // ArrayBuffer
        // if(buff) {
        //     JSTHROW("not support array buffer")
        // }

        // // array 
        // else if(JS_IsArray(ctx, argv[0])) {
        //     JSTHROW("not support array")
        // }

        // Interger
        switch (that->regSize)
        {
            case 1: {
                ARGV_TO_UINT8(1, val)
                WRITE_REG(uint8_t)
                break;
            }
            case 2:{
                ARGV_TO_UINT16(1, val)
                WRITE_REG(uint16_t)
                break;
            }
            case 4: {
                ARGV_TO_UINT32(1, val)
                WRITE_REG(uint32_t)
                break;
            }
        }

        return JS_UNDEFINED ;
    }
}