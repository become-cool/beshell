#pragma once
#include "module/NativeModule.hpp"

namespace be {
    class Process: public NativeModule {
    private:
    public:
        Process() ;
        
        static JSValue reboot(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
    } ;
}