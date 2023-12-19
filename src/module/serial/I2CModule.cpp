#include "module/I2C.hpp"

using namespace std ;

namespace be {
    I2CModule::I2CModule(): NativeModule("i2c") {
        isGlobal = true ;
        isReplGlobal = true ;
    }
    I2CModule::~I2CModule() {
        if(classI2C) {
            delete classI2C ;
            classI2C = nullptr ;
        }
    }

    void I2CModule::init(JSRuntime * rt) {
    }
    void I2CModule::setup(JSContext * ctx) {
        // I2C * i2c0 = new I2C(ctx, 0) ;
        // I2C * i2c1 = new I2C(ctx, 1) ;
        // I2C * i2c2 = new I2C(ctx, 2) ;
    }
}