#include "WiFiModule.hpp"

namespace be {
    
    WiFiModule::WiFiModule(JSContext * ctx, const char * name)
        : NativeModule(ctx, name, 0)
    {
        // exportClass<ClassName>() ;
        exportFunction("func",jsFunc,0) ;
    }

    // void WiFiModule::import(JSContext *ctx) {
    // }

    JSValue WiFiModule::jsFunc(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        return JS_UNDEFINED ;
    }
}
