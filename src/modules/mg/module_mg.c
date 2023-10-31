#include "module_mg.h"
#include "utils.h"
#include "cutils.h"
#include "stack.h"
#include "mg_client.h"
#include "mg_server.h"
#include "mg_request.h"
#include "mg_response.h"


struct mg_mgr mgr ;

JSValue pkgShadow ;

const char * mg_event_const_to_name(int ev) {
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

int mg_event_name_to_const(const char * evname) {
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

bool mg_url_is_listening(const char * url) {
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





static void sntp_callback(mg_req_t * req, int64_t time) {
    MAKE_ARGV1( argv, JS_NewInt64(req->ctx, time) )
    JS_Call(req->ctx, req->callback, JS_UNDEFINED, 1, argv) ;
    free(argv) ;
}

static void sntp_cb(struct mg_connection *c, int ev, void *evd, void *fnd) {
    if (ev == MG_EV_POLL) {
        mg_req_t * req = (mg_req_t *)fnd ;
        req->poll_times++ ;
        if(++req->poll_times > 3000 ){
            sntp_callback((mg_req_t *)fnd, -1) ;
            c->is_closing = 1 ;
        }
    } else if (ev == MG_EV_CONNECT) {
        if (c->is_resolving) {
            sntp_callback((mg_req_t *)fnd, -2) ;
            c->is_closing = 1 ;
        }
        else {
            uint8_t buf[48] = {0};
            buf[0] = (0 << 6) | (4 << 3) | 3;
            mg_send(c, buf, sizeof(buf));
        }
    } else if (ev == MG_EV_READ) {
        struct timeval tv = {0, 0};
        if (mg_sntp_parse(c->recv.buf, c->recv.len, &tv) == 0) {
            mg_req_t * req = (mg_req_t *)fnd ;
            int64_t ms = tv.tv_sec ;
            ms*= 1000 ;
            sntp_callback((mg_req_t *)fnd, ms) ;
        }
        c->recv.len = 0;  // Clear receive buffer
        c->is_closing = 1 ;
    } else if (ev==MG_EV_ERROR) {
        mg_req_t * req = (mg_req_t *)fnd ;
        sntp_callback((mg_req_t *)fnd, -3) ;
        c->is_closing = 1 ;
    } else if (ev == MG_EV_CLOSE) {
        mg_req_t * req = (mg_req_t *)fnd ;
        JS_FreeValue(req->ctx, req->callback) ;
        free(req) ;
        req = NULL ;
        fnd = NULL ;
    }
    (void) evd;
}

static JSValue js_mg_sntp_request(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {

    CHECK_ARGC(2)

    ARGV_TO_STRING_E(0, url, "arg url must be a string")
    if(!JS_IsFunction(ctx, argv[1])) {
        THROW_EXCEPTION("arg callback must be a function")
    }

    mg_req_t * req = malloc(sizeof(mg_req_t)) ;
    if(!req) {
        THROW_EXCEPTION("out of memory?")
    }
    req->ctx = ctx ;
    req->callback = JS_DupValue(ctx,argv[1]) ;
    req->poll_times = 0 ;    

    struct mg_connection * conn = mg_sntp_connect(&mgr, url, sntp_cb, req) ;
    if(!conn) {
        sntp_callback(req, -4) ;
        free(req) ;
        JS_FreeValue(ctx,argv[1]) ;
    }
    else {
        // mg 内部机制 有 sntp 1小时访问一次的全局限制
        // 取消 mg 的 sntp 实现，在 sntp_cb() 函数中接管
        conn->pfn = NULL ;
    }

    if(url) {
        JS_FreeCString(ctx, url) ;
    }

    return JS_UNDEFINED ;
}

static JSValue js_mg_conn_peer(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {

    CHECK_ARGC(1)
    ARGV_TO_UINT16(0, idx)

    struct mg_connection * conn = mgr.conns ;
    uint16_t i = idx ;
    for(; conn && i--; conn=conn->next)
    {}
    if(!conn) {
        THROW_EXCEPTION("conn idx not exist: %d", idx) ;
    }

    char addr[30] ;
    mg_straddr(conn,addr,sizeof(addr)) ;

    return JS_NewString(ctx, addr) ;
}

static JSValue js_mg_conn_cnt(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    int cnt = 0 ;
    for(struct mg_connection * conn = mgr.conns ; conn; conn=conn->next) {
        cnt ++ ;
    }
    return JS_NewUint32(ctx, cnt) ;
}


static JSValue js_mg_get_dns(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    return JS_NewString(ctx,mgr.dns4.url) ;
}

static JSValue js_mg_parse_url(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    CHECK_ARGC(1)
    ARGV_TO_STRING_E(0,url,"arg url must be a string")

    JSValue obj = JS_NewObject(ctx) ;

    struct mg_str host = mg_url_host(url) ;
    JS_SetPropertyStr(ctx, obj, "host", JS_NewStringLen(ctx,host.ptr,host.len)) ;
    JS_SetPropertyStr(ctx, obj, "port", JS_NewUint32(ctx,mg_url_port(url))) ;
    JS_SetPropertyStr(ctx, obj, "uri", JS_NewString(ctx,mg_url_uri(url))) ;

    JS_FreeCString(ctx,url);

    return obj ;
}

static JSValue js_mg_set_log(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    CHECK_ARGC(1)
    ARGV_TO_STRING_E(0, log, "arg loglevel must be a string")

    mg_log_set(log) ;

    JS_FreeCString(ctx, log) ;
    return JS_UNDEFINED ;
}

void be_module_mg_init() {
    mg_mgr_init(&mgr) ;
    // mgr.dns4.url = "udp://1.1.1.1:53";

    pkgShadow = JS_UNDEFINED ;

    mg_log_set("1") ;

    be_module_mg_request_init() ;
    be_module_mg_response_init() ;
    be_module_mg_client_init() ;
    be_module_mg_server_init() ;
}

void be_module_mg_require(JSContext *ctx) {
    JSValue beapi = js_get_glob_prop(ctx, 1, "beapi") ;
    JSValue mg = JS_NewObject(ctx);
    JS_SetPropertyStr(ctx, beapi, "mg", mg);

    pkgShadow = JS_NewObject(ctx);

    JS_SetPropertyStr(ctx, mg, "sntpRequest", JS_NewCFunction(ctx, js_mg_sntp_request, "sntpRequest", 1));
    JS_SetPropertyStr(ctx, mg, "connCnt", JS_NewCFunction(ctx, js_mg_conn_cnt, "connCnt", 1));
    JS_SetPropertyStr(ctx, mg, "connPeer", JS_NewCFunction(ctx, js_mg_conn_peer, "connPeer", 1));
    JS_SetPropertyStr(ctx, mg, "getDNS", JS_NewCFunction(ctx, js_mg_get_dns, "getDNS", 1));
    JS_SetPropertyStr(ctx, mg, "setLog", JS_NewCFunction(ctx, js_mg_set_log, "setLog", 1));
    JS_SetPropertyStr(ctx, mg, "parseUrl", JS_NewCFunction(ctx, js_mg_parse_url, "parseUrl", 1));

    be_module_mg_request_require(ctx, mg, pkgShadow) ;
    be_module_mg_response_require(ctx, mg, pkgShadow) ;
    be_module_mg_client_require(ctx, mg, pkgShadow) ;
    be_module_mg_server_require(ctx, mg, pkgShadow) ;
    
    // JS_FreeValue(ctx, mg);
    JS_FreeValue(ctx, beapi);
}

inline void be_module_mg_loop(JSContext *ctx) {
    mgr.userdata = ctx ;
    mg_mgr_poll(&mgr, 0);
}

void be_module_mg_reset(JSContext *ctx) {
    JS_FreeValue(ctx, pkgShadow) ;
    // assert(VAR_REFCNT(pkgShadow)==0) ;

    // @todo
    // 关闭所有的 be_http_server_t.conn
}


struct mg_mgr * be_module_mg_mgr() {
    return & mgr ;
}