#pragma once

#include <EventEmitter.hpp>
#include "deps/mongoose/mongoose.h"

namespace be::mg {
    
    class Client ;
    
    typedef bool (*HTTPClientHandler) (Client * client, struct mg_connection *, int ev, void *ev_data, void *fn_data) ;

    class Client: public be::EventEmitter {
        DECLARE_NCLASS_META
    private:
        static std::vector<JSCFunctionListEntry> methods ;

        JSValue callback = JS_NULL ;
        struct mg_connection * conn = NULL ;
        std::string _host = ""; // 用于 tls
        bool is_ws: 1 = false ;
        bool is_tls:1  = false ;
        bool is_connected = false ;
        bool _enableChunkEvent:1  = false ;
        size_t headerLength = 0 ;
        size_t receivedBodyLength = 0 ;

        static void eventHandler(struct mg_connection * conn, int ev, void *ev_data) ;
        static void wsEventHandler(struct mg_connection * conn, int ev, void *ev_data) ;
    
        static HTTPClientHandler handler ;
    public:
        Client(JSContext * ctx, struct mg_connection *, JSValue callback) ;
        ~Client() ;
        void setConn(struct mg_connection * conn) ;
        
        static JSValue send(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue close(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        
        static JSValue connect(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue connectWS(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue isConnected(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;

        static JSValue enableChunkEvent(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
    
        static void setHandler(HTTPClientHandler handler) ;
        
        bool isWS() const ;
        bool isTLS() const ;
        std::string host() const ;
    
    } ;
}