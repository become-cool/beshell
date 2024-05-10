#pragma once

#include <BeShell.hpp>
#include <NativeClass.hpp>
#ifdef MODULE_LV
#include "deps/lvgl/lvgl.h"
#endif

namespace be::driver::display {

    #define coord_t int16_t
    #define color_t uint16_t

    class DisplayModule ;

    class Display: public be::NativeClass {
        DECLARE_NCLASS_META

    protected:
        static std::vector<JSCFunctionListEntry> methods ;

        uint16_t _width = 0 ;
        uint16_t _height = 0 ;

#ifdef MODULE_LV
        void * buff1 = nullptr ;
        void * buff2 = nullptr ;
        lv_display_t * lv_display = nullptr ;
#endif

    public:
        Display(JSContext * ctx, JSValue jsobj=JS_NULL, uint16_t width=0, uint16_t height=0) ;

        uint16_t width() const ;
        uint16_t height() const ;

#ifdef MODULE_LV
        virtual bool createBuff() ;
        virtual bool registerToLV() ;
#endif

        static JSValue constructor(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        
        virtual void drawRect(coord_t x1,coord_t y1,coord_t x2,coord_t y2,color_t * pixels) = 0;

        static JSValue jsDrawRect(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;

    friend class DisplayModule ;
    } ;

}