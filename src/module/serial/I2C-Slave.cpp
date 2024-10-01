#include "I2C.hpp"
#include "qjs_utils.h"
#include <JSEngine.hpp>

// for slave 
namespace be {
    #if SOC_I2C_SUPPORT_SLAVE
    typedef struct {
        uint8_t * data ;
        int len ;
    } i2c_chunk_t ;
    void I2C::task_i2c_slave(void *arg) {
        I2C * that = (I2C*)arg ;
        i2c_chunk_t chunk;
        uint8_t buff[32] ;
        while (1) {
            chunk.len = i2c_slave_read_buffer(that->busnum, buff, sizeof(buff), 1/portTICK_PERIOD_MS);
            if ( chunk.len > 0 ) {
                // 问答模式
                if(that->slaverRegisters && chunk.len==that->slaverRegisterAddrSize) {
                    uint16_t addr = 0;
                    for(int i=0; i<that->slaverRegisterAddrSize; i++) {
                        addr|= buff[i] << (i*8) ;
                    }
                    if( addr < that->slaverRegisterLength ) {
                        uint8_t * ptr = that->slaverRegisters+addr*that->slaverRegisterAddrSize ;
                        i2c_slave_write_buffer(
                            that->busnum
                            , ptr
                            , that->slaverRegisterSize
                            , 1/portTICK_PERIOD_MS
                        );
                        continue;
                    }
                }

                // callback
                chunk.data = (uint8_t *)malloc(chunk.len) ;
                memcpy(chunk.data, buff, chunk.len) ;
                if( xQueueSend(that->data_queue, &chunk, 0)!=pdPASS ){
                    free(chunk.data) ;
                }
            }
            vTaskDelay(1/portTICK_PERIOD_MS);
        }
    }

    #define DATA_QUEUE_LEN 10
    void I2C::loop(JSContext * ctx, void * opaque) {
        I2C * i2c = (I2C *) opaque ;
        i2c_chunk_t chunk ;
        if(xQueueReceive(i2c->data_queue, &chunk, 0)) {
            if(chunk.data) {
                JSValue ab = JS_NewArrayBuffer(ctx, chunk.data, chunk.len, freeArrayBuffer, NULL, false) ;
                JS_Call(ctx, i2c->slaveListener, JS_UNDEFINED, 1, &ab);
                JS_FreeValue(ctx, ab) ;
            }
        }
    }
    
    JSValue I2C::listen(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        ASSERT_ARGC(1)
        if( !JS_IsFunction(ctx, argv[0]) ){
            JSTHROW("arg callback must be a function")
        }
        THIS_NCLASS(I2C, that)
        JSCHECK_SLAVE
        
        if(that->data_queue==nullptr){
            that->data_queue = xQueueCreate(DATA_QUEUE_LEN, sizeof(i2c_chunk_t));
        }
        if(that->slaveListener!=JS_NULL){
            JS_FreeValue(ctx, that->slaveListener) ;
        }
        
        JSEngine::fromJSContext(ctx)->addLoopFunction(I2C::loop, (void *)that, true, 0) ;

        that->slaveListener = JS_DupValue(ctx, argv[0]) ;
        return JS_UNDEFINED ;
    }
    
    JSValue I2C::slaveWrite(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        ASSERT_ARGC(1)
        THIS_NCLASS(I2C, that)

        int len ;
        bool needfree = false ;
        uint8_t * data = (uint8_t *)JS_GetArrayBuffer(ctx, (size_t*)&len, argv[0]) ;
        if(!data) {
            if(!JS_IsArray(ctx, argv[0])) {
                JSTHROW("arg must be a array or ArrayBuffer")
            } else {
                data = JS_ArrayToBufferUint8(ctx, argv[0], &len) ;
                needfree = true ;
            }
        }
        if(data) {
            esp_err_t res = i2c_slave_write_buffer(that->busnum, data, len, 10/portTICK_PERIOD_MS) ;
            if(needfree) {
                free(data) ;
            }
            return res == ESP_OK? JS_TRUE: JS_FALSE ;
        } else {
            return JS_FALSE ;
        }
    }

    JSValue I2C::slaveWriteReg(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        ASSERT_ARGC(2)
        THIS_NCLASS(I2C, that)
        JSCHECK_SLAVE

        ARGV_TO_UINT32(0, reg_addr)
        ARGV_TO_UINT32(1, reg_value)

        if(reg_addr>=that->slaverRegisterLength) {
            JSTHROW("reg addr out of range")
        }

        for(int i=0; i<that->slaverRegisterSize; i++) {
            that->slaverRegisters[reg_addr*that->slaverRegisterAddrSize+i] = (reg_value>>(i*8))&0xff;
        }

        return JS_UNDEFINED ;
    }
    
    JSValue I2C::slaveWriteBuff(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        ASSERT_ARGC(2)
        THIS_NCLASS(I2C, that)
        JSCHECK_SLAVE

        ARGV_TO_UINT32(0, offset)

        int len ;
        bool needfree = false ;
        uint8_t * data = (uint8_t *)JS_GetArrayBuffer(ctx, (size_t*)&len, argv[1]) ;
        if(!data) {
            if(!JS_IsArray(ctx, argv[0])) {
                JSTHROW("arg must be a array or ArrayBuffer")
            } else {
                data = JS_ArrayToBufferUint8(ctx, argv[1], &len) ;
                needfree = true ;
            }
        }
        if(!data) {
            JSTHROW("invalid data")
        }
        if(len%that->slaverRegisterSize!=0) {
            if(needfree){
                free(data) ;
            }
            JSTHROW("data length must be a multiple of reg_size")
        }

        int reg_len = len / that->slaverRegisterSize ;
        if(offset + len > that->slaverRegisterLength * that->slaverRegisterSize) {
            if(needfree){
                free(data) ;
            }
            JSTHROW("out of range, buffer size is %d", that->slaverRegisterLength * that->slaverRegisterSize)
        }

        memcpy( that->slaverRegisters+offset, data, len ) ;
        if(needfree){
            free(data) ;
        }

        return JS_UNDEFINED ;
    }

    JSValue I2C::slaveReadBuff(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        THIS_NCLASS(I2C, that)
        JSCHECK_SLAVE
        ASSERT_ARGC(2)
        ARGV_TO_UINT32(0, offset)
        ARGV_TO_UINT32(1, length)
        if(offset + length > that->slaverRegisterLength * that->slaverRegisterSize) {
            JSTHROW("out of range, buffer size is %d", that->slaverRegisterLength * that->slaverRegisterSize)
        }
        return JS_NewArrayBufferCopy(ctx, that->slaverRegisters+offset, length) ;
    }
    #endif

    gpio_num_t I2C::sda() const {
        return _sda ;
    }
    gpio_num_t I2C::scl() const {
        return _scl ;
    }
    i2c_port_t I2C::number() const {
        return busnum ;
    }
}