#pragma once

#include <stdint.h>

namespace be {
    class NVS {
    private:
    public:
        NVS() ;
        void readOneTime(const char * key, uint8_t * value) const ;
    } ;
}