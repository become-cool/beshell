#ifndef __H_DRIVER_COMMON__
#define __H_DRIVER_COMMON__

#include "quickjs-libc.h"

void be_module_driver_common_init() ;
void be_module_driver_common_require(JSContext *ctx, JSValue pkg) ;
void be_module_driver_common_loop(JSContext *ctx) ;
void be_module_driver_common_reset(JSContext *ctx) ;

#endif