#include "UART.hpp"
#include "hal/gpio_types.h"
#include "quickjs.h"
#include <BeShell.hpp>
#include <JSEngine.hpp>
#include <driver/gpio.h>
#include <driver/uart.h>

using namespace std ;

#define RX_BUF_SIZE 1024

namespace be{

    static inline uart_port_t to_uart_port(int port) {
        return static_cast<uart_port_t>(port);
    }

    /**
     * UART 串口类
     * 
     * 用于配置和管理 ESP32 的 UART 串口。UART 是一种异步串行通信协议，
     * 常用于连接 GPS 模块、蓝牙模块、调试输出等。
     * 
     * ESP32 通常有 3 个 UART 端口（UART0、UART1、UART2），部分型号有更多。
     * - UART0 通常用于下载程序和调试输出
     * - UART1 和 UART2 可供用户自由使用
     * 
     * serial 模块会自动创建 UART 实例并通过 `uart0`, `uart1`, `uart2` 等导出。
     * 用户直接通过 serial 模块访问这些实例，无需手动创建。
     * 
     * **不同芯片型号导出的 UART 对象不同**：
     * - ESP32/ESP32-S3/C6：uart0, uart1, uart2（3 个 UART）
     * - ESP32-S2/C2/C3/H2：uart0, uart1（2 个 UART，无 uart2）
     * - ESP32-P4：uart0-uart4（5 个 HP UART），另有 uartlp0（低功耗 UART）
     * 
     * 示例：
     * ```javascript
     * import * as serial from "serial"
     * 
     * // 访问 UART2 实例（推荐）
     * const uart = serial.uart2
     * 
     * // 配置 UART
     * uart.setup({
     *     tx: 17,        // TX 引脚
     *     rx: 16,        // RX 引脚
     *     baudrate: 9600  // 波特率
     * })
     * 
     * // 发送数据
     * uart.write("Hello UART!")
     * 
     * // 接收数据
     * const data = uart.read(100)  // 读取最多 100 字节
     * console.log("Received:", data)
     * 
     * // 监听接收数据
     * uart.listen((data) => {
     *     console.log("Received:", data)
     * })
     * ```
     * 
     * @class UART
     * @module serial
     * @extends NativeClass
     */
    
    UART * UART::uart0 = nullptr ;
    #if SOC_UART_HP_NUM>1
    UART * UART::uart1 = nullptr ;
    #endif
    #if SOC_UART_HP_NUM>2
    UART * UART::uart2 = nullptr ;
    #endif
    #if SOC_UART_HP_NUM>3
    UART * UART::uart3 = nullptr ;
    #endif
    #if SOC_UART_HP_NUM>4
    UART * UART::uart4 = nullptr ;
    #endif
    #if SOC_UART_LP_NUM>0
    UART * UART::uartlp0 = nullptr ;
    #endif
    
    DEFINE_NCLASS_META(UART, NativeClass)
    std::vector<JSCFunctionListEntry> UART::methods = {
        JS_CFUNC_DEF("setup", 0, UART::setup),
        JS_CFUNC_DEF("unsetup", 0, UART::unsetup),
        JS_CFUNC_DEF("read", 0, UART::read),
        JS_CFUNC_DEF("write", 0, UART::write),
        JS_CFUNC_DEF("listen", 0, UART::listen),
    } ;

    UART::UART(JSContext * ctx, int uartNum)
        : NativeClass(ctx,build(ctx))
        , m_uartNum(uartNum)
    {}
    
    #define DEFINE_BUS(busconst, var)           \
        if(bus==busconst) {                     \
            if(!var) {                          \
                var = new UART(ctx, busconst) ; \
            }                                   \
            return var ;                        \
        }
    UART * UART::flyweight(JSContext * ctx, int bus) {
        DEFINE_BUS(UART_NUM_0, uart0)
        #if SOC_UART_HP_NUM>1
        else DEFINE_BUS(UART_NUM_1, uart1)
        #endif
        #if SOC_UART_HP_NUM>2
        else DEFINE_BUS(UART_NUM_2, uart2)
        #endif
        #if SOC_UART_HP_NUM>3
        else DEFINE_BUS(UART_NUM_3, uart3)
        #endif
        #if SOC_UART_HP_NUM>4
        else DEFINE_BUS(UART_NUM_4, uart4)
        #endif
        #if SOC_UART_LP_NUM>0
        else DEFINE_BUS(LP_UART_NUM_0, uartlp0)
        #endif
        return nullptr ;
    }

    int UART::uartNum() const {
        return m_uartNum ;
    }


    // typedef enum {
    //     UART_PARITY_DISABLE = 0x0,   // 无校验位
    //     UART_PARITY_EVEN = 0x2,      // 偶校验位
    //     UART_PARITY_ODD = 0x3        // 奇校验位
    // } uart_parity_t;
    /**
     * 配置并初始化 UART
     * 
     * 初始化 UART 串口，配置引脚、波特率等参数。
     * 
     * 示例：
     * ```javascript
     * import * as serial from "serial"
     * 
     * // 获取 UART2 实例
     * const uart = serial.uart2
     * 
     * // 基本配置
     * uart.setup({
     *     tx: 17,   // TX 引脚
     *     rx: 16    // RX 引脚
     * })
     * 
     * // 完整配置
     * uart.setup({
     *     tx: 17,
     *     rx: 16,
     *     baudrate: 9600,   // 波特率（默认 115200）
     *     stopbits: 1,      // 停止位（默认 1）
     *     parity: 0         // 校验位：0=无校验（默认），2=偶校验，3=奇校验
     * })
     * ```
     *
     * options 格式：
     * ```javascript
     * {
     *    tx:number,           // TX 引脚 GPIO 编号（必需）
     *    rx:number,           // RX 引脚 GPIO 编号（必需）
     *    baudrate:number=115200,  // 波特率（可选，默认 115200）
     *    stopbits:number=1,   // 停止位（可选，默认 1）
     *    parity:0|2|3=0,      // 校验位（可选，默认 0=无校验，2=偶校验，3=奇校验）
     * }
     * ```
     *
     * @module serial
     * @class UART
     * @function setup
     * @param options:object 配置选项对象
     *     {
     *         tx: number,            // TX 引脚 GPIO 编号
     *         rx: number,            // RX 引脚 GPIO 编号
     *         baudrate?: number,     // 波特率，默认 115200
     *         stopbits?: number,     // 停止位，默认 1
     *         parity?: 0 | 2 | 3     // 校验位：0=无校验，2=偶校验，3=奇校验，默认 0
     *     }
     * @return undefined
     * @throws UART 驱动已安装
     * @throws 配置失败
     */
    JSValue UART::setup(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        THIS_NCLASS(UART, uart)

        if(uart_is_driver_installed(to_uart_port(uart->m_uartNum))) {
            JSTHROW("uart%d driver already installed",uart->m_uartNum)
        }

        ASSERT_ARGC(1)

        gpio_num_t GET_GPIO_PROP(argv[0], "tx", tx, GPIO_NUM_NC)
        gpio_num_t GET_GPIO_PROP(argv[0], "rx", rx, GPIO_NUM_NC)
        int GET_UINT32_PROP_OPT(argv[0], "baudrate", baudrate, 115200)
        uart_stop_bits_t GET_INT_PROP_OPT(argv[0], "stopbits", stopbits, uart_stop_bits_t, UART_STOP_BITS_1)
        uart_parity_t GET_INT_PROP_OPT(argv[0], "parity", parity, uart_parity_t, UART_PARITY_DISABLE)
        BaseType_t GET_UINT32_PROP_OPT(argv[0], "core", core, 0 )

        // dn3(baudrate,stopbits,parity)

        esp_err_t ret = uart_driver_install(to_uart_port(uart->m_uartNum), RX_BUF_SIZE * 2, 0, 0, NULL, 0);
        if(ret!=0) {
            JSTHROW("uart setup failded(%s:%d)","install", ret)
        }

        // uart_port_t bus = uart->m_uartNum ;
        // esp_err_t ret ;
        // run_wait_on_core([&ret, bus](){

        //     ret = uart_driver_install(bus, RX_BUF_SIZE * 2, 0, 0, NULL, 0);

        // }, core) ;
        // if(ret!=0) {
        //     JSTHROW("uart setup failded(%s:%d)","install", ret)
        // }


        // Configure UART parameters
        uart_config_t uart_config = {
            .baud_rate = (int)baudrate,
            .data_bits = UART_DATA_8_BITS,
            .parity    = parity,
            .stop_bits = stopbits,
            .flow_ctrl = UART_HW_FLOWCTRL_DISABLE,
            .source_clk = UART_SCLK_DEFAULT,
        };
        ret = uart_param_config(to_uart_port(uart->m_uartNum), &uart_config) ;
        if(ret!=0) {
            JSTHROW("uart setup failded(%s:%d)","config", ret)
        }
        
        ret = uart_set_pin(to_uart_port(uart->m_uartNum), tx, rx, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE);
        if(ret!=0) {
            JSTHROW("uart setup failded(%s:%d)","setpin", ret)
        }

        return JS_UNDEFINED ;
    }


    /**
     * 从 UART 读取数据
     * 
     * 读取指定长度的数据，返回一个 ArrayBuffer 对象。如果没有数据可读，将返回一个空的 ArrayBuffer。
     * 
     * 通常情况下接收方无法确定数据到达的时机，再加上 JS 引擎主循环的延迟，`read()` 有时无法满足通讯需求，推荐使用 `listen()`。
     * 
     * @module serial
     * @class UART
     * @function read
     * @param len:number 要读取的字节数
     * @param timeout:number=20 读取超时时间（毫秒，默认 20）
     * @return ArrayBuffer 读取到的数据
     * @throws 内存分配失败
     */
    JSValue UART::read(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        THIS_NCLASS(UART, uart)
        ASSERT_ARGC(1)
        ARGV_TO_UINT32(0, len) ;
        ARGV_TO_UINT32_OPT(1, timeout, 20) ;

        uint8_t *buff = (uint8_t *) malloc(len);
        if(!buff) {
            JSTHROW("out of memory?")
        }
        int readlen = uart_read_bytes(to_uart_port(uart->m_uartNum), buff, len, timeout / portTICK_PERIOD_MS);
        return JS_NewArrayBuffer(ctx, buff, readlen, freeArrayBuffer, NULL, false) ;
    }

    /**
     * 向 UART 写入数据
     * 
     * > 当参数 data 是一个整数数组时，等效于 `new Uint8Array(data)`
     * 
     * @module serial
     * @class UART
     * @function write
     * @param data:ArrayBuffer|string|number[] 要写入的数据
     * @return number 实际写入的字节数
     */
    JSValue UART::write(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        THIS_NCLASS(UART, uart)
        ASSERT_ARGC(1)
        
        bool needfree = false ;
        size_t length = 0 ;
        uint8_t * buff = JS_GetArrayBuffer(ctx, &length, argv[0]) ;
        JS_GetException(ctx) ; // 如果不是 ArrayBuffer ，JS_GetArrayBuffer 会产生异常

        // ArrayBuffer
        if(buff) {
            const int txBytes = uart_write_bytes(to_uart_port(uart->m_uartNum), buff, length);
            return JS_NewInt32(ctx, txBytes) ;
        }

        // array 
        else if(JS_IsArray(ctx, argv[0])) {
            buff = JS_ArrayToBufferUint8(ctx, argv[0], (int *)&length) ;
            if(length && buff) {
                const int txBytes = uart_write_bytes(to_uart_port(uart->m_uartNum), buff, length);
                free(buff) ;
                return JS_NewInt32(ctx, txBytes) ;
            } else {
                return JS_NewInt32(ctx, 0) ;
            }
            
        }

        // string
        else {
            ARGV_TO_CSTRING_LEN(0, buff, length)
            const int txBytes = uart_write_bytes(to_uart_port(uart->m_uartNum), buff, length);
            JS_FreeCString(ctx, (const char *)buff) ;
            return JS_NewInt32(ctx, txBytes) ;
        }
    }

    typedef struct {
        uint8_t * data ;
        int len ;
    } uart_chunk_t ;

    void UART::task_listen(void * arg) {
        UART * uart = (UART *) arg ;
        uint8_t buff [32];
        uart_chunk_t chunk ;
        while(1) {
            chunk.len = uart_read_bytes(to_uart_port(uart->uartNum()), buff, sizeof(buff), 1);
            if(chunk.len>0) {
                chunk.data = (uint8_t *)malloc(chunk.len) ;
                memcpy(chunk.data, buff, chunk.len) ;
                if( xQueueSend(uart->data_queue, &chunk, 0)!=pdPASS ){
                    free(chunk.data) ;
                }
            }

            // 关键：仅在无数据时让出 CPU，避免吞吐上限过低
            if(!chunk.len) {
                vTaskDelay(1);
            }
        }
    }

    void UART::loop(JSContext * ctx, void * opaque) {
        UART * uart = (UART *) opaque ;
        uart_chunk_t chunk ;
        // 关键：旧实现每次 loop 只消费 1 个 chunk。
        // 在高波特率下 JS loop 的调用频率远小于 UART 生产速率，会导致队列迅速满并丢数据。
        // 这里改为每次 loop 尽量 drain 多个 chunk（带上限，避免一次占用太久）。
        int drained = 0;
        const int maxDrain = 64;
        while (drained < maxDrain && xQueueReceive(uart->data_queue, &chunk, 0)) {
            drained++;
            if(chunk.data) {
                JSValue ab = JS_NewArrayBuffer(ctx, chunk.data, chunk.len, freeArrayBuffer, NULL, false) ;
                JSValue ret = JS_Call(ctx, uart->listener, JS_UNDEFINED, 1, &ab);
                if( JS_IsException(ret) ) {
                    JSEngine::fromJSContext(ctx)->dumpError() ;
                }
                JS_FreeValue(ctx, ret);
                JS_FreeValue(ctx, ab) ;
            }
        }
    }

    // listen 队列：旧值 10 在大吞吐下太容易满导致丢数据
    #define DATA_QUEUE_LEN 128
    /**
     * 监听 UART 接收到的数据
     * 
     * 回调函数的原型：
     *     callback(data: ArrayBuffer)
     *
     * @module serial
     * @class UART
     * @function listen
     * @param callback:function 数据接收回调函数，参数为 ArrayBuffer
     * @return undefined
     * @throws UART 驱动未安装
     * @throws 回调不是函数
     */
    JSValue UART::listen(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        THIS_NCLASS(UART, uart)
        
        if(!uart_is_driver_installed(to_uart_port(uart->m_uartNum))) {
            JSTHROW("uart%d driver not installed, call setup() first",uart->m_uartNum)
        }

        ASSERT_ARGC(1)
        if( !JS_IsFunction(ctx, argv[0]) ){
            JSTHROW("arg callback must be a function")
        }

        if(JS_IsNull(uart->listener)) {
            JS_FreeValue(ctx, uart->listener) ;
        }
        uart->listener = JS_DupValue(ctx, argv[0]) ;

        if(uart->taskListenerHandle == nullptr) {
            xTaskCreatePinnedToCore(task_listen, "task-listen", 1024, uart, 5, &uart->taskListenerHandle, 1);
        }
        
        if(uart->data_queue==nullptr){
            uart->data_queue = xQueueCreate(DATA_QUEUE_LEN, sizeof(uart_chunk_t));
        }

        JSEngine::fromJSContext(ctx)->addLoopFunction(UART::loop, (void *)uart, true, 0) ;

        return JS_UNDEFINED ;
    }

    /**
     * 关闭 UART 并释放资源
     * 
     * @module serial
     * @class UART
     * @function unsetup
     * @return undefined
     */
    JSValue UART::unsetup(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        THIS_NCLASS(UART, uart)

        JSEngine::fromJSContext(ctx)->removeLooping(UART::loop, (void *)uart) ;

        if(uart->taskListenerHandle){
            vTaskDelete(uart->taskListenerHandle) ;
            uart->taskListenerHandle = nullptr ;
        }

        if(uart->data_queue){
            uart_chunk_t chunk ;
            while(xQueueReceive(uart->data_queue, &chunk, 0)==pdTRUE){
                if(chunk.data){
                    free(chunk.data) ;
                }
            }
            vQueueDelete(uart->data_queue) ;
            uart->data_queue = nullptr ;
        }

        if(!JS_IsNull(uart->listener)){
            JS_FreeValue(ctx, uart->listener) ;
            uart->listener = JS_NULL ;
        }

        uart_driver_delete(to_uart_port(uart->m_uartNum)) ;
        return JS_UNDEFINED ;
    }
    /**
     * 检查 UART 驱动是否已安装
     * 
     * 是 ESP IDF `uart_is_driver_installed` 的封装，无论 c++ 还是 js 启用了 uart ，该函数都会返回 true。
     *
     * @module serial
     * @class UART
     * @function isInstalled
     * @return bool 是否已安装
     */
    JSValue UART::isInstalled(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        THIS_NCLASS(UART, uart)
        return uart_is_driver_installed(to_uart_port(uart->m_uartNum))? JS_TRUE : JS_FALSE ;
    }
}