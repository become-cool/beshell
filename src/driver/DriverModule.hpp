#pragma once

#include "BeShell.hpp"
#include "NativeModule.hpp"
#include <vector>

namespace be::driver {

    class DriverModule ;
    
	typedef void (*DriverProvider)(DriverModule * dm) ;

    class DriverModule: public be::NativeModule {
    public:
        DriverModule(JSContext * ctx, const char * name) ;

        static std::vector<DriverProvider> providers ;
    } ;

}


        