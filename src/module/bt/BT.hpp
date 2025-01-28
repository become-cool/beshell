#pragma once

#include "BeShell.hpp"
#include "NativeModule.hpp"

namespace be{
    class BT: public be::EventModule {
    public:
        static const char * const name ;
        BT(JSContext * ctx, const char * name) ;

        void onNativeEvent(JSContext *ctx, void * param) ;
        
        static void use(be::BeShell * beshell) ;

        static JSValue init(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue scan(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue setMTU(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue connect(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue disconnect(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue search(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue read(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue write(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue subscribe(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
    } ;
}
