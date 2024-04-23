#pragma once

#include "BeShell.hpp"
#include "NativeModule.hpp"
#include "Server.hpp"
#include "Client.hpp"
#include "HTTPRequest.hpp"
#include "Response.hpp"
#include "mongoose/mongoose.h"


namespace be::mg {
    class MgModule: public be::NativeModule {
    private :
        static struct mg_mgr mgr ;
    public:
        static const char * name ;

        MgModule(JSContext * ctx, const char * name) ;

        static std::string ca_path ;
        static std::string cert_path ;
        static std::string certkey_path ;

        static void loop(const BeShell & beshell, void * data) ;

        static void use(be::BeShell * beshell) {
            // beshell.useModule<MgModule>("mg") ;
            beshell->addLoopFunction(loop, nullptr) ;

            ca_path = beshell->fs->toVFSPath("/var/ca.pem") ;
            cert_path = beshell->fs->toVFSPath("/var/cert.pem") ;
            certkey_path = beshell->fs->toVFSPath("/var/key.pem") ;
        }

        static const char * eventName(int ev) ;
        static int eventConst(const char * evname) ;

        static bool isListening(const char * url) ;

        static JSValue sntpHTTPRequest(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue connPeer(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue connCount(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue getDNS(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue parseUrl(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue setLog(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;

    friend class Server ;
    friend class Client ;
    } ;
}
