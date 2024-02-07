#pragma once

#include "BeShell.hpp"
#include "NativeModule.hpp"

class DemoModule: public be::NativeModule {
public:
    DemoModule(JSContext * ctx, const char * name) ;

    void import() ;

    static void use(be::BeShell & beshell) ;

    static JSValue jsFunc(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
} ;
