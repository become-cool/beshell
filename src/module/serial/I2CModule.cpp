#include "module/I2C.hpp"

using namespace std ;

namespace be {
    I2CModule::I2CModule(): NativeModule("i2c") {
        isGlobal = true ;
        isReplGlobal = true ;

        

        exportFunction("begin",jsBegin) ;
    }
    I2CModule::~I2CModule() {
        if(classI2C) {
            delete classI2C ;
            classI2C = nullptr ;
        }
    }

    void I2CModule::init(JSRuntime * rt) {
        // JS_NewClassID(&I2CObj::jsClassID);
    }
    void I2CModule::setup(JSContext * ctx) {
        classI2C = new I2CClass(ctx) ;
    }
    
    JSValue I2CModule::jsBegin(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        return JS_UNDEFINED ;
    }
}