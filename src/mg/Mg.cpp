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

    const char * Mg::name = "mg" ;

    struct mg_mgr Mg::mgr ;
    std::string Mg::ca_path ;
    std::string Mg::cert_path ;
    std::string Mg::certkey_path ;

    char Mg::dns4[28] ;

    Mg::Mg(JSContext * ctx, const char * name)
        : NativeModule(ctx, name, 0)
    {
        exportClass<Server>() ;
        exportClass<Client>() ;
        exportClass<HTTPRequest>() ;
        exportClass<Response>() ;

        exportName("get") ;
        exportName("download") ;

        // exportFunction("isListening",isListening,0) ;
        // exportFunction("sntpRequest",sntpRequest,0) ;
        exportFunction("connPeer",connPeer,0) ;
        exportFunction("connCount",connCount,0) ;
        exportFunction("getDNS",getDNS,0) ;
        exportFunction("setDNS",setDNS,0) ;
        exportFunction("getDNSTimeout",getDNSTimeout,0) ;
        exportFunction("setDNSTimeout",setDNSTimeout,0) ;
        exportFunction("parseUrl",parseUrl,0) ;
        exportFunction("setLog",setLog,0) ;

        exportFunction("listenHttp",Server::listenHttp,0) ;
        exportFunction("connect",Client::connect,0) ;
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

        ca_path = FS::toVFSPath("/var/ca.pem") ;
        cert_path = FS::toVFSPath("/var/cert.pem") ;
        certkey_path = FS::toVFSPath("/var/key.pem") ;
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
        mg_mgr_poll(&mgr, 0);
    }

    // static void sntp_callback(mg_req_t * req, int64_t time) {
    //     MAKE_ARGV1( argv, JS_NewInt64(req->ctx, time) )
    //     JS_Call(req->ctx, req->callback, JS_UNDEFINED, 1, argv) ;
    //     free(argv) ;
    // }

    // static void sntp_cb(struct mg_connection *c, int ev, void *evd, void *fnd) {
    //     if (ev == MG_EV_POLL) {
    //         mg_req_t * req = (mg_req_t *)fnd ;
    //         req->poll_times++ ;
    //         if(++req->poll_times > 3000 ){
    //             sntp_callback((mg_req_t *)fnd, -1) ;
    //             c->is_closing = 1 ;
    //         }
    //     } else if (ev == MG_EV_CONNECT) {
    //         if (c->is_resolving) {
    //             sntp_callback((mg_req_t *)fnd, -2) ;
    //             c->is_closing = 1 ;
    //         }
    //         else {
    //             uint8_t buf[48] = {0};
    //             buf[0] = (0 << 6) | (4 << 3) | 3;
    //             mg_send(c, buf, sizeof(buf));
    //         }
    //     } else if (ev == MG_EV_READ) {
    //         struct timeval tv = {0, 0};
    //         if (mg_sntp_parse(c->recv.buf, c->recv.len, &tv) == 0) {
    //             mg_req_t * req = (mg_req_t *)fnd ;
    //             int64_t ms = tv.tv_sec ;
    //             ms*= 1000 ;
    //             sntp_callback((mg_req_t *)fnd, ms) ;
    //         }
    //         c->recv.len = 0;  // Clear receive buffer
    //         c->is_closing = 1 ;
    //     } else if (ev==MG_EV_ERROR) {
    //         mg_req_t * req = (mg_req_t *)fnd ;
    //         sntp_callback((mg_req_t *)fnd, -3) ;
    //         c->is_closing = 1 ;
    //     } else if (ev == MG_EV_CLOSE) {
    //         mg_req_t * req = (mg_req_t *)fnd ;
    //         JS_FreeValue(req->ctx, req->callback) ;
    //         free(req) ;
    //         req = NULL ;
    //         fnd = NULL ;
    //     }
    //     (void) evd;
    // }

    // JSValue Mg::sntpRequest(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {

    //     ASSERT_ARGC(2)

    //     ARGV_TO_CSTRING_E(0, url, "arg url must be a string")
    //     if(!JS_IsFunction(ctx, argv[1])) {
    //         JSTHROW("arg callback must be a function")
    //     }

    //     mg_req_t * req = malloc(sizeof(mg_req_t)) ;
    //     if(!req) {
    //         JSTHROW("out of memory?")
    //     }
    //     req->ctx = ctx ;
    //     req->callback = JS_DupValue(ctx,argv[1]) ;
    //     req->poll_times = 0 ;    

    //     struct mg_connection * conn = mg_sntp_connect(&mgr, url, sntp_cb, req) ;
    //     if(!conn) {
    //         sntp_callback(req, -4) ;
    //         free(req) ;
    //         JS_FreeValue(ctx,argv[1]) ;
    //     }
    //     else {
    //         // mg 内部机制 有 sntp 1小时访问一次的全局限制
    //         // 取消 mg 的 sntp 实现，在 sntp_cb() 函数中接管
    //         conn->pfn = NULL ;
    //     }

    //     if(url) {
    //         JS_FreeCString(ctx, url) ;
    //     }

    //     return JS_UNDEFINED ;
    // }

    
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


    JSValue Mg::sntpHTTPRequest(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        return JS_UNDEFINED ;
    }

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
}