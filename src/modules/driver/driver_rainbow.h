#ifndef __H_DRIVER_RAINBOW__
#define __H_DRIVER_RAINBOW__

#include "quickjs-libc.h"

void be_module_driver_rainbow_init() ;
void be_module_driver_rainbow_require(JSContext *ctx,JSValue driver) ;
void be_module_driver_rainbow_loop(JSContext *ctx) ;
void be_module_driver_rainbow_reset(JSContext *ctx) ;

#endif