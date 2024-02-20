#pragma once

#include "BeShell.hpp"
#include "NativeModule.hpp"
// #include "UART.hpp"
#include "I2C.hpp"
#include "SPI.hpp"
// #include "I2S.hpp"

namespace be {
    class SerialModule: public NativeModule {
    protected:
        void import(JSContext *ctx) ;
    public:
        I2C * i2c0 = nullptr ;
        I2C * i2c1 = nullptr ;

        SPI * spi1 = nullptr ;
        SPI * spi2 = nullptr ;
        SPI * spi3 = nullptr ;

        SerialModule(JSContext * ctx, const char * name) ;
        ~SerialModule() ;

        inline static void use(BeShell & beshell) {
            beshell.addModule<SerialModule>("serial") ;
        }
        
    } ;
}