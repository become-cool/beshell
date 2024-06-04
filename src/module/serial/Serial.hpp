#pragma once

#include "NativeModule.hpp"


#ifndef SOC_UART_NUM

#ifdef CONFIG_IDF_TARGET_ESP32
#define SOC_UART_NUM                (3)
#define SOC_UART_HP_NUM             (3)
#define SOC_I2C_NUM                 (2)
#define SOC_SPI_PERIPH_NUM          (3)
#define SOC_I2S_NUM                 (2U)
#endif



#ifdef CONFIG_IDF_TARGET_ESP32S2
#define SOC_UART_NUM                (2)
#define SOC_UART_HP_NUM             (2)
#define SOC_I2C_NUM                 (2)
#define SOC_SPI_PERIPH_NUM          (3)
#define SOC_I2S_NUM                 (1)
#endif



#ifdef CONFIG_IDF_TARGET_ESP32S3
#define SOC_UART_NUM                (2)
#define SOC_UART_HP_NUM             (2)
#define SOC_I2C_NUM                 (2)
#define SOC_SPI_PERIPH_NUM          (3)
#define SOC_I2S_NUM                 (1)
#endif


#ifdef CONFIG_IDF_TARGET_ESP32C2
#define SOC_UART_NUM                (3)
#define SOC_UART_HP_NUM             (3)
#define SOC_I2C_NUM                 (2)
#define SOC_SPI_PERIPH_NUM          (3)
#define SOC_I2S_NUM                 (2)
#endif


#ifdef CONFIG_IDF_TARGET_ESP32C3
#define SOC_UART_NUM                (2)
#define SOC_UART_HP_NUM             (2)
#define SOC_I2C_NUM                 (1)
#define SOC_SPI_PERIPH_NUM          (2)
#define SOC_I2S_NUM                 (1)
#endif


#ifdef CONFIG_IDF_TARGET_ESP32C6
#define SOC_UART_NUM                (3)
#define SOC_UART_HP_NUM             (2)
#define SOC_UART_LP_NUM             (1)
#define SOC_I2C_NUM                 (1)
#define SOC_SPI_PERIPH_NUM          (2)
#define SOC_I2S_NUM                 (1)
#endif


#ifdef CONFIG_IDF_TARGET_ESP32H2
#define SOC_UART_NUM                (2)
#define SOC_UART_HP_NUM             (2)
#define SOC_I2C_NUM                 (2)
#define SOC_SPI_PERIPH_NUM          (2)
#define SOC_I2S_NUM                 (1)
#endif


#ifdef CONFIG_IDF_TARGET_ESP32H2
#define SOC_UART_NUM                (2)
#define SOC_UART_HP_NUM             (2)
#define SOC_I2C_NUM                 (2)
#define SOC_SPI_PERIPH_NUM          (2)
#define SOC_I2S_NUM                 (1)
#endif


#ifdef CONFIG_IDF_TARGET_ESP32P4
#define SOC_UART_NUM                (6)
#define SOC_UART_HP_NUM             (5)
#define SOC_UART_LP_NUM             (5)
#define SOC_I2C_NUM                 (2)
#define SOC_SPI_PERIPH_NUM          (3)
#define SOC_I2S_NUM                 (3)
#endif


#endif




namespace be {
    class Serial: public NativeModule {
    protected:
        void import(JSContext *ctx) ;
    public:

        static const char *name ;

        Serial(JSContext * ctx, const char * name) ;
        ~Serial() ;
        
    } ;
}