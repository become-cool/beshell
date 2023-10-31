#ifndef __H_MODULE_SERIAL_I2S__
#define __H_MODULE_SERIAL_I2S__

#include "quickjs-libc.h"
#include <stdbool.h>

#define FREE_BUS_I2S(busnum)    if(_i2s_bus_setup&(1<<(busnum))){ i2s_driver_uninstall(busnum); }
bool i2s_has_setup(uint8_t busnum) ;
void i2s_stop_play(uint8_t busnum) ;

void be_module_serial_i2s_init() ;
void be_module_serial_i2s_require(JSContext *ctx, JSValue pkg) ;
void be_module_serial_i2s_loop(JSContext *ctx) ;
void be_module_serial_i2s_reset(JSContext *ctx) ;

#endif