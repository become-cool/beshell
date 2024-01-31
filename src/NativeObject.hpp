#pragma once

#include "deps/quickjs/quickjs-libc.h"
#include <map>
#include <vector>
#include <iostream>

#include <quickjs_private.h>
#define dclass(ctx,classID) printf("class(%d) addr: %p, exotic: %p @%d\n", classID, & ctx->rt->class_array[classID], ctx->rt->class_array[classID].exotic, __LINE__) ;

namespace be {
    class NativeObject ;
    class NativeClass ;

	typedef void (*NativeClassDefineFunc)(NativeClass *) ;

    class NativeClass {
    private:
        JSClassID & classID ;
        JSContext * ctx ;
        JSValue proto = JS_NULL ;
        
    protected:
        std::vector<JSCFunctionListEntry> methods ;
        std::vector<JSCFunctionListEntry> staticMethods ;

    public:
        NativeClass(
            JSContext * ctx
            , JSClassID & classID
            , const char * name
            , JSCFunction * constructor = nullptr
            , NativeClass * parent=nullptr
            , const std::vector<JSCFunctionListEntry> && methods = {}
            , const std::vector<JSCFunctionListEntry> && staticMethods = {}
            , JSClassFinalizer * finalizer = nullptr
        ) ;
        ~NativeClass() ;
        
        void setParent(JSContext * ctx, NativeClass * parent) ;

        JSValue newJSObject(JSContext * ctx) ;

        friend class NativeObject ;

    } ;


    class NativeObject {
    private:
    protected:
        JSContext * ctx ;
        NativeClass * nclass;
    
    protected:
        NativeObject(JSContext * ctx, NativeClass * nclass) ;
        NativeObject(JSContext * ctx, NativeClass * nclass, JSValue jsobj) ;

    public:
        virtual ~NativeObject() ;

        JSValue jsobj ;
        inline static NativeObject * fromJSObject(JSValue obj) ;

        private:
            static JSValue constructor(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
            static void finalizer(JSRuntime *rt, JSValue val) ;
    } ;
}