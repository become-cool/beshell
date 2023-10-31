#ifndef _MODULE_GPIO_H
#define _MODULE_GPIO_H

#include "quickjs-libc.h"

void be_module_gpio_require(JSContext *ctx) ;

void be_module_gpio_init() ;
void be_module_gpio_reset(JSContext *ctx) ;
void be_module_gpio_loop(JSContext *ctx) ;

#endif