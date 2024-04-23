#include "SerialModule.hpp"
#include "UART.hpp"
#include "I2C.hpp"
#include "SPI.hpp"
#include "I2S.hpp"

using namespace std ;

namespace be {

    const char * SerialModule::name = "serial" ;

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
        exportName("i2s0") ;
    }
    
    SerialModule::~SerialModule() {
    }

    void SerialModule::import(JSContext *ctx) {
        assert(ctx) ;
        assert(m) ;

        UART * uart0 = new UART(ctx, UART_NUM_0) ;
        UART * uart1 = new UART(ctx, UART_NUM_1) ;
        UART * uart2 = new UART(ctx, UART_NUM_2) ;
        exportValue("uart0", JS_DupValue(ctx,uart0->jsobj)) ;
        exportValue("uart1", JS_DupValue(ctx,uart1->jsobj)) ;
        exportValue("uart2", JS_DupValue(ctx,uart2->jsobj)) ;
        
        I2C * i2c0 = I2C::flyweight(ctx, I2C_NUM_0) ;
        I2C * i2c1 = I2C::flyweight(ctx, I2C_NUM_1) ;
        exportValue("i2c0", JS_DupValue(ctx,i2c0->jsobj)) ;
        exportValue("i2c1", JS_DupValue(ctx,i2c1->jsobj)) ;
        
        SPI * spi0 = SPI::flyweight(ctx, SPI1_HOST) ;
        SPI * spi1 = SPI::flyweight(ctx, SPI2_HOST) ;
        SPI * spi2 = SPI::flyweight(ctx, SPI3_HOST) ;
        exportValue("spi0", spi0->jsobj) ;
        exportValue("spi1", spi1->jsobj) ;
        exportValue("spi2", spi2->jsobj) ;
        
        I2S * i2s0 = I2S::flyweight(ctx, I2S_NUM_0) ;
        exportValue("i2s0", JS_DupValue(ctx,i2s0->jsobj)) ;
    }
    
}