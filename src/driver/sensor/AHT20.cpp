#include "AHT20.hpp"
#include <iostream>

using namespace std ;

namespace be{
    DEFINE_NCLASS_META(AHT20, NativeClass)
    std::vector<JSCFunctionListEntry> AHT20::methods = {
        JS_CFUNC_DEF("method", 0, AHT20::jsMethod),
    } ;

    AHT20::AHT20(JSContext * ctx, JSValue _jsobj)
        : NativeClass(ctx,build(ctx,_jsobj))
    {
    }

    JSValue AHT20::constructor(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        auto obj = new AHT20(ctx) ;
        return obj->jsobj ;
    }

    JSValue AHT20::jsMethod(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        cout << "not implement" << endl ;
        return JS_UNDEFINED ;
    }

    
    void AHT20::provider(DriverModule * dm) {
        dm->exportClass<BMP280>() ;
    }
    void AHT20::use() {
        DriverModule::providers.push_back(provider) ;
    }
}