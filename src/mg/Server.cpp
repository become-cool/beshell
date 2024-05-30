#include "Server.hpp"
#include "Mg.hpp"
#include "module/WiFi.hpp"


#ifdef ESP_PLATFORM
#define CHECK_WIFI_INITED           \
    if(!WiFi::hasInited()) {        \
        JSTHROW("wifi not init")    \
    }
#else
#define CHECK_WIFI_INITED
#endif

using namespace std ;

namespace be::mg {
    DEFINE_NCLASS_META(Server, NativeClass)
    std::vector<JSCFunctionListEntry> Server::methods = {
        JS_CFUNC_DEF("close", 0, Server::close),
        JS_CFUNC_DEF("startTelweb", 0, Server::startTelweb),
        JS_CFUNC_DEF("stopTelweb", 0, Server::stopTelweb),
        JS_CFUNC_DEF("setHandler", 0, Server::setHandler),
    } ;
    
    Server::Server(JSContext * ctx, struct mg_connection * conn, JSValue callback)
        : NativeClass(ctx,build(ctx))
        , conn(conn)
        , callback(JS_DupValue(ctx,callback))
    {
        if(conn) {
            conn->fn_data = this ;
        }
    }

    Server::~Server() {
        if(conn && conn->fn_data==nullptr) {
            conn->is_closing = true ;
            conn->fn_data = nullptr ;
        }
        JS_FreeValue(ctx, callback) ;
    }

    JSValue Server::close(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        THIS_NCLASS(Server,server)
        server->conn->is_closing = true ;
        JS_FreeValue(ctx,this_val) ;
        return JS_UNDEFINED ;
    }

    JSValue Server::startTelweb(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        THIS_NCLASS(Server,server)
        server->telweb = true ;
        return JS_UNDEFINED ;
    }
    JSValue Server::stopTelweb(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        THIS_NCLASS(Server,server)
        server->telweb = false ;
        return JS_UNDEFINED ;
    }

    JSValue Server::setHandler(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        ASSERT_ARGC(1)
        if( !JS_IsFunction(ctx, argv[0]) ){
            JSTHROW("arg callback must be a function")
        }
        THIS_NCLASS(Server,server)
        server->callback = JS_DupValue(ctx,argv[0]) ;
        return JS_UNDEFINED ;
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

    void Server::eventHandler(struct mg_connection * conn, int ev, void *ev_data, void *fn_data) {

        if(ev== MG_EV_POLL || !fn_data) {
            return ;
        }

        #define SERVER ((Server *)fn_data)
        // be_http_server_t * server = (be_http_server_t *)fn_data ;
        // printf("\nevent:%s, conn:%p, server:%p, server->conn:%p, \n", mg_event_const_to_name(ev), conn, server, server->conn) ;
        // printf("%s %s \n", ((server->conn==NULL || server->conn==conn)?"server":"client"), mg_event_const_to_name(ev)) ;

        if(ev==MG_EV_ACCEPT && SERVER->ssl) {
            struct mg_tls_opts opts = {
                .cert = Mg::cert_path.c_str(),    // Certificate file
                .certkey = Mg::certkey_path.c_str(),  // Private key file
            };
            mg_tls_init(conn, &opts);
            printf("mg_tls_init()\n") ;
            return ;
        }


        // @todo 未实现
        // if(SERVER->telweb) {
        //     if( telnet_ws_response(conn, ev, ev_data, fn_data) ) {
        //         return ;
        //     }
        //     // c telweb 函数没有处理的请求, 由 js 函数接着处理
        //     // 这种情况下 open 事件是在 c 函数内处理的，
        //     // 需要在此创建 Response 
        //     if(!conn->userdata){
        //         conn->userdata = new Response(SERVER->ctx, conn) ;
        //     }
        // }

        if(!JS_IsFunction(SERVER->ctx, SERVER->callback)) {
            printf("callback is not a function, event:%s\n", Mg::eventName(ev)) ;
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
                    conn->userdata = new Response(SERVER->ctx, conn) ; 
                    return ;
                }
                case MG_EV_CLOSE:
                    if(conn->userdata) {
                        Response * rspn = (Response *)conn->userdata ;

                        JSValueConst * MALLOC_ARGV3(cbargv, JS_NewString(SERVER->ctx, "close"), JS_NULL, rspn->jsobj)
                        JSValue ret = JS_Call(SERVER->ctx, SERVER->callback, JS_UNDEFINED, 3, cbargv) ;
                        free(cbargv) ;
                        if(JS_IsException(ret)) {
                            js_std_dump_error(SERVER->ctx) ;
                            JS_FreeValue(SERVER->ctx, ret) ;
                        }

                        delete rspn ;
                        conn->userdata = NULL ;
                    }
                    return ;

                case MG_EV_HTTP_MSG:
                case MG_EV_HTTP_CHUNK: {
                    if(!conn->userdata) {
                        printf("conn->userdata == NULL ??") ;
                        return ;
                    }
                    Response * rspn = (Response *)conn->userdata ;
                    HTTPRequest * req = new HTTPRequest(SERVER->ctx,(struct mg_http_message *)ev_data);

                    MAKE_ARGV3(cbargv, JS_NewString(SERVER->ctx, Mg::eventName(ev)), req->jsobj, rspn->jsobj)
                    JSValue ret = JS_Call(SERVER->ctx, SERVER->callback, JS_UNDEFINED, 3, cbargv) ;

                    delete req ;
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
                    Response * rspn = (Response *)conn->userdata ;

                    struct mg_ws_message * msg = (struct mg_ws_message *)ev_data ;

                    JSValue jsmsg = JS_NewObject(SERVER->ctx) ;
                    uint8_t op = msg->flags&15 ;
                    JS_SetPropertyStr(SERVER->ctx, jsmsg, "type", JS_NewUint32(SERVER->ctx, op)) ;

                    if(op == WEBSOCKET_OP_TEXT) {
                        JS_SetPropertyStr(SERVER->ctx, jsmsg, "data", JS_NewStringLen(SERVER->ctx, msg->data.ptr, msg->data.len)) ;
                    }
                    else if(op == WEBSOCKET_OP_BINARY) {
                        JS_SetPropertyStr(SERVER->ctx, jsmsg, "data", JS_NewArrayBuffer(SERVER->ctx,(uint8_t*)msg->data.ptr, msg->data.len,NULL,NULL,false)) ;
                    }

                    MAKE_ARGV3(cbargv, JS_NewString(SERVER->ctx, Mg::eventName(ev)), jsmsg, rspn->jsobj)
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

    

    /**
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
    JSValue Server::listenHttp(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        CHECK_WIFI_INITED
        ASSERT_ARGC(1)

        string addr ;
        JSValue callback = JS_NULL ;
        bool ssl = false ;
        bool telweb = false ;

        // options
        if( JS_IsObject(argv[0]) ){

            GET_PROP(argv[0], "callback", callback)
            if( !JS_IsFunction(ctx, callback) ) {
                JSTHROW("arg callback must be a function") ;
            }

            GET_STR_PROP(argv[0], "addr", addr, )

            GET_BOOL_PROP_OPT(argv[0], "ssl", ssl, false)
            GET_BOOL_PROP_OPT(argv[0], "telweb", telweb, false)
        }

        // addr + callback
        else {
            ASSERT_ARGC(2)

            if( !JS_IsFunction(ctx, argv[1]) ) {
                JSTHROW("arg callback must be a function") ;
            }

            ARGV_TO_STRING(0, addr)
            
            callback = argv[1] ;
        }

        if(!Mg::isListening(addr.c_str())) {
            JSTHROW("addr %s has listened", addr)
        }

        struct mg_connection * conn = mg_http_listen(&Mg::mgr, addr.c_str(), eventHandler, NULL) ;
        if(conn==NULL) {
            JSTHROW("could not listen addr: %s", addr) ;
        }

        Server * server = new Server(ctx, conn, callback) ;
        server->ssl = ssl ;
        server->telweb = telweb ;

        return server->jsobj ;
    }
}