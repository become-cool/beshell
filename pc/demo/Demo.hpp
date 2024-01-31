#pragma once

#include <iostream>
#include "BeShell.hpp"
#include "NativeClass.hpp"
#include "module/NativeModule.hpp"

using namespace std ;
using namespace be ;

template <typename T>
class A {
public:
    void xxx() {
        T::out() ;
    }
} ;


class B: public A<B> {
protected:
    static void out() {
        std::cout << "out()()()" << std::endl;
    }
    
    friend class A ;
} ;


class DemoClass: public NativeClass<DemoClass> {
public:




    // DemoClass(JSContext * ctx):
    //     NativeObject(ctx, defineClass(ctx))
    // {
        
    // }

    // static void create(JSContext * ctx) {
    //     JSValue jsobj = defineClass(ctx)->newJSObject(ctx) ;
    // }

public:
    static JSClassID classID ;
    static const char * className ;
    static std::map<JSContext*, be::NativeClass<DemoClass>*> mapCtxClasses ;

    // static JSValue constructor(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;


} ;

// class DemoModule: public be::NativeModule {

// } ;

void useDemo(JSContext * ctx) ;