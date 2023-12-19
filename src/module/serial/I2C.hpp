#pragma once

#include "module/NativeObject.hpp"

namespace be {

    class I2C: public NativeObject {
    private:
        uint8_t busnum = 0 ;
    public:
        I2C(JSContext * ctx, uint8_t _busnum=0) ;
        static void defineClass(NativeClass *) ;
        static JSClassID classID ;
    } ;
}