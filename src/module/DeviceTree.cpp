#include "DeviceTree.hpp"
#include "qjs_utils.h"
#include "js/device.c"

namespace be{

    char const * const DeviceTree::name = "dt" ;

    const char * DeviceTree::deviceJsonPath = nullptr ;

    DeviceTree::DeviceTree(JSContext * ctx, const char * name)
        : NativeModule(ctx, name, 0)
    {
        exportName("device") ;
        exportName("load") ;
        exportName("registerDriver") ;
        exportName("unregisterDriver") ;
    }

    void DeviceTree::exports(JSContext *ctx) {
        exportValue("device", JS_NewObject(ctx)) ;
        
        JSEngineEvalEmbeded(ctx, device)
    }

    void DeviceTree::setDeviceJsonPath(const char * path) {
        deviceJsonPath = path ;
    }
}


