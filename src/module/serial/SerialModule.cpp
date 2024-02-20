#include "SerialModule.hpp"

using namespace std ;

namespace be {

    
    SerialModule::SerialModule(JSContext * ctx, const char * name)
        : NativeModule(ctx,name,0)
    {
        exportName("i2c0") ;
        exportName("i2c1") ;
        exportName("spi1") ;
        exportName("spi2") ;
        exportName("spi3") ;
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

    void SerialModule::import(JSContext *ctx) {
        assert(ctx) ;
        assert(m) ;

        dp(ctx)

        i2c0 = new I2C(ctx, I2C_NUM_0) ;
        i2c1 = new I2C(ctx, I2C_NUM_1) ;
        
        spi1 = new SPI(ctx, 1) ;
        spi2 = new SPI(ctx, 2) ;
        spi3 = new SPI(ctx, 3) ;
        
        exportValue("i2c0", JS_DupValue(ctx,i2c1->jsobj)) ;
        exportValue("i2c0", JS_DupValue(ctx,i2c1->jsobj)) ;

        exportValue("spi1", spi1->jsobj) ;
        exportValue("spi2", spi2->jsobj) ;
        exportValue("spi3", spi3->jsobj) ;
    }
    
}