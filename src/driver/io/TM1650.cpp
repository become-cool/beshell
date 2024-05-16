#include "TM1650.hpp"
#include <iostream>

using namespace std ;

namespace be::driver::io {
    DEFINE_NCLASS_META(TM1650, I2CDevice)

    std::vector<JSCFunctionListEntry> TM1650::methods = {
            JS_CFUNC_DEF("setup", 0, I2CDevice::setup),
            JS_CFUNC_DEF("read", 0, TM1650::read),
    };

    TM1650::TM1650(JSContext *ctx, JSValue _jsobj)
            : I2CDevice(ctx, build(ctx, _jsobj)) {
    }

    bool TM1650::read(uint8_t & val) {
        return true ;
    }

    JSValue TM1650::constructor(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        auto obj = new TM1650(ctx);
        obj->shared() ;
        return obj->jsobj;
    }
    
    JSValue TM1650::read(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        return JS_UNDEFINED ;
    }
}