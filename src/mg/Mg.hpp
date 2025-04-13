#pragma once

#include "BeShell.hpp"
#include "NativeModule.hpp"
#include "http/HTTPServer.hpp"
#include "http/HTTPClient.hpp"
#include "http/HTTPRequest.hpp"
#include "http/HTTPResponse.hpp"
#include "mqtt/MQTTClient.hpp"
#include "deps/mongoose/mongoose.h"


namespace be::mg {
    class Mg: public be::NativeModule {

    public:
        static char const * const name ;

        Mg(JSContext * ctx, const char * name) ;

        static void loop(const BeShell & beshell, void * data) ;

        static void use(be::BeShell * beshell) ;
        void exports(JSContext *ctx) ;

        static const char * eventName(int ev) ;
        static int eventConst(const char * evname) ;

        static void setCA(const char * _ca) ;
        static bool isListening(const char * url) ;

        static JSValue sntpRequest(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue connPeer(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue connCount(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue getDNS(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue setDNS(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue getDNSTimeout(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue setDNSTimeout(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue parseUrl(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue setLog(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue connect(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue setCA(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue startCaptivePortal(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;

    private :
        static struct mg_mgr mgr ;
        static char dns4[28] ;

        static std::string ca ;

        static void captivePortalHandler(struct mg_connection *c, int ev, void *ev_data, void *fn_data) ;
        static struct mg_connection * connCaptiveProtal ;

    friend class Server ;
    friend class Client ;
    friend class MQTTClient ;
    } ;

    
    bool mg_addr_ip_eq(const struct mg_addr & a, const struct mg_addr & b) ;
    char *mg_ntoa(const struct mg_addr *addr, char *buf, size_t len) ;
}
