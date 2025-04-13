#include "./HTTPClient.hpp"
#include "./HTTPRequest.hpp"
#include "../Mg.hpp"
#include "qjs_utils.h"
#include "quickjs/quickjs.h"

using namespace std ;

namespace be::mg {
    DEFINE_NCLASS_META(Client, EventEmitter)
    std::vector<JSCFunctionListEntry> Client::methods = {
        JS_CFUNC_DEF("send", 0, Client::send),
        JS_CFUNC_DEF("close", 0, Client::close),
        JS_CFUNC_DEF("enableChunkEvent", 0, Client::enableChunkEvent),
    } ;

    Client::Client(JSContext * ctx, struct mg_connection * conn, JSValue callback)
        : EventEmitter(ctx,build(ctx))
        , callback(JS_DupValue(ctx,callback))
        , conn(conn)
    {
        if(conn) {
            conn->fn_data = this ;
        }
    }
    
    HTTPClientHandler Client::handler = nullptr ;

    Client::~Client(){
        if(conn) {
            if( conn->fn_data == this ){
                conn->fn_data = nullptr ;
            }
        }
        JS_FreeValue(ctx, callback) ;
        callback = JS_NULL ;
    }
    
    void Client::setConn(struct mg_connection * conn){
        this->conn = conn ;
    }

    /**
     * 发送数据
     * 
     * @method send
     * @param data:string 数据
     * @return bool
     */
    JSValue Client::send(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        ASSERT_ARGC(1)
        THIS_NCLASS(Client,client)

        bool res = false ;

        // ws/wss
        if(client->is_ws) {
            if( JS_IsArrayBuffer(argv[0]) ){
                size_t size ;
                char * buff = (char *)JS_GetArrayBuffer(ctx, &size, argv[0]) ;
                res = mg_ws_send(client->conn, buff, size, WEBSOCKET_OP_BINARY);
            }
            else {
                ARGV_TO_CSTRING_LEN_E(0, data, len, "arg data must be a string")
                res = mg_ws_send(client->conn, data, len, WEBSOCKET_OP_TEXT);
                JS_FreeCString(client->ctx, data) ;
            }
        }

        // http/https
        else {
            if( JS_IsArrayBuffer(argv[0]) ){
                size_t datalen = 0;
                uint8_t * data = (uint8_t *)JS_GetArrayBuffer(ctx, &datalen, argv[0]) ;
                res = mg_send(client->conn, data, datalen) ;
            }
            else {
                ARGV_TO_CSTRING_LEN_E(0, data, len, "arg data must be a string")
                res = mg_send(client->conn, data, len) ;
                JS_FreeCString(ctx, data) ;
            }
        }

        return res? JS_TRUE : JS_FALSE;
    }

    /**
     * 关闭连接
     * 
     * @method close
     * @return undefined
     */
    JSValue Client::close(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        THIS_NCLASS(Client,client)
        client->conn->is_closing = true ;
        return JS_UNDEFINED ;
    }


    // enum {
    // MG_EV_ERROR,      // Error                        char *error_message
    // MG_EV_OPEN,       // Connection created           NULL
    // MG_EV_POLL,       // mg_mgr_poll iteration        uint64_t *uptime_millis
    // MG_EV_RESOLVE,    // Host name is resolved        NULL
    // MG_EV_CONNECT,    // Connection established       NULL
    // MG_EV_ACCEPT,     // Connection accepted          NULL
    // MG_EV_TLS_HS,     // TLS handshake succeeded      NULL
    // MG_EV_READ(7),       // Data received from socket    long *bytes_read
    // MG_EV_WRITE,      // Data written to socket       long *bytes_written
    // MG_EV_CLOSE,      // Connection closed            NULL
    // MG_EV_HTTP_HDRS(10),  // HTTP headers                 struct mg_http_message *
    // MG_EV_HTTP_MSG,   // Full HTTP request/response   struct mg_http_message *
    // MG_EV_WS_OPEN,    // Websocket handshake done     struct mg_http_message *
    // MG_EV_WS_MSG,     // Websocket msg, text or bin   struct mg_ws_message *
    // MG_EV_WS_CTL,     // Websocket control msg        struct mg_ws_message *
    // MG_EV_MQTT_CMD,   // MQTT low-level command       struct mg_mqtt_message *
    // MG_EV_MQTT_MSG,   // MQTT PUBLISH received        struct mg_mqtt_message *
    // MG_EV_MQTT_OPEN,  // MQTT CONNACK received        int *connack_status_code
    // MG_EV_SNTP_TIME,  // SNTP time received           uint64_t *epoch_millis
    // MG_EV_WAKEUP,     // mg_wakeup() data received    struct mg_str *data
    // MG_EV_USER        // Starting ID for user events
    // };
    void Client::eventHandler(struct mg_connection * conn, int ev, void * ev_data) {
        
        // if(ev!=MG_EV_POLL) {
        //     printf("Client::eventHandler() event: %d\n", ev) ;
        // }

        if(!conn->fn_data) {
            return ;
        }
        Client * client = (Client *)conn->fn_data ;
        
        if(handler && client && handler(client, conn, ev, ev_data, conn->fn_data)) {
            return ;
        }

        switch(ev) {

            case MG_EV_CONNECT: {

                if(client && client->is_tls && Mg::ca.length()>0) {
                    struct mg_tls_opts opts = {
                        .ca = mg_str(Mg::ca.c_str()),
                        .name = mg_str(client->_host.c_str())
                    };
                    mg_tls_init(conn, &opts);
                }

                JSValue evname = JS_NewString(client->ctx, Mg::eventName(ev)) ;
                JS_CALL_ARG1(client->ctx, client->callback, evname)
                JS_FreeValue(client->ctx, evname) ;
                break ;
            }

            case MG_EV_HTTP_HDRS: {
                // 
                if( client->headerLength==0 ){
                    const char *header_end = strnstr((char *)conn->recv.buf, "\r\n\r\n", conn->recv.len) ;
                    if(header_end) {
                        struct mg_http_message *hm = (struct mg_http_message *) ev_data;
                        HTTPRequest * req = new HTTPRequest(client->ctx, hm) ;
                        req->shared() ; // 智能回收模式 (jsobj gc 回收时自动 delete req)

                        JSValue evname = JS_NewString(client->ctx, "http.head") ;
                        
                        JS_CALL_ARG2(client->ctx, client->callback, evname, req->jsobj)

                        JS_FreeValue(client->ctx, evname) ;
                        JS_FreeValue(client->ctx, req->jsobj) ;

                        client->headerLength = header_end - (char *)conn->recv.buf + 4 ; // 4 = "\r\n\r\n" 的长度
                    }
                }
                
                if (client->_enableChunkEvent && client->headerLength) {

                    struct mg_http_message *hm = (struct mg_http_message *) ev_data;

                    // Found the end of the header
                    size_t bodyLength = conn->recv.len - client->headerLength;

                    size_t chunkLenght = bodyLength - client->receivedBodyLength ;
                    uint8_t * chunk = conn->recv.buf + client->headerLength + client->receivedBodyLength ;

                    client->receivedBodyLength = bodyLength ;

                    JSValue evname = JS_NewString(client->ctx, "http.chunk") ;
                    JSValue ab = JS_NewArrayBuffer(client->ctx, chunk, chunkLenght, nofreeArrayBuffer, nullptr, false) ;
                    JS_CALL_ARG2(client->ctx, client->callback, evname, ab)
                    JS_FreeValue(client->ctx, evname) ;
                    JS_FreeValue(client->ctx, ab) ;
                }
                
                break;
            }
            
            case MG_EV_HTTP_MSG: 
            {
                // moogose https 协议，会在 close 事件以后触发 msg 事件
                if(!client || !client->ctx) {
                    break ;
                }

                struct mg_http_message *hm = (struct mg_http_message *) ev_data;
                HTTPRequest * req = new HTTPRequest(client->ctx, hm) ;
                req->shared() ; // 智能回收模式 (jsobj gc 回收时自动 delete req)

                JSValue evname = JS_NewString(client->ctx, Mg::eventName(ev)) ;

                JS_CALL_ARG2(client->ctx, client->callback, evname, req->jsobj)

                JS_FreeValue(client->ctx, req->jsobj) ;
                JS_FreeValue(client->ctx, evname) ;

                // mg_http_delete_chunk(conn, hm) ;

                break ;
            }

            case MG_EV_CLOSE : {
            
                JSValue evname = JS_NewString(client->ctx, "close") ;
                JS_CALL_ARG1(client->ctx, client->callback, evname)
                JS_FreeValue(client->ctx, evname) ;
                
                // moogose https 协议，会在 close 事件以后触发 msg 事件
                if(conn->fn_data==client) {
                    conn->fn_data = NULL ;

                    delete client ;
                    client = NULL ;
                    conn->fn_data = NULL ;
                }
                break ;
            }

            case MG_EV_ERROR:
                if(ev_data) {
                    JSValue evname = JS_NewString(client->ctx, Mg::eventName(ev)) ;
                    JSValue msg = JS_NewString(client->ctx, (const char *)ev_data) ;
                    JS_CALL_ARG2(client->ctx, client->callback, evname, msg)
                    JS_FreeValue(client->ctx, evname) ;
                    JS_FreeValue(client->ctx, msg) ;
                    break ;
                }

            default: {
                JSValue evname = JS_NewString(client->ctx, Mg::eventName(ev)) ;
                JS_CALL_ARG1(client->ctx, client->callback, evname)
                JS_FreeValue(client->ctx, evname) ;
                break ;
            }
        }
    }

    JSValue Client::enableChunkEvent(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        THIS_NCLASS(Client,client)
        client->_enableChunkEvent = true ;
        return JS_UNDEFINED ;
    }

    JSValue Client::connect(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {

        ASSERT_ARGC(2)
        if( !JS_IsFunction(ctx, argv[1]) ) {
            JSTHROW("arg callback must be a function")
        }
        ARGV_TO_CSTRING_LEN_E(0, url, urlLen, "arg url must be a string")

        Client * client = new Client(ctx, nullptr, argv[1]) ;
        struct mg_connection * conn = NULL ;
        if ( strncmp(url,"http://", 7)==0 || strncmp(url,"https://", 8)==0 ) {
            conn = mg_http_connect(&Mg::mgr, url, Client::eventHandler, client) ;
            client->is_ws = false ;

            if(url[4]=='s') {
                client->is_tls = true ;
            }
        }

        else if( strncmp(url,"ws://", 5)==0 || strncmp(url,"wss://", 6)==0 ) {
            conn = mg_ws_connect(&Mg::mgr, url, Client::wsEventHandler, client, NULL) ;
            client->is_ws = true ;
            
            if(url[2]=='s') {
                client->is_tls = true ;
            }
        }

        else {
            delete client ;
            JS_FreeCString(ctx, url) ;
            JSTHROW("not support url protocol")
        }

        if(client->is_tls) {
            struct mg_str host = mg_url_host(url) ;
            client->_host = string(host.buf, host.len) ;
        }

        client->conn = conn ;
        if(conn==NULL) {
            JS_ThrowReferenceError(ctx, "could not connect to url: %s", url) ;
            JS_FreeCString(ctx, url) ;
            return JS_EXCEPTION ;
        }

        JS_FreeCString(ctx, url) ;
        
        return client->jsobj ;
    }

    void Client::setHandler(HTTPClientHandler _handler) {
        handler = _handler ;
    }

    
    bool Client::isTLS() const {
        return is_tls ;
    }
    bool Client::isWS() const {
        return is_ws ;
    }
    std::string Client::host() const {
        return _host ;
    }
}
