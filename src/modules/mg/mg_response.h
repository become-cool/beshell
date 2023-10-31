#ifndef ONCE_H_MG_RESPONSE
#define ONCE_H_MG_RESPONSE

#include "quickjs-libc.h"
#include <stdbool.h>
#include "mongoose.h"


void be_module_mg_response_init() ;
void be_module_mg_response_require(JSContext *ctx, JSValue mg, JSValue pkgShadow) ;

#endif