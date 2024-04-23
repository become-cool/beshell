#pragma once

#include "BeShell.hpp"
#include "NativeModule.hpp"

namespace be{
    class DeviceModule: public be::NativeModule {

    private:
        static const char * deviceJsonPath ;
    
    public:
        static const char * name ;
        DeviceModule(JSContext * ctx, const char * name) ;

        void import(JSContext *ctx) ;

        static void setDeviceJsonPath(const char * path) ;
    } ;
}
