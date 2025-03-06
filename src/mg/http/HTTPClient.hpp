#pragma once

#include <EventEmitter.hpp>
#include "deps/mongoose/mongoose.h"

namespace be::mg {
    
    class Client: public be::EventEmitter {
        DECLARE_NCLASS_META
    private:
        static std::vector<JSCFunctionListEntry> methods ;
        
        struct mg_connection * conn ;
        JSValue callback ;
        uint16_t poll_times = 0 ;
        bool is_ws = false ;
        bool is_tls = false ;

        static void eventHandler(struct mg_connection * conn, int ev, void *ev_data, void *fnd) ;
        static void wsEventHandler(struct mg_connection * conn, int ev, void *ev_data, void *fnd) ;
    public:
        Client(JSContext * ctx, struct mg_connection *, JSValue callback) ;
        ~Client() ;
        void setConn(struct mg_connection * conn) ;
        
        static JSValue send(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue close(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        // static JSValue initTLS(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        
        static JSValue connect(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue connectWS(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
    } ;
}