/**
 * Response 类用于 mg.Server 回调函数的参数 rspn , 不需要创建该类: 
 * 
 * * [mg.listenHttp()](../mg/#%E5%87%BD%E6%95%B0-listenhttp)
 * 
 * * [mg.Server.setHandler()](Server.html#%E6%96%B9%E6%B3%95-sethandler)
 * 
 * 
 * 
 * @class Response
 */

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

    /**
     * 回复内容
     * 
     * @method reply
     * @param content:string 回复内容
     * @param code:number=200 回复状态码, 默认为 200
     * @param header:string=null 回复头, 格式为 "key: value\r\n"
     * @return undefined
     */
    JSValue Response::reply(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        ASSERT_ARGC(1)
        THIS_NCLASS(Response, rspn)
        NOT_WS_FUNC("mg.HTTPResponse.reply")
        
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


    /**
     * 重定向请求
     * 
     * @method redirect
     * @param url:string 重定向链接地址
     * @return undefined
     */
    JSValue Response::redirect(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        ASSERT_ARGC(1)
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

    /**
     * 服务器主动关闭访问连接
     * 
     * @method close
     * @return undefined
     */
    JSValue Response::close(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        THIS_NCLASS(Response, rspn)
        rspn->conn->is_closing = 1 ;
        return JS_UNDEFINED ;
    }


    /**
     * 执行文件服务
     * 
     * 该方法用于实现静态文件服务器，用法请参考：[简单 HTTP Web 后端的例子](../../guide/http-server.html#_2-%E7%AE%80%E5%8D%95-http-web-%E5%90%8E%E7%AB%AF%E7%9A%84%E4%BE%8B%E5%AD%90)
     * 
     * @method serveDir
     * @param req:[HTTPRequest](HTTPRequest.md) 请求对象，将事件回调函数传入的 req 直接传递给 serveDir 即可
     * @param dir:string 文件根目录
     * @return undefined
     */
    JSValue Response::serveDir(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        ASSERT_ARGC(2)
        THIS_NCLASS(Response, rspn)
        NOT_WS_FUNC("mg.HTTPResponse.replay")

        JSVALUE_TO_NCLASS(HTTPRequest,argv[0],req)

        ARGV_TO_CSTRING_E(1, _path, "arg path must be a string")
        string path = FS::toVFSPath(_path) ;

        struct mg_http_serve_opts opts = {.root_dir = path.c_str()};
        mg_http_serve_dir(rspn->conn, req->opaque(), &opts);

        JS_FreeCString(ctx, _path) ;

        return JS_UNDEFINED ;
    }

    /**
     * http 协议升级
     * 
     * 将 http 请求升级为 WebSocket 通讯，用法请参考：[WebSocket 的例子](../../guide/http-server.html#_3-websocket-%E7%9A%84%E4%BE%8B%E5%AD%90)
     * 
     * @method httpUpgrade
     * @param req:[HTTPRequest](HTTPRequest.md) 请求对象，将事件回调函数传入的 req 直接传递给 serveDir 即可
     * @return undefined
     */
    JSValue Response::httpUpgrade(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        THIS_NCLASS(Response, rspn)
        ASSERT_ARGC(1)
        NOT_WS_FUNC("mg.HTTPResponse.upgrade")
        JSVALUE_TO_NCLASS(HTTPRequest,argv[0],req)
        struct mg_http_message * msg = req->opaque() ;
        if(!msg) {
            JS_ThrowReferenceError(ctx, "mg.HttpMessage object has free.");
            return JS_EXCEPTION ;
        }
        mg_ws_upgrade(rspn->conn, msg, NULL);
        return JS_UNDEFINED ;
    }

    /**
     * 发送 WebSocket 数据帧
     * 
     * 用法请参考：[WebSocket 的例子](../../guide/http-server.html#_3-websocket-%E7%9A%84%E4%BE%8B%E5%AD%90)
     * 
     * @method wsSend
     * @param data:string 数据帧内容
     * @return undefined
     */
    JSValue Response::wsSend(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        ASSERT_ARGC(1)
        THIS_NCLASS(Response, rspn)
        MUST_BE_WS_FUNC("mg.HTTPResponse.wsSend")

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