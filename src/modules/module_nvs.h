#ifndef __H_MODULE_NVS__
#define __H_MODULE_NVS__

#include "quickjs-libc.h"

void nvs_read_onetime(const char * key, uint8_t*pvalue) ;

void be_module_nvs_init() ;
void be_module_nvs_require(JSContext *ctx) ;
void be_module_nvs_loop(JSContext *ctx) ;
void be_module_nvs_reset(JSContext *ctx) ;

#endif