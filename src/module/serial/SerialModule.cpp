#include "SerialModule.hpp"

using namespace std ;

namespace be {

    
    SerialModule::SerialModule(JSContext * ctx, const char * name)
        : NativeModule(ctx,name,0)
    {
        exportName("uart0") ;
        exportName("uart1") ;
        exportName("uart2") ;
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
        DELETE_MEMBER(uart0)
        DELETE_MEMBER(uart1)
        DELETE_MEMBER(uart2)
        DELETE_MEMBER(i2c0)
        DELETE_MEMBER(i2c1)
        DELETE_MEMBER(spi1)
        DELETE_MEMBER(spi2)
        DELETE_MEMBER(spi3)
    }

    void SerialModule::import(JSContext *ctx) {
        assert(ctx) ;
        assert(m) ;

        uart0 = new UART(ctx, UART_NUM_0) ;
        uart1 = new UART(ctx, UART_NUM_1) ;
        uart2 = new UART(ctx, UART_NUM_2) ;
        exportValue("uart0", JS_DupValue(ctx,uart0->jsobj)) ;
        exportValue("uart1", JS_DupValue(ctx,uart1->jsobj)) ;
        exportValue("uart2", JS_DupValue(ctx,uart2->jsobj)) ;
        
        i2c0 = I2C::flyweight(ctx, I2C_NUM_0) ;
        i2c1 = I2C::flyweight(ctx, I2C_NUM_1) ;
        exportValue("i2c0", JS_DupValue(ctx,i2c0->jsobj)) ;
        exportValue("i2c1", JS_DupValue(ctx,i2c1->jsobj)) ;
        
        // spi1 = new SPI(ctx, 1) ;
        // spi2 = new SPI(ctx, 2) ;
        // spi3 = new SPI(ctx, 3) ;
        // exportValue("spi1", spi1->jsobj) ;
        // exportValue("spi2", spi2->jsobj) ;
        // exportValue("spi3", spi3->jsobj) ;
    }
    
}