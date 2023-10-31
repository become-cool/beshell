#ifndef __H_MODULE_PROCESS__
#define __H_MODULE_PROCESS__

#include "quickjs-libc.h"

void be_module_process_init() ;
void be_module_process_require(JSContext *ctx) ;

#endif