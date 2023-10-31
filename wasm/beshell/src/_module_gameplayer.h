#ifndef __H_MODULE_GAMEPLAYER__
#define __H_MODULE_GAMEPLAYER__

#include "quickjs-libc.h"

void be_module_gameplayer_init() ;
void be_module_gameplayer_require(JSContext *ctx) ;
void be_module_gameplayer_loop(JSContext *ctx) ;
void be_module_gameplayer_reset(JSContext *ctx) ;

#endif