#pragma once

#include <cstdint>
#include "NativeObject.hpp"

#ifdef PLATFORM_ESP32
#define coord_t uint16_t
#else
#define coord_t uint32_t
#endif


namespace be {
namespace driver {
namespace display {

    class Display: public NativeObject {
    private:
    public:
        // Display(
        //     JSContext * _ctx
        //     , JSClassID classID
        //     , const char * name
        //     , NativeObject * parent=nullptr ) ;
        virtual void drawPixel(coord_t x, coord_t y, uint8_t r, uint8_t g, uint8_t b) = 0 ;
        virtual void drawRect(coord_t x1, coord_t y1,coord_t x2, coord_t y2, const uint8_t * buff) = 0 ;
    } ;

}}}