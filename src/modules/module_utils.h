#ifndef _MODULE_UTILS_H
#define _MODULE_UTILS_H

#include "quickjs-libc.h"
#include <stdbool.h>

void hsv2rgb(uint32_t h, uint32_t s, uint32_t v, uint32_t *r, uint32_t *g, uint32_t *b) ;
uint8_t gamma8_correct(uint8_t val) ;

int readPartId() ;
int readPartVersion() ;

void be_module_utils_require(JSContext *ctx) ;

#endif