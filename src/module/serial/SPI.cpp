#include "SPI.hpp"

using namespace std ;

namespace be {
    
    SPI * SPI::spi0 = nullptr ;
    SPI * SPI::spi1 = nullptr ;
    SPI * SPI::spi2 = nullptr ;

    DEFINE_NCLASS_META(SPI, NativeClass)
    std::vector<JSCFunctionListEntry> SPI::methods = {
        JS_CFUNC_DEF("setup", 0, SPI::setup),
        JS_CFUNC_DEF("spiNum", 0, SPI::spiNum),
        // JS_CFUNC_DEF("addDevice", 0, SPI::addDevice),
        // JS_CFUNC_DEF("removeDevice", 0, SPI::removeDevice),
        // JS_CFUNC_DEF("send", 0, SPI::send),
        // JS_CFUNC_DEF("sendU8", 0, SPI::sendU8),
        // JS_CFUNC_DEF("sendU16", 0, SPI::sendU16),
        // JS_CFUNC_DEF("sendU32", 0, SPI::sendU32),
        // JS_CFUNC_DEF("recvU8", 0, SPI::recvU8),
        // JS_CFUNC_DEF("recvU16", 0, SPI::recvU16),
        // JS_CFUNC_DEF("recvU32", 0, SPI::recvU32),
        // JS_CFUNC_DEF("transU8", 0, SPI::transU8),
        // JS_CFUNC_DEF("transU16", 0, SPI::transU16),
        // JS_CFUNC_DEF("transU32", 0, SPI::transU32),
    } ;
    

    SPI::SPI(JSContext * ctx, spi_host_device_t busnum)
        : NativeClass(ctx, build(ctx))
        , busnum(busnum)
    {
    }


    #define DEFINE_BUS(busconst, var)           \
        if(bus==busconst) {                     \
            if(!var) {                          \
                var = new SPI(ctx, busconst) ;  \
            }                                   \
            return var ;                        \
        }
    SPI * SPI::flyweight(JSContext * ctx, spi_host_device_t bus) {
        DEFINE_BUS(SPI1_HOST, spi0)
        else DEFINE_BUS(SPI2_HOST, spi1)
        else DEFINE_BUS(SPI3_HOST, spi2)
        return nullptr ;
    }


    spi_host_device_t SPI::spiNum() const {
        return busnum ;
    }
    JSValue SPI::spiNum(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv){
        THIS_NCLASS(SPI, that)
        return JS_NewInt32(ctx, that->busnum) ;
    }

    /**
     * options: {
     *   miso=-1
     *   mosi=-1
     *   sck
     *   quadwp_io_num=-1
     *   quadhd_io_num=-1
     *   max_transfer_sz=20480
     * }
     */
    JSValue SPI::setup(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv){
        THIS_NCLASS(SPI, that)
        ASSERT_ARGC(1)

        gpio_num_t GET_INT32_PROP_OPT(argv[0], "miso", misopin, GPIO_NUM_NC)
        gpio_num_t GET_INT32_PROP_OPT(argv[0], "mosi", mosipin, GPIO_NUM_NC)
        gpio_num_t GET_INT32_PROP(argv[0], "sck", sckpin, )
        int GET_INT32_PROP_OPT(argv[0], "quadwp_io_num", quadwp_io_num, -1)
        int GET_INT32_PROP_OPT(argv[0], "quadhd_io_num", quadhd_io_num, -1)
        int GET_INT32_PROP_OPT(argv[0], "max_transfer_sz", max_transfer_sz, 20480)
        
        printf("spi[%d] miso=%d, mosi=%d, clk=%d\n", that->busnum, misopin, mosipin, sckpin);
        // dn(busnum)

        spi_bus_config_t buscfg = {
            .mosi_io_num=mosipin,
            .miso_io_num=misopin,
            .sclk_io_num=sckpin,
            .quadwp_io_num=quadwp_io_num,
            .quadhd_io_num=quadhd_io_num,
            .max_transfer_sz=max_transfer_sz
        } ;

        esp_err_t ret = spi_bus_initialize(that->busnum, &buscfg, SPI_DMA_CH_AUTO);
        if(ret!=ESP_OK) {
            printf("spi_bus_initialize() failed with err: %d\n", ret) ;
        }

        return JS_NewInt32(ctx, ret) ;
    }

    // /**
    //  * cspin pin (-1 表示不使用)
    //  * freq
    //  * mode
    //  */
    // JSValue SPI::addDevice(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv){

    //     spi_device_handle_t handle = NULL ;
    //     ASSERT_ARGC(3)
    //     ARGV_TO_UINT8(0, cspin)
    //     ARGV_TO_INT32(1, freq)
    //     ARGV_TO_UINT8(2, mode)
        
    //     spi_device_interface_config_t devcfg = {
    //         .clock_speed_hz=freq,
    //         .mode=mode,
    //         .spics_io_num=cspin,
    //         .queue_size=7,                          //We want to be able to queue 7 transactions at a time
    //         .pre_cb=NULL,
    //     };
    //     esp_err_t ret = spi_bus_add_device(busnum, &devcfg, &handle);
    //     if(ret!=ESP_OK) {
    //         JSTHROW("spi_bus_add_device() failed with err: %d", ret)
    //     }

    //     return JS_NewInt32(ctx, ret!=0? -1: spiidx) ;
    // }
/*
    #define ARGV_TO_SPI_HANDLE(i, handle)                           \
        ARGV_TO_UINT8(i, spiidx)                                    \
        spi_device_handle_t handle = spi_handle_with_id(spiidx) ;   \
        if(handle==NULL) {                                          \
            JSTHROW("unknow spi device id:%d",spiidx)               \
        }
*/
    // /**
    //  * 
    //  * bus (1-3)
    //  * data {string|ArrayBuffer}
    //  * offset?
    //  * length?
    //  */
    // JSValue SPI::send(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv){
    //     ASSERT_ARGC(2)
    //     ARGV_TO_SPI_HANDLE(0, handle)

    //     int offset = 0 ;
    //     if(argc>=3) {
    //         if( JS_ToInt32(ctx, &offset, argv[2])!=0 ) {
    //             JSTHROW("Invalid param type for offset");
    //         }
    //     }
    //     int length = -1 ;
    //     if(argc>=4) {
    //         if( JS_ToInt32(ctx, &length, argv[3])!=0 ) {
    //             JSTHROW("Invalid param type for length");
    //         }
    //     }

    //     spi_transaction_t t;
    //     memset(&t, 0, sizeof(t));

    //     esp_err_t ret = -1 ;
    //     if(JS_IsString(argv[1])) {
    //         t.tx_buffer = JS_ToCStringLen(ctx, &t.length, argv[1]) ;
    //         if(length>-1 && length<t.length) {
    //             t.length = length ;
    //         }
    //         t.length*= 8 ;
    //         ret = spi_device_transmit(handle, &t) ;

    //         JS_FreeCString(ctx, t.tx_buffer) ;
    //         goto end ;
    //     }

    //     t.tx_buffer = JS_GetArrayBuffer(ctx, &t.length, argv[1]) ;
    //     if(t.tx_buffer) {
    //         if(length>-1 && length<t.length) {
    //             t.length = length ;
    //         }
    //         t.length*= 8 ;
    //         ret = spi_device_transmit(handle, &t) ;
    //         // JS_FreeValue(ctx, argv[1]) ;
    //     }
    //     else {
    //         JSTHROW("Invalid data")
    //     }

    // end:
    //     return JS_NewInt32(ctx, ret) ;
    // }

/*
    #define ARGV_TO_SPI_OUT_NUMBER(i, type, var)            \
        type var = 0 ;                                      \
        if( !JS_IsUndefined(var) && !JS_IsUndefined(var) ) {\
            if(JS_ToUint32(ctx, &out, argv[i])!=0) {        \
                JSTHROW("arg must be a number")     \
            }                                               \
        }
        */

    // inline esp_err_t spi_trans_int(spi_device_handle_t handle, uint8_t * rx_buff, uint8_t * tx_buff, size_t bit_length) {
    //     spi_transaction_t t;
    //     memset(&t, 0, sizeof(t));

    //     // t.flags = SPI_TRANS_USE_RXDATA | SPI_TRANS_USE_TXDATA ;
    //     t.tx_buffer = tx_buff ;
    //     t.rx_buffer = rx_buff ;
    //     t.length = bit_length ;
    //     t.rxlength = 0 ; // same to length
        
    //     return spi_device_transmit(handle, &t) ;
    // }

    

    // /**
    //  * 
    //  * dev id
    //  * u8
    //  */
    // JSValue SPI::sendU8(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv){
    //     ASSERT_ARGC(2)
    //     ARGV_TO_SPI_HANDLE(0, handle)

    //     uint8_t in = 0 ;
    //     ARGV_TO_SPI_OUT_NUMBER(1, uint8_t, out)

    //     esp_err_t ret = spi_trans_int(handle, (uint8_t*)&in, (uint8_t*)&out, 8) ;
    //     if(ret!=ESP_OK) {
    //         JSTHROW("spi bus transmit failed:%d", ret)
    //     }

    //     return JS_NewInt32(ctx, in) ;
    // }

    // /**
    //  * 
    //  * dev id
    //  * u16
    //  */
    // JSValue SPI::sendU16(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv){
    //     ASSERT_ARGC(2)
    //     ARGV_TO_SPI_HANDLE(0, handle)

    //     uint16_t in = 0 ;
    //     ARGV_TO_SPI_OUT_NUMBER(1, uint16_t, out)

    //     esp_err_t ret = spi_trans_int(handle, (uint8_t*)&in, (uint8_t*)&out, 16) ;
    //     if(ret!=ESP_OK) {
    //         JSTHROW("spi bus transmit failed:%d", ret)
    //     } 

    //     return JS_NewInt32(ctx, in) ;
    // }

    // /**
    //  * 
    //  * dev id
    //  * u32
    //  */
    // JSValue SPI::sendU32(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv){
    //     ASSERT_ARGC(2)
    //     ARGV_TO_SPI_HANDLE(0, handle)
    //     ARGV_TO_UINT32(1, val)

    //     spi_transaction_t t;
    //     memset(&t, 0, sizeof(t));

    //     t.tx_buffer = (uint8_t *) & val ;
    //     t.length = 32 ;

    //     esp_err_t ret = spi_device_transmit(handle, &t) ;
    //     return JS_NewInt32(ctx, ret) ;
    // }
/*
    // 双工收发, 或 接受
    #define SPI_TRANS(h, in, out, bit_length)                       \
            esp_err_t ret = spi_trans_int(h, in, out, bit_length) ; \
            if(ret!=ESP_OK) {                                       \
                JSTHROW("spi bus transmit failed:%d", ret)  \
            }                                                       \
            return JS_NewInt32(ctx, in) ;

    #define SPI_TRANS_FUNC(type, bit_length) \
        ASSERT_ARGC(1) \
        ARGV_TO_SPI_HANDLE(0, handle) \
        type in_var = 0 ; \
        if( argc>1 && !JS_IsUndefined(argv[1]) && !JS_IsUndefined(argv[1]) ) { \
            type out_var ; \
            if(JS_ToUint32(ctx, &out_var, argv[1])!=0) { \
                JSTHROW("arg must be a number") \
            } \
            SPI_TRANS(handle, in_var, (uint8_t*)&out_var, bit_length) \
        } \ 
        else { \
            SPI_TRANS(handle, in_var, NULL, bit_length) \
        }
*/

    // JSValue SPI::transU8(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv){
    //     SPI_TRANS_FUNC(uint8_t, 8)
    // }
    // JSValue SPI::transU16(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv){
    //     SPI_TRANS_FUNC(uint16_t, 16)
    // }
    // JSValue SPI::transU32(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv){
    //     SPI_TRANS_FUNC(uint32_t, 32)
    // }

/*
    #define SPI_RECV_FUNC(type, bit_length)         \
        ASSERT_ARGC(1)                               \
        ARGV_TO_SPI_HANDLE(0, handle)               \
        type in_var = 0 ;                           \
        SPI_TRANS(handle, in_var, NULL, bit_length)
        */

    // JSValue SPI::recvU8(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv){
    //     SPI_RECV_FUNC(uint8_t, 8)
    // }
    // JSValue SPI::recvU16(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv){
    //     SPI_RECV_FUNC(uint16_t, 16)
    // }
    // JSValue SPI::recvU32(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv){
    //     SPI_RECV_FUNC(uint32_t, 32)
    // }

}