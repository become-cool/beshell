#include "Client.hpp"
#include "HTTPRequest.hpp"
#include "Mg.hpp"
#include "quickjs/quickjs.h"

using namespace std ;

namespace be::mg {

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
    void Client::wsEventHandler(struct mg_connection * conn, int ev, void * ev_data, void *fnd) {
        Client * client = (Client *)fnd ;
        
        if (ev == MG_EV_WS_OPEN) { // WebSocket 连接成功
            // printf("WebSocket connection established\n");

            JSValue eventName = JS_NewString(client->ctx, "connect") ;
            JS_CALL_ARG1(client->ctx, client->callback, eventName)
            JS_FreeValue(client->ctx, eventName) ;
                
            // // 发送文本消息
            // const char *text_message = "Hello, this is a text message!";
            // mg_ws_send(c, text_message, strlen(text_message), WEBSOCKET_OP_TEXT);

            // // 发送二进制消息
            // uint8_t binary_message[] = {0x01, 0x02, 0x03, 0x04};
            // mg_ws_send(c, binary_message, sizeof(binary_message), WEBSOCKET_OP_BINARY);

        } else if (ev == MG_EV_WS_MSG) { // 收到 WebSocket 消息
            struct mg_ws_message *wm = (struct mg_ws_message *) ev_data;

            // 检测消息类型
            JSValue message = JS_UNDEFINED ;
            JSValue isBinary = JS_FALSE ;
            if (wm->flags & WEBSOCKET_OP_TEXT) {
                // printf("Received TEXT message: %.*s\n", (int) wm->data.len, wm->data.ptr);
                message = JS_NewStringLen(client->ctx, wm->data.ptr, wm->data.len) ;
            }

            else if (wm->flags & WEBSOCKET_OP_BINARY) {
                message = JS_NewArrayBufferCopy(client->ctx, (const uint8_t*)wm->data.ptr, wm->data.len) ;
                isBinary = JS_TRUE ;
            }

            JSValue eventName = JS_NewString(client->ctx, "ws.msg") ;

            JS_CALL_ARG3(client->ctx, client->callback, eventName, message, isBinary)

            JS_FreeValue(client->ctx, eventName) ;
            JS_FreeValue(client->ctx, message) ;
            JS_FreeValue(client->ctx, isBinary) ;
        }
        
        else if (ev == MG_EV_CLOSE) { // WebSocket 连接关闭

                // moogose https 协议，会在 close 事件以后触发 msg 事件
                // if(conn->fn_data==client) {
                //     conn->fn_data = NULL ;
                // }
                
                JSValue eventName = JS_NewString(client->ctx, "close") ;
                JS_CALL_ARG1(client->ctx, client->callback, eventName)
                JS_FreeValue(client->ctx, eventName) ;

                delete client ;
                client = NULL ;
                fnd = NULL ;
        }
        else if (ev == MG_EV_ERROR) { // WebSocket 连接关闭
            if(ev_data) {
                JSValue evname = JS_NewString(client->ctx, Mg::eventName(ev)) ;
                JSValue msg = JS_NewString(client->ctx, (const char *)ev_data) ;
                JS_CALL_ARG2(client->ctx, client->callback, evname, msg)
                JS_FreeValue(client->ctx, evname) ;
                JS_FreeValue(client->ctx, msg) ;
            }
        }
        else {
            JSValue evname = JS_NewString(client->ctx, Mg::eventName(ev)) ;
            JS_CALL_ARG1(client->ctx, client->callback, evname)
            JS_FreeValue(client->ctx, evname) ;
        }

    }

}