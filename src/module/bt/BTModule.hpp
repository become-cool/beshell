#pragma once

#include "BeShell.hpp"
#include "NativeModule.hpp"

namespace be {

    class BTModule: public be::NativeModule {
    public:
        static const char * const name ;
        BTModule(JSContext * ctx, const char * name) ;

        static JSValue jsFunc(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
    } ;

}