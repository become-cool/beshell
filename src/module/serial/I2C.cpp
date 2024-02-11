#include "I2C.hpp"

using namespace std ;

namespace be {

    DEFINE_NCLASS_META(I2C, NativeClass)
    std::vector<JSCFunctionListEntry> I2C::methods = {
        JS_CFUNC_DEF("method", 0, I2C::jsMethod),
    } ;

    I2C::I2C(JSContext * ctx, JSValue _jsobj, uint8_t _busnum)
        : NativeClass(ctx, build(ctx, _jsobj))
        , busnum(_busnum)
    {

    }

    JSValue I2C::jsMethod(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        return JS_UNDEFINED ;
    }
    
    JSValue I2C::constructor(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        auto obj = new I2C(ctx) ;
        obj->self = std::shared_ptr<I2C> (obj) ;
        return obj->jsobj ;
    }

}