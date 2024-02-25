#include "BTModule.hpp"

namespace be {

    BTModule::BTModule(JSContext * ctx, const char * name)
        : NativeModule(ctx, name, 0)
    {
        // exportClass<ClassName>() ;
        exportFunction("func",jsFunc,0) ;
    }

    // void BTModule::import(JSContext *ctx) {
    // }

    JSValue BTModule::jsFunc(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        return JS_UNDEFINED ;
    }
    
}