#include "Response.hpp"

using namespace std ;

namespace be::mg {
    DEFINE_NCLASS_META(Response, NativeClass)
    std::vector<JSCFunctionListEntry> Response::methods = {
        JS_CFUNC_DEF("method", 0, Response::jsMethod),
    } ;

    Response::Response(JSContext * ctx, JSValue _jsobj)
        : NativeClass(ctx,build(ctx,_jsobj))
    {
    }

    JSValue Response::constructor(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        auto obj = new Response(ctx) ;
        return obj->jsobj ;
    }

    JSValue Response::jsMethod(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        return JS_UNDEFINED ;
    }
}