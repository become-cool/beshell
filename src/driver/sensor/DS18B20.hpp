#pragma once

#include <NativeClass.hpp>
#include <driver/gpio.h>

namespace be::driver::sensor {
    class DS18B20: public be::NativeClass {
        DECLARE_NCLASS_META
    private:
        static std::vector<JSCFunctionListEntry> methods ;
        // static std::vector<JSCFunctionListEntry> staticMethods ;

        gpio_num_t pin ;

        unsigned char init(void) ;
        void writeBit(char bit) ;
        unsigned char readBit(void) ;
        void writeByte(unsigned char data) ;
        unsigned char readByte(void) ;
        
    public:
        DS18B20(JSContext * ctx, JSValue _jsobj=JS_NULL) ;
        static JSValue constructor(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;

        static JSValue setup(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue measure(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue read(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
    
    } ;
}