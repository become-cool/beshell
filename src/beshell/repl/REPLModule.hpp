#pragma once

// #include "BeShell.class.hpp"
#include "../NativeModule.hpp"


namespace be{

    class BeShell ;
    class REPLChannel ;
    class Package ;
    
	typedef void (*NativeModuleExportorFunc)(JSContext * ctx, NativeModule * module) ;

    class REPLModule: public be::EventModule {

        static std::vector<NativeModuleExportorFunc> exportors ;

    public:
        static char const * const name ;

        REPLModule(JSContext * ctx, const char * name) ;
        static void registerExportor(NativeModuleExportorFunc func) ;

        static JSValue enableCrypto(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue disableCrypto(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue setCryptoKey(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue setPassword(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue enableUnknownPkgEvent(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue disableUnknownPkgEvent(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;

        static REPLModule * singleton ;

        void exports(JSContext *ctx) ;

    private:
        static void unknownPkgRelay(REPLChannel * ch, std::unique_ptr<Package> pkg) ;

    } ;
}
