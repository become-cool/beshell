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

        static JSValue enableCrypto(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue disableCrypto(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue setCryptoKey(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;

        void exports(JSContext *ctx) ;

    } ;
}
