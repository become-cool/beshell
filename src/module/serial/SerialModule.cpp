#include "module/serial/SerialModule.hpp"

using namespace std ;

namespace be {
    
    static SerialModule* factory(JSContext * ctx, const char * name) {
        return new SerialModule(ctx,name,0) ;
    }

    #define DELETE_MEMBER(var) \
        if(var) {              \
            delete var ;       \
            var = nullptr ;    \
        }
    SerialModule::~SerialModule() {
        DELETE_MEMBER(i2c0)
        DELETE_MEMBER(i2c1)
        DELETE_MEMBER(spi1)
        DELETE_MEMBER(spi2)
        DELETE_MEMBER(spi3)
    }

    void SerialModule::init(JSRuntime * rt) {
    }
    void SerialModule::setup(JSContext * ctx) {
        i2c0 = new I2C(ctx, 0) ;
        i2c1 = new I2C(ctx, 1) ;
        spi1 = new SPI(ctx, 1) ;
        spi2 = new SPI(ctx, 2) ;
        spi3 = new SPI(ctx, 3) ;
    }
}