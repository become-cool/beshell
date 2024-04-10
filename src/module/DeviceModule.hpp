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
            beshell.useModule<DeviceModule>("device") ;
        }

        static void setDeviceJsonPath(const char * path) ;
    } ;
}
