#pragma once

#include "BeShell.hpp"
#include "NativeModule.hpp"

namespace be{
    
	typedef void (*NativeModuleExportorFunc)(JSContext * ctx, NativeModule * module) ;

    class TelnetModule: public be::EventModule {

        static std::vector<NativeModuleExportorFunc> exportors ;

    public:
        static char const * const name ;

        TelnetModule(JSContext * ctx, const char * name) ;

        void exports(JSContext *ctx) ;

    } ;
}
