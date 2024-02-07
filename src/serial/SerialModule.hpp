#pragma once

#include "NativeModule.hpp"
// #include "module/serial/UART.hpp"
// #include "module/serial/I2C.hpp"
// #include "module/serial/I2S.hpp"
// #include "module/serial/SPI.hpp"

namespace be {
    class SerialModule: public NativeModule {
    protected:
        void import() ;
    public:
        // I2C * i2c0 = nullptr ;
        // I2C * i2c1 = nullptr ;

        // SPI * spi1 = nullptr ;
        // SPI * spi2 = nullptr ;
        // SPI * spi3 = nullptr ;

        SerialModule(JSContext * ctx, const char * name) ;
        // using NativeModule::NativeModule;

        ~SerialModule() ;
        
    } ;
}