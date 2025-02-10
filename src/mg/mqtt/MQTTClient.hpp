#pragma once

#include <EventEmitter.hpp>
#include "deps/mongoose/mongoose.h"

namespace be::mg{

    class MQTTClient: public be::EventEmitter {
        DECLARE_NCLASS_META

    public:
        static JSValue connect(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue ping(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue push(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue sub(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue unsub(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue disconnect(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;

    protected:

#ifdef ESP_PLATFORM
        virtual void onNativeEvent(JSContext *ctx, void * param) ;
#endif

    private:
        static std::vector<JSCFunctionListEntry> methods ;
        
        struct mg_connection * conn ;
        JSValue callback ;
        uint16_t poll_times = 0 ;
        bool is_tls = false ;

        MQTTClient(JSContext * ctx, struct mg_connection * conn, JSValue callback) ;
        ~MQTTClient() ;

        static void eventHandler(struct mg_connection * c, int ev, void * ev_data) ;
        
        void emitCallback(const char * eventName, std::initializer_list<JSValue> args) ;
    } ;
}