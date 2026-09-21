#include "DriverClass.hpp"

using namespace std ;

namespace be::driver {

    DEFINE_NCLASS_META(DriverClass, EventEmitter)

    std::vector<JSCFunctionListEntry> DriverClass::methods = {
    } ;

    DriverClass::DriverClass(JSContext * ctx, JSValue _jsobj)
        : EventEmitter(ctx, build(ctx, _jsobj))
    {
    }

    JSValue DriverClass::constructor(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        auto obj = new DriverClass(ctx, this_val) ;
        obj->shared() ;
        return obj->jsobj ;
    }

}
