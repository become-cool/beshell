#pragma once

#include "Display.hpp"

#include "esp_lcd_panel_io.h"
#include "esp_lcd_panel_rgb.h"
#include "esp_lcd_panel_vendor.h"
#include "esp_lcd_panel_ops.h"

#include "freertos/FreeRTOS.h"
#include "freertos/semphr.h"
#include "freertos/task.h"

namespace be::driver::display {

    class RGB565: public Display {
        DECLARE_NCLASS_META
    private:
        static std::vector<JSCFunctionListEntry> methods ;
        bool playing = true ;

    protected:
        esp_lcd_panel_handle_t handle = nullptr;

    public:
        RGB565(JSContext * ctx, JSValue jsobj=JS_NULL, uint16_t width=0, uint16_t height=0) ;
        
        bool createBuff() ;
        void drawRect(coord_t x1,coord_t y1,coord_t x2,coord_t y2,color_t * pixels) ;
        
        static JSValue setup(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue init(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue reset(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue pause(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue resume(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue off(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue on(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
    } ;

}