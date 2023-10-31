#ifndef __H_MODULE_MEDIA__
#define __H_MODULE_MEDIA__

#include "quickjs-libc.h"

void be_module_media_init() ;
void be_module_media_require(JSContext *ctx) ;
void be_module_media_loop(JSContext *ctx) ;
void be_module_media_reset(JSContext *ctx) ;

#endif