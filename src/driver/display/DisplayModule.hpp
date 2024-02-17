#pragma once

#include "BeShell.hpp"
#include "NativeModule.hpp"

namespace be::driver::display {

    class DisplayModule: public be::NativeModule {
    public:
        DisplayModule(JSContext * ctx, const char * name) ;

        static JSValue jsFromRGB(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue jsFromRGB565(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue jsToRGB(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue jsToRGB565(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
    } ;

    inline void use(be::BeShell & beshell) {
        beshell.addModule<DisplayModule>("driver/display") ;
    }

    uint16_t fromRGB(uint8_t r,uint8_t g,uint8_t b) ;
    uint16_t fromRGB565(uint8_t r,uint8_t g,uint8_t b) ;
    
    void toRGB(uint16_t value, uint8_t * r,uint8_t * g,uint8_t * b) ;
    void toRGB565(uint16_t value, uint8_t * r,uint8_t * g,uint8_t * b) ;
}