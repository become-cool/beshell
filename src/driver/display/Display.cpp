#include <iostream>
#include <algorithm>
#include "Display.hpp"
#include "qjs_utils.h"

using namespace std ;


namespace be::driver::display {

    DEFINE_NCLASS_META(Display, NativeClass)
    std::vector<JSCFunctionListEntry> Display::methods = {
        JS_CFUNC_DEF("drawRect", 0, Display::jsDrawRect),
        JS_CFUNC_DEF("registerToLV", 0, Display::jsRegisterToLV),
    } ;

    Display::Display(JSContext * ctx, JSValue _jsobj, uint16_t width, uint16_t height)
        : NativeClass(ctx, build(ctx,_jsobj))
        , _width(width)
        , _height(height)
    {
    }

    uint16_t Display::width() const {
        return _width ;
    }
    uint16_t Display::height() const {
        return _height ;
    }

    // void Display::drawRect(coord_t x1,coord_t y1,coord_t x2,coord_t y2,color_t * pixels) {
    //     cout << "not implements" << endl ;
    // }

    JSValue Display::jsDrawRect(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        Display * disp = (Display *)fromJS(this_val) ;
        if(!disp) {
            THROW_EXCEPTION("not a valid object")
        }
        CHECK_ARGC(5)
        ARGV_TO_INT16(0,x1)
        ARGV_TO_INT16(1,y1)
        ARGV_TO_INT16(2,x2)
        ARGV_TO_INT16(3,y2)
        ARGV_TO_UINT16(4,color)

        int16_t w = x2-x1+1 ;
        int16_t h = y2-y1+1 ;
        size_t size = w*h ;
        uint16_t * buff = new uint16_t[size] ;
        if(!buff) {
            THROW_EXCEPTION("out of memory?")
        }

        std::fill_n(buff, size, color);

        disp->drawRect(x1,y1,x2,y2,buff) ;

        delete[] buff ;

        return JS_UNDEFINED ;
    }

    JSValue Display::constructor(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        return JS_UNDEFINED ;
    }

#ifdef MODULE_LV
    void disp_flush_cb(lv_display_t * lvdisp, const lv_area_t * area, unsigned char * color_p) {

        Display* disp = (Display*)lv_display_get_driver_data(lvdisp) ;
        if(disp) {
            disp->drawRect(area->x1,area->y1,area->x2,area->y2, (color_t*)color_p) ;
        }

        dn4(area->x1,area->y1,area->x2,area->y2)
        lv_display_flush_ready(lvdisp);
    }

    bool Display::createBuff() {
        uint16_t fact = (_height+5)/10;
        if(!buff1) {
            buff1 = heap_caps_malloc(_width * fact * sizeof(lv_color_t), MALLOC_CAP_SPIRAM);  // MALLOC_CAP_SPIRAM
            if(!buff1) {
                return false ;
            }
        }
        if(!buff2) {
            buff2 = heap_caps_malloc(_width * fact * sizeof(lv_color_t), MALLOC_CAP_SPIRAM);
            if(!buff2) {
                free(buff1) ;
                return false ;
            }
        }
        
        lv_display_set_buffers(lv_display, buff1, buff2, _width*fact*sizeof(lv_color_t), LV_DISPLAY_RENDER_MODE_PARTIAL);
        return true ;
    }

    bool Display::registerToLV() {

        if(lv_display) {
            return true ;
        }

        lv_display = lv_display_create(_width, _height) ;

        if(!createBuff()) {
            return false ;
        }

        lv_display_set_flush_cb(lv_display, disp_flush_cb);

        lv_display_set_driver_data(lv_display,this) ;

        return true ;
    }

    JSValue Display::jsRegisterToLV(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        THIS_NCLASS(Display,thisobj)
        if(!thisobj->registerToLV()){
            THROW_EXCEPTION("out of memory?")
        }
        return JS_UNDEFINED ;
    }

#endif
}