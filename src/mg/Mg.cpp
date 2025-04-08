/**
 * BeShell 集成了 [Mongoose 库](https://mongoose.ws)，用来处理网络应用层的协议，包括 HTTP(S)、WebSocket、MQTT、SNTP、SMTP 等。
 * 
 * 关于 mg 模块的例子请参考: [在 ESP32 上用 JavaScript 开发 WEB 后端](../../guide/http-server.md)
 * 
 * ## 事件
 * 
 * ```
 * error: 发生错误
 * open: 客户端连接
 * poll: 轮询事件
 * resolve: 域名解析完成
 * connect: 客户端连接建立完成
 * accept: 客户端连接接受完成
 * read: 客户端数据接收完成
 * write: 客户端数据发送完成
 * close: 客户端连接关闭
 * http.msg: HTTP 请求/响应消息接收完成
 * ws.open: WebSocket 握手完成
 * ws.msg: WebSocket 消息接收完成
 * ws.ctl: WebSocket 控制消息接收完成
 * mqtt.cmd: MQTT 低级命令接收完成
 * mqtt.msg: MQTT PUBLISH 消息接收完成
 * mqtt.open: MQTT CONNACK 接收完成
 * sntp.time: SNTP 时间接收完成
 * ```
 * 
 * @module mg
 */

#include "Mg.hpp"
#include <cstdint>
#include "../js/mg.c"

namespace be::mg {

    char const * const Mg::name = "mg" ;

    struct mg_mgr Mg::mgr ;

    char Mg::dns4[28] ;

    std::string Mg::ca = "" ;
    
    struct mg_connection * Mg::connCaptiveProtal = nullptr ;

    Mg::Mg(JSContext * ctx, const char * name)
        : NativeModule(ctx, name, 0)
    {
        exportClass<Server>() ;
        exportClass<Client>() ;
        exportClass<HTTPRequest>() ;
        exportClass<Response>() ;
        exportClass<MQTTClient>() ;
        exportClass<NativeClass>() ;

        exportName("request") ;
        exportName("get") ;
        exportName("download") ;
        exportName("post") ;

        // exportFunction("isListening",isListening,0) ;
        exportFunction("sntpRequest",sntpRequest,0) ;
        exportFunction("connPeer",connPeer,0) ;
        exportFunction("connCount",connCount,0) ;
        exportFunction("getDNS",getDNS,0) ;
        exportFunction("setDNS",setDNS,0) ;
        exportFunction("getDNSTimeout",getDNSTimeout,0) ;
        exportFunction("setDNSTimeout",setDNSTimeout,0) ;
        exportFunction("parseUrl",parseUrl,0) ;
        exportFunction("setLog",setLog,0) ;

        exportFunction("listenHttp",Server::listenHttp,0) ;
        exportFunction("connect",connect,0) ;
        exportFunction("setCA",setCA,0) ;
        exportFunction("startCaptivePortal",startCaptivePortal,0) ;
    }

    void Mg::exports(JSContext *ctx) {
        JSEngineEvalEmbeded(ctx, mg)
    }

    bool Mg::isListening(const char * url) {
        struct mg_addr addr ;
        memset(&addr, 0, sizeof(struct mg_addr));

        addr.port = mg_htons(mg_url_port(url));
        if (!mg_aton(mg_url_host(url), &addr)) {
            return false ;
        }
        for(struct mg_connection * c = mgr.conns; c != NULL; c = c->next) {
            if(c->peer.ip==addr.ip && c->peer.port==addr.port){
                return false ;
            }
        }

        return true ;
    }

    void Mg::use(be::BeShell * beshell) {

        strcpy(dns4,"udp://8.8.8.8:53") ;
        mgr.dns4.url = dns4 ;

        mgr.dnstimeout = 3000 ;

        beshell->use<WiFi>() ;

        beshell->addLoopFunction(loop, NULL) ;
    }

    const char * Mg::eventName(int ev) {
        switch(ev){
            case MG_EV_ERROR: return "error" ;
            case MG_EV_OPEN: return "open" ;
            case MG_EV_POLL: return "poll" ;
            case MG_EV_RESOLVE: return "resolve" ;
            case MG_EV_CONNECT: return "connect" ;
            case MG_EV_ACCEPT: return "accept" ;
            case MG_EV_READ: return "read" ;
            case MG_EV_WRITE: return "write" ;
            case MG_EV_CLOSE: return "close" ;
            case MG_EV_HTTP_MSG: return "http.msg" ;
            case MG_EV_HTTP_CHUNK: return "http.chunk" ;
            case MG_EV_WS_OPEN: return "ws.open" ;
            case MG_EV_WS_MSG: return "ws.msg" ;
            case MG_EV_WS_CTL: return "ws.ctl" ;
            case MG_EV_MQTT_CMD: return "mqtt.cmd" ;
            case MG_EV_MQTT_MSG: return "mqtt.msg" ;
            case MG_EV_MQTT_OPEN: return "mqtt.open" ;
            case MG_EV_SNTP_TIME: return "sntp.time" ;
            case MG_EV_USER: return "user" ;
            default:
                return "unknow" ;
        }
    }

    int Mg::eventConst(const char * evname) {
        if(strcmp(evname,"error")==0) { return MG_EV_ERROR ;}
        else if(strcmp(evname,"open")==0) { return MG_EV_OPEN ;}
        else if(strcmp(evname,"poll")==0) { return MG_EV_POLL ;}
        else if(strcmp(evname,"resolve")==0) { return MG_EV_RESOLVE ;}
        else if(strcmp(evname,"connect")==0) { return MG_EV_CONNECT ;}
        else if(strcmp(evname,"accept")==0) { return MG_EV_ACCEPT ;}
        else if(strcmp(evname,"read")==0) { return MG_EV_READ ;}
        else if(strcmp(evname,"write")==0) { return MG_EV_WRITE ;}
        else if(strcmp(evname,"close")==0) { return MG_EV_CLOSE ;}
        else if(strcmp(evname,"http.msg")==0) { return MG_EV_HTTP_MSG ;}
        else if(strcmp(evname,"http.chunk")==0) { return MG_EV_HTTP_CHUNK ;}
        else if(strcmp(evname,"ws.open")==0) { return MG_EV_WS_OPEN ;}
        else if(strcmp(evname,"ws.msg")==0) { return MG_EV_WS_MSG ;}
        else if(strcmp(evname,"ws.ctl")==0) { return MG_EV_WS_CTL ;}
        else if(strcmp(evname,"mqtt.cmd")==0) { return MG_EV_MQTT_CMD ;}
        else if(strcmp(evname,"mqtt.msg")==0) { return MG_EV_MQTT_MSG ;}
        else if(strcmp(evname,"mqtt.open")==0) { return MG_EV_MQTT_OPEN ;}
        else if(strcmp(evname,"sntp.time")==0) { return MG_EV_SNTP_TIME ;}
        else if(strcmp(evname,"user")==0) { return MG_EV_USER ;}
        else { return -1 ;}
    }

    void Mg::loop(const BeShell & beshell, void * data) {
        mg_mgr_poll(&mgr, 10);
    }
    
    typedef struct  {
        JSValue callback ;
        JSContext * ctx ;
    } sntp_callback_t ;

    static void sntp_event_handler(struct mg_connection *c, int ev, void *ev_data, void *fn_data) {
        if (ev == MG_EV_SNTP_TIME) {
            struct timeval *tv = (struct timeval *) ev_data;
            if(c->fn_data) {
                int64_t timestamp = (int64_t)tv->tv_sec*1000 ;
                timestamp+= tv->tv_usec/1000 ;
                MAKE_ARGV2(argv, JS_NULL, JS_NewInt64(((sntp_callback_t*)c->fn_data)->ctx, timestamp))
                JSValue ret = JS_Call(
                    ((sntp_callback_t*)c->fn_data)->ctx ,
                    ((sntp_callback_t*)c->fn_data)->callback ,
                    JS_NULL, 2, argv
                ) ;
                JS_FreeValue( ((sntp_callback_t*)c->fn_data)->ctx , ret ) ;
                free(argv) ;
            }
            // 标记连接关闭并释放资源
            c->is_closing = 1;
        }
        
        else if (ev == MG_EV_ERROR) {
            if(c->fn_data) {
                JSValue argv = JS_NewString( ((sntp_callback_t*)c->fn_data)->ctx, (char *)ev_data ) ;
                JSValue ret = JS_Call(
                    ((sntp_callback_t*)c->fn_data)->ctx ,
                    ((sntp_callback_t*)c->fn_data)->callback ,
                    JS_NULL, 1, (JSValueConst*) & argv
                ) ;
                JS_FreeValue( ((sntp_callback_t*)c->fn_data)->ctx , ret ) ;
                JS_FreeValue( ((sntp_callback_t*)c->fn_data)->ctx , argv ) ;
            }
        }
        
        else if (ev == MG_EV_CLOSE) {
            if(c->fn_data) {
                JS_FreeValue( ((sntp_callback_t*)c->fn_data)->ctx, ((sntp_callback_t*)c->fn_data)->callback ) ;
                free(c->fn_data) ;
                c->fn_data = nullptr ;
            }
        } 
    }

    JSValue Mg::sntpRequest(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        ASSERT_ARGC(1)
        if( argc>1 && !JS_IsFunction(ctx,argv[1]) ) {
            JSTHROW("arg callback must be a function")
        }

        ARGV_TO_CSTRING(0, url) ;

        // 连接到SNTP服务器（例如pool.ntp.org的UDP 123端口， "udp://pool.ntp.org:123"）
        struct mg_connection *c = mg_sntp_connect(&mgr, url, sntp_event_handler, &mgr);
        JS_FreeCString(ctx, url);

        if (c == NULL) {
            JSTHROW("connet SNTP server failed");
        }

        if(argc>1) {
            sntp_callback_t * cb = (sntp_callback_t*)malloc(sizeof(sntp_callback_t)) ;
            cb->callback = JS_DupValue(ctx,argv[1]) ;
            cb->ctx = ctx ;
            c->fn_data = cb ;
        }

        mg_sntp_send(c, 0);  // 发送SNTP请求（参数0表示立即请求）

        return JS_UNDEFINED ;
    }

    
    /**
     * 创建一个处理 http 协议的服务器实例。
     * 
     * 此函数的例子参考：[简单 HTTP Web 后端的例子](../../guide/http-server.md#_2-%E7%AE%80%E5%8D%95-http-web-%E5%90%8E%E7%AB%AF%E7%9A%84%E4%BE%8B%E5%AD%90)
     * 
     * 第一个参数可以是 [ip:port] 格式的字符串表示服务器地址，例如 `"0.0.0.0:8080"`
     * 
     * 第一个参数也可以是一个对象：
     * 
     * ```typescript
     * {
     *     addr: string ,
     *     ssl: boolean ,
     *     callback: (ev:string, req, rspn)=>void
     * }
     * ```
     * 
     * 回调函数的原型：
     * 
     * ```typescript
     * callback(event:string, request:HTTPRequest, response:Response): void
     * ```
     * 
     * 其中 event 参数参考：[mg 事件](#%E4%BA%8B%E4%BB%B6)
     * 
     * 
     * @function listenHttp
     * @param addrOrOptions:string|object 该参数可以是 [ip:port] 格式的字符串表示服务器地址，或选项对象
     * @param callback:function 服务器事件回调函数，该函数接收三个参数：事件名称、请求对象、响应对象
     * @return [Server](Server.md)
     */


    /**
     * 创建并执行一个客户端连接
     * 
     * 该函数的第一个参数是连接地址，第二个参数是一个回调函数，该函数接收三个参数：事件名称、请求对象。
     * 
     * 回调函数的原型:
     * 
     * ```typescript
     * callback(event:string, request:HTTPRequest): void
     * ```
     * 
     * 其中 event 参数参考：[mg 事件](#%E4%BA%8B%E4%BB%B6)
     * 
     * @method connect
     * @param url:string 连接地址，例如 `"http://www.example.com/path"`
     * @param callback:function 事件回调函数
     * 
     * @return [Client](Client.md)
     */

    /**
     * 返回指定客户端连接的对端地址 [ip:port]
     * 
     * @function connPeer
     * @param idx:number 表示第几个客户端
     * @return string
     */
    JSValue Mg::connPeer(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {

        ASSERT_ARGC(1)
        ARGV_TO_UINT16(0, idx)

        struct mg_connection * conn = mgr.conns ;
        uint16_t i = idx ;
        for(; conn && i--; conn=conn->next)
        {}
        if(!conn) {
            JSTHROW("conn idx not exist: %d", idx) ;
        }

        char addr[30] ;
        mg_straddr(conn,addr,sizeof(addr)) ;

        return JS_NewString(ctx, addr) ;
    }

    /**
     * 连接到服务器的客户端数量
     * 
     * @function connCount
     * @return number
     */
    JSValue Mg::connCount(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        int cnt = 0 ;
        for(struct mg_connection * conn = mgr.conns ; conn; conn=conn->next) {
            cnt ++ ;
        }
        return JS_NewUint32(ctx, cnt) ;
    }

    /**
     * 返回当前 dns 服务器地址
     * 
     * @function getDNS
     * @return string
     */
    JSValue Mg::getDNS(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        if(mgr.dns4.url) {
            return JS_NewString(ctx,mgr.dns4.url) ;
        }
        else {
            return JS_NULL ;
        }
    }

    /**
     * 设置 dns 服务器地址
     * 
     * @function setDNS
     * @param url:string  eg: 1.1.1.1:53
     */
    JSValue Mg::setDNS(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        CHECK_ARGC(1)
        ARGV_TO_CSTRING(0, url)
        if( strlen(url)>=sizeof(dns4) ){
            JS_FreeCString(ctx,url) ;
            JSTHROW("url is too longger")
        }

        strcpy(dns4,url) ;

        JS_FreeCString(ctx,url) ;
        return JS_UNDEFINED ;
    }
    
    /**
     * 返回当前 dns 请求超时时间设定，单位毫秒
     * 
     * @function getDNSTimeout
     * @return number
     */
    JSValue Mg::getDNSTimeout(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        return JS_NewUint32(ctx,mgr.dnstimeout) ;
    }
    
    /**
     * 设置 dns 请求超时时间，单位毫秒
     * 
     * @function setDNSTimeout
     * @param ms:number
     */
    JSValue Mg::setDNSTimeout(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        CHECK_ARGC(1)
        ARGV_TO_UINT32(0, ms)
        mgr.dnstimeout = ms ;
        return JS_UNDEFINED ;
    }

    /**
     * 解析 URL 字符串，返回对象：
     * 
     * ```typescript
     * {
     *     host: string ,
     *     port: number ,
     *     uri: string
     * }
     * ```
     * 
     * @function parseUrl
     * @param url:string 要解析的 URL 字符串
     * @return object
     */
    JSValue Mg::parseUrl(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        ASSERT_ARGC(1)
        ARGV_TO_CSTRING_E(0,url,"arg url must be a string")

        JSValue obj = JS_NewObject(ctx) ;

        struct mg_str host = mg_url_host(url) ;
        JS_SetPropertyStr(ctx, obj, "host", JS_NewStringLen(ctx,host.ptr,host.len)) ;
        JS_SetPropertyStr(ctx, obj, "port", JS_NewUint32(ctx,mg_url_port(url))) ;
        JS_SetPropertyStr(ctx, obj, "uri", JS_NewString(ctx,mg_url_uri(url))) ;

        JS_FreeCString(ctx,url);

        return obj ;
    }

    /**
     * 设置 mg 的日志级别
     * 
     * @function setLog
     * @param log:string 日志级别
     * @return undefined
     */
    JSValue Mg::setLog(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        ASSERT_ARGC(1)
        ARGV_TO_CSTRING_E(0, log, "arg loglevel must be a string")

        mg_log_set(log) ;

        JS_FreeCString(ctx, log) ;
        return JS_UNDEFINED ;
    }
    
    JSValue Mg::connect(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        ASSERT_ARGC(1)
        ARGV_TO_CSTRING_LEN_E(0, url, urlLen, "arg url must be a string")
        if ( strncmp(url,"http://", 7)==0 || strncmp(url,"https://", 8)==0 || strncmp(url,"ws://", 5)==0 || strncmp(url,"wss://", 6)==0 ) {
            JS_FreeCString(ctx, url) ;
            return Client::connect(ctx, this_val, argc, argv) ;
        }
        else if ( strncmp(url,"mqtt://", 7)==0 || strncmp(url,"mqtts://", 8)==0 ) {
            JS_FreeCString(ctx, url) ;
            return MQTTClient::connect(ctx, this_val, argc, argv) ;
        }
        
        JS_FreeCString(ctx, url) ;
        JSTHROW("url not support")
    }


    /**
     * tls 需要较大 ram ，通常启用 WiFi 后剩余 ram 不足，此时会打印错误 `mg_error   4  setup err 0x7f00` 。
     * 可以尝试将 mbedtls 的内容放在 psram 中( CONFIG_MBEDTLS_EXTERNAL_MEM_ALLOC=y )
     * 
     * @function setCA
     * @param cert:ArrayBuffer TLS证书数据
     * @return undefined
     */
    JSValue Mg::setCA(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        ASSERT_ARGC(1)
        ARGV_TO_STRING(0, Mg::ca)
        return JS_UNDEFINED;
    }

    void Mg::setCA(const char * _ca) {
        ca = _ca ;
    }

    

    // DNS answer section. We response with IP 1.2.3.4 - you can change it
    //  in the last 4 bytes of this array
    uint8_t answer[] = {
        0xc0, 0x0c,          // Point to the name in the DNS question
        0,    1,             // 2 bytes - record type, A
        0,    1,             // 2 bytes - address class, INET
        0,    0,    0, 120,  // 4 bytes - TTL
        0,    4,             // 2 bytes - address length
        192,    168,    4, 1     // 4 bytes - IP address
    };

    void Mg::captivePortalHandler(struct mg_connection *c, int ev, void *ev_data, void *fn_data) {
        if (ev == MG_EV_OPEN) {
            c->is_hexdumping = 1;
        } else if (ev == MG_EV_READ) {
            struct mg_dns_rr rr;  // Parse first question, offset 12 is header size
            size_t n = mg_dns_parse_rr(c->recv.buf, c->recv.len, 12, true, &rr);
            // MG_INFO(("DNS request parsed, result=%d", (int) n));
            if (n > 0) {
            char buf[512];
            struct mg_dns_header *h = (struct mg_dns_header *) buf;
            memset(buf, 0, sizeof(buf));  // Clear the whole datagram
            h->txnid = ((struct mg_dns_header *) c->recv.buf)->txnid;  // Copy tnxid
            h->num_questions = mg_htons(1);  // We use only the 1st question
            h->num_answers = mg_htons(1);    // And only one answer
            h->flags = mg_htons(0x8400);     // Authoritative response
            memcpy(buf + sizeof(*h), c->recv.buf + sizeof(*h), n);  // Copy question
            memcpy(buf + sizeof(*h) + n, answer, sizeof(answer));   // And answer
            mg_send(c, buf, 12 + n + sizeof(answer));               // And send it!
            }
            mg_iobuf_del(&c->recv, 0, c->recv.len);
        }
        (void) fn_data;
        (void) ev_data;
    }


    JSValue Mg::startCaptivePortal(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {

        if(connCaptiveProtal) {
            JSTHROW("captive portal already started")
        }

        connCaptiveProtal = mg_listen(&mgr, "udp://0.0.0.0:53", captivePortalHandler, NULL);

        return JS_UNDEFINED ;
    }

}