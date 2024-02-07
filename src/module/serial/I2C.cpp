#include "I2C.hpp"

using namespace std ;

namespace be {

DEFINE_NCLASS_META(I2C)
std::vector<JSCFunctionListEntry> I2C::methods = {
    JS_CFUNC_DEF("method", 0, I2C::jsMethod),
} ;

I2C::I2C(JSContext * ctx, uint8_t _busnum)
    : NativeClass(ctx)
    , busnum(_busnum)
{

}

JSValue I2C::jsMethod(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    return JS_UNDEFINED ;
}

}