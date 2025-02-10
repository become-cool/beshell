#pragma once

#include "Display.hpp"

namespace be::driver::disp {

    class ST7789: public Display {
        DECLARE_NCLASS_META

    public:
        ST7789(JSContext * ctx, JSValue jsobj=JS_NULL, uint16_t width=0, uint16_t height=0) ;
        static JSValue constructor(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        
        static JSValue setup(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue test(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;

    private:
        static std::vector<JSCFunctionListEntry> methods ;
    } ;


}