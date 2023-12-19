#pragma once
#include "module/NativeModule.hpp"

namespace be {
    class ProcessModule: public NativeModule {
    private:
    protected:
        void defineExports() ;
    public:
        ProcessModule() ;
        
        static JSValue reboot(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
    } ;
}