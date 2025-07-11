#include "UART.hpp"
#include <BeShell.hpp>
#include <JSEngine.hpp>
#include <driver/gpio.h>

using namespace std ;

#define RX_BUF_SIZE 1024

namespace be{
    
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
        JS_CFUNC_DEF("read", 0, UART::read),
        JS_CFUNC_DEF("write", 0, UART::write),
        JS_CFUNC_DEF("listen", 0, UART::listen),
    } ;

    UART::UART(JSContext * ctx, uart_port_t uartNum)
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
    UART * UART::flyweight(JSContext * ctx, uart_port_t bus) {
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

    uart_port_t UART::uartNum() const {
        return m_uartNum ;
    }


    // typedef enum {
    //     UART_PARITY_DISABLE = 0x0,   // 无校验位
    //     UART_PARITY_EVEN = 0x2,      // 偶校验位
    //     UART_PARITY_ODD = 0x3        // 奇校验位
    // } uart_parity_t;
    /**
     * 
     * options 格式：
     * ```javascript
     * {
     *    tx:number,
     *    rx:number,
     *    baudrate:number=115200,
     *    stopbits:number=1,
     *    parity:0|2|3=0,
     * }
     * ```
     * parity: 0=none, 2=even, 3=odd
     *
     * @param options:object uart options
     * @return undefined
     */
    JSValue UART::setup(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        THIS_NCLASS(UART, uart)

        if(uart_is_driver_installed(uart->m_uartNum)) {
            JSTHROW("uart%d driver already installed",uart->m_uartNum)
        }

        ASSERT_ARGC(1)

        gpio_num_t GET_INT32_PROP(argv[0], "tx", tx, UART_PIN_NO_CHANGE)
        gpio_num_t GET_INT32_PROP(argv[0], "rx", rx, UART_PIN_NO_CHANGE)
        int GET_UINT32_PROP_OPT(argv[0], "baudrate", baudrate, 115200)
        uart_stop_bits_t GET_UINT32_PROP_OPT(argv[0], "stopbits", stopbits, UART_STOP_BITS_1)
        uart_parity_t GET_UINT32_PROP_OPT(argv[0], "parity", parity, UART_PARITY_DISABLE)

        // dn3(baudrate,stopbits,parity)

        esp_err_t ret = uart_driver_install(uart->m_uartNum, RX_BUF_SIZE * 2, 0, 0, NULL, NULL);
        if(ret!=0) {
            JSTHROW("uart setup failded(%s:%d)","install", ret)
        }


        // Configure UART parameters
        uart_config_t uart_config = {
            .baud_rate = (int)baudrate,
            .data_bits = UART_DATA_8_BITS,
            .parity    = parity,
            .stop_bits = stopbits,
            .flow_ctrl = UART_HW_FLOWCTRL_DISABLE,
            .source_clk = UART_SCLK_DEFAULT,
        };
        ret = uart_param_config(uart->m_uartNum, &uart_config) ;
        if(ret!=0) {
            JSTHROW("uart setup failded(%s:%d)","config", ret)
        }
        
        ret = uart_set_pin(uart->m_uartNum, tx, rx, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE);
        if(ret!=0) {
            JSTHROW("uart setup failded(%s:%d)","setpin", ret)
        }

        return JS_UNDEFINED ;
    }

    /**
     * length
     * timeout=20ms
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
        int readlen = uart_read_bytes(uart->m_uartNum, buff, len, timeout / portTICK_PERIOD_MS);
        return JS_NewArrayBuffer(ctx, buff, readlen, freeArrayBuffer, NULL, false) ;
    }

    JSValue UART::write(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        THIS_NCLASS(UART, uart)
        ASSERT_ARGC(1)
        
        bool needfree = false ;
        size_t length = 0 ;
        uint8_t * buff = JS_GetArrayBuffer(ctx, &length, argv[0]) ;

        // ArrayBuffer
        if(buff) {
            const int txBytes = uart_write_bytes(uart->m_uartNum, buff, length);
            return JS_NewInt32(ctx, txBytes) ;
        }

        // array 
        else if(JS_IsArray(ctx, argv[0])) {
            buff = JS_ArrayToBufferUint8(ctx, argv[0], (int *)&length) ;
            if(length && buff) {
                const int txBytes = uart_write_bytes(uart->m_uartNum, buff, length);
                free(buff) ;
                return JS_NewInt32(ctx, txBytes) ;
            } else {
                return JS_NewInt32(ctx, 0) ;
            }
            
        }

        // string
        else {
            ARGV_TO_CSTRING_LEN(0, buff, length)
            const int txBytes = uart_write_bytes(uart->m_uartNum, buff, length);
            JS_FreeCString(ctx, (const char *)buff) ;
            return JS_NewInt32(ctx, txBytes) ;
        }
    }

    typedef struct {
        uint8_t * data ;
        int len ;
    } uart_chunk_t ;

    void UART::task_listen(void * arg) {
        uart_event_t event;
        UART * uart = (UART *) arg ;
        uint8_t buff [32];
        uart_chunk_t chunk ;
        while(1) {
            chunk.len = uart_read_bytes(uart->uartNum(), buff, sizeof(buff), 1);
            if(chunk.len) {
                chunk.data = (uint8_t *)malloc(chunk.len) ;
                memcpy(chunk.data, buff, chunk.len) ;
                if( xQueueSend(uart->data_queue, &chunk, 0)!=pdPASS ){
                    free(chunk.data) ;
                }
            }

            vTaskDelay(1);
        }
    }

    void UART::loop(JSContext * ctx, void * opaque) {
        UART * uart = (UART *) opaque ;
        uart_chunk_t chunk ;
        if(xQueueReceive(uart->data_queue, &chunk, 0)) {
            if(chunk.data) {
                JSValue ab = JS_NewArrayBuffer(ctx, chunk.data, chunk.len, freeArrayBuffer, NULL, false) ;
                JSValue ret = JS_Call(ctx, uart->listener, JS_UNDEFINED, 1, &ab);
                if( JS_IsException(ret) ) {
                    JSEngine::fromJSContext(ctx)->dumpError() ;
                }
                JS_FreeValue(ctx, ab) ;
            }
        }
    }

    #define DATA_QUEUE_LEN 10
    JSValue UART::listen(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        THIS_NCLASS(UART, uart)
        
        if(!uart_is_driver_installed(uart->m_uartNum)) {
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

    JSValue UART::unsetup(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        THIS_NCLASS(UART, uart)
        uart_driver_delete(uart->m_uartNum) ;
        return JS_UNDEFINED ;
    }
    JSValue UART::isInstalled(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        THIS_NCLASS(UART, uart)
        return uart_is_driver_installed(uart->m_uartNum)? JS_TRUE : JS_FALSE ;
    }
}