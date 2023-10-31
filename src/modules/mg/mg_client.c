#include "mg_client.h"
#include "module_mg.h"
#include "module_fs.h"
#include "utils.h"
#include "cutils.h"


char * ca_path = NULL ;



// =================================================
// class Client


#define THIS_CLIENT(var)                                                \
    mg_req_t * var = JS_GetOpaque(this_val, js_mg_client_class_id) ;    \
    if(!var || !var->conn) {                                                \
        JS_ThrowReferenceError(ctx, "mg.Client object has closed.");    \
        return JS_EXCEPTION ;                                               \
    }



static JSClassID js_mg_client_class_id ;
static JSValue js_mg_client_constructor(JSContext *ctx, JSValueConst new_target, int argc, JSValueConst *argv){
    JSValue jsobj = JS_NewObjectClass(ctx, js_mg_client_class_id) ;
    return jsobj ;
}
static void js_mg_client_finalizer(JSRuntime *rt, JSValue this_val){
    // printf("js_mg_client_finalizer()\n") ;
}
static JSClassDef js_mg_client_class = {
    "mg.Client",
    .finalizer = js_mg_client_finalizer,
} ;

static JSValue js_mg_client_send(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    CHECK_ARGC(1)
    THIS_CLIENT(req)

    ARGV_TO_STRING_LEN_E(0, data, len, "arg data must be a string")
    bool res = mg_send(req->conn, data, len) ;
    JS_FreeCString(ctx, data) ;

    return res? JS_TRUE : JS_FALSE;
}

static JSValue js_mg_client_close(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    THIS_CLIENT(req)
    req->conn->is_closing = true ;
    return JS_UNDEFINED ;
}


static JSValue js_mg_client_init_tls(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    CHECK_ARGC(1)
    ARGV_TO_STRING_E(0, host, "arg host must be a string")

    THIS_CLIENT(req)

    struct mg_tls_opts opts = {.ca = ca_path, .srvname = mg_str(host) };
    mg_tls_init(req->conn, &opts);
    
    JS_FreeCString(ctx, host) ;
    return JS_UNDEFINED ;
}


static const JSCFunctionListEntry js_mg_client_proto_funcs[] = {
    JS_CFUNC_DEF("close", 0, js_mg_client_close),
    JS_CFUNC_DEF("send", 0, js_mg_client_send),
    JS_CFUNC_DEF("initTLS", 0, js_mg_client_init_tls),
} ;


// enum {
//   MG_EV_ERROR,       // Error                         char *error_message
//   MG_EV_OPEN,        // Client created           NULL
//   MG_EV_POLL,        // mg_mgr_poll iteration        unsigned long *millis
//   MG_EV_RESOLVE,     // Host name is resolved        NULL
//   MG_EV_CONNECT,     // Client established       NULL
//   MG_EV_ACCEPT,      // Client accepted          NULL
//   MG_EV_READ,        // Data received from socket    struct mg_str *
//   MG_EV_WRITE,       // Data written to socket       long *bytes_written
//   MG_EV_CLOSE,       // Client closed            NULL
//   MG_EV_HTTP_MSG,    // HTTP request/response        struct mg_http_message *
//   MG_EV_HTTP_CHUNK,  // HTTP chunk (partial msg)     struct mg_http_message *
//   MG_EV_WS_OPEN,     // Websocket handshake done     struct mg_http_message *
//   MG_EV_WS_MSG,      // Websocket msg, text or bin   struct mg_ws_message *
//   MG_EV_WS_CTL,      // Websocket control msg        struct mg_ws_message *
//   MG_EV_MQTT_CMD,    // MQTT low-level command       struct mg_mqtt_message *
//   MG_EV_MQTT_MSG,    // MQTT PUBLISH received        struct mg_mqtt_message *
//   MG_EV_MQTT_OPEN,   // MQTT CONNACK received        int *connack_status_code
//   MG_EV_SNTP_TIME,   // SNTP time received           struct timeval *
//   MG_EV_USER,        // Starting ID for user events
// };
static void http_connection_event_handler(struct mg_connection * conn, int ev, void *ev_data, void *fnd) {
    mg_req_t * req = (mg_req_t *)fnd ;
    switch(ev) {
        case MG_EV_POLL:
            req->poll_times++ ;
            if(++req->poll_times > 5000 ){
                JS_CALL_ARG1(req->ctx, req->callback, JS_NewString(req->ctx, "timeout"))
                conn->is_closing = 1 ;
            }
            break ;

        case MG_EV_HTTP_MSG: 
        case MG_EV_HTTP_CHUNK: 
        {
            // moogose https 协议，会在 close 事件以后触发 msg 事件
            if(!req || !req->ctx) {
                break ;
            }

            struct mg_http_message *hm = (struct mg_http_message *) ev_data;

            JSValue jsmsg = JS_NewObjectClass(req->ctx, js_mg_http_message_class_id) ;
            JS_SetOpaque(jsmsg, hm) ;

            JSValue evname = JS_NewString(req->ctx, mg_event_const_to_name(ev)) ;

            MAKE_ARGV2(argv, evname, jsmsg)
            JS_Call(req->ctx, req->callback, JS_UNDEFINED, 2, argv) ;
            free(argv) ;

            JS_SetOpaque(jsmsg, NULL) ;
            JS_FreeValue(req->ctx, jsmsg) ;

            JS_FreeValue(req->ctx, evname) ;

            break ;
        }

        case MG_EV_CLOSE :

            JS_SetOpaque(req->jsconn, NULL) ;
            JS_FreeValue(req->ctx, req->callback) ;
            JS_FreeValue(req->ctx, req->jsconn) ;
            
            req->ctx = NULL ;
            req->callback = JS_UNDEFINED ;

            // moogose https 协议，会在 close 事件以后触发 msg 事件
            if(conn->fn_data==req) {
                conn->fn_data = NULL ;
            }

            free(req) ;
            req = NULL ;
            fnd = NULL ;

            break ;

        default:
            JS_CALL_ARG1(req->ctx, req->callback, JS_NewString(req->ctx, mg_event_const_to_name(ev)))
            break ;
    }

}

static JSValue js_mg_connect(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {

    CHECK_ARGC(2)
    if( !JS_IsFunction(ctx, argv[1]) ) {
        THROW_EXCEPTION("arg callback must be a function")
    }
    ARGV_TO_STRING_E(0, url, "arg url must be a string") ;

    mg_req_t * req = malloc(sizeof(mg_req_t)) ;
    if(!req) {
        JS_FreeCString(ctx, url) ;
        THROW_EXCEPTION("out of memory?") ;
    }
    
    req->ctx  = ctx ;
    req->callback = JS_DupValue(ctx,argv[1]) ;
    req->jsconn = JS_NewObjectClass(ctx, js_mg_client_class_id) ;
    JS_SetOpaque(req->jsconn, req) ;
    
    struct mg_connection * conn = mg_http_connect(be_module_mg_mgr(), url, http_connection_event_handler, req) ;
    
    JS_FreeCString(ctx, url) ;

    if(conn==NULL) {
        free(req) ;
        THROW_EXCEPTION_FREE({ JS_FreeCString(ctx, url) ;}, "could not listen addr: %s", url)
    }
    
    req->conn = conn ;
    req->poll_times = 0 ;
    return JS_DupValue(ctx, req->jsconn) ;
}


void be_module_mg_client_init() {
    JS_NewClassID(&js_mg_client_class_id);
    ca_path = vfspath_to_fs("/var/ca.pem") ;
}


void be_module_mg_client_require(JSContext *ctx, JSValue mg, JSValue pkgShadow) {

    QJS_DEF_CLASS(mg_client, "Client", "mg.Client", JS_UNDEFINED, pkgShadow)

    JS_SetPropertyStr(ctx, mg, "connect", JS_NewCFunction(ctx, js_mg_connect, "connect", 1));
}

