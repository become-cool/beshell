#include "MQTTClient.hpp"
#include "../Mg.hpp"
#include "mongoose/mongoose.h"
#include <cstring>

using namespace std ;


namespace be::mg {

    typedef struct {
        int ev ;
        union {
            struct mg_mqtt_message msg ;
        } data ;
    } mg_event_wrapper_t ;

    DEFINE_NCLASS_META(MQTTClient, EventEmitter)
    std::vector<JSCFunctionListEntry> MQTTClient::methods = {
        JS_CFUNC_DEF("ping", 0, MQTTClient::ping),
        JS_CFUNC_DEF("push", 0, MQTTClient::push),
        JS_CFUNC_DEF("sub", 0, MQTTClient::sub),
        JS_CFUNC_DEF("unsub", 0, MQTTClient::unsub),
        JS_CFUNC_DEF("disconnect", 0, MQTTClient::disconnect),
    } ;

    MQTTClientHandler MQTTClient::handler = nullptr ;

    MQTTClient::MQTTClient(JSContext * ctx, struct mg_connection * conn)
            : EventEmitter(ctx,build(ctx))
            , conn(conn)
    {
        if(conn) {
            conn->fn_data = this ;
        }
        enableNativeEvent(ctx, sizeof(mg_event_wrapper_t), 5) ;
    }

    MQTTClient::~MQTTClient() {

        // conn 尚未断开时 delete js 或 c++ 对象
        if(conn) {
            if( conn->fn_data == this ){
                conn->is_closing = 1 ;
                conn->fn_data = nullptr ;
            }
        }
    }

    #define FREE_MG_STR(mgstr)      \
            if(mgstr.buf)           \
                free( mgstr.buf ) ; \
            mgstr.buf = nullptr ;   \
            mgstr.len = 0 ;
    #define FREE_MG_MSG(msg)        \
            FREE_MG_STR(msg.topic)  \
            FREE_MG_STR(msg.data)   \
            FREE_MG_STR(msg.dgram)

    void MQTTClient::eventHandler(struct mg_connection * c, int ev, void * ev_data) {

        MQTTClient * nobj = (MQTTClient *)c->fn_data ;
        if(handler && nobj && handler(nobj, c, ev, ev_data, c->fn_data)) {
            return ;
        }

        if(ev==MG_EV_POLL) {
            return ;
        }
        if(!nobj) {
            return ;
        }

        mg_event_wrapper_t ev_wrapper = { ev } ;
        memset(&ev_wrapper.data, 0, sizeof(ev_wrapper.data)) ;

        switch (ev)
        {
        case MG_EV_CONNECT:
            if (nobj->is_tls) {
                struct mg_tls_opts opts = {
                    .ca = mg_str(Mg::ca.c_str()),
                    .name = mg_str(nobj->_host.c_str())
                };
                mg_tls_init(c, &opts);
            }
            break ;

        case MG_EV_MQTT_MSG: {
            struct mg_mqtt_message * msg = (struct mg_mqtt_message *) ev_data;
            ev_wrapper.data.msg.topic = mg_strdup(msg->topic) ;
            ev_wrapper.data.msg.data = mg_strdup(msg->data) ;
            // ev_wrapper.data.msg.dgram = mg_strdup(msg->dgram) ;
            ev_wrapper.data.msg.id = msg->id ;
            ev_wrapper.data.msg.cmd = msg->cmd ;
            ev_wrapper.data.msg.qos = msg->qos ;
            ev_wrapper.data.msg.ack = msg->ack ;
            break ;
        }

        case MG_EV_ERROR:
            if (ev_data) {
                printf("mg error: %p %s\n", c->fd, (char *) ev_data);
            }
            break;

        case MG_EV_CLOSE:
            nobj->conn = nullptr ;
            c->fn_data = nullptr ;
            break;
        
        default:
            break;
        }
        
        if(!nobj->emitNativeEvent((void *)&ev_wrapper)){
            if(ev_wrapper.ev == MG_EV_MQTT_MSG) {
                FREE_MG_MSG(ev_wrapper.data.msg) ;
            }
        }
    }

    void MQTTClient::onNativeEvent(JSContext *ctx, void * param) {
        
        mg_event_wrapper_t * event = ((mg_event_wrapper_t*)param) ;
        // if(event->ev!=MG_EV_POLL) {
        //     printf("onNativeEvent() event: %d\n", event->ev) ;
        // }

        switch (event->ev) {
            case MG_EV_OPEN:
                emitSync("open") ;
                break;
            case MG_EV_CONNECT:
                emitSync("connect") ;
                // emitCallback("connect", {}) ;
                break;
            case MG_EV_ERROR:
                emitSync("error") ;
                break;
            case MG_EV_MQTT_OPEN:
                emitSync("mqtt.open") ;
                break;
            case MG_EV_MQTT_MSG: {
                JSValue msg = JS_NewObject(ctx) ;
                JS_SetPropertyStr(ctx, msg, "topic", JS_NewStringLen(ctx, event->data.msg.topic.buf, event->data.msg.topic.len));
                JS_SetPropertyStr(ctx, msg, "data", JS_NewStringLen(ctx, event->data.msg.data.buf, event->data.msg.data.len));
                // JS_SetPropertyStr(ctx, msg, "dgram", JS_NewStringLen(ctx, event->data.msg.dgram.buf, event->data.msg.dgram.len));
                JS_SetPropertyStr(ctx, msg, "id", JS_NewUint32(ctx, event->data.msg.id));
                JS_SetPropertyStr(ctx, msg, "cmd", JS_NewUint32(ctx, event->data.msg.cmd));
                JS_SetPropertyStr(ctx, msg, "qos", JS_NewUint32(ctx, event->data.msg.qos));
                JS_SetPropertyStr(ctx, msg, "ack", JS_NewUint32(ctx, event->data.msg.ack));
                FREE_MG_MSG((event->data.msg)) ;
                emitSync("msg", {msg});
                break;
            }
            case MG_EV_CLOSE:
                emitSync("close") ;
                break;
            default:
                break;
        }
    }

    #define PROP_TO_MGSTR(obj, propName, cvar)                          \
        {                                                               \
            JSValue jsvar = JS_GetPropertyStr(ctx,obj, propName) ;      \
            if(JS_IsString(jsvar)) {                                    \
                size_t len = 0 ;                                        \
                const char * val = JS_ToCStringLen(ctx, &len, jsvar) ;  \
                cvar = mg_strdup(mg_str_n(val, len)) ;                  \
                JS_FreeCString(ctx, val) ;                              \
            }                                                           \
            JS_FreeValue(ctx, jsvar) ;                                  \
        }

    JSValue MQTTClient::connect(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        
        ASSERT_ARGC(1)
        ARGV_TO_CSTRING_LEN_E(0, url, urlLen, "arg url must be a string")

        struct mg_mqtt_opts opts = {.clean = true};
        if(argc>1) {
            if( !JS_IsObject(argv[1]) ) {
                JSTHROW("arg opts must be an object") ;
            }

            PROP_TO_MGSTR(argv[1], "user", opts.user)
            PROP_TO_MGSTR(argv[1], "pass", opts.pass)
            PROP_TO_MGSTR(argv[1], "client_id", opts.client_id)
            PROP_TO_MGSTR(argv[1], "will_topic", opts.topic)
            PROP_TO_MGSTR(argv[1], "will_message", opts.message)

            GET_BOOL_PROP_OPT(argv[1], "will_retain", opts.retain, false)
            GET_BOOL_PROP_OPT(argv[1], "clean", opts.clean, true)
            GET_UINT8_PROP_OPT(argv[1], "qos", opts.qos, 0)
            GET_UINT16_PROP_OPT(argv[1], "keepalive", opts.keepalive, 0)
        }

        // ds(opts.user.buf)
        // ds(opts.pass.buf)
        // ds(opts.client_id.buf)
        // ds(opts.will_topic.buf)
        // ds(opts.will_message.buf)
        // dn(opts.will_retain)
        // dn(opts.clean)
        // dn(opts.qos)
        // dn(opts.keepalive)

        MQTTClient * client = new MQTTClient(ctx, nullptr) ;

        struct mg_connection * conn = mg_mqtt_connect(&Mg::mgr, url, &opts, (mg_event_handler_t)eventHandler, (void*)client);
        
        if(url[4]=='s') {
            client->is_tls = true ;
            
            struct mg_str host = mg_url_host(url) ;
            client->_host = string(host.buf, host.len) ;
        }

        JS_FreeCString(ctx, url) ;

        if(conn==NULL) {
            JS_ThrowReferenceError(ctx, "could not listen addr: %s", url) ;
            JS_FreeCString(ctx, url) ;
            return JS_EXCEPTION ;
        }

        client->conn = conn ;
        client->shared() ;
        return client->jsobj ;
    }

    JSValue MQTTClient::push(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {    
        CHECK_ARGC(2)
        THIS_NCLASS(MQTTClient, that)
        if(!that->conn){
            JSTHROW("mqtt client is not connected")
        }

        ARGV_TO_CSTRING(0, url)
        
        struct mg_str topic = mg_str(url);
        struct mg_str msg;
        
        // Handle payload as string or ArrayBuffer
        if (JS_IsString(argv[1])) {
            ARGV_TO_CSTRING(1, payload)
            msg = mg_str(payload);
            JS_FreeCString(ctx, payload);
        } 
        else if (JS_IsArrayBuffer(argv[1])) {
            size_t length = 0;
            uint8_t* buffer = JS_GetArrayBuffer(ctx, &length, argv[1]);
            if (!buffer) {
                JS_FreeCString(ctx, url);
                JSTHROW("Failed to get ArrayBuffer data");
            }
            msg = mg_str_n((const char*)buffer, length);
        } 
        else {
            JS_FreeCString(ctx, url);
            JSTHROW("payload must be a string or ArrayBuffer");
        }
        
        ARGV_TO_UINT8_OPT(2, qos, 1)

        bool retain = false;
        if (argc > 3) {
            retain = JS_ToBool(ctx, argv[3]);
        }

        JS_FreeCString(ctx, url);
        
        struct mg_mqtt_opts pub_opts = {
            .topic = topic,
            .message = msg,
            .qos = qos,
            .retain = retain
        };
        mg_mqtt_pub(that->conn, &pub_opts);

        return JS_UNDEFINED;
    }

    JSValue MQTTClient::sub(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        CHECK_ARGC(1)
        THIS_NCLASS(MQTTClient, that)
        if(!that->conn){
            JSTHROW("mqtt client is not connected")
        }

        std::string ARGV_TO_STRING(0, ctopic)
        ARGV_TO_UINT8_OPT(1, qos, 1)

        struct mg_mqtt_opts sub_opts = {
            .topic = mg_str(ctopic.c_str()),
            .qos = qos
        };
        mg_mqtt_sub(that->conn, &sub_opts) ;

        return JS_UNDEFINED ;
    }

    JSValue MQTTClient::unsub(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        
    // static uint16_t s_id;
    // uint32_t total_len = 2 + (uint32_t) topic->len + 2;
    // mg_mqtt_send_header(c, MQTT_CMD_UNSUBSCRIBE, 2, total_len);
    // if (++s_id == 0) ++s_id;
    // mg_send_u16(c, mg_htons(s_id));
    // mg_send_u16(c, mg_htons((uint16_t) topic->len));
    // mg_send(c, topic->ptr, topic->len);
  

        return JS_UNDEFINED ;
    }

    JSValue MQTTClient::ping(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        THIS_NCLASS(MQTTClient, that)
        if(!that->conn){
            JSTHROW("mqtt client is not connected")
        }
        mg_mqtt_ping(that->conn) ;
        return JS_UNDEFINED ;
    }
    JSValue MQTTClient::disconnect(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        THIS_NCLASS(MQTTClient, that)
        if(!that->conn){
            JSTHROW("mqtt client is not connected")
        }
        mg_mqtt_disconnect(that->conn, NULL) ;
        return JS_UNDEFINED ;
    }

    void MQTTClient::setHandler(MQTTClientHandler _handler) {
        handler = _handler ;
    }
}