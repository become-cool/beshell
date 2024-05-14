#pragma once

#include "BeShell.hpp"
#include "NativeModule.hpp"

namespace be{
    class DeviceTree: public be::NativeModule {

    private:
        static const char * deviceJsonPath ;

    public:
        static const char * name ;
        DeviceTree(JSContext * ctx, const char * name) ;

        void import(JSContext *ctx) ;

        static void setDeviceJsonPath(const char * path) ;
    } ;
}
