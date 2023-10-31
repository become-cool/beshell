#ifndef __H_DRIVER_STEPPER_TIMER__
#define __H_DRIVER_STEPPER_TIMER__

#include "quickjs-libc.h"

void be_module_driver_stepper_timer_init() ;
void be_module_driver_stepper_timer_require(JSContext *ctx,JSValue driver) ;
void be_module_driver_stepper_timer_loop(JSContext *ctx) ;
void be_module_driver_stepper_timer_reset(JSContext *ctx) ;

#endif