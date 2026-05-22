#include "SPI.hpp"

#include "driver/gpio.h"
#include "driver/spi_master.h"

using namespace std ;

namespace be {

    /**
     * SPI 总线类
     * 
     * 用于配置和管理 ESP32 的 SPI 总线。SPI 是一种高速同步串行通信协议，
     * 常用于连接显示屏、SD 卡、以太网模块、传感器等外设。
     * 
     * ESP32 通常有 3 个 SPI 总线（SPI1、SPI2、SPI3），其中：
     * - SPI1 通常用于连接 Flash，不建议用户使用
     * - SPI2 和 SPI3 可供用户自由使用
     * 
     * serial 模块会自动创建 SPI 实例并通过 `spi1`, `spi2`（部分型号有 `spi3`）导出。
     * 用户直接通过 serial 模块访问这些实例，无需手动创建。
     * 
     * **不同芯片型号导出的 SPI 对象不同**：
     * - ESP32/ESP32-S3：spi1, spi2
     * - ESP32-C2/C3/C6/H2：spi1, spi2
     * - ESP32-P4：spi1, spi2, spi3
     * 
     * 示例：
     * ```javascript
     * import * as serial from "serial"
     * 
     * // 访问 SPI2 总线实例（推荐）
     * const spi = serial.spi2
     * 
     * // 配置 SPI 引脚
     * spi.setup({
     *     miso: 19,  // MISO 引脚（可选）
     *     mosi: 23,  // MOSI 引脚（可选）
     *     sck: 18    // 时钟引脚（必需）
     * })
     * 
     * // 获取总线编号
     * console.log("SPI 总线:", spi.spiNum())
     * ```
     * 
     * @class SPI
     * @module serial
     * @extends NativeClass
     */
    SPI * SPI::spi0 = nullptr ;    
    #if SOC_SPI_PERIPH_NUM > 1
    SPI * SPI::spi1 = nullptr ;
    #endif
    #if SOC_SPI_PERIPH_NUM > 2
    SPI * SPI::spi2 = nullptr ;
    #endif
    #if SOC_SPI_PERIPH_NUM > 3
    SPI * SPI::spi3 = nullptr ;
    #endif

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
    

    SPI::SPI(JSContext * ctx, int busnum)
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
    SPI * SPI::flyweight(JSContext * ctx, int bus) {
        DEFINE_BUS(SPI1_HOST, spi0)
        #if SOC_SPI_PERIPH_NUM > 1
        else DEFINE_BUS(SPI2_HOST, spi1)
        #endif
        #if SOC_SPI_PERIPH_NUM > 2
        else DEFINE_BUS(SPI3_HOST, spi2)
        #endif
        #if SOC_SPI_PERIPH_NUM > 3
        else DEFINE_BUS(SPI4_HOST, spi3)
        #endif
        return nullptr ;
    }


    int SPI::spiNum() const {
        return busnum ;
    }
    /**
     * 获取当前 SPI 实例的总线编号
     * 
     * 示例：
     * ```javascript
     * import * as serial from "serial"
     * 
     * // 获取 SPI2 总线实例
     * const spi = serial.spi2
     * 
     * // 获取总线编号
     * console.log(spi.spiNum())  // 输出: 2
     * ```
     *
     * @module serial
     * @class SPI
     * @function spiNum
     * @return number SPI 总线编号
     */
    JSValue SPI::spiNum(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv){
        THIS_NCLASS(SPI, that)
        return JS_NewInt32(ctx, that->busnum) ;
    }

    /**
     * 配置并初始化 SPI 总线
     * 
     * 初始化 SPI 总线，配置引脚和传输参数。SPI 总线初始化后，
     * 可以被多个 SPI 设备共享使用（通过不同的 CS 引脚）。
     * 
     * 示例：
     * ```javascript
     * import * as serial from "serial"
     * 
     * // 获取 SPI2 总线实例
     * const spi = serial.spi2
     * 
     * // 基本配置（半双工模式，只需要 MOSI 和 SCK）
     * let ret = spi.setup({
     *     mosi: 23,  // MOSI 引脚
     *     sck: 18    // 时钟引脚
     * })
     * if(ret != 0) {
     *     console.log("SPI 初始化失败")
     * }
     * 
     * // 全双工配置（需要 MISO、MOSI 和 SCK）
     * const spi2 = serial.spi2
     * ret = spi2.setup({
     *     miso: 19,  // MISO 引脚
     *     mosi: 23,  // MOSI 引脚
     *     sck: 18,   // 时钟引脚
     *     max_transfer_sz: 4096  // 最大传输字节数
     * })
     * 
     * // 与 W5500 以太网模块配合使用
     * import { W5500 } from "eth"
     * const eth = new W5500()
     * 
     * // 先初始化 SPI 总线
     * const spi = serial.spi2
     * spi.setup({
     *     mosi: 23,
     *     miso: 19,
     *     sck: 18
     * })
     * 
     * // 然后初始化 W5500（使用相同的 SPI 总线）
     * eth.setup({
     *     spi: 2,   // 使用 SPI2（对应 serial.spi2）
     *     cs: 5     // CS 引脚
     * })
     * 
     * // 与 SD 卡配合使用
     * import { SDCard } from "sdcard"
     * const sd = new SDCard()
     * 
     * // 使用 SPI2 初始化 SD 卡
     * sd.setup({
     *     spi: 2,       // 使用 SPI2（对应 serial.spi2）
     *     cs: 13,       // CS 引脚
     *     mount: "/sd"  // 挂载路径
     * })
     * ```
     *
     * @module serial
     * @class SPI
     * @function setup
     * @param options:object 配置选项对象
     *     {
     *         miso?: number,         // MISO 引脚 GPIO 编号，默认 -1
     *         mosi?: number,         // MOSI 引脚 GPIO 编号，默认 -1
     *         sck: number,           // SCK 引脚 GPIO 编号（必需）
     *         quadwp_io_num?: number, // Quad WP IO 编号，默认 -1
     *         quadhd_io_num?: number, // Quad HD IO 编号，默认 -1
     *         max_transfer_sz?: number, // 最大传输大小，默认 20480
     *         flags?: number          // SPI 总线标志位（如 SPICOMMON_BUSFLAG_QUAD），默认 0
     *     }
     *     注意：当 quadwp_io_num 和 quadhd_io_num 均被设置时，会自动添加 SPICOMMON_BUSFLAG_QUAD 标志
     * @return number 错误代码，0 表示成功
     * @throws SPI 总线初始化失败
     */
    JSValue SPI::setup(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv){
        THIS_NCLASS(SPI, that)
        ASSERT_ARGC(1)

        gpio_num_t GET_GPIO_PROP_OPT(argv[0], "miso", misopin, GPIO_NUM_NC)
        gpio_num_t GET_GPIO_PROP_OPT(argv[0], "mosi", mosipin, GPIO_NUM_NC)
        gpio_num_t GET_GPIO_PROP(argv[0], "sck", sckpin, )
        int GET_INT32_PROP_OPT(argv[0], "quadwp_io_num", quadwp_io_num, -1)
        int GET_INT32_PROP_OPT(argv[0], "quadhd_io_num", quadhd_io_num, -1)
        int GET_INT32_PROP_OPT(argv[0], "max_transfer_sz", max_transfer_sz, 20480)
        int GET_INT32_PROP_OPT(argv[0], "flags", flags, 0)

        if(quadwp_io_num!=-1 && quadhd_io_num!=-1) {
            flags |= SPICOMMON_BUSFLAG_QUAD ;
        }

        spi_bus_config_t buscfg = {
            .mosi_io_num=mosipin,
            .miso_io_num=misopin,
            .sclk_io_num=sckpin,
            .quadwp_io_num=quadwp_io_num,
            .quadhd_io_num=quadhd_io_num,
            .max_transfer_sz=max_transfer_sz
        } ;
        buscfg.flags = (uint32_t)flags ;

        esp_err_t ret = spi_bus_initialize((spi_host_device_t)that->busnum, &buscfg, SPI_DMA_CH_AUTO);
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