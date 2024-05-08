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
        
        #ifdef UART_NUM_1
        exportName("uart1") ;
        #endif

        #ifdef UART_NUM_2
        exportName("uart2") ;
        #endif

        exportName("i2c0") ;

        #ifdef I2C_NUM_1
        exportName("i2c1") ;
        #endif
        
        #ifdef SPI1_HOST
        exportName("spi1") ;
        #endif

        #ifdef SPI2_HOST
        exportName("spi2") ;
        #endif

        #ifdef SPI3_HOST
        exportName("spi3") ;
        #endif

        exportName("i2s0") ;
    }
    
    SerialModule::~SerialModule() {
    }

    void SerialModule::import(JSContext *ctx) {
        assert(ctx) ;
        assert(m) ;

        UART * uart0 = new UART(ctx, UART_NUM_0) ;
        exportValue("uart0", JS_DupValue(ctx,uart0->jsobj)) ;
        #ifdef UART_NUM_1
        UART * uart1 = new UART(ctx, UART_NUM_1) ;
        exportValue("uart1", JS_DupValue(ctx,uart1->jsobj)) ;
        #endif
        #ifdef UART_NUM_2
        UART * uart2 = new UART(ctx, UART_NUM_2) ;
        exportValue("uart2", JS_DupValue(ctx,uart2->jsobj)) ;
        #endif
        
        I2C * i2c0 = I2C::flyweight(ctx, I2C_NUM_0) ;
        exportValue("i2c0", JS_DupValue(ctx,i2c0->jsobj)) ;
        #ifdef I2C_NUM_1
        I2C * i2c1 = I2C::flyweight(ctx, I2C_NUM_1) ;
        exportValue("i2c1", JS_DupValue(ctx,i2c1->jsobj)) ;
        #endif
        
        #ifdef SPI1_HOST
        SPI * spi1 = SPI::flyweight(ctx, SPI1_HOST) ;
        exportValue("spi1", spi1->jsobj) ;
        #endif
        
        #ifdef SPI2_HOST
        SPI * spi2 = SPI::flyweight(ctx, SPI2_HOST) ;
        exportValue("sp2", spi2->jsobj) ;
        #endif

        #ifdef SPI3_HOST
        SPI * spi3 = SPI::flyweight(ctx, SPI3_HOST) ;
        exportValue("spi3", spi3->jsobj) ;
        #endif
        
        I2S * i2s0 = I2S::flyweight(ctx, I2S_NUM_0) ;
        exportValue("i2s0", JS_DupValue(ctx,i2s0->jsobj)) ;
        
        #ifdef I2S_NUM_1
        I2S * i2s1 = I2S::flyweight(ctx, I2S_NUM_1) ;
        exportValue("i2s1", JS_DupValue(ctx,i2s1->jsobj)) ;
        #endif
    }
    
}