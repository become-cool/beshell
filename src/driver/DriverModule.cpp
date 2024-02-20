#include "DriverModule.hpp"

namespace be::driver {

    std::vector<DriverProvider> DriverModule::providers ;

    DriverModule::DriverModule(JSContext * ctx, const char * name)
        : NativeModule(ctx, name, 0)
    {
        for(auto providerFunc : providers) {
            providerFunc(this) ;
        }
    }

}