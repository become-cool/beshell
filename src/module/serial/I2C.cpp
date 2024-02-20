#include "I2C.hpp"
#include "qjs_utils.h"

using namespace std ;

namespace be {

    DEFINE_NCLASS_META(I2C, NativeClass)
    std::vector<JSCFunctionListEntry> I2C::methods = {
        JS_CFUNC_DEF("ping", 1, I2C::ping),
        JS_CFUNC_DEF("send", 2, I2C::send),
        JS_CFUNC_DEF("write8", 2, I2C::write8),
        JS_CFUNC_DEF("write16", 2, I2C::write16),
        JS_CFUNC_DEF("write32", 2, I2C::write32),
        JS_CFUNC_DEF("recv", 2, I2C::recv),
        JS_CFUNC_DEF("recv8", 2, I2C::recv8),
        JS_CFUNC_DEF("read8", 2, I2C::read8),
        JS_CFUNC_DEF("read16", 2, I2C::read16),
        JS_CFUNC_DEF("read32", 2, I2C::read32),
        JS_CFUNC_DEF("readU8", 2, I2C::readU8),
        JS_CFUNC_DEF("readU16", 2, I2C::readU16),
        JS_CFUNC_DEF("readU32", 2, I2C::readU32),
    } ;

    I2C::I2C(JSContext * ctx, i2c_port_t _busnum)
        : NativeClass(ctx, build(ctx))
        , busnum(_busnum)
        , sema(xSemaphoreCreateMutex())
    {
        dn(busnum)
    }
    I2C::~I2C() {
        vSemaphoreDelete(sema) ;
    }
    
    JSValue I2C::constructor(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        i2c_port_t busnum = I2C_NUM_0 ;
        if(argc>0) {
            JS_ToUint32(ctx, (uint32_t*)&busnum, argv[0]) ;
        }
        auto obj = new I2C(ctx,busnum) ;
        obj->self = std::shared_ptr<I2C> (obj) ;
        return obj->jsobj ;
    }
    
    bool I2C::ping(uint8_t addr) {
        I2C_BEGIN_WRITE(addr)
        I2C_COMMIT(busnum)
        return res==ESP_OK;
    }
    bool I2C::send(uint8_t addr, uint8_t * data, size_t data_len) {
        I2C_BEGIN_WRITE(addr)
        if(data_len>0 && data) {
            i2c_master_write(cmd, data, data_len, true) ;
        }
        I2C_COMMIT(busnum) ;
        return res == ESP_OK ;
    }

    JSValue I2C::ping(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        CHECK_ARGC(1)
        THIS_NCLASS(I2C, thisobj)
        ARGV_TO_UINT8(0, addr)
        return thisobj->ping(addr)? JS_TRUE: JS_FALSE ;
    }
    JSValue I2C::send(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        CHECK_ARGC(2)
        THIS_NCLASS(I2C, thisobj)
        ARGV_TO_UINT8(0, addr)
        if(!JS_IsArray(ctx, argv[1])) {
            JSTHROW("arg must be a array")
        }
        uint32_t len = 0 ;
        if(JS_ToUint32(ctx, &len, JS_GetPropertyStr(ctx, argv[1], "length"))!=0) {
            return JS_FALSE ;
        }
        uint8_t * data = NULL ;
        if(len) {
            data = (uint8_t*)malloc(len) ;
            if(!data) {
                JSTHROW("out of memory?")
            }
            for(uint32_t i=0;i<len;i++) {
                JSValue val = JS_GetPropertyUint32(ctx, argv[1], i) ;
                uint32_t nval = 0 ;
                JS_ToUint32(ctx, &nval, val) ;
                data[i] = nval ;
                JS_FreeValue(ctx, val) ;
            }
        }
        bool res = thisobj->send(addr,data,len) ;
        if(data) {
            free(data) ;
        }
        return res? JS_TRUE: JS_FALSE ;
    }
    
    bool I2C::recv(uint8_t addr, uint8_t * buff, size_t buffsize) {
        I2C_BEGIN_READ(addr)
        I2C_RECV(buff,buffsize)
        I2C_COMMIT(busnum)
        return ESP_OK!=res ;
    }

    #define I2C_WRITE(type, ARGV_CONVERT)   \
        CHECK_ARGC(3)                       \
        THIS_NCLASS(I2C, thisobj)           \
        ARGV_TO_UINT8(0, addr)              \
        ARGV_TO_UINT8(1, reg)               \
        ARGV_CONVERT(2, byte)               \
        return  thisobj->write<uint8_t, type>(addr, reg, byte)? JS_TRUE: JS_FALSE ;

    JSValue I2C::write8(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        I2C_WRITE(uint8_t,ARGV_TO_UINT8)
    }
    JSValue I2C::write16(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        I2C_WRITE(uint16_t,ARGV_TO_UINT16)
    }
    JSValue I2C::write32(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        I2C_WRITE(uint32_t,ARGV_TO_UINT32)
    }

    JSValue I2C::recv(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        CHECK_ARGC(2)
        ARGV_TO_UINT8(0, addr)
        ARGV_TO_UINT8(1, len)
        THIS_NCLASS(I2C, thisobj)
        if(len<1) {
            JSTHROW("invalid recv length")
        }
        uint8_t * buffer = (uint8_t*)malloc(len) ;
        if(!buffer) {
            JSTHROW("out of memory?") ;
        }
        if(!thisobj->recv(addr,buffer,len)){
            free(buffer) ;
            return JS_NULL;
        }
        return JS_NewArrayBuffer(ctx, buffer, len, freeArrayBuffer, NULL, false) ;
    }

    JSValue I2C::recv8(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        CHECK_ARGC(1)
        ARGV_TO_UINT8(0, addr)
        THIS_NCLASS(I2C, thisobj)
        uint8_t byte ;
        if(!thisobj->recv(addr,&byte,1)){
            JSTHROW("i2c recv failed")
        }
        return JS_NewUint32(ctx,byte) ;
    }

    #define READ_REG(type)                          \
        CHECK_ARGC(2)                               \
        ARGV_TO_UINT8(0, addr)                      \
        ARGV_TO_UINT8(1, reg)                       \
        THIS_NCLASS(I2C, thisobj)                   \
        type v = 0 ;                                \
        if( !thisobj->read(addr, reg, v) ) {        \
            JSTHROW("i2c recv failed")              \
        }

    JSValue I2C::read8(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        READ_REG(int8_t)
        return JS_NewInt32(ctx, v) ;
    }
    JSValue I2C::read16(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        READ_REG(int16_t)
        v = ((v&0xFF00)>>8) | ((v&0xFF)<<8) ;
        return JS_NewInt32(ctx, v) ;
    }
    JSValue I2C::read32(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        READ_REG(int32_t)
        v = ((v&0xFF000000)>>24) | ((v&0xFF0000)>>8) | ((v&0xFF00)<<8) | ((v&0xFF)<<24) ;
        return JS_NewInt32(ctx, v) ;
    }
    JSValue I2C::readU8(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        READ_REG(uint8_t)
        return JS_NewUint32(ctx, v) ;
    }
    JSValue I2C::readU16(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        READ_REG(uint16_t)
        v = ((v&0xFF00)>>8) | ((v&0xFF)<<8) ;
        return JS_NewUint32(ctx, v) ;
    }
    JSValue I2C::readU32(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        READ_REG(uint32_t)
        v = ((v&0xFF000000)>>24) | ((v&0xFF0000)>>8) | ((v&0xFF00)<<8) | ((v&0xFF)<<24) ;
        return JS_NewUint32(ctx, v) ;
    }
}