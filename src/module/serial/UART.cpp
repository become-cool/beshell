#include "UART.hpp"

using namespace std ;

#define RX_BUF_SIZE 1024

namespace be{
    
    UART * UART::uart0 = nullptr ;
    UART * UART::uart1 = nullptr ;
    UART * UART::uart2 = nullptr ;
    
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
        else DEFINE_BUS(UART_NUM_1, uart1)
        else DEFINE_BUS(UART_NUM_2, uart2)
        return nullptr ;
    }

    uart_port_t UART::uartNum() const {
        return m_uartNum ;
    }

    /**
     * @param tx pin 
     * @param rx pin
     * @param baudrate=115200
     */
    JSValue UART::setup(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        THIS_NCLASS(UART, uart)
        CHECK_ARGC(2)
        ARGV_TO_UINT8(0, tx)
        ARGV_TO_UINT8(1, rx)
        ARGV_TO_UINT32_OPT(2, baudrate, 115200)
        
        esp_err_t ret = uart_driver_install(uart->m_uartNum, RX_BUF_SIZE * 2, 0, 0, NULL, 0);
        if(ret!=0) {
            JSTHROW("uart setup failded(%s:%d)","install", ret)
        }

        // Configure UART parameters
        uart_config_t uart_config = {
            .baud_rate = (int)baudrate,
            .data_bits = UART_DATA_8_BITS,
            .parity = UART_PARITY_DISABLE,
            .stop_bits = UART_STOP_BITS_1,
            .flow_ctrl = UART_HW_FLOWCTRL_CTS_RTS,
            .rx_flow_ctrl_thresh = 122,
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
        CHECK_ARGC(1)
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
        CHECK_ARGC(1)

        size_t length = 0 ;
        uint8_t * buff = JS_GetArrayBuffer(ctx, &length, argv[0]) ;

        // if string
        if(!buff || !length) {
            ARGV_TO_CSTRING_LEN(0, buff, length)
            const int txBytes = uart_write_bytes(uart->m_uartNum, buff, length);
            JS_FreeCString(ctx, (const char *)buff) ;
            return JS_NewInt32(ctx, txBytes) ;
        }
        // if ArrayBuffer
        else {
            const int txBytes = uart_write_bytes(uart->m_uartNum, buff, length);
            return JS_NewInt32(ctx, txBytes) ;
        }

    }

    JSValue UART::listen(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        return JS_UNDEFINED ;
    }

    JSValue UART::unsetup(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        JSTHROW("not implemented")
        return JS_UNDEFINED ;
    }
}