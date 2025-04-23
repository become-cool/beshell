#pragma once

#include <EventEmitter.hpp>
#include "deps/mongoose/mongoose.h"

namespace be::mg {

    class MQTTClient ;

    typedef bool (*MQTTClientHandler) (MQTTClient * client, struct mg_connection *, int ev, void *ev_data, void *fn_data) ;

    class MQTTClient: public be::EventEmitter {
        DECLARE_NCLASS_META

    public:
        static JSValue connect(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue ping(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue push(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue sub(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue unsub(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue disconnect(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;

        static void setHandler(MQTTClientHandler handler) ;

    protected:

#ifdef ESP_PLATFORM
        virtual void onNativeEvent(JSContext *ctx, void * param) ;
#endif

    private:
        static std::vector<JSCFunctionListEntry> methods ;
        
        struct mg_connection * conn ;
        uint16_t poll_times = 0 ;
        std::string _host = ""; // 用于 tls
        bool is_tls = false ;

        MQTTClient(JSContext * ctx, struct mg_connection * conn) ;
        ~MQTTClient() ;

        static void eventHandler(struct mg_connection * c, int ev, void * ev_data) ;
        
        void emitCallback(const char * eventName, std::initializer_list<JSValue> args) ;

        static MQTTClientHandler handler ;

    } ;
} 