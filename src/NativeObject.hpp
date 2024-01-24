#pragma once

#include "deps/quickjs/quickjs-libc.h"
#include <map>
#include <vector>
#include <iostream>

namespace be {
    class NativeObject ;
    class NativeClass ;

	typedef void (*NativeClassDefineFunc)(NativeClass *) ;

    class NativeClass {
    private:
        JSClassID & classID ;
        JSContext * ctx ;
        JSValue proto = JS_NULL ;
    public:
        NativeClass(
            JSContext * ctx
            , JSClassID & classID
            , const char * name
            , JSCFunction * constructor = nullptr
            , JSClassFinalizer * finalizer = nullptr
            , const JSCFunctionListEntry * methods=nullptr, int methods_size=0
            , const JSCFunctionListEntry * staticMethods=nullptr, int staticMethods_size=0
        ) ;
        ~NativeClass() ;
        
        void setParent(JSContext * ctx, NativeClass * parent) ;

        JSValue newJSObject(JSContext * ctx) ;

        static NativeClass * fromClassID(JSContext * ctx, JSClassID & classID) ;
        static void registerClass(JSContext * ctx, JSClassID classID , const char * name , NativeClass * parent=nullptr) ;

        friend class NativeObject ;
    private:
        static std::map<JSContext*, std::map<JSClassID, NativeClass *>> mapClasses ;

    } ;


    class NativeObject {
    private:
    protected:
        NativeClass * nclass;
        JSContext * ctx ;
        
        virtual void constructor(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst * argv) ;
        virtual void finalize(JSRuntime *rt, JSValue val) ;
    public:
        NativeObject(
            JSContext * _ctx
            , JSClassID classID
            , const char * name
            , const JSCFunctionListEntry * methods=nullptr, int methods_size=0
            , const JSCFunctionListEntry * staticMethods=nullptr, int staticMethods_size=0
            , NativeObject * parent=nullptr
            , NativeClassDefineFunc funcDefineClass=nullptr
        ) ;

        virtual ~NativeObject() ;

        JSValue jsobj ;
        
        inline static NativeObject * fromJSObject(JSValue obj) ;

        private:
            static JSValue jsConstructor(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
            static void jsFinalizer(JSRuntime *rt, JSValue val) ;

    } ;
}