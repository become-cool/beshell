#include "mg_server.h"
#include "module_mg.h"
#include "module_fs.h"
#include "telnet_ws.h"
#include "utils.h"
#include "cutils.h"

#ifdef PLATFORM_ESP32
#include "module_wifi.h"
#else
#define CHECK_WIFI_INITED
#endif

// =================================================
// class Server


static JSClassID js_mg_server_class_id ;


#define THIS_SERVER(var)                                                    \
    be_http_server_t * var = JS_GetOpaque(this_val, js_mg_server_class_id) ;\
    if(!var || !var->conn) {                                                \
        JS_ThrowReferenceError(var->ctx, "mg.Server object has closed.");     \
        return JS_EXCEPTION ;                                               \
    }

static JSValue js_mg_server_constructor(JSContext *ctx, JSValueConst new_target, int argc, JSValueConst *argv){
    JSValue jsobj = JS_NewObjectClass(ctx, js_mg_server_class_id) ;
    return jsobj ;
}
static void js_mg_server_finalizer(JSRuntime *rt, JSValue this_val){
    // printf("js_mg_server_finalizer()") ;
    
    be_http_server_t * server = JS_GetOpaque(this_val, js_mg_server_class_id) ;
    if(server ) {
        free(server) ;
    }

    // @todo
    // 未断开的客户端连接 (chrome等现代浏览器会保持长连接)
}
static JSClassDef js_mg_server_class = {
    "mg.Server",
    .finalizer = js_mg_server_finalizer,
} ;

static JSValue js_mg_server_close(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    THIS_SERVER(server)
    server->conn->is_closing = true ;
    JS_FreeValue(ctx,this_val) ;
    return JS_UNDEFINED ;
}

static JSValue js_mg_server_start_telweb(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    THIS_SERVER(server)
    server->telweb = true ;
    return JS_UNDEFINED ;
}
static JSValue js_mg_server_stop_telweb(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    THIS_SERVER(server)
    server->telweb = false ;
    return JS_UNDEFINED ;
}

static JSValue js_mg_server_set_handler(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    CHECK_ARGC(1)
    if( !JS_IsFunction(ctx, argv[0]) ){
        THROW_EXCEPTION("arg callback must be a function")
    }
    THIS_SERVER(server)
    server->callback = JS_DupValue(ctx,argv[0]) ;
    return JS_UNDEFINED ;
}

static const JSCFunctionListEntry js_mg_server_proto_funcs[] = {
    JS_CFUNC_DEF("close", 0, js_mg_server_close),
    JS_CFUNC_DEF("startTelweb", 0, js_mg_server_start_telweb),
    JS_CFUNC_DEF("stopTelweb", 0, js_mg_server_stop_telweb),
    JS_CFUNC_DEF("setHandler", 0, js_mg_server_set_handler),
} ;

static response_t * response_new(JSContext *ctx, struct mg_connection * conn) {
    response_t * rspn = malloc(sizeof(response_t)) ;
    rspn->conn = conn ;
    rspn->jsrspn = JS_NewObjectClass(ctx, js_mg_http_rspn_class_id) ;
    JS_SetOpaque(rspn->jsrspn, rspn) ;
    return rspn ;
}

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

static void http_server_event_handler(struct mg_connection * conn, int ev, void *ev_data, void *fn_data) {

    if(ev== MG_EV_POLL || !fn_data) {
        return ;
    }
        
    #define SERVER ((be_http_server_t *)fn_data)
    // be_http_server_t * server = (be_http_server_t *)fn_data ;
    // printf("\nevent:%s, conn:%p, server:%p, server->conn:%p, \n", mg_event_const_to_name(ev), conn, server, server->conn) ;
    // printf("%s %s \n", ((server->conn==NULL || server->conn==conn)?"server":"client"), mg_event_const_to_name(ev)) ;

    if(ev==MG_EV_ACCEPT && SERVER->ssl) {
        struct mg_tls_opts opts = {
            .cert = vfspath_to_fs("/var/cert.pem"),    // Certificate file
            .certkey = vfspath_to_fs("/var/key.pem"),  // Private key file
        };
        mg_tls_init(conn, &opts);
        printf("mg_tls_init()\n") ;
        return ;
    }


    if(SERVER->telweb) {
        if( telnet_ws_response(conn, ev, ev_data, fn_data) ) {
            return ;
        }
        // c telweb 函数没有处理的请求, 由 js 函数接着处理
        // 这种情况下 open 事件是在 c 函数内处理的，
        // 需要在此创建 response_t 
        if(!conn->userdata){
            conn->userdata = response_new(SERVER->ctx, conn) ;
        }
    }
    if(!JS_IsFunction(SERVER->ctx, SERVER->callback)) {
        printf("callback is not a function, event:%s\n", mg_event_const_to_name(ev)) ;
        return ;
    }

    // server connection
    if(SERVER->conn==NULL || SERVER->conn==conn) {
        switch(ev) {
            case MG_EV_CLOSE: 
                JS_FreeValue(SERVER->ctx, SERVER->callback) ;
                SERVER->callback = JS_NULL ;
                return ;
        }
    }

    // client connections
    else {
        switch(ev) {
            case MG_EV_OPEN: {
                conn->userdata = response_new(SERVER->ctx, conn) ; 
                return ;
            }
            case MG_EV_CLOSE:
                if(conn->userdata) {
                    response_t * rspn = (response_t *)conn->userdata ;

                    JSValueConst * MALLOC_ARGV3(cbargv, JS_NewString(SERVER->ctx, "close"), JS_NULL, rspn->jsrspn)
                    JSValue ret = JS_Call(SERVER->ctx, SERVER->callback, JS_UNDEFINED, 3, cbargv) ;
                    free(cbargv) ;
                    if(JS_IsException(ret)) {
                        js_std_dump_error(SERVER->ctx) ;
                    }

                    JS_SetOpaque(rspn->jsrspn, NULL) ;
                    JS_FreeValue(SERVER->ctx, rspn->jsrspn) ;
                    free(rspn) ;
                    conn->userdata = NULL ;
                }
                return ;

            case MG_EV_HTTP_MSG:
            case MG_EV_HTTP_CHUNK: {
                if(!conn->userdata) {
                    printf("conn->userdata == NULL ??") ;
                    return ;
                }
                response_t * rspn = (response_t *)conn->userdata ;

                JSValue jsreq = JS_NewObjectClass(SERVER->ctx, js_mg_http_message_class_id) ;
                JS_SetOpaque(jsreq, ev_data) ;

                MAKE_ARGV3(cbargv, JS_NewString(SERVER->ctx, mg_event_const_to_name(ev)), jsreq, rspn->jsrspn)
                JSValue ret = JS_Call(SERVER->ctx, SERVER->callback, JS_UNDEFINED, 3, cbargv) ;

                JS_SetOpaque(jsreq, NULL) ;  // mg 事件函数结束后 mg_http_message 对象销毁
                JS_FreeValue(SERVER->ctx, jsreq) ;
                free(cbargv) ;

                if(JS_IsException(ret)) {
                    js_std_dump_error(SERVER->ctx) ;
                }

                return ;
            }
            case MG_EV_WS_OPEN: {
                return ;
            }
            case MG_EV_WS_CTL: 
            case MG_EV_WS_MSG: {

                if(!conn->userdata) {
                    printf("conn->userdata == NULL ??") ;
                    return ;
                }
                response_t * rspn = (response_t *)conn->userdata ;

                struct mg_ws_message * msg = (struct mg_ws_message *)ev_data ;

                JSValue jsmsg = JS_NewObject(SERVER->ctx) ;
                uint8_t op = msg->flags&15 ;
                JS_SetPropertyStr(SERVER->ctx, jsmsg, "type", JS_NewUint32(SERVER->ctx, op)) ;

                if(op == WEBSOCKET_OP_TEXT) {
                    JS_SetPropertyStr(SERVER->ctx, jsmsg, "data", JS_NewStringLen(SERVER->ctx, msg->data.ptr, msg->data.len)) ;
                }
                else if(op == WEBSOCKET_OP_BINARY) {
                    JS_SetPropertyStr(SERVER->ctx, jsmsg, "data", JS_NewArrayBuffer(SERVER->ctx,msg->data.ptr, msg->data.len,NULL,NULL,false)) ;
                }

                MAKE_ARGV3(cbargv, JS_NewString(SERVER->ctx, mg_event_const_to_name(ev)), jsmsg, rspn->jsrspn)
                JSValue ret = JS_Call(SERVER->ctx, SERVER->callback, JS_UNDEFINED, 3, cbargv) ;

                if( JS_IsException(ret) ){
                    js_std_dump_error(SERVER->ctx) ;
                }
                JS_FreeValue(SERVER->ctx, jsmsg) ;
                free(cbargv) ;

                return ;
            }
        }
    }
}


JSValue be_http_server_new(JSContext *ctx, struct mg_connection * conn, JSValue callback) {

    be_http_server_t * server = malloc(sizeof(be_http_server_t)) ;
    memset(server, 0, sizeof(be_http_server_t));
    server->ctx  = ctx ;


    server->callback = JS_DupValue(ctx,callback) ;

    server->telweb = false ;
    server->conn = conn ;
    conn->fn_data = server ;

    JSValue jsobj = JS_NewObjectClass(ctx, js_mg_server_class_id) ;
    JS_SetOpaque(jsobj, server) ;
    
    return jsobj ;
}

/*
参数：
options: {
    addr: string ,
    ssl: boolean ,
    callback: (ev:string, req, rspn)=>void
}

参数：
addr: string
callback: (ev:string, req, rspn)=>void
*/
static JSValue js_mg_mgr_http_listen(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    CHECK_WIFI_INITED
    CHECK_ARGC(1)

    char * addr = NULL ;

    be_http_server_t * server = malloc(sizeof(be_http_server_t)) ;
    memset(server, 0, sizeof(be_http_server_t));
    server->ctx  = ctx ;

    // options
    if( JS_IsObject(argv[0]) ){
        ASSIGN_STR_PROP_C(argv[0], "addr", addr, {
            THROW_GOTO(failed,"missing option addr")
        })

        ASSIGN_PROP(argv[0], "callback", server->callback)
        if( !JS_IsFunction(ctx, server->callback) ) {
            THROW_GOTO(failed,"arg callback must be a function")
        }
        JS_DupValue(ctx, server->callback) ;

        ASSIGN_BOOL_PROP(argv[0], "ssl", server->ssl)
        ASSIGN_BOOL_PROP(argv[0], "telweb", server->telweb)
    }

    // addr + callback
    else {
        CHECK_ARGC(2)

        ARGV_AS_STRING_C(0, addr, {
            THROW_GOTO(failed,"arg addr must be a string")
        }) ;

        if( !JS_IsFunction(ctx, argv[1]) ) {
            THROW_GOTO(failed, "arg callback must be a function")
        }
        server->callback = JS_DupValue(ctx,argv[1]) ;
        server->telweb = false ;
        server->ssl = false ;
    }

    if(!mg_url_is_listening(addr)) {
        THROW_GOTO(failed, "addr %s has listened", addr) ;
    }

    struct mg_connection * conn = mg_http_listen(be_module_mg_mgr(), addr, http_server_event_handler, NULL) ;
    if(conn==NULL) {
        THROW_GOTO(failed, "could not listen addr: %s", addr)
    }
    JS_FreeCString(ctx, addr) ;
    server->conn = conn ;
    conn->fn_data = server ;

    JSValue jsobj = JS_NewObjectClass(ctx, js_mg_server_class_id) ;
    JS_SetOpaque(jsobj, server) ;

    // 这个引用在监听结束后 free
    JS_DupValue(ctx, jsobj) ;
    
    return jsobj ;

failed:
    if(addr) {
        JS_FreeCString(ctx, addr) ;
        addr = NULL ;
    }
    if(server) {
        JS_FreeValue(ctx, server->callback) ;
        free(server) ;
        server = NULL ;
    }
    return JS_EXCEPTION ;

}



void be_module_mg_server_init() {
    JS_NewClassID(&js_mg_server_class_id);
}
void be_module_mg_server_require(JSContext *ctx, JSValue mg, JSValue pkgShadow) {
    QJS_DEF_CLASS(mg_server, "Server", "mg.Server", JS_UNDEFINED, pkgShadow)
    JS_SetPropertyStr(ctx, mg, "httpListen", JS_NewCFunction(ctx, js_mg_mgr_http_listen, "httpListen", 1));
}