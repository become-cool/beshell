#pragma once
#include "module/NativeModule.hpp"

namespace be {
    class ProcessModule: public NativeModule {
    private:
    protected:
        void defineExports() ;
    public:
        using NativeModule::NativeModule;
        static NativeModule* factory(JSContext * ctx, const char * name) ;
        
        static JSValue reboot(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
    } ;
}