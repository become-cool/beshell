#pragma once
#include "NativeModule.hpp"

namespace be {
    class ProcessModule: public NativeModule {
    public:
        ProcessModule(JSContext * ctx, const char * name,uint8_t flagGlobal) ;
        static NativeModule* factory(JSContext * ctx, const char * name) ;
        
        static JSValue reboot(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
    } ;
}