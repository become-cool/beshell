#pragma once

#include "BeShell.hpp"
#include "Demo.hpp"
#include "NativeModule.hpp"



class DemoModule: public be::NativeModule {
public:
    DemoModule(JSContext * ctx, const char * name)
        : NativeModule(ctx, name, 1)
    {
        dd
        exportClass<Demo>() ;
    }

    void import(JSContext *ctx, JSModuleDef *m) {
    }

    static void use(be::BeShell & beshell) {
        beshell.engine->mloader.add<DemoModule>();
    }
} ;


void useDemo(be::BeShell & beshell, JSContext * ctx) ;