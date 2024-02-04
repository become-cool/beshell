#include "Demo.hpp"

using namespace std ;

DEFINE_NCLASS_META(Demo)
std::vector<JSCFunctionListEntry> Demo::methods = {
    JS_CFUNC_DEF("method", 0, Demo::jsMethod),
} ;

Demo::Demo(JSContext * ctx)
    : NativeClass(ctx)
{
}

JSValue Demo::jsMethod(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    return JS_UNDEFINED ;
}