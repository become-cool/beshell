#include "module/I2CModule.hpp"
#include "I2CObj.hpp"

using namespace std ;

namespace be {
    I2CModule::I2CModule(): NativeModule("i2c") {
        isGlobal = true ;
        isReplGlobal = true ;

        exportFunction("begin",jsBegin) ;
    }

    void I2CModule::init(JSRuntime * rt) {
        JS_NewClassID(&I2CObj::jsClassID);
    }
    void I2CModule::setup(JSContext * ctx) {
        
    }
    
    JSValue I2CModule::jsBegin(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        return JS_UNDEFINED ;
    }
}