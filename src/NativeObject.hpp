#pragma once

#include "deps/quickjs/quickjs-libc.h"
#include <map>
#include <vector>
#include <iostream>

namespace be {
    class NativeObject ;
    class NativeClass {
    private:
        JSClassID & classID ;
        JSContext * ctx ;
        JSValue constructor = JS_NULL ;
        JSValue proto = JS_NULL ;
    public:
        NativeClass(
            JSContext * ctx
            , const char * name
            , JSClassID & classID
            , JSClassFinalizer * finalizer
        ) ;
        ~NativeClass() ;
        
        void setParent(JSContext * ctx, NativeClass * parent) ;

        static NativeClass * fromClassID(JSContext * ctx, JSClassID & classID) ;

        JSValue newJSObject(JSContext * ctx) ;

        friend class NativeObject ;
    private:
        static std::map<JSContext*, std::map<JSClassID, NativeClass *>> mapClasses ;

    } ;


	typedef void (*NativeClassDefineFunc)(NativeClass *) ;

    class NativeObject {
    private:
    protected:
        NativeClass * nclass;
        JSContext * ctx ;
        JSValue jsobj ;
    public:
        NativeObject(
            JSContext * _ctx
            , JSClassID classID
            , const char * name
            , NativeClassDefineFunc funcDefineClass=nullptr
            , NativeObject * parent=nullptr
        ) ;
        
        ~NativeObject() ;
        
        virtual void constructor(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst * argv) ;
        virtual void finalize(JSRuntime *rt, JSValue val) ;
        inline static NativeObject * fromJSObject(JSValue obj) ;

        private:
            static JSValue jsConstructor(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
            static void jsFinalizer(JSRuntime *rt, JSValue val) ;

    } ;
}