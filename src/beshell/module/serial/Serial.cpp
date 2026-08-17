/**
 * Serial 串口模块
 * 
 * 提供对 ESP32 硬件串口外设的访问，包括 UART、I2C、SPI 等。
 * 这些类不需要实例化，serial 模块会根据芯片的型号，自动初始化并导出可用的串口对象。
 * 
 * ## 使用方式
 * 
 * ```javascript
 * import * as serial from "serial"
 * 
 * // 访问 UART2
 * serial.uart2.setup({ tx: 17, rx: 16, baudrate: 115200 })
 * 
 * // 访问 I2C0
 * serial.i2c0.setup({ sda: 21, scl: 22 })
 * 
 * // 访问 SPI2
 * serial.spi2.setup({ mosi: 23, miso: 19, sck: 18 })
 * 
 * // 访问 I2S0
 * serial.i2s0.setup({ bck: 26, ws: 25, dout: 22 })
 * ```
 * 
 * ## 硬件资源差异
 * 
 * **重要**：不同型号的 ESP32 芯片，所提供的硬件串口外设数量不一样。
 * serial 模块只会导出当前芯片实际拥有的外设对象。
 * 
 * | 芯片型号 | UART | I2C | SPI | 说明 |
 * |---------|------|-----|-----|------|
 * | ESP32   | 3 (uart0-2) | 2 (i2c0-1) | 2 (spi1-2) | 经典款，资源最丰富 |
 * | ESP32-S2| 2 (uart0-1) | 2 (i2c0-1) | 2 (spi1-2) | 无 uart2 |
 * | ESP32-S3| 3 (uart0-2) | 2 (i2c0-1) | 2 (spi1-2) | 与 ESP32 相同 |
 * | ESP32-C2| 2 (uart0-1) | 1 (i2c0)   | 2 (spi1-2) | 精简版，只有 1 个 I2C |
 * | ESP32-C3| 2 (uart0-1) | 1 (i2c0)   | 2 (spi1-2) | 与 C2 相同 |
 * | ESP32-C6| 3 (uart0-2) | 1 (i2c0)   | 2 (spi1-2) | 新增 LP UART |
 * | ESP32-H2| 2 (uart0-1) | 2 (i2c0-1) | 2 (spi1-2) | 低功耗蓝牙芯片 |
 * | ESP32-P4| 6 (uart0-5) | 2 (i2c0-1) | 3 (spi1-3) | 高性能，资源最丰富 |
 * 
 * > **注意**：`spi0` 用于内部 Flash/PSRAM，不对外导出。
 * > SPI 导出的对象是 `spi1`, `spi2`（部分型号有 `spi3`）。
 * 
 * ## 各芯片导出对象一览
 * 
 * ### ESP32 / ESP32-S3
 * ```javascript
 * {
 *     uart0, uart1, uart2,    // 3 个 UART
 *     i2c0, i2c1,             // 2 个 I2C
 *     spi1, spi2              // 2 个 SPI（spi0 内部使用）
 * }
 * ```
 * 
 * ### ESP32-S2 / ESP32-C2 / ESP32-C3
 * ```javascript
 * {
 *     uart0, uart1,           // 2 个 UART（无 uart2）
 *     i2c0,                   // 1 个 I2C（C2/C3 只有 i2c0）
 *     spi1, spi2              // 2 个 SPI
 * }
 * ```
 * 
 * ### ESP32-C6
 * ```javascript
 * {
 *     uart0, uart1, uart2,    // 3 个 UART
 *     i2c0,                   // 1 个 I2C
 *     spi1, spi2,             // 2 个 SPI
 *     // 另有 uartlp0（低功耗 UART）
 * }
 * ```
 * 
 * ### ESP32-H2
 * ```javascript
 * {
 *     uart0, uart1,           // 2 个 UART
 *     i2c0, i2c1,             // 2 个 I2C
 *     spi1, spi2              // 2 个 SPI
 * }
 * ```
 * 
 * ### ESP32-P4
 * ```javascript
 * {
 *     uart0, uart1, uart2, uart3, uart4,  // 5 个 HP UART
 *     i2c0, i2c1,                         // 2 个 I2C
 *     spi1, spi2, spi3,                   // 3 个 SPI
 *     // 另有 uartlp0（低功耗 UART）
 * }
 * ```
 * 
 * ## 对象说明
 * 
 * ### UART 对象
 * - [uart0](UART.html) - UART0，通常用于调试输出和程序下载
 * - [uart1](UART.html) - UART1，用户可自由使用
 * - [uart2](UART.html) - UART2，用户可自由使用（部分型号没有）
 * - uartlp0 - 低功耗 UART（仅部分型号支持）
 * 
 * ### I2C 对象
 * - [i2c0](I2C.html) - I2C0，主/从模式支持
 * - [i2c1](I2C.html) - I2C1，主/从模式支持（部分型号没有）
 * 
 * ### SPI 对象
 * - [spi1](SPI.html) - SPI1，通常对应 SPI2_HOST
 * - [spi2](SPI.html) - SPI2，通常对应 SPI3_HOST
 * - spi3 - SPI3，仅 ESP32-P4 支持
 * 
 * ### I2S 对象
 * - [i2s0](I2S.html) - I2S0，音频播放/录音（STD 模式）
 * - i2s1 - I2S1（部分型号没有）
 * 
 * > I2S 导出数量由芯片的 `SOC_I2S_NUM` 决定：ESP32/S3/P4 有 i2s0、i2s1，
 * > S2/C3/C6/H2 只有 i2s0，C2 无 I2S 外设。
 * 
 * ## 使用建议
 * 
 * 1. **UART0**：保留给调试和程序下载使用，应用程序建议使用 uart1/uart2
 * 2. **SPI**：使用前需要先调用 `setup()` 初始化总线，再配置外设（如 W5500、SD 卡）
 * 3. **I2C**：多个设备可以共享同一个 I2C 总线，通过设备地址区分
 * 
 * ## 示例代码
 * 
 * ```javascript
 * import * as serial from "serial"
 * import { W5500 } from "eth"
 * 
 * // 检查可用的串口对象
 * console.log("Available UARTs:", Object.keys(serial).filter(k => k.startsWith("uart")))
 * console.log("Available I2Cs:", Object.keys(serial).filter(k => k.startsWith("i2c")))
 * console.log("Available SPIs:", Object.keys(serial).filter(k => k.startsWith("spi")))
 * 
 * // 使用 UART2 连接 GPS 模块
 * if (serial.uart2) {
 *     serial.uart2.setup({
 *         tx: 17,
 *         rx: 16,
 *         baudrate: 9600
 *     })
 *     serial.uart2.listen((data) => {
 *         console.log("GPS data:", data)
 *     })
 * }
 * 
 * // 使用 I2C0 连接传感器
 * if (serial.i2c0) {
 *     serial.i2c0.setup({
 *         sda: 21,
 *         scl: 22,
 *         freq: 400000
 *     })
 *     serial.i2c0.scan()  // 扫描设备
 * }
 * 
 * // 使用 SPI2 连接 W5500 以太网模块
 * if (serial.spi2) {
 *     serial.spi2.setup({
 *         mosi: 23,
 *         miso: 19,
 *         sck: 18
 *     })
 *     
 *     const eth = new W5500()
 *     eth.setup({
 *         spi: 2,   // 对应 serial.spi2
 *         cs: 5
 *     })
 * }
 * ```
 * 
 * @module serial
 */
#include "Serial.hpp"
#include "UART.hpp"
#include "I2C.hpp"
#include "SPI.hpp"
#include "I2S.hpp"
#include "I2S-legacy.hpp"
#include <driver/uart.h>
#include <driver/spi_master.h>


using namespace std ;  

namespace be {

    char const * const Serial::name = "serial" ;

    Serial::Serial(JSContext * ctx, const char * name)
        : NativeModule(ctx,name,0)
    {
        // UART ---------------
#if CONFIG_BESHELL_SERIAL_USE_UART
        exportName("uart0") ;
        
        #if SOC_UART_HP_NUM>1
        exportName("uart1") ;
        #endif

        #if SOC_UART_HP_NUM>2
        exportName("uart2") ;
        #endif

        #if SOC_UART_HP_NUM>3
        exportName("uart3") ;
        #endif

        #if SOC_UART_HP_NUM>4
        exportName("uart4") ;
        #endif

        #if SOC_UART_LP_NUM>0
        exportName("uartlp0") ;
        #endif
#endif

        // I2C ---------------
#if CONFIG_BESHELL_SERIAL_USE_I2C
        exportName("i2c0") ;

        #if SOC_HP_I2C_NUM > 1
        exportName("i2c1") ;
        #endif

        #if SOC_LP_I2C_NUM > 0
        exportName("i2clp0") ;
        #endif
#endif

        // SPI ---------------
#if CONFIG_BESHELL_SERIAL_USE_SPI
        #if SOC_SPI_PERIPH_NUM>0
        exportName("spi0") ;
        #endif

        #if SOC_SPI_PERIPH_NUM>1
        exportName("spi1") ;
        #endif

        #if SOC_SPI_PERIPH_NUM>2
        exportName("spi2") ;
        #endif

        #if SOC_SPI_PERIPH_NUM>3
        exportName("spi3") ;
        #endif
#endif

        // I2S ---------------
#if CONFIG_BESHELL_SERIAL_USE_I2S
        #if SOC_I2S_NUM>0
        exportName("i2s0") ;
        #endif

        #if SOC_I2S_NUM>1
        exportName("i2s1") ;
        #endif
#endif

    }
    
    Serial::~Serial() {
    }

    void Serial::exports(JSContext *ctx) {
        assert(ctx) ;
        assert(m) ;

        // UART ---------------
#if CONFIG_BESHELL_SERIAL_USE_UART
        UART * uart0 = UART::flyweight(ctx, UART_NUM_0) ;
        exportValue("uart0", JS_DupValue(ctx,uart0->jsobj)) ;

        #if SOC_UART_HP_NUM>1
        UART * uart1 = UART::flyweight(ctx, UART_NUM_1) ;
        if(uart1) {
            exportValue("uart1", JS_DupValue(ctx,uart1->jsobj)) ;
        }
        #endif

        #if SOC_UART_HP_NUM>2
        UART * uart2 = UART::flyweight(ctx, UART_NUM_2) ;
        if(uart2) {
            exportValue("uart2", JS_DupValue(ctx,uart2->jsobj)) ;
        }
        #endif

        #if SOC_UART_HP_NUM>3
        UART * uart3 = UART::flyweight(ctx, UART_NUM_3) ;
        if(uart3) {
            exportValue("uart3", JS_DupValue(ctx,uart3->jsobj)) ;
        }
        #endif

        #if SOC_UART_HP_NUM>4
        UART * uart4 = UART::flyweight(ctx, UART_NUM_4) ;
        if(uart4) {
            exportValue("uart4", JS_DupValue(ctx,uart4->jsobj)) ;
        }
        #endif

        #if SOC_UART_LP_NUM>0
        UART * uartlp0 = UART::flyweight(ctx, LP_UART_NUM_0) ;
        if(uartlp0) {
            exportValue("uartlp0", JS_DupValue(ctx,uartlp0->jsobj)) ;
        }
        #endif
#endif


        // I2C ---------------        
#if CONFIG_BESHELL_SERIAL_USE_I2C
        I2C * i2c0 = I2C::flyweight(ctx, I2C_NUM_0) ;
        exportValue("i2c0", JS_DupValue(ctx,i2c0->jsobj)) ;

        #if SOC_HP_I2C_NUM > 1
        I2C * i2c1 = I2C::flyweight(ctx, I2C_NUM_1) ;
        if(i2c1) {
            exportValue("i2c1", JS_DupValue(ctx,i2c1->jsobj)) ;
        }
        #endif
        
        #if SOC_LP_I2C_NUM > 0
        I2C * i2clp0 = I2C::flyweight(ctx, LP_I2C_NUM_0) ;
        if(i2clp0) {
            exportValue("i2clp0", JS_DupValue(ctx,i2clp0->jsobj)) ;
        }
        #endif
#endif

        // SPI ---------------
#if CONFIG_BESHELL_SERIAL_USE_SPI
        #if SOC_SPI_PERIPH_NUM > 0
        SPI * spi0 = SPI::flyweight(ctx, SPI1_HOST) ;
        if(spi0) {
            exportValue("spi0", spi0->jsobj) ;
        }
        #endif
        
        #if SOC_SPI_PERIPH_NUM > 1
        SPI * spi1 = SPI::flyweight(ctx, SPI2_HOST) ;
        if(spi1) {
            exportValue("spi1", spi1->jsobj) ;
        }
        #endif

        #if SOC_SPI_PERIPH_NUM > 2
        SPI * spi2 = SPI::flyweight(ctx, SPI3_HOST) ;
        if(spi2) {
            exportValue("spi2", spi2->jsobj) ;
            }
        #endif

        #if SOC_SPI_PERIPH_NUM > 3
        SPI * spi3 = SPI::flyweight(ctx, SPI4_HOST) ;
        if(spi3) {
            exportValue("spi3", spi3->jsobj) ;
        }
        #endif
#endif

        // I2S ---------------
#if CONFIG_BESHELL_SERIAL_USE_I2S
        #if SOC_I2S_NUM > 0
        I2S * i2s0 = I2S::flyweight(ctx, I2S_NUM_0) ;
        if(i2s0) {
            exportValue("i2s0", JS_DupValue(ctx,i2s0->jsobj)) ;
        }
        #endif

        #if SOC_I2S_NUM > 1
        I2S * i2s1 = I2S::flyweight(ctx, I2S_NUM_1) ;
        if(i2s1) {
            exportValue("i2s1", JS_DupValue(ctx,i2s1->jsobj)) ;
        }
        #endif
#endif
    }
    
}
