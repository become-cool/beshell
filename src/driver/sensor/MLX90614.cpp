#include "MLX90614.hpp"
#include <iostream>

using namespace std ;

namespace be::driver::sensor {
    DEFINE_NCLASS_META(MLX90614, I2CDevice)

    std::vector<JSCFunctionListEntry> MLX90614::methods = {
            JS_CFUNC_DEF("read", 0, MLX90614::read),
    };

    MLX90614::MLX90614(JSContext *ctx, JSValue _jsobj)
            : I2CDevice(ctx, build(ctx, _jsobj)) {
    }

    bool MLX90614::read(uint8_t & val) {
        return true ;
    }

    JSValue MLX90614::constructor(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        auto obj = new MLX90614(ctx);
        obj->shared() ;
        return obj->jsobj;
    }
    
    JSValue MLX90614::read(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        return JS_UNDEFINED ;
    }
}