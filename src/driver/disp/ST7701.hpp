#pragma once

#include "RGB565.hpp"
#include "driver/gpio.h"

#ifdef CONFIG_IDF_TARGET_ESP32S3
// #if ESP_IDF_VERSION_MAJOR >= 5

namespace be::driver::disp {

    class ST7701: public RGB565 {
        DECLARE_NCLASS_META
        static std::vector<JSCFunctionListEntry> methods ;
        // static std::vector<JSCFunctionListEntry> staticMethods ;

    private:
        void initReg() ;
        
        void writeData(uint8_t data) ;
        void writeCmd(uint8_t cmd) ;
        void write9b(uint16_t data) ;

        gpio_num_t pin_sda = GPIO_NUM_NC ;
        gpio_num_t pin_sck = GPIO_NUM_NC ;
        gpio_num_t pin_cs = GPIO_NUM_NC ;
        gpio_num_t pin_rst = GPIO_NUM_NC ;

    public:
        ST7701(JSContext * ctx, JSValue jsobj=JS_NULL, uint16_t width=0, uint16_t height=0) ;
        static JSValue constructor(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue setup(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
    } ;

}

// #endif
#endif