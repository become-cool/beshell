#ifndef __H_MODULE_DRIVERS__
#define __H_MODULE_DRIVERS__

#include "quickjs-libc.h"

void be_module_driver_init() ;
void be_module_driver_require(JSContext *ctx) ;
void be_module_driver_loop(JSContext *ctx) ;
void be_module_driver_reset(JSContext *ctx) ;

#endif