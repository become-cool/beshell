#include "Request.hpp"

using namespace std ;

namespace be::mg {
    DEFINE_NCLASS_META(Request, NativeClass)
    std::vector<JSCFunctionListEntry> Request::methods = {
        JS_CFUNC_DEF("method", 0, Request::jsMethod),
    } ;

    Request::Request(JSContext * ctx, JSValue _jsobj)
        : NativeClass(ctx,build(ctx,_jsobj))
    {
    }

    JSValue Request::constructor(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        auto obj = new Request(ctx) ;
        return obj->jsobj ;
    }

    JSValue Request::jsMethod(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        return JS_UNDEFINED ;
    }
}