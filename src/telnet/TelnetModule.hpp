#pragma once

#include "BeShell.hpp"
#include "NativeModule.hpp"

namespace be{
    
	typedef void (*NativeModuleExportorFunc)(JSContext * ctx, NativeModule * module) ;

    class TelnetModule: public be::NativeModule {

        static std::vector<NativeModuleExportorFunc> exportors ;
        static void exportBT(JSContext *ctx, NativeModule * module) ;

    public:
        static char const * const name ;

        TelnetModule(JSContext * ctx, const char * name) ;

        void exports(JSContext *ctx) ;

        static void useBLE() {
            exportors.push_back(exportBT) ;
        }

    } ;
}
