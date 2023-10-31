#ifndef ONCE_H_MG_SERVER
#define ONCE_H_MG_SERVER

#include "quickjs-libc.h"
#include <stdbool.h>
#include "mongoose.h"


void be_module_mg_server_init() ;
void be_module_mg_server_require(JSContext *ctx, JSValue mg, JSValue pkgShadow) ;
#endif