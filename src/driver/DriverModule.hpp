#pragma once

#include "BeShell.hpp"
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
        
        template <typename D>
        static void use() {
            providers.push_back(provider<D>);
        }
    } ;

}


        