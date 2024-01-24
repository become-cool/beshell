#include "GPIOModule.hpp"

using namespace std ;

namespace be {

    
    NativeModule* GPIOModule::factory(JSContext * ctx, const char * name) {
        return new GPIOModule(ctx,name) ;
    }
    
    JSValue GPIOModule::jsMethod(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        return JS_UNDEFINED ;
    }
}