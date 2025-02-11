#include <cstdint>
#include <iostream>
#include <algorithm>
#include "Display.hpp"
#include "NativeClass.hpp"
#include "qjs_utils.h"
#include "esp_heap_caps.h"
#include "esp_lcd_panel_io.h"
#include "esp_lcd_panel_vendor.h"
#include "esp_lcd_panel_ops.h"
#include "quickjs/quickjs.h"

using namespace std ;


namespace be::driver::disp {

    DEFINE_NCLASS_META(Display, NativeClass)
    std::vector<JSCFunctionListEntry> Display::methods = {
        JS_CFUNC_DEF("setup", 0, Display::setup),
        JS_CFUNC_DEF("drawRect", 0, Display::drawRect),
        JS_CFUNC_DEF("fillRect", 0, Display::fillRect),
        JS_CFUNC_DEF("reset", 0, Display::reset),
        JS_CFUNC_DEF("init", 0, Display::init),
        JS_CFUNC_DEF("off", 0, Display::off),
        JS_CFUNC_DEF("on", 0, Display::on),
        JS_CFUNC_DEF("pause", 0, Display::pause),
        JS_CFUNC_DEF("resume", 0, Display::resume),
        JS_CFUNC_DEF("setOffset", 0, Display::setOffset),
        JS_CFUNC_DEF("mirror", 0, Display::mirror),
        JS_CFUNC_DEF("swapXY", 0, Display::swapXY),
        JS_CFUNC_DEF("invertColor", 0, Display::invertColor),
    } ;

    std::vector<JSCFunctionListEntry> Display::staticMethods = {
        JS_CFUNC_DEF("RGB", 0, Display::RGB),
        JS_CFUNC_DEF("RGB565", 0, Display::RGB565),
        JS_CFUNC_DEF("toRGB", 0, Display::toRGB),
        JS_CFUNC_DEF("toRGB565", 0, Display::toRGB565),
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

    #define CHECK_HANDLE                    \
            if(!that->handle) {             \
                JSTHROW("call setup first") \
            }

    JSValue Display::drawRect(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        THIS_NCLASS(Display, that)
        CHECK_HANDLE
        ASSERT_ARGC(5)
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
            JSTHROW("out of memory?")
        }

        std::fill_n(buff, size, color);

        dn4(x1,y1,x2,y2)
        that->drawRect(x1,y1,x2,y2,buff) ;

        delete[] buff ;

        return JS_UNDEFINED ;
    }


    JSValue Display::fillRect(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        THIS_NCLASS(Display, that)
        CHECK_HANDLE
        ASSERT_ARGC(5)
        ARGV_TO_INT16(0,x1)
        ARGV_TO_INT16(1,y1)
        ARGV_TO_INT16(2,x2)
        ARGV_TO_INT16(3,y2)
        ARGV_TO_UINT16(4,color)

        that->fillRect(x1,y1,x2,y2,color) ;

        return JS_UNDEFINED ;
    }

    JSValue Display::constructor(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        return JS_UNDEFINED ;
    }
    
    JSValue Display::setup(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {

        THIS_NCLASS(Display,that)
        ASSERT_ARGC(1)
        CHECK_HANDLE

        int16_t GET_INT16_PROP_OPT(argv[0], "offsetX", offsetX, 0)
        int16_t GET_INT16_PROP_OPT(argv[0], "offsetY", offsetY, 0)
        CALL_IDF_API( esp_lcd_panel_set_gap(that->handle, offsetX, offsetY), "esp_lcd_panel_set_gap() failed" )

        bool GET_BOOL_PROP_OPT(argv[0], "swapXY", swapXY, false)
        CALL_IDF_API( esp_lcd_panel_swap_xy(that->handle, swapXY), "esp_lcd_panel_swap_xy() failed" )

        bool GET_BOOL_PROP_OPT(argv[0], "invertColor", invertColor, false)
        CALL_IDF_API( esp_lcd_panel_invert_color(that->handle, invertColor), "esp_lcd_panel_invert_color() failed" )

        bool GET_BOOL_PROP_OPT(argv[0], "mirrorX", mirrorX, false)
        bool GET_BOOL_PROP_OPT(argv[0], "mirrorY", mirrorY, false)
        CALL_IDF_API( esp_lcd_panel_mirror(that->handle, mirrorX, mirrorY), "esp_lcd_panel_mirror() failed" )

        return JS_UNDEFINED ;
    }

    bool Display::createBuff() {
        _buffSize = sizeof(color_t) * _width * 20;
        if(!this->_buff1) {
            this->_buff1 = heap_caps_malloc(_buffSize, MALLOC_CAP_SPIRAM);  // MALLOC_CAP_SPIRAM
            if(!this->_buff1) {
                return false ;
            }
        }
        if(!this->_buff2) {
            this->_buff2 = heap_caps_malloc(_buffSize, MALLOC_CAP_SPIRAM);
            if(!this->_buff2) {
                free(this->_buff1) ;
                return false ;
            }
        }
        return true ;
    }
    
    void * Display::buff1() const {
        return this->_buff1 ;
    }
    void * Display::buff2() const {
        return this->_buff2 ;
    }
    size_t Display::buffSize() const {
        return _buffSize ;
    }

    
    uint16_t RGB(uint8_t r,uint8_t g,uint8_t b) {
        r = r*((float)31/255) + 0.5 ;
        g = g*((float)63/255) + 0.5 ;
        b = b*((float)31/255) + 0.5 ;
        return RGB565(r,g,b) ;
    }
    uint16_t RGB565(uint8_t r,uint8_t g,uint8_t b) {
        return ((r&31)<<11) | ((g&63)<<5) | (b&31) ;
    }

    void toRGB(uint16_t value, uint8_t * r,uint8_t * g,uint8_t * b) {
        toRGB565(value,r,g,b) ;
        *r = (uint8_t) (((float)*r/31) * 255 + 0.5) ;
        *g = (uint8_t) (((float)*g/63) * 255 + 0.5) ;
        *b = (uint8_t) (((float)*b/31) * 255 + 0.5) ;
    }
    void toRGB565(uint16_t value, uint8_t * r,uint8_t * g,uint8_t * b) {
        *r = (value>>11) & 31 ;
        *g = (value>>5) & 63 ;
        *b = value & 31 ;
    }

    JSValue Display::RGB(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        ASSERT_ARGC(3)
        ARGV_TO_UINT8(0,r)
        ARGV_TO_UINT8(1,g)
        ARGV_TO_UINT8(2,b)
        return JS_NewUint32(ctx, be::driver::disp::RGB(r,g,b)) ;
    }
    JSValue Display::RGB565(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        ASSERT_ARGC(3)
        ARGV_TO_UINT8(0,r)
        ARGV_TO_UINT8(1,g)
        ARGV_TO_UINT8(2,b)
        return JS_NewUint32(ctx, be::driver::disp::RGB565(r,g,b)) ;
    }

    JSValue Display::toRGB(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        return JS_UNDEFINED ;
    }
    JSValue Display::toRGB565(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        return JS_UNDEFINED ;
    }
    
    void Display::fillRect(coord_t x1,coord_t y1,coord_t x2,coord_t y2,color_t color) {

        uint16_t line_size = x2-x1;
        uint16_t lines = y2-y1;

        uint16_t * buff = (uint16_t *)heap_caps_malloc(line_size*lines*sizeof(color_t), MALLOC_CAP_DMA) ;
        if(!buff) {
            buff = (uint16_t*) malloc(line_size*lines*sizeof(color_t)) ;
        }

        for(int i=0;i<line_size*lines;i++) {
            buff[i] = color ;
        }

        drawRect(x1, y1, x2+1, y2+1, (uint16_t*)buff) ;

        free(buff) ;
    }

    JSValue Display::pause(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        THIS_NCLASS(Display,that)
        that->playing = false ;
        return JS_UNDEFINED ;
    }
    JSValue Display::resume(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        THIS_NCLASS(Display,that)
        that->playing = true ;
        return JS_UNDEFINED ;
    }

    JSValue Display::reset(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        THIS_NCLASS(Display, that)
        CHECK_HANDLE
        return esp_lcd_panel_reset(that->handle) == ESP_OK? JS_TRUE: JS_FALSE ;
    }
    JSValue Display::init(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        THIS_NCLASS(Display, that)
        CHECK_HANDLE
        esp_lcd_panel_reset(that->handle) ;
        return esp_lcd_panel_init(that->handle) == ESP_OK? JS_TRUE: JS_FALSE ;
    }
    JSValue Display::off(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        THIS_NCLASS(Display, that)
        CHECK_HANDLE
        return esp_lcd_panel_disp_on_off(that->handle,true) == ESP_OK? JS_TRUE: JS_FALSE ;
    }
    JSValue Display::on(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        THIS_NCLASS(Display, that)
        CHECK_HANDLE
        return esp_lcd_panel_disp_on_off(that->handle,false) == ESP_OK? JS_TRUE: JS_FALSE ;
    }

    void Display::drawRect(coord_t x1,coord_t y1,coord_t x2,coord_t y2,const color_t * pixels) {
        if(!playing) {
            return ;
        }
        if(!handle) {
            return;
        }
        esp_lcd_panel_draw_bitmap(handle, x1, y1, x2+1, y2+1, pixels);
    }

    JSValue Display::setOffset(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        THIS_NCLASS(Display, that)
        CHECK_HANDLE
        ARGV_TO_INT16_OPT(0, x, 0)
        ARGV_TO_INT16_OPT(1, y, 0)
        CALL_IDF_API( esp_lcd_panel_set_gap(that->handle, x, y), "esp_lcd_panel_set_gap() failed" )
        return JS_UNDEFINED ;
    }
    JSValue Display::mirror(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        THIS_NCLASS(Display, that)
        CHECK_HANDLE
        bool x = false ;
        bool y = false ;
        if(argc>0) {
            x = JS_ToBool(ctx, argv[0]) ;
        }
        if(argc>1) {
            y = JS_ToBool(ctx, argv[1]) ;
        }
        CALL_IDF_API( esp_lcd_panel_mirror(that->handle, x, y), "esp_lcd_panel_mirror() failed" )
        return JS_UNDEFINED ;
    }
    JSValue Display::swapXY(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        THIS_NCLASS(Display, that)
        CHECK_HANDLE
        bool swap = true ;
        if(argc>0) {
            swap = JS_ToBool(ctx, argv[0]) ;
        }
        CALL_IDF_API( esp_lcd_panel_swap_xy(that->handle, swap), "esp_lcd_panel_swap_xy() failed" )
        return JS_UNDEFINED ;
    }
    JSValue Display::invertColor(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        THIS_NCLASS(Display, that)
        CHECK_HANDLE
        bool invert = true ;
        if(argc>0) {
            invert = JS_ToBool(ctx, argv[0]) ;
        }
        CALL_IDF_API( esp_lcd_panel_invert_color(that->handle, invert), "esp_lcd_panel_invert_color() failed" )
        return JS_UNDEFINED ;
    }
}