#pragma once

#include "NativeObject.hpp"

namespace be {

    class SPI: public NativeObject {
    private:
        uint8_t busnum = 0 ;
    public:
        SPI(JSContext * ctx, uint8_t _busnum=0) ;
        static void defineClass(NativeClass *) ;
        static JSClassID classID ;
    } ;
}