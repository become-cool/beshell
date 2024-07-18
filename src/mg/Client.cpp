#include "Client.hpp"
#include "HTTPRequest.hpp"
#include "Mg.hpp"

using namespace std ;

namespace be::mg {
    DEFINE_NCLASS_META(Client, NativeClass)
    std::vector<JSCFunctionListEntry> Client::methods = {
        JS_CFUNC_DEF("send", 0, Client::send),
        JS_CFUNC_DEF("close", 0, Client::close),
        JS_CFUNC_DEF("initTLS", 0, Client::initTLS),
    } ;

    Client::Client(JSContext * ctx, struct mg_connection * conn, JSValue callback)
        : NativeClass(ctx,build(ctx))
        , conn(conn)
        , callback(JS_DupValue(ctx,callback))
    {
        if(conn) {
            conn->fn_data = this ;
        }
    }

    Client::~Client(){
        if(conn) {
            if( conn->fn_data == this ){
                conn->fn_data = nullptr ;
            }
        }
        JS_FreeValue(ctx, callback) ;
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

        ARGV_TO_CSTRING_LEN_E(0, data, len, "arg data must be a string")
        bool res = mg_send(client->conn, data, len) ;
        JS_FreeCString(ctx, data) ;

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


    /**
     * 初始化连接的 TLS 
     * 
     * @method initTLS
     * @param host:string 主机名
     * @return undefined
     */
    JSValue Client::initTLS(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        ASSERT_ARGC(1)
        ARGV_TO_CSTRING_E(0, host, "arg host must be a string")

        THIS_NCLASS(Client,client)

        struct mg_tls_opts opts = {.ca = Mg::ca_path.c_str(), .srvname = mg_str(host) };
        mg_tls_init(client->conn, &opts);
        
        JS_FreeCString(ctx, host) ;
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
    void Client::eventHandler(struct mg_connection * conn, int ev, void * ev_data, void *fnd) {
        Client * client = (Client *)fnd ;
        switch(ev) {
            case MG_EV_POLL:
                if(++client->poll_times > 15000 ){
                    JS_CALL_ARG1(client->ctx, client->callback, JS_NewString(client->ctx, "timeout"))
                    conn->is_closing = 1 ;
                }
                break ;

            case MG_EV_HTTP_MSG: 
            case MG_EV_HTTP_CHUNK: 
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

                break ;
            }

            case MG_EV_CLOSE :

                // moogose https 协议，会在 close 事件以后触发 msg 事件
                if(conn->fn_data==client) {
                    conn->fn_data = NULL ;
                }

                delete client ;
                client = NULL ;
                fnd = NULL ;
                break ;

            case MG_EV_ERROR:
                if(ev_data) {
                    JSValue evname = JS_NewString(client->ctx, Mg::eventName(ev)) ;
                    JSValue msg = JS_NewString(client->ctx, (const char *)ev_data) ;
                    JS_CALL_ARG2(client->ctx, client->callback, evname, msg)
                    JS_FreeValue(client->ctx, evname) ;
                    JS_FreeValue(client->ctx, msg) ;
                    break ;
                }

            default:
                    JSValue evname = JS_NewString(client->ctx, Mg::eventName(ev)) ;
                    JS_CALL_ARG1(client->ctx, client->callback, evname)
                    JS_FreeValue(client->ctx, evname) ;
                break ;
        }

    }
    
    JSValue Client::connect(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {

        ASSERT_ARGC(2)
        if( !JS_IsFunction(ctx, argv[1]) ) {
            JSTHROW("arg callback must be a function")
        }
        ARGV_TO_CSTRING_E(0, url, "arg url must be a string")

        Client * client = new Client(ctx, nullptr, argv[1]) ;
        struct mg_connection * conn = mg_http_connect(&Mg::mgr, url, Client::eventHandler, client) ;
        client->conn = conn ;
        if(conn==NULL) {
            JS_ThrowReferenceError(ctx, "could not listen addr: %s", url) ;
            JS_FreeCString(ctx, url) ;
            return JS_EXCEPTION ;
        }

        dp(client)
        dp(client->conn)

        JS_FreeCString(ctx, url) ;
        
        return client->jsobj ;
    }

}