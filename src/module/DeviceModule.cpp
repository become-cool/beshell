#include "DeviceModule.hpp"

namespace be{

    const char * DeviceModule::deviceJsonPath = nullptr ;

    DeviceModule::DeviceModule(JSContext * ctx, const char * name)
        : NativeModule(ctx, name, 1)
    {
        exportName("deviceJsonPath") ;
    }

    void DeviceModule::import(JSContext *ctx) {
        exportValue("deviceJsonPath", JS_NewString(ctx, deviceJsonPath)) ;
    }

    void DeviceModule::setDeviceJsonPath(const char * path) {
        deviceJsonPath = path ;
    }
}