#include "module_serial_uart.h"
#include "utils.h"
#include <string.h>
#include "driver/uart.h"

static uint8_t _uart_setup = 0 ;

#define RX_BUF_SIZE 1024

/**
 * @param uartnum 
 * @param tx pin 
 * @param rx pin
 * @param baudrate=115200
 */
static JSValue js_uart_setup(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    CHECK_ARGC(3)
    ARGV_TO_UINT8(0, uartnum)
    ARGV_TO_UINT8(1, tx)
    ARGV_TO_UINT8(2, rx)

    ARGV_TO_UINT32_OPT(3, baudrate, 115200)
    
    esp_err_t ret = uart_driver_install(uartnum, RX_BUF_SIZE * 2, 0, 0, NULL, 0);
    if(ret!=0) {
        THROW_EXCEPTION("uart setup failded(%s:%d)","install", ret)
    }
    _uart_setup|= 1<<uartnum ;

    // Configure UART parameters
    uart_config_t uart_config = {
        .baud_rate = baudrate,
        .data_bits = UART_DATA_8_BITS,
        .parity = UART_PARITY_DISABLE,
        .stop_bits = UART_STOP_BITS_1,
        .flow_ctrl = UART_HW_FLOWCTRL_CTS_RTS,
        .rx_flow_ctrl_thresh = 122,
    };
    ret = uart_param_config(uartnum, &uart_config) ;
    if(ret!=0) {
        THROW_EXCEPTION("uart setup failded(%s:%d)","config", ret)
    }
    
    ret = uart_set_pin(uartnum, tx, rx, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE);
    if(ret!=0) {
        THROW_EXCEPTION("uart setup failded(%s:%d)","setpin", ret)
    }

    return JS_UNDEFINED ;
}

static JSValue js_uart_write(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    CHECK_ARGC(2)

    ARGV_TO_UINT8(0, uartnum)

    // todo ArrayBuffer
    ARGV_TO_STRING_LEN(1, data, len)

    const int txBytes = uart_write_bytes(uartnum, data, len);

    return JS_NewInt32(ctx, txBytes) ;
}


static JSValue js_uart_read(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    THROW_EXCEPTION("not implemented")
    return JS_UNDEFINED ;
}

static JSValue js_uart_unsetup(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    THROW_EXCEPTION("not implemented")
    return JS_UNDEFINED ;
}

void be_module_serial_uart_init() {
}

void be_module_serial_uart_require(JSContext *ctx, JSValue pkg) {

    JSValue uart = JS_NewObject(ctx);
    JS_SetPropertyStr(ctx, pkg, "uart", uart);
    JS_SetPropertyStr(ctx, uart, "setup", JS_NewCFunction(ctx, js_uart_setup, "setup", 1));
    JS_SetPropertyStr(ctx, uart, "read", JS_NewCFunction(ctx, js_uart_read, "read", 1));
    JS_SetPropertyStr(ctx, uart, "write", JS_NewCFunction(ctx, js_uart_write, "write", 1));
    JS_SetPropertyStr(ctx, uart, "unsetup", JS_NewCFunction(ctx, js_uart_unsetup, "unsetup", 1));

}

void be_module_serial_uart_loop(JSContext *ctx) {
}


#define FREE_UART(num)    if(_uart_setup&(1<<(num))){ uart_driver_delete(num); }

void be_module_serial_uart_reset(JSContext *ctx) {
    FREE_UART(0)
    FREE_UART(1)
    FREE_UART(2)
    _uart_setup = 0 ;
}

