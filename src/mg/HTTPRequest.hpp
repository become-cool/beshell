#pragma once

#include <NativeClass.hpp>
#include "deps/mongoose/mongoose.h"

namespace be::mg {
    class HTTPRequest: public be::NativeClass {
        DECLARE_NCLASS_META
    private:
        static std::vector<JSCFunctionListEntry> methods ;
        
        struct mg_http_message * mg_msg ;

    public:
        HTTPRequest(JSContext * ctx, struct mg_http_message * mg_msg) ;

        struct mg_http_message * opaque() ;
    
        static JSValue method(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue uri(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue query(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue proto(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue header(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue all_headers(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue body(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue chunk(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue raw(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue rawHead(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue match_uri(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;

    
    } ;
}