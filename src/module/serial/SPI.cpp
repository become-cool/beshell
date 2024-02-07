#include "SPI.hpp"

using namespace std ;

DEFINE_NCLASS_META(SPI)
std::vector<JSCFunctionListEntry> SPI::methods = {
    JS_CFUNC_DEF("method", 0, SPI::jsMethod),
} ;

SPI::SPI(JSContext * ctx, uint8_t _busnum)
    : NativeClass(ctx)
    , busnum(_busnum)
{
}

JSValue SPI::jsMethod(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    return JS_UNDEFINED ;
}