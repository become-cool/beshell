#include "Serial.hpp"
#include "UART.hpp"
#include "I2C.hpp"
#include "SPI.hpp"
#include "I2S.hpp"

using namespace std ;

namespace be {

    const char * Serial::name = "serial" ;

    Serial::Serial(JSContext * ctx, const char * name)
        : NativeModule(ctx,name,0)
    {
        // UART ---------------
        exportName("uart0") ;
        
        #if SOC_UART_HP_NUM>1
        exportName("uart1") ;
        #endif

        #if SOC_UART_HP_NUM>2
        exportName("uart2") ;
        #endif

        #if SOC_UART_HP_NUM>3
        exportName("uart3") ;
        #endif

        #if SOC_UART_HP_NUM>4
        exportName("uart4") ;
        #endif

        #if SOC_UART_LP_NUM>0
        exportName("uartlp0") ;
        #endif

        // I2C ---------------
        exportName("i2c0") ;

        #if SOC_I2C_NUM > 1
        exportName("i2c1") ;
        #endif

        #if SOC_LP_I2C_NUM > 0
        exportName("i2clp0") ;
        #endif
        

        // I2S ---------------
        exportName("i2s0") ;

        #if SOC_I2S_NUM>1
        exportName("i2s1") ;
        #endif

        // SPI ---------------
        #if SOC_SPI_PERIPH_NUM>1
        exportName("spi1") ;
        #endif

        #if SOC_SPI_PERIPH_NUM>2
        exportName("spi2") ;
        #endif

        #if SOC_SPI_PERIPH_NUM>3
        exportName("spi3") ;
        #endif

        #if SOC_SPI_PERIPH_NUM>4
        exportName("spi4") ;
        #endif
    }
    
    Serial::~Serial() {
    }

    void Serial::import(JSContext *ctx) {
        assert(ctx) ;
        assert(m) ;

        // UART ---------------
        UART * uart0 = UART::flyweight(ctx, UART_NUM_0) ;
        exportValue("uart0", JS_DupValue(ctx,uart0->jsobj)) ;

        #if SOC_UART_HP_NUM>1
        UART * uart1 = UART::flyweight(ctx, UART_NUM_1) ;
        exportValue("uart1", JS_DupValue(ctx,uart1->jsobj)) ;
        #endif

        #if SOC_UART_HP_NUM>2
        UART * uart2 = UART::flyweight(ctx, UART_NUM_2) ;
        exportValue("uart2", JS_DupValue(ctx,uart2->jsobj)) ;
        #endif

        #if SOC_UART_HP_NUM>3
        UART * uart3 = UART::flyweight(ctx, UART_NUM_3) ;
        exportValue("uart3", JS_DupValue(ctx,uart3->jsobj)) ;
        #endif

        #if SOC_UART_HP_NUM>4
        UART * uart4 = UART::flyweight(ctx, UART_NUM_4) ;
        exportValue("uart4", JS_DupValue(ctx,uart4->jsobj)) ;
        #endif

        #if SOC_UART_LP_NUM>0
        UART * uartlp0 = UART::flyweight(ctx, LP_UART_NUM_0) ;
        exportValue("uartlp0", JS_DupValue(ctx,uartlp0->jsobj)) ;
        #endif


        // I2C ---------------        
        I2C * i2c0 = I2C::flyweight(ctx, I2C_NUM_0) ;
        exportValue("i2c0", JS_DupValue(ctx,i2c0->jsobj)) ;

        #if SOC_I2C_NUM > 1
        I2C * i2c1 = I2C::flyweight(ctx, I2C_NUM_1) ;
        exportValue("i2c1", JS_DupValue(ctx,i2c1->jsobj)) ;
        #endif
        
        #if SOC_LP_I2C_NUM > 0
        I2C * i2clp0 = I2C::flyweight(ctx, LP_I2C_NUM_0) ;
        exportValue("i2clp0", JS_DupValue(ctx,i2clp0->jsobj)) ;
        #endif

        
        // I2S ---------------
        I2S * i2s0 = I2S::flyweight(ctx, I2S_NUM_0) ;
        exportValue("i2s0", JS_DupValue(ctx,i2s0->jsobj)) ;
        
        #if SOC_I2S_NUM>1
        I2S * i2s1 = I2S::flyweight(ctx, I2S_NUM_1) ;
        exportValue("i2s1", JS_DupValue(ctx,i2s1->jsobj)) ;
        #endif


        // SPI ---------------
        #if SOC_SPI_PERIPH_NUM > 1
        SPI * spi1 = SPI::flyweight(ctx, SPI1_HOST) ;
        exportValue("spi1", spi1->jsobj) ;
        #endif
        
        #if SOC_SPI_PERIPH_NUM > 2
        SPI * spi2 = SPI::flyweight(ctx, SPI2_HOST) ;
        exportValue("sp2", spi2->jsobj) ;
        #endif

        #if SOC_SPI_PERIPH_NUM > 3
        SPI * spi3 = SPI::flyweight(ctx, SPI3_HOST) ;
        exportValue("spi3", spi3->jsobj) ;
        #endif

        #if SOC_SPI_PERIPH_NUM > 4
        SPI * spi4 = SPI::flyweight(ctx, SPI3_HOST) ;
        exportValue("spi4", spi4->jsobj) ;
        #endif
    }
    
}