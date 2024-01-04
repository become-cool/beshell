#include "GPIOModule.hpp"

using namespace std ;

namespace be {
    GPIOModule::GPIOModule(): NativeModule("GPIOModule") {
        exportFunction("method",jsMethod) ;
    }
    
    JSValue GPIOModule::jsMethod(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        return JS_UNDEFINED ;
    }
}