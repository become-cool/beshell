#pragma once

#include <NativeClass.hpp>
#include "deps/mongoose/mongoose.h"

namespace be::mg {
    class Response: public ::be::NativeClass {
        DECLARE_NCLASS_META
    private:
        static std::vector<JSCFunctionListEntry> methods ;
        struct mg_connection * conn = nullptr ;

    public:
        Response(JSContext * ctx, struct mg_connection * conn) ;
    
        static JSValue wsSend(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue httpUpgrade(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue serveDir(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue close(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue redirect(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue reply(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;

    } ;
}