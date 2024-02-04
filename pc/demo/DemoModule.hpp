#pragma once

#include "BeShell.hpp"
#include "NativeModule.hpp"



class DemoModule: public be::NativeModule {
public:
    DemoModule(JSContext * ctx, const char * name)
        : NativeModule(ctx, name, 0)
    {
    }

    static be::NativeModule* factory(JSContext * ctx, const char * name) {
        return new DemoModule(ctx,name,1) ;
    }

    static void use(be::BeShell & beshell) {

    }
} ;


void useDemo(be::BeShell & beshell, JSContext * ctx) ;