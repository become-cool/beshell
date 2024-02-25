#pragma once

#include "BeShell.hpp"
#include "NativeModule.hpp"

namespace be {

    class BTModule: public be::NativeModule {
    public:
        BTModule(JSContext * ctx, const char * name) ;

        // void import(JSContext *ctx) ;

        inline static void use(be::BeShell & beshell) {
            beshell.addModule<BTModule>("bt") ;
        }

        static JSValue jsFunc(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
    } ;

}