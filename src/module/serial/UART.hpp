#pragma once

#include "NativeObject.hpp"

namespace be {

    class UART: public NativeObject {
    private:
        uint8_t busnum = 0 ;
    public:
        UART(JSContext * ctx, uint8_t _busnum=0) ;
        static void defineClass(NativeClass *) ;
        static JSClassID classID ;
    } ;
}