#pragma once

#include <NativeClass.hpp>
#include "esp_lcd_types.h"

namespace be::driver::disp {

    #define coord_t int16_t
    #define color_t uint16_t

    class Display: public be::NativeClass {
        DECLARE_NCLASS_META

    public:
        Display(JSContext * ctx, JSValue jsobj=JS_NULL, uint16_t width=0, uint16_t height=0) ;

        uint16_t width() const ;
        uint16_t height() const ;

        virtual bool createBuff() ;
        void * buff1() const ;
        void * buff2() const ;
        size_t buffSize() const ;

        static JSValue constructor(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        
        static JSValue drawRect(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue fillRect(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue RGB(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue RGB565(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue toRGB(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue toRGB565(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        
        static JSValue init(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue reset(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue off(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue on(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue pause(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue resume(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue setOffset(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue invertColor(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue mirror(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue swapXY(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;


        virtual void drawRect(coord_t x1,coord_t y1,coord_t x2,coord_t y2,const color_t * pixels) ;
        void fillRect(coord_t x1,coord_t y1,coord_t x2,coord_t y2,color_t color) ;

        bool _usedForLVGL = false ;


    protected:
        esp_lcd_panel_handle_t handle = nullptr;
        bool playing = true ;
        
        static std::vector<JSCFunctionListEntry> methods ;
        static std::vector<JSCFunctionListEntry> staticMethods ;

        uint16_t _width = 0 ;
        uint16_t _height = 0 ;

        void * _buff1 = nullptr ;
        void * _buff2 = nullptr ;
        size_t _buffSize = 0 ;
    } ;

    uint16_t RGB(uint8_t r,uint8_t g,uint8_t b) ;
    uint16_t RGB565(uint8_t r,uint8_t g,uint8_t b) ;
    void toRGB(uint16_t value, uint8_t * r,uint8_t * g,uint8_t * b) ;
    void toRGB565(uint16_t value, uint8_t * r,uint8_t * g,uint8_t * b) ;
}