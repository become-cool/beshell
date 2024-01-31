#pragma once

#include "deps/quickjs/quickjs-libc.h"
#include <map>
#include <vector>
#include <iostream>

#include <quickjs_private.h>
#define dclass(ctx,classID) printf("class(%d) addr: %p, exotic: %p @%d\n", classID, & ctx->rt->class_array[classID], ctx->rt->class_array[classID].exotic, __LINE__) ;

namespace be {

    class NativeClassDef ;

	typedef void (*NativeClassDefDefineFunc)(NativeClassDef *) ;


    template <typename T>
    class NativeClass {
    private:
    protected:
        JSContext * ctx ;
        NativeClassDef * nclass;
    
    protected:
        NativeClass(JSContext * ctx) {
            nclass = defineClass(ctx) ;
        }

        std::vector<JSCFunctionListEntry> methods ;
        std::vector<JSCFunctionListEntry> staticMethods ;

        static JSValue constructor(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static void finalizer(JSRuntime *rt, JSValue val) ;
    
    public:
        virtual ~NativeClass() ;

        JSValue jsobj ;
        inline static NativeClass * fromJSObject(JSValue obj) ;

        // static JSClassID classID ;
        inline static NativeClassDef* defineClass(JSContext * ctx) {
            if(T::mapCtxClasses.count(ctx)<1) {
                T::mapCtxClasses[ctx] = new be::NativeClassDef(
                    ctx, T::classID, T::className
                    , T::constructor
                    , T::methods
                    , T::staticMethods
                    , T::finalizer) ;
            }
            return T::mapCtxClasses[ctx] ;
        }
    } ;

    
    class NativeClassDef {
    private:
        JSClassID & classID ;
        JSContext * ctx ;
        JSValue proto = JS_NULL ;

    public:
        NativeClassDef(
            JSContext * ctx
            , JSClassID & classID
            , const char * name
            , JSCFunction * constructor = nullptr
            // , NativeClassDef * parent=nullptr
            , const std::vector<JSCFunctionListEntry> & methods = {}
            , const std::vector<JSCFunctionListEntry> & staticMethods = {}
            , JSClassFinalizer * finalizer = nullptr
        ) ;
        ~NativeClassDef() ;
        
        void setParent(JSContext * ctx, NativeClassDef * parent) ;

        JSValue newJSObject(JSContext * ctx) ;

        // friend class NativeClass ;
    } ;
}