#pragma once
#include "NativeModule.hpp"

namespace be {
    class GPIOModule: public NativeModule {
    private:
    public:
        using NativeModule::NativeModule;
        static NativeModule* factory(JSContext * ctx, const char * name) ;
        
        static JSValue jsMethod(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
    } ;
}