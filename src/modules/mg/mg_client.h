#ifndef ONCE_H_MG_CLIENT
#define ONCE_H_MG_CLIENT

#include "quickjs-libc.h"
#include <stdbool.h>
#include "mongoose.h"


typedef struct _mg_req {
    struct mg_connection * conn ;
    JSValue jsconn ;

    JSContext * ctx ;
    JSValue callback ;
    uint16_t poll_times ;
} mg_req_t ;


void be_module_mg_client_init() ;
void be_module_mg_client_require(JSContext *ctx, JSValue mg, JSValue pkgShadow) ;

#endif