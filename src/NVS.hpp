#pragma once

#include <stdint.h>

namespace beshell {
    class NVS {
    private:
    public:
        NVS() ;
        void readOneTime(const char * key, uint8_t * value) const ;
    } ;
}