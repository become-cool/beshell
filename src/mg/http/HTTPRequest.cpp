/**
 * HTTPRequest 类用于 mg.Server 回调函数的参数 req , 不需要创建该类: 
 * 
 * * [mg.listenHttp()](../mg/#%E5%87%BD%E6%95%B0-listenhttp)
 * 
 * * [mg.connect()](../mg/#%E5%87%BD%E6%95%B0-connect)
 * 
 * * [mg.Server.setHandler()](Server.html#%E6%96%B9%E6%B3%95-sethandler)
 * 
 * 
 * 
 * @class HTTPRequest
 */
#include "./HTTPRequest.hpp"

using namespace std ;

namespace be::mg {
    DEFINE_NCLASS_META(HTTPRequest, NativeClass)
    std::vector<JSCFunctionListEntry> HTTPRequest::methods = {
        JS_CFUNC_DEF("method", 0, HTTPRequest::method),
        JS_CFUNC_DEF("uri", 0, HTTPRequest::uri),
        JS_CFUNC_DEF("query", 0, HTTPRequest::query),
        JS_CFUNC_DEF("proto", 0, HTTPRequest::proto),
        JS_CFUNC_DEF("header", 0, HTTPRequest::header),
        JS_CFUNC_DEF("allHeaders", 0, HTTPRequest::all_headers),
        JS_CFUNC_DEF("body", 0, HTTPRequest::body),
        JS_CFUNC_DEF("bodyLength", 0, HTTPRequest::bodyLength),
        JS_CFUNC_DEF("chunk", 0, HTTPRequest::chunk),
        JS_CFUNC_DEF("chunkLength", 0, HTTPRequest::chunkLength),
        JS_CFUNC_DEF("raw", 0, HTTPRequest::raw),
        JS_CFUNC_DEF("rawHead", 0, HTTPRequest::rawHead),
        JS_CFUNC_DEF("matchURI", 0, HTTPRequest::match_uri),
        JS_CFUNC_DEF("isCaptivePortalRequest", 0, HTTPRequest::isCaptivePortalRequest),
    } ;

    HTTPRequest::HTTPRequest(JSContext * ctx, struct mg_http_message * mg_msg)
        : NativeClass(ctx,build(ctx))
        , mg_msg(mg_msg)
    {}

    struct mg_http_message * HTTPRequest::opaque() {
        return mg_msg ;
    }

    /**
     * 返回 http 请求的方法
     * 
     * "GET", "POST", "PUT", "DELETE", "HEAD", "OPTIONS", "TRACE", "CONNECT" 等
     * 
     * @method method
     * @return string
     */
    JSValue HTTPRequest::method(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        THIS_NCLASS(HTTPRequest,req)
        return JS_NewStringLen(ctx, (const char*)req->mg_msg->method.buf, (size_t)req->mg_msg->method.len) ;
    }
    
    /**
     * 返回 http 请求的 uri
     * 
     * @method uri
     * @return string
     */
    JSValue HTTPRequest::uri(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        THIS_NCLASS(HTTPRequest,req)
        return JS_NewStringLen(ctx, (const char*)req->mg_msg->uri.buf, (size_t)req->mg_msg->uri.len) ;
    }
    
    /**
     * 返回 http 请求的 query 部分
     * 
     * @method query
     * @return string
     */
    JSValue HTTPRequest::query(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        THIS_NCLASS(HTTPRequest,req)
        return JS_NewStringLen(ctx, (const char*)req->mg_msg->query.buf, (size_t)req->mg_msg->query.len) ;
    }

    /**
     * 返回 http 请求的协议
     * 
     * "http", "https", "ws", "wss" 等
     * 
     * @method proto
     * @return string
     */
    JSValue HTTPRequest::proto(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        THIS_NCLASS(HTTPRequest,req)
        return JS_NewStringLen(ctx, (const char*)req->mg_msg->proto.buf, (size_t)req->mg_msg->proto.len) ;
    }

    /**
     * 返回 http 请求的 header 内容
     * 
     * @method header
     * @param name:string header的名称，如 "Content-Type", "Content-Length", "Host" 等
     * @return string
     */
    JSValue HTTPRequest::header(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        THIS_NCLASS(HTTPRequest,req)
        ASSERT_ARGC(1)
        ARGV_TO_CSTRING_E(0,name, "arg headerName must be a string")
        struct mg_str * val = mg_http_get_header(req->mg_msg, name) ;
        JS_FreeCString(ctx, name) ;
        return JS_NewStringLen(ctx, val->buf, val->len) ;
    }

    /**
     * 返回 http 请求的所有 header 内容
     * 
     * @method allHeaders
     * @return object
     */
    JSValue HTTPRequest::all_headers(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        THIS_NCLASS(HTTPRequest,req)
        JSValue jsheaders = JS_NewObject(ctx) ;
        int max = sizeof(req->mg_msg->headers) / sizeof(req->mg_msg->headers[0]);
        for (int i = 0; i < max && req->mg_msg->headers[i].name.len > 0; i++) {
            JS_SetPropertyStr(
                ctx, jsheaders,
                req->mg_msg->headers[i].name.buf ,
                JS_NewStringLen(ctx, req->mg_msg->headers[i].value.buf, req->mg_msg->headers[i].value.len)
            ) ;
        }
        return jsheaders ;
    }
    
    /**
     * 返回 http 请求的 body 内容
     * 
     * @method body
     * @return ArrayBuffer
     */
    JSValue HTTPRequest::body(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        THIS_NCLASS(HTTPRequest,req)
        return JS_NewArrayBufferCopy(ctx, (const uint8_t*)req->mg_msg->body.buf, req->mg_msg->body.len) ;
    }
    
    /**
     * 返回 http 请求的 body 数据长度
     * 
     * @method bodyLength
     * @return number
     */
    JSValue HTTPRequest::bodyLength(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        THIS_NCLASS(HTTPRequest,req)
        return JS_NewUint32(ctx, req->mg_msg->body.len) ;
    }

    /**
     * 返回 http 请求的 body 分段数据内容
     * 
     * 所有数据接收完毕后，触发 `http.msg` 事件。
     * 
     * @method chunk
     * @return ArrayBuffer
     */
    JSValue HTTPRequest::chunk(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        
        JSTHROW("not support chunk()") ;
        
        // THIS_NCLASS(HTTPRequest,req)
        // return JS_NewArrayBufferCopy(ctx, (const uint8_t*)req->mg_msg->chunk.buf, req->mg_msg->chunk.len) ;
        // return JS_NULL ;
    }

    
    /**
     * 返回 http 请求的 body 分段数据长度
     * 
     * > 参考 chunk() 方法
     * 
     * @method chunkLength
     * @return number
     */
    JSValue HTTPRequest::chunkLength(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        
        JSTHROW("not support chunkLength()") ;
        
        // THIS_NCLASS(HTTPRequest,req)
        // return JS_NewUint32(ctx, req->mg_msg->chunk.len) ;
        // return JS_NewUint32(ctx, 0) ;
    }

    
    /**
     * 返回 http 请求的原始数据内容
     * 
     * @method raw
     * @return string
     */
    JSValue HTTPRequest::raw(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        THIS_NCLASS(HTTPRequest,req)
        return JS_NewStringLen(ctx, req->mg_msg->message.buf, req->mg_msg->message.len) ;
    }
    
    /**
     * 返回 http 请求原始的 header 内容
     * 
     * @method rawHead
     * @return string
     */
    JSValue HTTPRequest::rawHead(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        THIS_NCLASS(HTTPRequest,req)
        return JS_NewStringLen(ctx, req->mg_msg->head.buf, req->mg_msg->head.len) ;
    }
    
    /**
     * 由 mg 提供的 uri 匹配检查函数
     * 
     * 该函数是对 Mongoose mg_http_match_uri api 的封装
     * 
     * @method matchURI
     * @param pattern:string uri匹配模式
     * @return bool
     */
    JSValue HTTPRequest::match_uri(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        ASSERT_ARGC(1)
        THIS_NCLASS(HTTPRequest,req)
        ARGV_TO_CSTRING(0, uri) ;

        bool ret = mg_match(req->mg_msg->uri, mg_str(uri),NULL) ;
        JS_FreeCString(ctx, uri) ;
        return ret? JS_TRUE: JS_FALSE ;
    }
    
    
    // www.msftconnecttest.com
    // ipv6.msftconnecttest.com
    // http://captive.apple.com/hotspot-detect.html
    // http://connectivitycheck.android.com/generate_204
    // http://connectivitycheck.smartisan.com/wifi.html
    // ****/generate_204
    
    /**
     * 检查是否是 Captive Portal 请求
     * 
     * @method isCaptivePortalRequest
     * @return bool
     */
    JSValue HTTPRequest::isCaptivePortalRequest(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        THIS_NCLASS(HTTPRequest,req)

        struct mg_str * host = mg_http_get_header(req->mg_msg, "Host") ;
        if(!host) {
            printf("unknow host for captive_portal\n") ;
            return JS_FALSE ;
        }
        printf("%.*s\n",host->len,host->buf) ;
    
        if(mg_strcmp(*host, mg_str("captive.apple.com"))==0) {
            return JS_TRUE ;
        }
        // http://connectivitycheck.android.com/generate_204
        // http://connectivitycheck.smartisan.com/wifi.html
        else if( host->len>=18 && strncmp(host->buf, "connectivitycheck.", 18)==0 ) {
            return JS_TRUE ;
        }
        else if(mg_match(req->mg_msg->uri,mg_str("/generate_204"), NULL)) {
            return JS_TRUE ;
        }
        else if(mg_strcmp(*host, mg_str("www.msftconnecttest.com"))==0) {
            return JS_TRUE ;
        }
    
        // printf("%.*s/%.*s\n", host->len, host->buf, hm->uri.len, hm->uri.buf) ;
    

        return JS_FALSE ;
    }
}