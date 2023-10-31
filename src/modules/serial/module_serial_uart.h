#ifndef __H_MODULE_SERIAL_UART__
#define __H_MODULE_SERIAL_UART__

#include "quickjs-libc.h"

void be_module_serial_uart_init() ;
void be_module_serial_uart_require(JSContext *ctx, JSValue pkg) ;
void be_module_serial_uart_loop(JSContext *ctx) ;
void be_module_serial_uart_reset(JSContext *ctx) ;

#endif