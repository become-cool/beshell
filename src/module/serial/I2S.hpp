#pragma once

#include <NativeClass.hpp>
#include "driver/i2s.h"

namespace be {
    class I2S: public be::NativeClass {
        DECLARE_NCLASS_META
    private:
        static std::vector<JSCFunctionListEntry> methods ;
        // static std::vector<JSCFunctionListEntry> staticMethods ;

        static I2S * i2s0 ;

        #if SOC_I2S_NUM > 1
        static I2S * i2s1 ;
        #endif
        
        i2s_port_t busnum ;

    public:
        I2S(JSContext * ctx, i2s_port_t busnum) ;

        static I2S * flyweight(JSContext *, i2s_port_t) ;

        static JSValue setup(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
    
    } ;
}