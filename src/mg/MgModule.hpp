#pragma once

#include "BeShell.hpp"
#include "NativeModule.hpp"
#include "Server.hpp"
#include "Client.hpp"
#include "Request.hpp"
#include "Response.hpp"
#include "mongoose/mongoose.h"


namespace be::mg {
    class MgModule: public be::NativeModule {
    private :
        static struct mg_mgr mgr ;
    public:
        MgModule(JSContext * ctx, const char * name) ;

        // void import(JSContext *ctx) ;

        static void loop(const BeShell & beshell) ;

        inline static void use(be::BeShell & beshell) {
            beshell.addModule<MgModule>("mg") ;
            beshell.addLoopFunction(loop) ;
        }

        static const char * eventName(int ev) ;
        static int eventConst(const char * evname) ;

        static bool isListening(const char * url) ;

        static JSValue sntpRequest(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue connPeer(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue connCount(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue getDNS(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue parseUrl(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue setLog(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;

    friend class Server ;
    } ;
}
