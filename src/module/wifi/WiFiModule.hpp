#pragma once

#include "BeShell.hpp"
#include "NativeModule.hpp"

namespace be {
    class WiFiModule: public be::NativeModule {
    public:
        WiFiModule(JSContext * ctx, const char * name) ;

        // void import(JSContext *ctx) ;

        inline static void use(be::BeShell & beshell) {
            beshell.addModule<WiFiModule>("wifi") ;
        }

        static JSValue jsFunc(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
    } ;
}