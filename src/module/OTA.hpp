#pragma once

#include "BeShell.hpp"
#include "NativeModule.hpp"

namespace be {

    class OTA: public be::EventModule {
    public:
        static char const * const name ;
        OTA(JSContext * ctx, const char * name=nullptr) ;
        
        static void use(be::BeShell * beshell) ;

        static const char * whichFS(const char * defaultPartName="fsroot_0") ;
    
        virtual void exports(JSContext *ctx) ;

        static JSValue getBootPartition(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue setBootPartition(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue getRunningPartition(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue getNextUpdatePartition(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue markValid(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue markInvalid(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue rollback(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue isRollbackEnabled(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
    } ;

}
