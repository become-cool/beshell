#pragma once
#include "module/NativeModule.hpp"
#include "module/I2CClass.hpp"

namespace be {
    class I2CModule: public NativeModule {
    private:
    public:
        I2CClass * classI2C = nullptr ;

        I2CModule() ;
        ~I2CModule() ;
        
        void init(JSRuntime * rt) ;
        void setup(JSContext * ctx) ;

        static JSValue jsBegin(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
    } ;
}