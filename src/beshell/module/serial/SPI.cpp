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
        JS_CFUNC_DEF("addDevice", 0, SPI::addDevice),
        JS_CFUNC_DEF("removeDevice", 0, SPI::removeDevice),
        JS_CFUNC_DEF("send", 0, SPI::send),
        JS_CFUNC_DEF("trans", 0, SPI::trans),
        JS_CFUNC_DEF("sendU8", 0, SPI::sendU8),
        JS_CFUNC_DEF("sendU16", 0, SPI::sendU16),
        JS_CFUNC_DEF("sendU32", 0, SPI::sendU32),
        JS_CFUNC_DEF("recvU8", 0, SPI::recvU8),
        JS_CFUNC_DEF("recvU16", 0, SPI::recvU16),
        JS_CFUNC_DEF("recvU32", 0, SPI::recvU32),
        JS_CFUNC_DEF("transU8", 0, SPI::transU8),
        JS_CFUNC_DEF("transU16", 0, SPI::transU16),
        JS_CFUNC_DEF("transU32", 0, SPI::transU32),
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

    // ================================================================
    // 内部辅助函数
    // ================================================================

    /**
     * 执行一次 SPI 传输 (底层)
     * @param handle SPI 设备句柄
     * @param tx 发送缓冲区 (可为 NULL，此时发送 0)
     * @param rx 接收缓冲区 (可为 NULL，丢弃接收数据)
     * @param bit_length 传输位数
     */
    static inline esp_err_t spi_transfer(spi_device_handle_t handle,
                                          const void* tx, void* rx,
                                          size_t bit_length) {
        spi_transaction_t t = {};
        t.tx_buffer = tx;
        t.rx_buffer = rx;
        t.length = bit_length;
        return spi_device_transmit(handle, &t);
    }

    // ================================================================
    // addDevice — 添加 SPI 设备
    // ================================================================
    /**
     * 向 SPI 总线添加一个设备。
     *
     * ```javascript
     * // 基本用法（无 CS 引脚，如 74HC165）
     * let devId = spi.addDevice({ cs: -1, freq: 1000000 })
     *
     * // 标准用法（带 CS 引脚）
     * let devId = spi.addDevice({ cs: 5, freq: 8000000, mode: 0 })
     *
     * // 高级用法
     * let devId = spi.addDevice({
     *     cs: 10,
     *     freq: 20000000,
     *     mode: 3,
     *     queue_size: 4,
     *     flags: 0
     * })
     * ```
     *
     * @param options 配置对象
     *   - cs: CS 引脚号, -1 表示不使用 (默认 -1)
     *   - freq: 时钟频率 Hz (默认 1000000)
     *   - mode: SPI 模式 0-3 (默认 0)
     *   - queue_size: 事务队列大小 (默认 1)
     *   - flags: 设备标志位 (默认 0)
     * @return number 设备 ID，用于后续的 send/recv/trans 操作
     */
    JSValue SPI::addDevice(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        THIS_NCLASS(SPI, that)
        ASSERT_ARGC(1)

        int GET_INT32_PROP_OPT(argv[0], "cs", cs, -1)
        int GET_INT32_PROP_OPT(argv[0], "freq", freq, 1000000)
        int GET_INT32_PROP_OPT(argv[0], "mode", mode, 0)
        int GET_INT32_PROP_OPT(argv[0], "queue_size", queue_size, 1)
        int GET_INT32_PROP_OPT(argv[0], "flags", flags, 0)

        spi_device_interface_config_t devcfg = {};
        devcfg.clock_speed_hz = freq;
        devcfg.mode = (uint8_t)mode;
        devcfg.spics_io_num = cs;
        devcfg.queue_size = queue_size;
        devcfg.flags = flags;

        spi_device_handle_t handle = nullptr;
        esp_err_t ret = spi_bus_add_device((spi_host_device_t)that->busnum, &devcfg, &handle);
        if (ret != ESP_OK) {
            JSTHROW("spi_bus_add_device() failed, err: %d", ret)
        }

        int devId = that->nextDeviceId++;
        that->devices[devId] = handle;
        return JS_NewInt32(ctx, devId);
    }

    // ================================================================
    // removeDevice — 移除 SPI 设备
    // ================================================================
    /**
     * 从 SPI 总线移除一个设备。
     *
     * ```javascript
     * spi.removeDevice(devId)
     * ```
     *
     * @param devId 设备 ID (addDevice 返回的)
     */
    JSValue SPI::removeDevice(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        THIS_NCLASS(SPI, that)
        ASSERT_ARGC(1)
        ARGV_TO_INT32(0, devId)

        auto it = that->devices.find(devId);
        if (it == that->devices.end()) {
            JSTHROW("Unknown SPI device id: %d", devId)
        }

        spi_bus_remove_device(it->second);
        that->devices.erase(it);
        return JS_UNDEFINED;
    }

    // ================================================================
    // sendU8 / sendU16 / sendU32 — 单值发送 (TX only)
    // ================================================================
    /**
     * 发送一个 8 位值，丢弃接收数据。
     * @param devId 设备 ID
     * @param value 要发送的 8 位值
     * @return number 错误码，0 表示成功
     */
    JSValue SPI::sendU8(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        THIS_NCLASS(SPI, that)
        ASSERT_ARGC(2)
        ARGV_TO_INT32(0, devId)
        ARGV_TO_UINT8(1, val)

        auto it = that->devices.find(devId);
        if (it == that->devices.end()) {
            JSTHROW("Unknown SPI device id: %d", devId)
        }

        uint8_t tx = val;
        esp_err_t ret = spi_transfer(it->second, &tx, nullptr, 8);
        return JS_NewInt32(ctx, ret);
    }

    /**
     * 发送一个 16 位值，丢弃接收数据。
     * @param devId 设备 ID
     * @param value 要发送的 16 位值
     * @return number 错误码，0 表示成功
     */
    JSValue SPI::sendU16(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        THIS_NCLASS(SPI, that)
        ASSERT_ARGC(2)
        ARGV_TO_INT32(0, devId)
        ARGV_TO_UINT16(1, val)

        auto it = that->devices.find(devId);
        if (it == that->devices.end()) {
            JSTHROW("Unknown SPI device id: %d", devId)
        }

        uint16_t tx = val;
        esp_err_t ret = spi_transfer(it->second, &tx, nullptr, 16);
        return JS_NewInt32(ctx, ret);
    }

    /**
     * 发送一个 32 位值，丢弃接收数据。
     * @param devId 设备 ID
     * @param value 要发送的 32 位值
     * @return number 错误码，0 表示成功
     */
    JSValue SPI::sendU32(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        THIS_NCLASS(SPI, that)
        ASSERT_ARGC(2)
        ARGV_TO_INT32(0, devId)
        ARGV_TO_UINT32(1, val)

        auto it = that->devices.find(devId);
        if (it == that->devices.end()) {
            JSTHROW("Unknown SPI device id: %d", devId)
        }

        uint32_t tx = val;
        esp_err_t ret = spi_transfer(it->second, &tx, nullptr, 32);
        return JS_NewInt32(ctx, ret);
    }

    // ================================================================
    // recvU8 / recvU16 / recvU32 — 单值接收 (RX only)
    // ================================================================
    /**
     * 接收一个 8 位值（发送 0x00）。
     * @param devId 设备 ID
     * @return number 接收到的 8 位值 (0-255)
     */
    JSValue SPI::recvU8(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        THIS_NCLASS(SPI, that)
        ASSERT_ARGC(1)
        ARGV_TO_INT32(0, devId)

        auto it = that->devices.find(devId);
        if (it == that->devices.end()) {
            JSTHROW("Unknown SPI device id: %d", devId)
        }

        uint8_t rx = 0;
        esp_err_t ret = spi_transfer(it->second, nullptr, &rx, 8);
        if (ret != ESP_OK) {
            JSTHROW("SPI receive failed, err: %d", ret)
        }
        return JS_NewInt32(ctx, rx);
    }

    /**
     * 接收一个 16 位值（发送 0x0000）。
     * @param devId 设备 ID
     * @return number 接收到的 16 位值 (0-65535)
     */
    JSValue SPI::recvU16(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        THIS_NCLASS(SPI, that)
        ASSERT_ARGC(1)
        ARGV_TO_INT32(0, devId)

        auto it = that->devices.find(devId);
        if (it == that->devices.end()) {
            JSTHROW("Unknown SPI device id: %d", devId)
        }

        uint16_t rx = 0;
        esp_err_t ret = spi_transfer(it->second, nullptr, &rx, 16);
        if (ret != ESP_OK) {
            JSTHROW("SPI receive failed, err: %d", ret)
        }
        return JS_NewInt32(ctx, rx);
    }

    /**
     * 接收一个 32 位值（发送 0x00000000）。
     * @param devId 设备 ID
     * @return number 接收到的 32 位值
     */
    JSValue SPI::recvU32(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        THIS_NCLASS(SPI, that)
        ASSERT_ARGC(1)
        ARGV_TO_INT32(0, devId)

        auto it = that->devices.find(devId);
        if (it == that->devices.end()) {
            JSTHROW("Unknown SPI device id: %d", devId)
        }

        uint32_t rx = 0;
        esp_err_t ret = spi_transfer(it->second, nullptr, &rx, 32);
        if (ret != ESP_OK) {
            JSTHROW("SPI receive failed, err: %d", ret)
        }
        return JS_NewInt32(ctx, (int32_t)rx);  // JS 只有 int32，需要处理符号
    }

    // ================================================================
    // transU8 / transU16 / transU32 — 单值全双工传输
    // ================================================================
    /**
     * 全双工传输 1 字节。
     * @param devId 设备 ID
     * @param outVal 发送的值 (可选，默认 0)
     * @return number 接收到的 8 位值
     */
    JSValue SPI::transU8(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        THIS_NCLASS(SPI, that)
        ASSERT_ARGC(1)
        ARGV_TO_INT32(0, devId)

        auto it = that->devices.find(devId);
        if (it == that->devices.end()) {
            JSTHROW("Unknown SPI device id: %d", devId)
        }

        uint8_t tx = 0, rx = 0;
        if (argc > 1 && !JS_IsUndefined(argv[1])) {
            ARGV_TO_UINT8(1, tx)
        }

        esp_err_t ret = spi_transfer(it->second, &tx, &rx, 8);
        if (ret != ESP_OK) {
            JSTHROW("SPI transceive failed, err: %d", ret)
        }
        return JS_NewInt32(ctx, rx);
    }

    /**
     * 全双工传输 2 字节。
     * @param devId 设备 ID
     * @param outVal 发送的值 (可选，默认 0)
     * @return number 接收到的 16 位值
     */
    JSValue SPI::transU16(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        THIS_NCLASS(SPI, that)
        ASSERT_ARGC(1)
        ARGV_TO_INT32(0, devId)

        auto it = that->devices.find(devId);
        if (it == that->devices.end()) {
            JSTHROW("Unknown SPI device id: %d", devId)
        }

        uint16_t tx = 0, rx = 0;
        if (argc > 1 && !JS_IsUndefined(argv[1])) {
            ARGV_TO_UINT16(1, tx)
        }

        esp_err_t ret = spi_transfer(it->second, &tx, &rx, 16);
        if (ret != ESP_OK) {
            JSTHROW("SPI transceive failed, err: %d", ret)
        }
        return JS_NewInt32(ctx, rx);
    }

    /**
     * 全双工传输 4 字节。
     * @param devId 设备 ID
     * @param outVal 发送的值 (可选，默认 0)
     * @return number 接收到的 32 位值
     */
    JSValue SPI::transU32(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        THIS_NCLASS(SPI, that)
        ASSERT_ARGC(1)
        ARGV_TO_INT32(0, devId)

        auto it = that->devices.find(devId);
        if (it == that->devices.end()) {
            JSTHROW("Unknown SPI device id: %d", devId)
        }

        uint32_t tx = 0, rx = 0;
        if (argc > 1 && !JS_IsUndefined(argv[1])) {
            ARGV_TO_UINT32(1, tx)
        }

        esp_err_t ret = spi_transfer(it->second, &tx, &rx, 32);
        if (ret != ESP_OK) {
            JSTHROW("SPI transceive failed, err: %d", ret)
        }
        return JS_NewInt32(ctx, (int32_t)rx);
    }

    // ================================================================
    // send — 缓冲区发送 (TX only)
    // ================================================================
    /**
     * 发送缓冲区数据 (TX only，丢弃接收数据)。
     *
     * ```javascript
     * // 发送字符串
     * spi.send(devId, "hello")
     *
     * // 发送 ArrayBuffer
     * let buf = new ArrayBuffer(4)
     * spi.send(devId, buf)
     *
     * // 从指定偏移量开始发送
     * spi.send(devId, buf, 2)
     *
     * // 发送指定长度
     * spi.send(devId, buf, 0, 4)
     * ```
     *
     * @param devId 设备 ID
     * @param data 要发送的数据 (string 或 ArrayBuffer)
     * @param offset 起始偏移量 (可选，默认 0)
     * @param length 发送长度 (可选，默认全部)
     * @return number 错误码，0 表示成功
     */
    JSValue SPI::send(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        THIS_NCLASS(SPI, that)
        ASSERT_ARGC(2)
        ARGV_TO_INT32(0, devId)

        auto it = that->devices.find(devId);
        if (it == that->devices.end()) {
            JSTHROW("Unknown SPI device id: %d", devId)
        }
        spi_device_handle_t handle = it->second;

        int offset = 0;
        if (argc >= 3) {
            ARGV_TO_INT32(2, offset)
        }
        int userLength = -1;
        if (argc >= 4) {
            ARGV_TO_INT32(3, userLength)
        }

        spi_transaction_t t = {};
        esp_err_t ret = ESP_FAIL;

        if (JS_IsString(argv[1])) {
            size_t strLen;
            const char* strData = JS_ToCStringLen(ctx, &strLen, argv[1]);
            size_t dataLen = strLen;
            if (offset < 0) offset = 0;
            if ((size_t)offset > dataLen) offset = (int)dataLen;
            size_t remaining = dataLen - offset;
            size_t xferLen = (userLength >= 0 && (size_t)userLength < remaining)
                                ? (size_t)userLength : remaining;
            t.tx_buffer = strData + offset;
            t.length = xferLen * 8;
            ret = spi_device_transmit(handle, &t);
            JS_FreeCString(ctx, strData);
        } else {
            size_t dataLen;
            uint8_t* buf = JS_GetArrayBuffer(ctx, &dataLen, argv[1]);
            if (!buf) {
                JSTHROW("data must be a string or ArrayBuffer")
            }
            if (offset < 0) offset = 0;
            if ((size_t)offset > dataLen) offset = (int)dataLen;
            size_t remaining = dataLen - offset;
            size_t xferLen = (userLength >= 0 && (size_t)userLength < remaining)
                                ? (size_t)userLength : remaining;
            t.tx_buffer = buf + offset;
            t.length = xferLen * 8;
            ret = spi_device_transmit(handle, &t);
        }

        return JS_NewInt32(ctx, ret);
    }

    // ================================================================
    // trans — 缓冲区全双工传输
    // ================================================================
    /**
     * 全双工缓冲区传输，同时发送和接收数据。
     *
     * ```javascript
     * // 发送 ArrayBuffer，返回接收到的 ArrayBuffer
     * let txBuf = new ArrayBuffer(4)
     * let rxBuf = spi.trans(devId, txBuf)
     *
     * // 从指定偏移量开始
     * let rxBuf = spi.trans(devId, txBuf, 2)
     *
     * // 指定长度
     * let rxBuf = spi.trans(devId, txBuf, 0, 4)
     *
     * // 发送字符串
     * let rxBuf = spi.trans(devId, "hello")
     * ```
     *
     * @param devId 设备 ID
     * @param data 要发送的数据 (string 或 ArrayBuffer)
     * @param offset 起始偏移量 (可选，默认 0)
     * @param length 传输长度 (可选，默认全部)
     * @return ArrayBuffer 接收到的数据，长度与发送数据相同
     */
    JSValue SPI::trans(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        THIS_NCLASS(SPI, that)
        ASSERT_ARGC(2)
        ARGV_TO_INT32(0, devId)

        auto it = that->devices.find(devId);
        if (it == that->devices.end()) {
            JSTHROW("Unknown SPI device id: %d", devId)
        }
        spi_device_handle_t handle = it->second;

        int offset = 0;
        if (argc >= 3) {
            ARGV_TO_INT32(2, offset)
        }
        int userLength = -1;
        if (argc >= 4) {
            ARGV_TO_INT32(3, userLength)
        }

        const void* txPtr = nullptr;
        size_t xferLen = 0;
        const char* strData = nullptr;

        // 计算传输长度和 TX 指针
        if (JS_IsString(argv[1])) {
            size_t dataLen;
            strData = JS_ToCStringLen(ctx, &dataLen, argv[1]);
            if (offset < 0) offset = 0;
            if ((size_t)offset > dataLen) offset = (int)dataLen;
            size_t remaining = dataLen - offset;
            xferLen = (userLength >= 0 && (size_t)userLength < remaining)
                        ? (size_t)userLength : remaining;
            txPtr = strData + offset;
        } else {
            size_t dataLen;
            uint8_t* buf = JS_GetArrayBuffer(ctx, &dataLen, argv[1]);
            if (!buf) {
                JSTHROW("data must be a string or ArrayBuffer")
            }
            if (offset < 0) offset = 0;
            if ((size_t)offset > dataLen) offset = (int)dataLen;
            size_t remaining = dataLen - offset;
            xferLen = (userLength >= 0 && (size_t)userLength < remaining)
                        ? (size_t)userLength : remaining;
            txPtr = buf + offset;
        }

        if (xferLen == 0) {
            if (strData) JS_FreeCString(ctx, strData);
            return JS_NewArrayBufferCopy(ctx, nullptr, 0);
        }

        // 分配接收缓冲区
        uint8_t* rxBuf = (uint8_t*)malloc(xferLen);
        if (!rxBuf) {
            if (strData) JS_FreeCString(ctx, strData);
            JSTHROW("Failed to allocate RX buffer")
        }

        spi_transaction_t t = {};
        t.tx_buffer = txPtr;
        t.rx_buffer = rxBuf;
        t.length = xferLen * 8;

        esp_err_t ret = spi_device_transmit(handle, &t);

        if (strData) JS_FreeCString(ctx, strData);

        if (ret != ESP_OK) {
            free(rxBuf);
            JSTHROW("SPI transceive failed, err: %d", ret)
        }

        JSValue result = JS_NewArrayBufferCopy(ctx, rxBuf, xferLen);
        free(rxBuf);
        return result;
    }

}