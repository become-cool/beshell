#pragma once

#include <iostream>
#include "BeShell.hpp"
#include "NativeClass.hpp"
#include "module/NativeModule.hpp"

using namespace std ;
using namespace be ;


class DemoClass: public NativeClass<DemoClass> {
    NATIVE_CLASS_META
    // std::vector<JSCFunctionListEntry> NativeClass<DemoClass>::methods = {};
    // std::vector<JSCFunctionListEntry> NativeClass<DemoClass>::staticMethods ={};

public:
    DemoClass(JSContext * ctx, int a=0): NativeClass(ctx) {
        dd
    }

} ;




void useDemo(JSContext * ctx) ;