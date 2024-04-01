#include "DriverModule.hpp"

namespace be::driver {

    std::vector<DriverProvider> DriverModule::providers ;

    DriverModule::DriverModule(JSContext * ctx, const char * name)
        : NativeModule(ctx, name, 0)
    {
        for(auto providerFunc : providers) {
            providerFunc(this) ;
        }
        
        exportName("device") ;
        exportFunction("loadDeviceFromJson", loadeDeviceFromJson, 0) ;
    }

    void DriverModule::import(JSContext *ctx) {
        exportValue("device", JS_NewObject(ctx)) ;
    }
    
    JSValue DriverModule::loadeDeviceFromJson(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        return JS_UNDEFINED ;
    }
}