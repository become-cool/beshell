#include "HTTPRequest.hpp"

using namespace std ;

namespace be::mg {
    DEFINE_NCLASS_META(HTTPRequest, NativeClass)
    std::vector<JSCFunctionListEntry> HTTPRequest::methods = {
        JS_CFUNC_DEF("method", 0, HTTPRequest::method),
        JS_CFUNC_DEF("uri", 0, HTTPRequest::uri),
        JS_CFUNC_DEF("query", 0, HTTPRequest::query),
        JS_CFUNC_DEF("proto", 0, HTTPRequest::proto),
        JS_CFUNC_DEF("header", 0, HTTPRequest::header),
        JS_CFUNC_DEF("all_headers", 0, HTTPRequest::all_headers),
        JS_CFUNC_DEF("body", 0, HTTPRequest::body),
        JS_CFUNC_DEF("chunk", 0, HTTPRequest::chunk),
        JS_CFUNC_DEF("raw", 0, HTTPRequest::raw),
        JS_CFUNC_DEF("rawHead", 0, HTTPRequest::rawHead),
        JS_CFUNC_DEF("match_uri", 0, HTTPRequest::match_uri),
    } ;

    HTTPRequest::HTTPRequest(JSContext * ctx, struct mg_http_message * mg_msg)
        : NativeClass(ctx,build(ctx))
        , mg_msg(mg_msg)
    {}

    struct mg_http_message * HTTPRequest::opaque() {
        return mg_msg ;
    }

    JSValue HTTPRequest::method(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        THIS_NCLASS(HTTPRequest,req)
        return JS_NewStringLen(ctx, (const char*)req->mg_msg->method.ptr, (size_t)req->mg_msg->method.len) ;
    }
    JSValue HTTPRequest::uri(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        THIS_NCLASS(HTTPRequest,req)
        return JS_NewStringLen(ctx, (const char*)req->mg_msg->uri.ptr, (size_t)req->mg_msg->uri.len) ;
    }
    JSValue HTTPRequest::query(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        THIS_NCLASS(HTTPRequest,req)
        return JS_NewStringLen(ctx, (const char*)req->mg_msg->query.ptr, (size_t)req->mg_msg->query.len) ;
    }
    JSValue HTTPRequest::proto(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        THIS_NCLASS(HTTPRequest,req)
        return JS_NewStringLen(ctx, (const char*)req->mg_msg->proto.ptr, (size_t)req->mg_msg->proto.len) ;
    }
    JSValue HTTPRequest::header(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        THIS_NCLASS(HTTPRequest,req)
        ASSERT_ARGC(1)
        ARGV_TO_CSTRING_E(0,name, "arg headerName must be a string")
        struct mg_str * val = mg_http_get_header(req->mg_msg, name) ;
        JS_FreeCString(ctx, name) ;
        return JS_NewStringLen(ctx, val->ptr, val->len) ;
    }
    JSValue HTTPRequest::all_headers(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        THIS_NCLASS(HTTPRequest,req)
        JSValue jsheaders = JS_NewObject(ctx) ;
        int max = sizeof(req->mg_msg->headers) / sizeof(req->mg_msg->headers[0]);
        for (int i = 0; i < max && req->mg_msg->headers[i].name.len > 0; i++) {
            JS_SetPropertyStr(
                ctx, jsheaders,
                req->mg_msg->headers[i].name.ptr ,
                JS_NewStringLen(ctx, req->mg_msg->headers[i].value.ptr, req->mg_msg->headers[i].value.len)
            ) ;
        }
        return jsheaders ;
    }
    JSValue HTTPRequest::body(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        THIS_NCLASS(HTTPRequest,req)
        return JS_NewStringLen(ctx, req->mg_msg->body.ptr, req->mg_msg->body.len) ;
    }
    JSValue HTTPRequest::chunk(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        THIS_NCLASS(HTTPRequest,req)
        return JS_NewStringLen(ctx, req->mg_msg->chunk.ptr, req->mg_msg->chunk.len) ;
    }
    JSValue HTTPRequest::raw(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        THIS_NCLASS(HTTPRequest,req)
        return JS_NewStringLen(ctx, req->mg_msg->message.ptr, req->mg_msg->message.len) ;
    }
    JSValue HTTPRequest::rawHead(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        THIS_NCLASS(HTTPRequest,req)
        return JS_NewStringLen(ctx, req->mg_msg->head.ptr, req->mg_msg->head.len) ;
    }
    JSValue HTTPRequest::match_uri(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        ASSERT_ARGC(1)
        THIS_NCLASS(HTTPRequest,req)
        ARGV_TO_CSTRING(0, uri) ;
        bool ret = mg_http_match_uri(req->mg_msg, uri) ;
        JS_FreeCString(ctx, uri) ;
        return ret? JS_TRUE: JS_FALSE ;
    }
}