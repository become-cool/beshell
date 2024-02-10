#pragma once

#include "Display.hpp"

#include "esp_lcd_panel_io.h"
#include "esp_lcd_panel_rgb.h"
#include "esp_lcd_panel_vendor.h"
#include "esp_lcd_panel_ops.h"

namespace be {
namespace driver {
namespace display {

class RGB565: public NativeClass<RGB565>, public Display {
    DECLARE_NCLASS_META
    static std::vector<JSCFunctionListEntry> methods ;
    // static std::vector<JSCFunctionListEntry> staticMethods ;

protected:
    esp_lcd_panel_handle_t handle = nullptr;

public:
    RGB565(JSContext * ctx) ;

    void drawRect(coord_t x1,coord_t y1,coord_t x2,coord_t y2,void  * pixels) ;

    static JSValue jsTest(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
} ;

}}}