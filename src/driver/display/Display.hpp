#pragma once

#include <cstdint>

#ifdef PLATFORM_ESP32
#define cord_t uint16_t ; 
#else
#define cord_t uint32_t ; 
#endif


namespace be {
namespace driver {
namespace display {

    class Display {
    private:
    public:
        virtual void drawPixel(cord_t x, cord_t y, uint8_t r, uint8_t g, uint8_t b) = 0 ;
        virtual void drawRect(cord_t x1, cord_t y1,cord_t x2, cord_t y2, ) = 0 ;
    } ;

}}}