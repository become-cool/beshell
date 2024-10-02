#include "DriverModule.hpp"
#include "I2CDevice.hpp"

namespace be::driver {

    std::vector<NClassProvider> DriverModule::providers ;
    std::map<std::string, ExtProvider> DriverModule::extProviders ;

    DriverModule::DriverModule(JSContext * ctx, const char * name)
        : NativeModule(ctx, name, 0)
    {
        for(auto providerFunc : providers) {
            providerFunc(this) ;
        }
        exportClass<I2CDevice>() ;

        for (const auto& pair : extProviders) {
            exportName(pair.first.c_str()) ;
        }
    }

    void DriverModule::exports(JSContext * ctx) {
        for (const auto& pair : extProviders) {
            pair.second(ctx, this) ;
        }
    }

}