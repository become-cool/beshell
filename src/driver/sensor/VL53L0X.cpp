#include "VL53L0X.hpp"

using namespace std ;

namespace be::driver {
    DEFINE_NCLASS_META(VL53L0X, NativeClass)
    std::vector<JSCFunctionListEntry> VL53L0X::methods = {
        JS_CFUNC_DEF("method", 0, VL53L0X::jsMethod),
    } ;

    VL53L0X::VL53L0X(JSContext * ctx, JSValue _jsobj)
        : NativeClass(ctx,build(ctx,_jsobj))
    {
    }

    JSValue VL53L0X::constructor(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        auto obj = new VL53L0X(ctx) ;
        return obj->jsobj ;
    }

    JSValue VL53L0X::jsMethod(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        return JS_UNDEFINED ;
    }

    
    void VL53L0X::provider(DriverModule * dm) {
        dm->exportClass<VL53L0X>() ;
    }
    void VL53L0X::use() {
        DriverModule::providers.push_back(provider) ;
    }
}