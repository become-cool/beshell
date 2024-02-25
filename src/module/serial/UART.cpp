#include "UART.hpp"

using namespace std ;

namespace be{
    DEFINE_NCLASS_META(UART, NativeClass)
    std::vector<JSCFunctionListEntry> UART::methods = {
        JS_CFUNC_DEF("method", 0, UART::jsMethod),
    } ;

    UART::UART(JSContext * ctx, JSValue _jsobj)
        : NativeClass(ctx,build(ctx,_jsobj))
    {
    }

    JSValue UART::constructor(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        auto obj = new UART(ctx) ;
        return obj->jsobj ;
    }

    JSValue UART::jsMethod(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        return JS_UNDEFINED ;
    }
}