#include "I2C.hpp"
#include "qjs_utils.h"
#include <JSEngine.hpp>

using namespace std ;

#define JSCHECK_MASTER                              \
    if(that->mode!=I2C_MODE_MASTER) {               \
        JSTHROW("I2C is not in %s mode", "master") ;\
    }
#define JSCHECK_SLAVE                               \
    if(that->mode!=I2C_MODE_SLAVE) {                \
        JSTHROW("I2C is not in %s mode", "slave") ; \
    }


namespace be {
    
    DEFINE_NCLASS_META(I2C, NativeClass)

    I2C * I2C::i2c0 = nullptr ;
    I2C * I2C::i2c1 = nullptr ;

    std::vector<JSCFunctionListEntry> I2C::methods = {
        JS_CFUNC_DEF("setup", 1, I2C::setup),
        JS_CFUNC_DEF("unsetup", 1, I2C::unsetup),
        JS_CFUNC_DEF("ping", 1, I2C::ping),
        JS_CFUNC_DEF("scan", 1, I2C::scan),
        JS_CFUNC_DEF("send", 2, I2C::send),
        JS_CFUNC_DEF("writeInt8", 2, I2C::write8),
        JS_CFUNC_DEF("writeInt16", 2, I2C::write16),
        JS_CFUNC_DEF("writeInt32", 2, I2C::write32),
        JS_CFUNC_DEF("writeUint8", 2, I2C::write8),
        JS_CFUNC_DEF("writeUint16", 2, I2C::write16),
        JS_CFUNC_DEF("writeUint32", 2, I2C::write32),
        JS_CFUNC_DEF("recv", 2, I2C::recv),
        JS_CFUNC_DEF("recvUint8", 2, I2C::recvUint8),
        JS_CFUNC_DEF("readR8", 2, I2C::readR8),
        JS_CFUNC_DEF("readR16", 2, I2C::readR16),
        JS_CFUNC_DEF("readR32", 2, I2C::readR32),
        JS_CFUNC_DEF("listen", 2, I2C::listen),
        JS_CFUNC_DEF("slaveWrite", 2, I2C::slaveWrite),
    } ;

    I2C::I2C(JSContext * ctx, i2c_port_t busnum)
        : NativeClass(ctx, build(ctx))
        , busnum(busnum)
        , sema(xSemaphoreCreateMutex())
    {}
    
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
    
    void I2C::take() {
        xSemaphoreTake(sema, portMAX_DELAY) ;
    }
    void I2C::give() {
        xSemaphoreGive(sema) ;
    }
    
    I2C * I2C::flyweight(JSContext * ctx, i2c_port_t bus) {
        if(bus==I2C_NUM_0) {
            if(!i2c0) {
                i2c0 = new I2C(ctx, I2C_NUM_0) ;
            }
            return i2c0 ;
        }
        else if(bus==I2C_NUM_1) {
            if(!i2c1) {
                i2c1 = new I2C(ctx, I2C_NUM_1) ;
            }
            return i2c1 ;
        }
        return nullptr ;
    }
    
    /**
     * options: {
     *   sda
     *   scl
     *   mode=0
     *   rx_buffer_len = 0
     *   tx_buffer_len = 0
     *   freq=400000
     *   addr_10bit_en=0
     *   slave_addr
     *   timeout=1000
     * }
     */
    JSValue I2C::setup(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        THIS_NCLASS(I2C, that)
        CHECK_ARGC(1)

        gpio_num_t GET_INT32_PROP(argv[0], "sda", sda, )
        gpio_num_t GET_INT32_PROP(argv[0], "scl", scl, )
        i2c_mode_t GET_UINT32_PROP_OPT(argv[0], "mode", mode, I2C_MODE_MASTER)
        int GET_INT32_PROP_OPT(argv[0], "timeout", timeout, 1000)

        i2c_config_t i2c_config = {
            .mode = mode,
            .sda_io_num = sda,
            .scl_io_num = scl,
            .sda_pullup_en = GPIO_PULLUP_ENABLE,
            .scl_pullup_en = GPIO_PULLUP_ENABLE,
            .clk_flags = 0
        };

        size_t rx_buffer_len = 0 ;
        size_t tx_buffer_len = 0 ;

        if(mode==I2C_MODE_MASTER) {
            GET_INT32_PROP_OPT(argv[0], "freq", i2c_config.master.clk_speed, 100000)
            GET_UINT32_PROP_OPT(argv[0], "rx_buffer_len", rx_buffer_len, 0)
            GET_UINT32_PROP_OPT(argv[0], "tx_buffer_len", tx_buffer_len, 0)

        } else if(mode==I2C_MODE_SLAVE) {
            GET_UINT32_PROP_OPT(argv[0], "rx_buffer_len", rx_buffer_len, 1024 )
            GET_UINT32_PROP_OPT(argv[0], "tx_buffer_len", tx_buffer_len, 1024 )
            GET_INT32_PROP_OPT(argv[0], "addr_10bit_en", i2c_config.slave.addr_10bit_en, 0)
            GET_INT32_PROP(argv[0], "slave_addr", i2c_config.slave.slave_addr, )
        }
        else {
            JSTHROW("invalid mode")
        }

        that->mode = mode ;

        if(i2c_param_config(that->busnum, &i2c_config)!=ESP_OK) {
            return JS_FALSE ;
        }
        esp_err_t res = i2c_driver_install(that->busnum, mode, rx_buffer_len, tx_buffer_len, 0) ;
        if(res!=ESP_OK) {
            return JS_FALSE ;
        }
        
	    // i2c_set_timeout(that->busnum, timeout) ;

        return JS_TRUE ;
    }

    JSValue I2C::unsetup(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        THIS_NCLASS(I2C, that)
        return i2c_driver_delete(that->busnum)==ESP_OK? JS_TRUE: JS_FALSE ;
    }

    bool I2C::ping(uint8_t addr) {
        if(mode!=I2C_MODE_MASTER) {
            return false ;
        }
        I2C_BEGIN_WRITE(addr)
        I2C_COMMIT(busnum)
        return res==ESP_OK;
    }

    void I2C::scan(uint8_t from, uint8_t to) {
        if(mode!=I2C_MODE_MASTER) {
            return ;
        }
        for(uint8_t addr=from; addr<=to; addr++) {
            if( ping(addr) ){
                printf("found device: 0x%02x\n", addr) ;
            }
        }
    }

    bool I2C::send(uint8_t addr, uint8_t * data, size_t data_len) {
        I2C_BEGIN_WRITE(addr)
        if(data_len>0 && data) {
            i2c_master_write(cmd, data, data_len, 0x1) ;
        }
        I2C_COMMIT(busnum) ;
        return res == ESP_OK ;
    }

    JSValue I2C::ping(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        CHECK_ARGC(1)
        THIS_NCLASS(I2C, that)
        JSCHECK_MASTER
        ARGV_TO_UINT8(0, addr)
        return that->ping(addr)? JS_TRUE: JS_FALSE ;
    }
    
    JSValue I2C::scan(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        THIS_NCLASS(I2C, that)
        JSCHECK_MASTER
        ARGV_TO_UINT8_OPT(0, from, 0)
        ARGV_TO_UINT8_OPT(1, to, 127)
        that->scan(from,to) ;
        return JS_UNDEFINED ;
    }
    
    JSValue I2C::send(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        CHECK_ARGC(2)
        THIS_NCLASS(I2C, that)
        JSCHECK_MASTER
        ARGV_TO_UINT8(0, addr)
        if(!JS_IsArray(ctx, argv[1])) {
            JSTHROW("arg must be a array")
        }
        int len ;
        uint8_t * data = JS_ArrayToBufferUint8(ctx, argv[1], &len) ;
        if(data) {
            bool res = that->send(addr,data,len) ;
            if(data) {
                free(data) ;
            }
            return res? JS_TRUE: JS_FALSE ;
        } else {
            return JS_FALSE ;
        }
    }
    
    bool I2C::recv(uint8_t addr, uint8_t * buff, size_t buffsize) {
        I2C_BEGIN_READ(addr)
        i2c_master_read(cmd, buff, buffsize, I2C_MASTER_LAST_NACK) ;
        I2C_COMMIT(busnum)
        return ESP_OK==res ;
    }

    #define I2C_WRITE(type, ARGV_CONVERT)   \
        CHECK_ARGC(3)                       \
        THIS_NCLASS(I2C, that)              \
        JSCHECK_MASTER                      \
        ARGV_TO_UINT8(0, addr)              \
        ARGV_TO_UINT8(1, reg)               \
        ARGV_CONVERT(2, byte)               \
        return  that->write<uint8_t, type>(addr, reg, byte)? JS_TRUE: JS_FALSE ;

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
        THIS_NCLASS(I2C, that)
        JSCHECK_MASTER
        if(len<1) {
            JSTHROW("invalid recv length")
        }
        uint8_t * buffer = (uint8_t*)malloc(len) ;
        if(!buffer) {
            JSTHROW("out of memory?") ;
        }
        if(!that->recv(addr,buffer,len)){
            free(buffer) ;
            return JS_NULL;
        }
        return JS_NewArrayBuffer(ctx, buffer, len, freeArrayBuffer, NULL, false) ;
    }

    JSValue I2C::recvUint8(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        CHECK_ARGC(1)
        ARGV_TO_UINT8(0, addr)
        THIS_NCLASS(I2C, that)
        JSCHECK_MASTER
        uint8_t byte ;
        if(!that->recv(addr,&byte,1)){
            JSTHROW("i2c recv failed")
        }
        return JS_NewUint32(ctx,byte) ;
    }

    #define READ_REG(bits)                                      \
        THIS_NCLASS(I2C, that)                                  \
        JSCHECK_MASTER                                          \
        CHECK_ARGC(3)                                           \
        ARGV_TO_UINT8(0, addr)                                  \
        ARGV_TO_UINT##bits(1, reg)                              \
        ARGV_TO_UINT32(2, len)                                  \
        uint8_t * buffer = (uint8_t*)malloc(len) ;              \
        if(!buffer) {                                           \
            JSTHROW("out of memory?") ;                         \
        }                                                       \
        if(!that->read<uint##bits##_t>(addr,reg,buffer,len)){\
            free(buffer) ;                                      \
            return JS_NULL;                                     \
        }                                                       \
        JS_NewArrayWithUint8Buff(arr, buffer, len)              \
        return arr ;

    JSValue I2C::readR8(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        READ_REG(8)
    }
    JSValue I2C::readR16(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        READ_REG(16)
    }
    JSValue I2C::readR32(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        READ_REG(32)
    }

    // for slave 
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
            if (chunk.len > 0 ) {
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
        CHECK_ARGC(1)
        if( !JS_IsFunction(ctx, argv[0]) ){
            JSTHROW("arg callback must be a function")
        }
        THIS_NCLASS(I2C, that)
        JSCHECK_SLAVE
        
        if(that->data_queue==nullptr){
            that->data_queue = xQueueCreate(DATA_QUEUE_LEN, sizeof(i2c_chunk_t));
        }
        if(that->slaveTask == nullptr) {
            xTaskCreatePinnedToCore(task_i2c_slave, "task-i2c-slave", 1024, that, 10, &that->slaveTask, 1);
        }
        if(that->slaveListener!=JS_NULL){
            JS_FreeValue(ctx, that->slaveListener) ;
        }
        
        JSEngine::fromJSContext(ctx)->addLoopFunction(I2C::loop, (void *)that) ;

        that->slaveListener = JS_DupValue(ctx, argv[0]) ;
        return JS_UNDEFINED ;
    }

    JSValue I2C::slaveWrite(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        CHECK_ARGC(1)
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
}