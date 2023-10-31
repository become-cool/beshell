#ifndef __H_DRIVER_RMT__
#define __H_DRIVER_RMT__

#include "quickjs-libc.h"

void be_module_driver_rmt_init() ;
void be_module_driver_rmt_require(JSContext *ctx,JSValue driver) ;
void be_module_driver_rmt_loop(JSContext *ctx) ;
void be_module_driver_rmt_reset(JSContext *ctx) ;

#endif