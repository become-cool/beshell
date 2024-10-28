#include <iostream>
#include <algorithm>
#include "Display.hpp"
#include "NativeClass.hpp"
#include "qjs_utils.h"
#include "esp_heap_caps.h"

using namespace std ;


namespace be::driver::disp {

    DEFINE_NCLASS_META(Display, NativeClass)
    std::vector<JSCFunctionListEntry> Display::methods = {
        JS_CFUNC_DEF("drawRect", 0, Display::drawRect),
        JS_CFUNC_DEF("fillRect", 0, Display::fillRect),
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

    JSValue Display::drawRect(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        Display * disp = (Display *)fromJS(this_val) ;
        if(!disp) {
            JSTHROW("not a valid object")
        }
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

        disp->drawRect(x1,y1,x2,y2,buff) ;

        delete[] buff ;

        return JS_UNDEFINED ;
    }


    JSValue Display::fillRect(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        THIS_NCLASS(Display, that)
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

    bool Display::createBuff() {
        _buffSize = sizeof(color_t) * _width * (_height+5)/10;
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

        uint16_t line_size = x2-x1+1;
        uint8_t * buff = (uint8_t *)heap_caps_malloc(line_size*2, MALLOC_CAP_DMA) ;
        int index = 0;
        for(int i=0;i<line_size;i++) {
            buff[index++] = (color >> 8) & 0xFF;
            buff[index++] = color & 0xFF;
        }

        drawRect(x1, y1, x2, y2, (uint16_t*)buff) ;

        free(buff) ;
    }
}