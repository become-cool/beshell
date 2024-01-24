#pragma once

#include "NativeObject.hpp"

namespace be {

    class I2S: public NativeObject {
    private:
        uint8_t busnum = 0 ;
    public:
        I2S(JSContext * ctx, uint8_t _busnum=0) ;
        static JSClassID classID ;
    } ;
}