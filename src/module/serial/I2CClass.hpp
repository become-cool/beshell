#pragma once

#include "NativeClass.hpp"

namespace be {
    class I2CClass: public NativeClass {
    private:
    public:
        I2CClass() ;
        
        static JSClassID jsClassID ;
        static JSClassDef jsClassDef ;
    } ;
}