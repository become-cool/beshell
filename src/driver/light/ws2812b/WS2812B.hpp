#pragma once

#include <NativeClass.hpp>
#include <cstdint>
#include "led_strip.h"


namespace be::driver::light{
    class WS2812B: public be::NativeClass {
        DECLARE_NCLASS_META
    private:
        led_strip_handle_t led_strip = nullptr;
        int length = 0 ;

        static std::vector<JSCFunctionListEntry> methods ;
        // static std::vector<JSCFunctionListEntry> staticMethods ;

    public:
        WS2812B(JSContext * ctx, JSValue _jsobj=JS_NULL) ;
        ~WS2812B() ;
        
        static JSValue constructor(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;

        static JSValue setup(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue setPixel(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue clear(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue flush(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
    
    } ;
}