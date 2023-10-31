#include "mg_response.h"
#include "module_mg.h"
#include "module_fs.h"
#include "utils.h"
#include "cutils.h"
#include "telnet_ws.h"



// =================================================
// class HttpResponse

JSClassID js_mg_http_rspn_class_id ;

#define THIS_HTTP_RSPN(var)                                                 \
    response_t * var = JS_GetOpaque(this_val, js_mg_http_rspn_class_id) ;   \
    if(!var) {                                                              \
        JS_ThrowReferenceError(ctx, "mg.HttpResponse object has free.");    \
        return JS_EXCEPTION ;                                               \
    }

#define NOT_WS_FUNC(methodName)                                                     \
    if(rspn->conn->is_websocket) {                                                  \
        THROW_EXCEPTION(methodName"() do not invoke with a websocket connection.")  \
    }

#define MUST_BE_WS_FUNC(methodName)                                                     \
    if(!rspn->conn->is_websocket) {                                                  \
        THROW_EXCEPTION(methodName"() must be invoked with a websocket connection.")  \
    }

static JSValue js_mg_http_rspn_constructor(JSContext *ctx, JSValueConst new_target, int argc, JSValueConst *argv){
    return JS_NewObjectClass(ctx, js_mg_http_rspn_class_id) ;
}
static void js_mg_http_rspn_finalizer(JSRuntime *rt, JSValue this_val){
    // printf("js_mg_http_rspn_finalizer()\n") ;
}
static JSClassDef js_mg_http_rspn_class = {
    "mg.HttpResponse",
    .finalizer = js_mg_http_rspn_finalizer,
} ;

static JSValue js_mg_rspn_reply(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    CHECK_ARGC(1)
    THIS_HTTP_RSPN(rspn)
    NOT_WS_FUNC("mg.HttpResponse.reply")
    
    int code = 200 ;
    if(argc>1) {
        if( JS_ToInt32(ctx, &code, argv[1])!=0 ){
            THROW_EXCEPTION("arg code must be a number")
        }
    }

    if( JS_IsString(argv[0]) ) {
        const char * body = JS_ToCString(ctx, argv[0]) ;

        char * header = NULL ;
        if(argc>2){
            header = JS_ToCString(ctx, argv[2]) ;
        }

        mg_http_reply(rspn->conn, code, header, body) ;

        JS_FreeCString(ctx, body) ;
        if(header) {
            JS_FreeCString(ctx, header) ;
        }
    }

    else {
        THROW_EXCEPTION("invalid type of arg body")
    }

    return JS_UNDEFINED ;
}


static JSValue js_mg_rspn_redirect(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    CHECK_ARGC(1)
    THIS_HTTP_RSPN(rspn)
    const char * url = JS_ToCString(ctx, argv[0]) ;
    
    char * header = mallocf("Location: %s\r\n", url) ;
    if(!header) {
        THROW_EXCEPTION("out of memory?") ;
    }

    mg_http_reply(rspn->conn, 302, header, "Redirection") ;

    JS_FreeCString(ctx, url) ;
    free(header) ;
    return JS_UNDEFINED ;
}

static JSValue js_mg_rspn_close(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    THIS_HTTP_RSPN(rspn)
    rspn->conn->is_closing = 1 ;
    return JS_UNDEFINED ;
}


static JSValue js_mg_rspn_serve_dir(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    CHECK_ARGC(2)
    THIS_HTTP_RSPN(rspn)
    NOT_WS_FUNC("mg.HttpResponse.replay")

    if(!qjs_instanceof(ctx, argv[0], js_mg_http_message_class_id)){
        THROW_EXCEPTION("arg req must be an instance of mg.HttpMessage")
    }
    struct mg_http_message * msg = JS_GetOpaque(argv[0], js_mg_http_message_class_id) ;

    ARGV_TO_STRING_E(1, _path, "arg path must be a string")
    char * path = vfspath_to_fs(_path) ;

    struct mg_http_serve_opts opts = {.root_dir = path};
    mg_http_serve_dir(rspn->conn, msg, &opts);

    JS_FreeCString(ctx, _path) ;
    free(path) ;

    return JS_UNDEFINED ;
}

static JSValue js_mg_http_rspn_upgrade(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    THIS_HTTP_RSPN(rspn)
    CHECK_ARGC(1)
    NOT_WS_FUNC("mg.HttpResponse.upgrade")
    if( !qjs_instanceof(ctx, argv[0], js_mg_http_message_class_id) ){
        THROW_EXCEPTION("arg rspn must be a mg.HttpMessage")
    }
    struct mg_http_message * msg = JS_GetOpaque(argv[0], js_mg_http_message_class_id) ;
    if(!msg) {
        JS_ThrowReferenceError(ctx, "mg.HttpMessage object has free.");
        return JS_EXCEPTION ;
    }
    mg_ws_upgrade(rspn->conn, msg, NULL);
    return JS_UNDEFINED ;
}

static JSValue js_mg_rspn_ws_send(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    CHECK_ARGC(1)
    THIS_HTTP_RSPN(rspn)
    MUST_BE_WS_FUNC("mg.HttpResponse.wsSend")

    if( JS_IsString(argv[0]) ){
        ARGV_TO_STRING_LEN(0, str, len)
        mg_ws_send(rspn->conn, str, len, WEBSOCKET_OP_TEXT) ;
        JS_FreeCString(ctx, str) ;
    }
    
    // if( JS_IsArray(argv[0]) ){

    // }
    // else {

    // }

    return JS_UNDEFINED ;
}

/**
 * @param string event
 * @param mg.HttpMessage req
 */
// static JSValue js_mg_rspn_as_telweb(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
//     CHECK_ARGC(2)
//     THIS_HTTP_RSPN(rspn)

//     struct mg_http_message * msg ;
//     if( JS_IsUndefined(argv[1]) || JS_IsNull(argv[1]) ) {
//         msg = NULL ;
//     }
//     else {
//         msg = JS_GetOpaque(argv[1], js_mg_http_message_class_id) ;
//         if(!msg) {
//             THROW_EXCEPTION("arg reply must be a mg.HttpRequest")    
//         }
//     }
//     ARGV_TO_STRING(0, strev)

//     int ev = mg_event_name_to_const(strev) ;
//     JS_FreeCString(ctx, strev) ;

//     bool ret = telnet_ws_response(rspn->conn, ev, msg, NULL) ;

//     return ret ? JS_TRUE: JS_FALSE ;
// }

static const JSCFunctionListEntry js_mg_http_rspn_proto_funcs[] = {
    JS_CFUNC_DEF("reply", 0, js_mg_rspn_reply),
    JS_CFUNC_DEF("redirect", 0, js_mg_rspn_redirect),
    JS_CFUNC_DEF("serveDir", 0, js_mg_rspn_serve_dir),
    JS_CFUNC_DEF("upgrade", 0, js_mg_http_rspn_upgrade),
    JS_CFUNC_DEF("wsSend", 0, js_mg_rspn_ws_send),
    JS_CFUNC_DEF("close", 0, js_mg_rspn_close),
} ;


void be_module_mg_response_init() {
    JS_NewClassID(&js_mg_http_rspn_class_id);
}
void be_module_mg_response_require(JSContext *ctx, JSValue mg, JSValue pkgShadow) {
    QJS_DEF_CLASS(mg_http_rspn, "HttpResponse", "mg.HttpResponse", JS_UNDEFINED, pkgShadow)
}