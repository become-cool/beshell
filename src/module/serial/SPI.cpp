#include "SPI.hpp"

using namespace std ;

namespace be {

    DEFINE_NCLASS_META(SPI, NativeClass)
    std::vector<JSCFunctionListEntry> SPI::methods = {
        JS_CFUNC_DEF("method", 0, SPI::jsMethod),
    } ;

    SPI::SPI(JSContext * ctx, JSValue _jsobj, uint8_t _busnum)
        : NativeClass(ctx, build(ctx, _jsobj))
        , busnum(_busnum)
    {
    }

    JSValue SPI::jsMethod(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        return JS_UNDEFINED ;
    }

    JSValue SPI::constructor(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        auto obj = new SPI(ctx) ;
        obj->self = std::shared_ptr<SPI> (obj) ;
        return obj->jsobj ;
    }
}