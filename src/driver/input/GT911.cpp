#include "GT911.hpp"

using namespace std ;

namespace be::driver {

    DEFINE_NCLASS_META(GT911, NativeClass)

    std::vector<JSCFunctionListEntry> GT911::methods = {
        JS_CFUNC_DEF("method", 0, GT911::jsMethod),
    } ;

    GT911::GT911(JSContext * ctx, JSValue _jsobj)
        : NativeClass(ctx,build(ctx,_jsobj))
    {
    }

    JSValue GT911::constructor(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        auto obj = new GT911(ctx) ;
        obj->self = std::shared_ptr<GT911> (obj) ;
        return obj->jsobj ;
    }

    JSValue GT911::jsMethod(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        return JS_UNDEFINED ;
    }

    void GT911::provider(DriverModule * dm) {
        dm->exportClass<GT911>() ;
    }
    void GT911::use() {
        DriverModule::providers.push_back(provider) ;
    }
}