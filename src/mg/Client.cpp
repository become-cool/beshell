#include "Client.hpp"

using namespace std ;

namespace be::mg {
    DEFINE_NCLASS_META(Client, NativeClass)
    std::vector<JSCFunctionListEntry> Client::methods = {
        JS_CFUNC_DEF("method", 0, Client::jsMethod),
    } ;

    Client::Client(JSContext * ctx, JSValue _jsobj)
        : NativeClass(ctx,build(ctx,_jsobj))
    {
    }

    JSValue Client::constructor(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        auto obj = new Client(ctx) ;
        return obj->jsobj ;
    }

    JSValue Client::jsMethod(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        return JS_UNDEFINED ;
    }
}