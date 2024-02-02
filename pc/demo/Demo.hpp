#pragma once

#include <iostream>
#include <BeShell.hpp>
#include "BeShell.hpp"
#include "NativeClass.hpp"
#include "NativeModule.hpp"

using namespace std ;
using namespace be ;


class DemoClass: public NativeClass<DemoClass> {
    DECLARE_NCLASS_META
    // std::vector<JSCFunctionListEntry> NativeClass<DemoClass>::methods = {};
    // std::vector<JSCFunctionListEntry> NativeClass<DemoClass>::staticMethods ={};

public:
    DemoClass(JSContext * ctx, int a=0): NativeClass(ctx) {

    }

} ;



class DemoModule: public NativeModule {
public:
    DemoModule(JSContext * ctx, const char * name,uint8_t flagGlobal=1)
        : NativeModule(ctx, name, flagGlobal)
    {

    }
    static NativeModule* factory(JSContext * ctx, const char * name) {
        return new DemoModule(ctx,name,1) ;
    }
} ;

void useDemo(BeShell & beshell, JSContext * ctx) ;