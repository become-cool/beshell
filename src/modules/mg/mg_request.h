#ifndef ONCE_H_MG_REQUEST
#define ONCE_H_MG_REQUEST

#include "quickjs-libc.h"
#include <stdbool.h>
#include "mongoose.h"


void be_module_mg_request_init() ;
void be_module_mg_request_require(JSContext *ctx, JSValue mg, JSValue pkgShadow) ;

#endif