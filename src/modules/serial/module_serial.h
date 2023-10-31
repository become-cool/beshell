#ifndef _MODULE_SERIAL_H
#define _MODULE_SERIAL_H

#include "quickjs-libc.h"

#include "module_serial_uart.h"
#include "module_serial_spi.h"
#include "module_serial_i2c.h"
#include "module_serial_i2s.h"


void be_module_serial_require(JSContext *ctx) ;

void be_module_serial_reset(JSContext *ctx) ;

#endif