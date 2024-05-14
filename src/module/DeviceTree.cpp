#include "DeviceTree.hpp"
#include "qjs_utils.h"
#include "js/device.c"

namespace be{

    const char * DeviceTree::name = "dt" ;

    const char * DeviceTree::deviceJsonPath = nullptr ;

    DeviceTree::DeviceTree(JSContext * ctx, const char * name)
        : NativeModule(ctx, name, 0)
    {
        exportName("deviceJsonPath") ;
        exportName("device") ;
        exportName("load") ;
    }

    void DeviceTree::import(JSContext *ctx) {
        exportValue("deviceJsonPath", JS_NewString(ctx, deviceJsonPath)) ;
        exportValue("device", JS_NewObject(ctx)) ;

        JSEngineEvalEmbeded(ctx, device)
    }

    void DeviceTree::setDeviceJsonPath(const char * path) {
        deviceJsonPath = path ;
    }
}


