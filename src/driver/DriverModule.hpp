#pragma once

#include "NativeModule.hpp"
#include <vector>

namespace be::driver {

    class DriverModule ;
    
	typedef void (*DriverProvider)(DriverModule * dm) ;

    class DriverModule: public be::NativeModule {
    private:
        static std::vector<DriverProvider> providers ;
        
        template <typename D>
        static void provider(DriverModule *dm) {
            dm->exportClass<D>();
        }
    public:
        DriverModule(JSContext * ctx, const char * name) ;

        void import(JSContext *ctx) ;

        template <typename D>
        static void useDriver(BeShell * beshell) {
            providers.push_back(provider<D>);
        }

        static JSValue loadeDeviceFromJson(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;

    } ;

}


        