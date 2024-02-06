#pragma once

#include "NativeClass.hpp"

namespace be {

    class I2C: public NativeClass<I2C> {
        DECLARE_NCLASS_META
    public:
        I2C(JSContext * ctx, uint8_t _busnum=0) ;
    } ;
}