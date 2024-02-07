#include "I8080.hpp"

using namespace std ;

namespace be {

DEFINE_NCLASS_META(I8080)
std::vector<JSCFunctionListEntry> I8080::methods = {
    JS_CFUNC_DEF("method", 0, I8080::jsMethod),
} ;

I8080::I8080(JSContext * ctx)
    : NativeClass(ctx)
{
}

JSValue I8080::jsMethod(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    return JS_UNDEFINED ;
}

} // namespace be