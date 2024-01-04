#pragma once
#include "NativeModule.hpp"

namespace be {
    class GPIOModule: public NativeModule {
    private:
    public:
        GPIOModule() ;
        
        static JSValue jsMethod(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
    } ;
}