#include "Response.hpp"
#include "HTTPRequest.hpp"
#include "mallocf.h"
#include <fs/FS.hpp>

using namespace std ;



#define NOT_WS_FUNC(methodName)                                                 \
    if(rspn->conn->is_websocket) {                                              \
        JSTHROW(methodName"() do not invoke with a websocket connection.")      \
    }

#define MUST_BE_WS_FUNC(methodName)                                             \
    if(!rspn->conn->is_websocket) {                                             \
        JSTHROW(methodName"() must be invoked with a websocket connection.")    \
    }


namespace be::mg {
    DEFINE_NCLASS_META(Response, NativeClass)
    std::vector<JSCFunctionListEntry> Response::methods = {
        JS_CFUNC_DEF("wsSend", 0, Response::wsSend),
        JS_CFUNC_DEF("httpUpgrade", 0, Response::httpUpgrade),
        JS_CFUNC_DEF("serveDir", 0, Response::serveDir),
        JS_CFUNC_DEF("close", 0, Response::close),
        JS_CFUNC_DEF("redirect", 0, Response::redirect),
        JS_CFUNC_DEF("reply", 0, Response::reply),
    } ;

    Response::Response(JSContext * ctx, struct mg_connection * conn)
        : NativeClass(ctx,build(ctx))
        , conn(conn)
    {}

    JSValue Response::reply(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        CHECK_ARGC(1)
        THIS_NCLASS(Response, rspn)
        NOT_WS_FUNC("mg.HttpResponse.reply")
        
        int32_t code = 200 ;
        if(argc>1) {
            if( JS_ToInt32(ctx, &code, argv[1])!=0 ){
                JSTHROW("arg code must be a number")
            }
        }

        if( JS_IsString(argv[0]) ) {
            const char * body = JS_ToCString(ctx, argv[0]) ;

            char * header = NULL ;
            if(argc>2){
                header = (char *)JS_ToCString(ctx, argv[2]) ;
            }

            mg_http_reply(rspn->conn, code, header, body) ;

            JS_FreeCString(ctx, body) ;
            if(header) {
                JS_FreeCString(ctx, header) ;
            }
        }

        else {
            JSTHROW("invalid type of arg body")
        }

        return JS_UNDEFINED ;
    }


    JSValue Response::redirect(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        CHECK_ARGC(1)
        THIS_NCLASS(Response, rspn)
        const char * url = JS_ToCString(ctx, argv[0]) ;
        
        char * header = mallocf("Location: %s\r\n", (char*)url) ;
        if(!header) {
            JSTHROW("out of memory?") ;
        }

        mg_http_reply(rspn->conn, 302, header, "Redirection") ;

        JS_FreeCString(ctx, url) ;
        free(header) ;
        return JS_UNDEFINED ;
    }

    JSValue Response::close(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        THIS_NCLASS(Response, rspn)
        rspn->conn->is_closing = 1 ;
        return JS_UNDEFINED ;
    }


    JSValue Response::serveDir(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        CHECK_ARGC(2)
        THIS_NCLASS(Response, rspn)
        NOT_WS_FUNC("mg.HttpResponse.replay")

        JSVALUE_TO_NCLASS(HTTPRequest,argv[0],req)

        ARGV_TO_CSTRING_E(1, _path, "arg path must be a string")
        string path = FS::toVFSPath(ctx, _path) ;

        struct mg_http_serve_opts opts = {.root_dir = path.c_str()};
        mg_http_serve_dir(rspn->conn, req->opaque(), &opts);

        JS_FreeCString(ctx, _path) ;

        return JS_UNDEFINED ;
    }

    JSValue Response::httpUpgrade(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        THIS_NCLASS(Response, rspn)
        CHECK_ARGC(1)
        NOT_WS_FUNC("mg.HttpResponse.upgrade")
        JSVALUE_TO_NCLASS(HTTPRequest,argv[0],req)
        struct mg_http_message * msg = req->opaque() ;
        if(!msg) {
            JS_ThrowReferenceError(ctx, "mg.HttpMessage object has free.");
            return JS_EXCEPTION ;
        }
        mg_ws_upgrade(rspn->conn, msg, NULL);
        return JS_UNDEFINED ;
    }

    JSValue Response::wsSend(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        CHECK_ARGC(1)
        THIS_NCLASS(Response, rspn)
        MUST_BE_WS_FUNC("mg.HttpResponse.wsSend")

        if( JS_IsString(argv[0]) ){
            ARGV_TO_CSTRING_LEN(0, str, len)
            mg_ws_send(rspn->conn, str, len, WEBSOCKET_OP_TEXT) ;
            JS_FreeCString(ctx, str) ;
        }
        
        // if( JS_IsArray(argv[0]) ){

        // }
        // else {

        // }

        return JS_UNDEFINED ;
    }
}