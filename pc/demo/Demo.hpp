#pragma once

#include <iostream>
#include "BeShell.hpp"
#include "NativeObject.hpp"
#include "module/NativeModule.hpp"


class DemoClass: public be::NativeObject {
public:

    static JSClassID classID ;
    inline static be::NativeClass* defineClass(JSContext * ctx) {
        if(mapCtxClasses.count(ctx)<1) {
            mapCtxClasses[ctx] = new be::NativeClass(
                ctx, classID, "DemoClass"
                , DemoClass::constructor
                , nullptr
                , {}) ;
        }
        dn(classID)
        dp(DemoClass::constructor)
        return mapCtxClasses[ctx] ;
    }


    DemoClass(JSContext * ctx):
        NativeObject(ctx, defineClass(ctx))
    {
        
    }

    static void create(JSContext * ctx) {
        dd
        JSValue jsobj = defineClass(ctx)->newJSObject(ctx) ;
        dn(classID)
    }

private:
    static std::map<JSContext*, be::NativeClass*> mapCtxClasses ;
    static JSValue constructor(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;


} ;

class DemoModule: public be::NativeModule {

} ;

void useDemo(JSContext * ctx) ;