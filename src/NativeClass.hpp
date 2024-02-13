#pragma once

#include "quickjs/quickjs-libc.h"
#include <map>
#include <vector>
#include <cassert>
#include <iostream>
#include <memory>
#include <quickjs_private.h>
#include <string.h>
#include "qjs_utils.h"

#define DECLARE_NCLASS_META                                         \
    public:                                                         \
        static const char * className ;                             \
        JSValue build(JSContext * ctx, JSValue jsobj=JS_NULL) ;     \
        static JSValue defineClass(JSContext * ctx) ;               \
    protected:                                                      \
        static JSClassID classID ;                                  \
    friend class NativeClass ;

#define DEFINE_NCLASS_META(CLASS,PARENT_CLASS)                      \
    const char * CLASS::className = #CLASS ;                        \
    JSClassID CLASS::classID = 0 ;                                  \
    JSValue CLASS::build(JSContext * ctx, JSValue jsobj) {          \
        if(!JS_IsNull(jsobj)) {                                     \
            return jsobj ;                                          \
        }                                                           \
        CLASS::defineClass(ctx) ;                                   \
        return JS_NewObjectClass(ctx, CLASS::classID) ;             \
    }                                                               \
    JSValue CLASS::defineClass(JSContext * ctx) {                   \
        if(PARENT_CLASS::classID==0) {                              \
            PARENT_CLASS::defineClass(ctx) ;                        \
        }                                                           \
        return NativeClass::defineClass(                            \
                ctx,CLASS::classID,CLASS::className                 \
                , CLASS::methods, CLASS::staticMethods              \
                , CLASS::constructor                                \
                , CLASS::finalizer                                  \
                , PARENT_CLASS::classID                             \
            ) ;                                                     \
    }


#define THIS_NCLASS(CLASS,var)                                      \
    if( !be::NativeClass::instanceOf<CLASS>(ctx,this_val) ) {       \
        THROW_EXCEPTION("invalid method")                           \
    }                                                               \
    CLASS * var = (CLASS *) fromJS(this_val) ;                      \
    if(!var) {                                                      \
        THROW_EXCEPTION("invalid method")                           \
    }

namespace be {
    
	typedef JSValue (*NClassConstructorFunc)(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
	typedef void (*NClassFinalizerFunc)(JSRuntime *rt, JSValue val) ;

    class NativeClass {
    protected:
        JSContext * ctx ;
        std::shared_ptr<NativeClass> self ;

        static std::vector<JSCFunctionListEntry> methods ;
        static std::vector<JSCFunctionListEntry> staticMethods ;
        static void finalizer(JSRuntime *rt, JSValue val) ;

    protected :

        NativeClass(JSContext * ctx, JSValue jsobj) ;

        static std::map<JSContext*, std::map<JSClassID, JSValue>> mapCtxClassID2Proto ;
        static const JSClassID classID ;

    public:
        JSValue jsobj ;

        virtual ~NativeClass() ;


        static NativeClass * fromJS(JSValue jsObj) ;

        static JSValue defineClass(
                JSContext * ctx
                , JSClassID & classID
                , const char * className
                , const std::vector<JSCFunctionListEntry>& methods
                , const std::vector<JSCFunctionListEntry>& staticMethods
                , NClassConstructorFunc constructor
                , NClassFinalizerFunc finalizer=NativeClass::finalizer
                , JSClassID parentClassID=0
        ) ;
        static JSValue defineClass(JSContext * ctx);

        template <typename N>
        inline static bool instanceOf(JSContext * ctx, JSValue jsobj) {
            JSValue proto = JS_GetClassProto(ctx, N::classID);
            JSValue cotr = JS_GetPropertyStr(ctx, proto, "constructor") ;
            return JS_IsInstanceOf(ctx, jsobj, cotr) ;
        }
    } ;

    
}