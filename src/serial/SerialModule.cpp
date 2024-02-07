#include "SerialModule.hpp"

using namespace std ;

namespace be {

    
    SerialModule::SerialModule(JSContext * ctx, const char * name)
        : 
    {

    }
    
    #define DELETE_MEMBER(var) \
        if(var) {              \
            delete var ;       \
            var = nullptr ;    \
        }
    SerialModule::~SerialModule() {
        // DELETE_MEMBER(i2c0)
        // DELETE_MEMBER(i2c1)
        // DELETE_MEMBER(spi1)
        // DELETE_MEMBER(spi2)
        // DELETE_MEMBER(spi3)
    }

    void SerialModule::import() {
        assert(ctx) ;
        assert(m) ;

        // i2c0 = new I2C(ctx, 0) ;
        // i2c1 = new I2C(ctx, 1) ;
        // spi1 = new SPI(ctx, 1) ;
        // spi2 = new SPI(ctx, 2) ;
        // spi3 = new SPI(ctx, 3) ;
    }
}