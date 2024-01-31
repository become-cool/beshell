#pragma once

#include "NativeObject.hpp"
#include <map>

namespace be {

    class I2C: public NativeObject {
    private:
        uint8_t busnum = 0 ;
        static std::map<JSContext*, NativeClass*> mapCtxClasses ;

    protected:
    public:
        I2C(JSContext * ctx, uint8_t _busnum=0) ;
        inline static NativeClass* defineClass(JSContext * ctx) ;
        static JSClassID classID ;
    } ;
}