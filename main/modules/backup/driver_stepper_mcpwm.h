#ifndef __H_MODULE_DRIVER_STEPPER__
#define __H_MODULE_DRIVER_STEPPER__

#include "quickjs-libc.h"

void be_module_driver_stepper_mcpwm_init() ;
void be_module_driver_stepper_mcpwm_require(JSContext *ctx, JSValue driver) ;
void be_module_driver_stepper_mcpwm_loop(JSContext *ctx) ;
void be_module_driver_stepper_mcpwm_reset(JSContext *ctx) ;

#endif