#include "mg_request.h"
#include "module_mg.h"
#include "utils.h"
#include "cutils.h"

// =================================================
// class HttpMessage

JSClassID js_mg_http_message_class_id ;

#define THIS_HTTP_MSG(var)                                                                  \
    struct mg_http_message * var = JS_GetOpaque(this_val, js_mg_http_message_class_id) ;    \
    if(!var) {                                                                              \
        JS_ThrowReferenceError(ctx, "mg.HttpMessage object has free.");                     \
        return JS_EXCEPTION ;                                                               \
    }

static JSValue js_mg_http_message_constructor(JSContext *ctx, JSValueConst new_target, int argc, JSValueConst *argv){
    return JS_NewObjectClass(ctx, js_mg_http_message_class_id) ;
}
static void js_mg_http_message_finalizer(JSRuntime *rt, JSValue this_val){
    // printf("js_mg_http_message_finalizer()\n") ;
}
static JSClassDef js_mg_http_message_class = {
    "mg.HttpMessage",
    .finalizer = js_mg_http_message_finalizer,
} ;

static JSValue js_mg_http_message_method(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    THIS_HTTP_MSG(msg)
    return JS_NewStringLen(ctx, msg->method.ptr, msg->method.len) ;
}
static JSValue js_mg_http_message_uri(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    THIS_HTTP_MSG(msg)
    return JS_NewStringLen(ctx, msg->uri.ptr, msg->uri.len) ;
}
static JSValue js_mg_http_message_query(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    THIS_HTTP_MSG(msg)
    return JS_NewStringLen(ctx, msg->query.ptr, msg->query.len) ;
}
static JSValue js_mg_http_message_proto(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    THIS_HTTP_MSG(msg)
    return JS_NewStringLen(ctx, msg->proto.ptr, msg->proto.len) ;
}
static JSValue js_mg_http_message_header(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    THIS_HTTP_MSG(msg)
    CHECK_ARGC(1)
    ARGV_TO_STRING_E(0,name, "arg headerName must be a string")
    struct mg_str * val = mg_http_get_header(msg, name) ;
    JS_FreeCString(ctx, name) ;
    return JS_NewStringLen(ctx, val->ptr, val->len) ;
}
static JSValue js_mg_http_message_all_headers(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    THIS_HTTP_MSG(msg)
    JSValue jsheaders = JS_NewObject(ctx) ;
    int max = sizeof(msg->headers) / sizeof(msg->headers[0]);
    for (int i = 0; i < max && msg->headers[i].name.len > 0; i++) {
        JS_SetPropertyStr(
            ctx, jsheaders,
            msg->headers[i].name.ptr ,
            JS_NewStringLen(ctx, msg->headers[i].value.ptr, msg->headers[i].value.len)
        ) ;
    }
    return jsheaders ;
}
static JSValue js_mg_http_message_body(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    THIS_HTTP_MSG(msg)
    return JS_NewStringLen(ctx, msg->body.ptr, msg->body.len) ;
}
static JSValue js_mg_http_message_chunk(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    THIS_HTTP_MSG(msg)
    return JS_NewStringLen(ctx, msg->chunk.ptr, msg->chunk.len) ;
}
static JSValue js_mg_http_message_raw(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    THIS_HTTP_MSG(msg)
    return JS_NewStringLen(ctx, msg->message.ptr, msg->message.len) ;
}
static JSValue js_mg_http_message_rawHead(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    THIS_HTTP_MSG(msg)
    return JS_NewStringLen(ctx, msg->head.ptr, msg->head.len) ;
}
static JSValue js_mg_http_message_match_uri(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    CHECK_ARGC(1)
    THIS_HTTP_MSG(msg)
    ARGV_TO_STRING(0, uri) ;
    bool ret = mg_http_match_uri(msg, uri) ;
    JS_FreeCString(ctx, uri) ;
    return ret? JS_TRUE: JS_FALSE ;
}

static const JSCFunctionListEntry js_mg_http_message_proto_funcs[] = {
    JS_CFUNC_DEF("method", 0, js_mg_http_message_method),
    JS_CFUNC_DEF("uri", 0, js_mg_http_message_uri),
    JS_CFUNC_DEF("query", 0, js_mg_http_message_query),
    JS_CFUNC_DEF("proto", 0, js_mg_http_message_proto),
    JS_CFUNC_DEF("header", 0, js_mg_http_message_header),
    JS_CFUNC_DEF("allHeaders", 0, js_mg_http_message_all_headers),
    JS_CFUNC_DEF("body", 0, js_mg_http_message_body),
    JS_CFUNC_DEF("chunk", 0, js_mg_http_message_chunk),
    JS_CFUNC_DEF("raw", 0, js_mg_http_message_raw),
    JS_CFUNC_DEF("rawHead", 0, js_mg_http_message_rawHead),
    JS_CFUNC_DEF("matchURI", 0, js_mg_http_message_match_uri),
} ;



void be_module_mg_request_init() {
    JS_NewClassID(&js_mg_http_message_class_id);
}
void be_module_mg_request_require(JSContext *ctx, JSValue mg, JSValue pkgShadow) {
    QJS_DEF_CLASS(mg_http_message, "HttpMessage", "mg.HttpMessage", JS_UNDEFINED, pkgShadow)
}
