#pragma once

#include <NativeClass.hpp>
#include "deps/mongoose/mongoose.h"

namespace be::mg {
    class Server: public ::be::NativeClass {
        DECLARE_NCLASS_META
    private:
        static std::vector<JSCFunctionListEntry> methods ;

        struct mg_connection * conn ;
        JSValue callback ;
        bool telweb: 1 ;
        bool ssl: 1;
        void * ssl_cert_path;
        void * ssl_key_path;

        static void eventHandler(struct mg_connection * conn, int ev, void *ev_data, void *fn_data) ;

    public:
        Server(JSContext * ctx, struct mg_connection * conn, JSValue callback=JS_NULL) ;
        ~Server() ;
        
        static JSValue close(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue startTelweb(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue stopTelweb(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue setHandler(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        
        static JSValue listenHttp(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
    } ;
} 