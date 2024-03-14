#include "DeviceModule.hpp"
#include "qjs_utils.h"

namespace be{

    const char * DeviceModule::deviceJsonPath = nullptr ;

    DeviceModule::DeviceModule(JSContext * ctx, const char * name)
        : NativeModule(ctx, name, 0)
    {
        exportName("deviceJsonPath") ;
        exportName("device") ;
    }

    void DeviceModule::import(JSContext *ctx) {
        exportValue("deviceJsonPath", JS_NewString(ctx, deviceJsonPath)) ;
        exportValue("device", JS_NewObject(ctx)) ;
    }

    void DeviceModule::setDeviceJsonPath(const char * path) {
        deviceJsonPath = path ;
    }
}