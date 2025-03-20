#pragma once

#include "BeShell.hpp"
#include "NativeModule.hpp"

namespace be{
    class DeviceTree: public be::NativeModule {

    private:
        static const char * deviceJsonPath ;

    public:
        static char const * const name ;
        DeviceTree(JSContext * ctx, const char * name) ;

        void exports(JSContext *ctx) ;

        static void setDeviceJsonPath(const char * path) ;
    } ;
}
