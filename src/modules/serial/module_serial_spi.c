#include "module_serial_spi.h"
#include "utils.h"
#include <string.h>



static uint8_t _spi_bus_setup = 0 ;
spi_device_handle_t _spi_handle_pool1[8] = {0,0,0,0,0,0,0,0} ;

static int8_t find_free_spi_handle(spi_device_handle_t ** pool) {
    int8_t h = 0 ;
    for(h=0; h<8; h++) {
        if(_spi_handle_pool1[h]==NULL) {
            *pool = & _spi_handle_pool1 ;
            return h ;
        }
    }
    return -1 ;
}

spi_device_handle_t spi_handle_with_id(uint8_t idx) {
    if(idx<8) {
        return _spi_handle_pool1[idx] ;
    }
    return NULL ;
}

/**
 * spi bus num 1-3
 * miso
 * mosi
 * clk
 */
static JSValue js_spi_bus_setup(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv){

    CHECK_ARGC(4)

    ARGV_TO_UINT8(3, clkpin)
    
    // int8_t misopin = -1 ;
    // int8_t mosipin = -1 ;
    // if(!JS_IsUndefined(argv[2]) && !JS_IsNull(argv[2])) {
    //     if( JS_ToUint32(ctx, &mosipin, argv[2])!=0 ){
    //         THROW_EXCEPTION("MOSI pin invalid.")
    //     }
    // }
    // if(!JS_IsUndefined(argv[1]) && !JS_IsNull(argv[1])) {
    //     if( JS_ToUint32(ctx, &misopin, argv[1])!=0 ){
    //         THROW_EXCEPTION("MISO pin invalid.")
    //     }
    // }
    
    ARGV_TO_UINT8(2, mosipin)
    ARGV_TO_UINT8(1, misopin)

    ARGV_TO_UINT8(0, busnum)
    if(busnum<0 || busnum>4) {
        THROW_EXCEPTION("Bus num must be 0-3")
    }
// #ifdef CONFIG_IDF_TARGET_ESP32S3
//     if(busnum<0 || busnum>4) {
//         THROW_EXCEPTION("Bus num must be 0-3")
//     }
// #elif CONFIG_IDF_TARGET_ESP32
//     if(busnum<0 || busnum>3) {
//         THROW_EXCEPTION("Bus num must be 0-2")
//     }
// #endif


    printf("spi[%d] miso=%d, mosi=%d, clk=%d\n", busnum, misopin, mosipin, clkpin);
    // dn(busnum)

    spi_bus_config_t buscfg = {
        .miso_io_num=misopin,
        .mosi_io_num=mosipin,
        .sclk_io_num=clkpin,
        .quadwp_io_num=-1,
        .quadhd_io_num=-1,
        .max_transfer_sz=20480
    } ;

    esp_err_t ret = spi_bus_initialize(busnum, &buscfg, SPI_DMA_CH_AUTO);
    if(ret==0) {
        _spi_bus_setup|= 1<<busnum ;
    }
    else {
        printf("spi_bus_initialize() failed with err: %d\n", ret) ;
    }

    return JS_NewInt32(ctx, ret) ;
}

/**
 * spi bus num 0-2
 */
static JSValue js_spi_bus_free(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv){
    CHECK_ARGC(1)
    ARGV_TO_UINT8(0, busnum)
    if(busnum<0 || busnum>2) {
        THROW_EXCEPTION("Bus num must be 0-2")
    }
    esp_err_t ret = spi_bus_free(busnum) ;
    if(ret==0) {
        _spi_bus_setup&= ~(1<<busnum) ;
    }

    return JS_NewInt32(ctx, ret) ;
}

/**
 * spi bus num 1-3
 * cspin pin (-1 表示不使用)
 * freq
 * mode
 */
static JSValue js_spi_add_device(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv){

    spi_device_handle_t handle = NULL ;
    uint8_t spi_handle_index ;
    spi_device_handle_t * spipool ;
    int8_t spiidx = find_free_spi_handle(&spipool) ;
    if(spiidx<0) {
        THROW_EXCEPTION("SPI Handle too much")
    }

    CHECK_ARGC(4)
    ARGV_TO_UINT8(0, bus)
    ARGV_TO_UINT8(1, cspin)
    ARGV_TO_UINT32(2, freq)
    ARGV_TO_UINT8(3, mode)
    
    spi_device_interface_config_t devcfg={
        .clock_speed_hz=freq,
        .mode=mode,
        .spics_io_num=cspin,
        .queue_size=7,                          //We want to be able to queue 7 transactions at a time
        .pre_cb=NULL,
    };
    esp_err_t ret=spi_bus_add_device(bus, &devcfg, &spipool[spiidx]);
    return JS_NewInt32(ctx, ret!=0? -1: spiidx) ;
}
static JSValue js_spi_device_remove(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv){

    return JS_UNDEFINED ;
}

#define ARGV_TO_SPI_HANDLE(i, handle)                   \
    ARGV_TO_UINT8(i, spiidx)                            \
    spi_device_handle_t handle = spi_handle_with_id(spiidx) ; \
    if(handle==NULL) {                                  \
        THROW_EXCEPTION("unknow spi device id:%d",spiidx) \
    }

/**
 * 
 * bus (1-3)
 * data {string|ArrayBuffer}
 * offset?
 * length?
 */
static JSValue js_spi_send(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv){
    CHECK_ARGC(2)
    ARGV_TO_SPI_HANDLE(0, handle)

    int offset = 0 ;
    if(argc>=3) {
        if( JS_ToInt32(ctx, &offset, argv[2])!=0 ) {
            THROW_EXCEPTION("Invalid param type for offset");
        }
    }
    int length = -1 ;
    if(argc>=4) {
        if( JS_ToInt32(ctx, &length, argv[3])!=0 ) {
            THROW_EXCEPTION("Invalid param type for length");
        }
    }

    spi_transaction_t t;
    memset(&t, 0, sizeof(t));

    esp_err_t ret = -1 ;
    if(JS_IsString(argv[1])) {
        t.tx_buffer = JS_ToCStringLen(ctx, &t.length, argv[1]) ;
        if(length>-1 && length<t.length) {
            t.length = length ;
        }
        t.length*= 8 ;
        ret = spi_device_transmit(handle, &t) ;

        JS_FreeCString(ctx, t.tx_buffer) ;
        goto end ;
    }

    t.tx_buffer = JS_GetArrayBuffer(ctx, &t.length, argv[1]) ;
    if(t.tx_buffer) {
        if(length>-1 && length<t.length) {
            t.length = length ;
        }
        t.length*= 8 ;
        ret = spi_device_transmit(handle, &t) ;
        // JS_FreeValue(ctx, argv[1]) ;
    }
    else {
        THROW_EXCEPTION("Invalid data")
    }

end:
    return JS_NewInt32(ctx, ret) ;
}


#define ARGV_TO_SPI_OUT_NUMBER(i, type, var)            \
    type var = 0 ;                                      \
    if( !JS_IsUndefined(var) && !JS_IsUndefined(var) ) {\
        if(JS_ToUint32(ctx, &out, argv[i])!=0) {        \
            THROW_EXCEPTION("arg must be a number")     \
        }                                               \
    }

inline esp_err_t spi_trans_int(spi_device_handle_t handle, uint8_t * rx_buff, uint8_t * tx_buff, size_t bit_length) {
    spi_transaction_t t;
    memset(&t, 0, sizeof(t));

    // t.flags = SPI_TRANS_USE_RXDATA | SPI_TRANS_USE_TXDATA ;
    t.tx_buffer = tx_buff ;
    t.rx_buffer = rx_buff ;
    t.length = bit_length ;
    t.rxlength = 0 ; // same to length
    
    return spi_device_transmit(handle, &t) ;
}



/**
 * 
 * dev id
 * u8
 */
static JSValue js_spi_send_u8(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv){
    CHECK_ARGC(2)
    ARGV_TO_SPI_HANDLE(0, handle)

    uint8_t in = 0 ;
    ARGV_TO_SPI_OUT_NUMBER(1, uint8_t, out)

    esp_err_t ret = spi_trans_int(handle, (uint8_t*)&in, (uint8_t*)&out, 8) ;
    if(ret!=ESP_OK) {
        THROW_EXCEPTION("spi bus transmit failed:%d", ret)
    }

    return JS_NewInt32(ctx, in) ;
}

/**
 * 
 * dev id
 * u16
 */
static JSValue js_spi_send_u16(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv){
    CHECK_ARGC(2)
    ARGV_TO_SPI_HANDLE(0, handle)

    uint16_t in = 0 ;
    ARGV_TO_SPI_OUT_NUMBER(1, uint16_t, out)

    esp_err_t ret = spi_trans_int(handle, (uint8_t*)&in, (uint8_t*)&out, 16) ;
    if(ret!=ESP_OK) {
        THROW_EXCEPTION("spi bus transmit failed:%d", ret)
    } 

    return JS_NewInt32(ctx, in) ;
}

/**
 * 
 * dev id
 * u32
 */
static JSValue js_spi_send_u32(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv){
    CHECK_ARGC(2)
    ARGV_TO_SPI_HANDLE(0, handle)
    ARGV_TO_UINT32(1, val)

    spi_transaction_t t;
    memset(&t, 0, sizeof(t));

    t.tx_buffer = (uint8_t *) & val ;
    t.length = 32 ;

    esp_err_t ret = spi_device_transmit(handle, &t) ;
    return JS_NewInt32(ctx, ret) ;
}

// 双工收发, 或 接受
#define SPI_TRANS(h, in, out, bit_length)                       \
        esp_err_t ret = spi_trans_int(h, in, out, bit_length) ; \
        if(ret!=ESP_OK) {                                       \
            THROW_EXCEPTION("spi bus transmit failed:%d", ret)  \
        }                                                       \
        return JS_NewInt32(ctx, in) ;

#define SPI_TRANS_FUNC(type, bit_length) \
    CHECK_ARGC(1) \
    ARGV_TO_SPI_HANDLE(0, handle) \
    type in_var = 0 ; \
    if( argc>1 && !JS_IsUndefined(argv[1]) && !JS_IsUndefined(argv[1]) ) { \
        type out_var ; \
        if(JS_ToUint32(ctx, &out_var, argv[1])!=0) { \
            THROW_EXCEPTION("arg must be a number") \
        } \
        SPI_TRANS(handle, in_var, (uint8_t*)&out_var, bit_length) \
    } \ 
    else { \
        SPI_TRANS(handle, in_var, NULL, bit_length) \
    }


static JSValue js_spi_trans_u8(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv){
    SPI_TRANS_FUNC(uint8_t, 8)
}
static JSValue js_spi_trans_u16(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv){
    SPI_TRANS_FUNC(uint16_t, 16)
}
static JSValue js_spi_trans_u32(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv){
    SPI_TRANS_FUNC(uint32_t, 32)
}


#define SPI_RECV_FUNC(type, bit_length)         \
    CHECK_ARGC(1)                               \
    ARGV_TO_SPI_HANDLE(0, handle)               \
    type in_var = 0 ;                           \
    SPI_TRANS(handle, in_var, NULL, bit_length)

static JSValue js_spi_recv_u8(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv){
    SPI_RECV_FUNC(uint8_t, 8)
}
static JSValue js_spi_recv_u16(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv){
    SPI_RECV_FUNC(uint16_t, 16)
}
static JSValue js_spi_recv_u32(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv){
    SPI_RECV_FUNC(uint32_t, 32)
}

void be_module_serial_spi_require(JSContext *ctx, JSValue pkg) {
    JSValue spi = JS_NewObject(ctx);
    JS_SetPropertyStr(ctx, pkg, "spi", spi);
    JS_SetPropertyStr(ctx, spi, "setup", JS_NewCFunction(ctx, js_spi_bus_setup, "setup", 1));
    JS_SetPropertyStr(ctx, spi, "free", JS_NewCFunction(ctx, js_spi_bus_free, "free", 1));
    JS_SetPropertyStr(ctx, spi, "addDevice", JS_NewCFunction(ctx, js_spi_add_device, "addDevice", 1));
    JS_SetPropertyStr(ctx, spi, "removeDevice", JS_NewCFunction(ctx, js_spi_device_remove, "removeDevice", 1));
    JS_SetPropertyStr(ctx, spi, "send", JS_NewCFunction(ctx, js_spi_send, "send", 1));
    JS_SetPropertyStr(ctx, spi, "sendU8", JS_NewCFunction(ctx, js_spi_send_u8, "sendU8", 1));
    JS_SetPropertyStr(ctx, spi, "sendU16", JS_NewCFunction(ctx, js_spi_send_u16, "sendU16", 1));
    JS_SetPropertyStr(ctx, spi, "sendU32", JS_NewCFunction(ctx, js_spi_send_u32, "sendU32", 1));
    JS_SetPropertyStr(ctx, spi, "recvU8", JS_NewCFunction(ctx, js_spi_recv_u8, "recvU8", 1));
    JS_SetPropertyStr(ctx, spi, "recvU16", JS_NewCFunction(ctx, js_spi_recv_u16, "recvU16", 1));
    JS_SetPropertyStr(ctx, spi, "recvU32", JS_NewCFunction(ctx, js_spi_recv_u32, "recvU32", 1));
    JS_SetPropertyStr(ctx, spi, "transU8", JS_NewCFunction(ctx, js_spi_trans_u8, "transU8", 1));
    JS_SetPropertyStr(ctx, spi, "transU16", JS_NewCFunction(ctx, js_spi_trans_u16, "transU16", 1));
    JS_SetPropertyStr(ctx, spi, "transU32", JS_NewCFunction(ctx, js_spi_trans_u32, "transU32", 1));

}

void be_module_serial_spi_reset(JSContext *ctx) {
    
    // 回收 SPI 资源
    for(uint8_t h=0; h<8; h++) {
        if(_spi_handle_pool1[h]!=NULL) {
            spi_bus_remove_device(_spi_handle_pool1[h]) ;
            _spi_handle_pool1[h] = NULL ;
        }
    }
    FREE_BUS_SPI(1)
    FREE_BUS_SPI(2)
    FREE_BUS_SPI(3)
    _spi_bus_setup = 0 ;

}

