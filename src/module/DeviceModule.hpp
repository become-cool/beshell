#pragma once

#include "BeShell.hpp"
#include "NativeModule.hpp"

namespace be{
    class DeviceModule: public be::NativeModule {

    private:
        static const char * deviceJsonPath ;
    
    public:
        DeviceModule(JSContext * ctx, const char * name) ;

        void import(JSContext *ctx) ;

        inline static void use(be::BeShell & beshell) {
            beshell.addModule<DeviceModule>("device") ;
        }

        static void setDeviceJsonPath(const char * path) ;

        static JSValue jsFunc(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
    } ;
}
