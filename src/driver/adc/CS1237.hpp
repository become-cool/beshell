#pragma once

#include <NativeClass.hpp>
#include <driver/gpio.h>

namespace be::driver::adc {

    class CS1237: public be::NativeClass {
        DECLARE_NCLASS_META

    public:
        CS1237(JSContext * ctx, JSValue _jsobj=JS_NULL) ;
        static JSValue constructor(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;

        static JSValue setup(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue readConfig(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue writeConfig(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue ready(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue read(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue powerDown(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;

        void setPin(gpio_num_t dout, gpio_num_t sclk) ;
        uint8_t readConfig() ;
        void writeConfig(uint8_t value) ;
        bool read(int * adc_data) ;
        bool ready() ;
        void powerDown(bool down) ;
        
    
    private:
        static std::vector<JSCFunctionListEntry> methods ;
        
        gpio_num_t dout = GPIO_NUM_NC ;
        gpio_num_t sclk = GPIO_NUM_NC ;

    } ;
}