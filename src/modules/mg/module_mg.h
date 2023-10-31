#ifndef __H_MODULE_MG__
#define __H_MODULE_MG__

#include "quickjs-libc.h"
#include <stdbool.h>
#include "mongoose.h"


typedef struct _be_http_server {
    struct mg_connection * conn ;
    JSContext * ctx ;
    JSValue callback ;
    bool telweb: 1 ;
    bool ssl: 1;
    void * ssl_cert_path;
    void * ssl_key_path;
} be_http_server_t ;


typedef struct _response {
    struct mg_connection * conn ;
    JSValue jsrspn ;
} response_t ;


extern JSClassID js_mg_http_message_class_id ;
extern JSClassID js_mg_http_rspn_class_id ;

// JSValue be_http_server_new(JSContext *ctx, struct mg_connection * conn, JSValue callback) ;

bool mg_url_is_listening(const char * url) ;

const char * mg_event_const_to_name(int ev) ;
int mg_event_name_to_const(const char * evname) ;

struct mg_mgr * be_module_mg_mgr() ;
void be_module_mg_init() ;
void be_module_mg_require(JSContext *ctx) ;
void be_module_mg_loop(JSContext *ctx) ;
void be_module_mg_reset(JSContext *ctx) ;

#endif