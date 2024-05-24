#pragma once

#include "deps/quickjs/quickjs-libc.h"
#include <map>
#include <vector>
#include <cassert>
#include <iostream>
#include <memory>
#include <quickjs_private.h>
#include <string.h>
#include "qjs_utils.h"

namespace be {
    
	typedef JSValue (*NClassConstructorFunc)(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
	typedef void (*NClassFinalizerFunc)(JSRuntime *rt, JSValue val) ;

    class NativeClass {
    protected:
        JSContext * ctx ;
        std::shared_ptr<NativeClass> self ;

        static std::vector<JSCFunctionListEntry> methods ;
        static std::vector<JSCFunctionListEntry> staticMethods ;
        static JSValue constructor(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static void finalizer(JSRuntime *rt, JSValue val) ;

    protected :

        NativeClass(JSContext * ctx, JSValue jsobj) ;

        static std::map<JSContext*, std::map<JSClassID, JSValue>> mapCtxClassID2Constructor ;
        static const JSClassID classID ;

    public:
        JSValue jsobj ;

        virtual ~NativeClass() ;
        const std::shared_ptr<NativeClass> & shared() ;

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

        static JSValue getClass(JSContext * ctx, const JSClassID & classID) ;

        static JSValue newObject(JSContext * ctx, JSValue constructor) ;

        template <typename N>
        inline static bool instanceOf(JSContext * ctx, JSValue jsobj) {
            JSValue proto = JS_GetClassProto(ctx, N::classID);
            JSValue cotr = JS_GetPropertyStr(ctx, proto, "constructor") ;
            return JS_IsInstanceOf(ctx, jsobj, cotr) ;
        }
    } ;    
}

#define DECLARE_NCLASS_META                                             \
    public:                                                             \
        static const char * className ;                                 \
        static JSValue build(JSContext * ctx) ;                         \
        static JSValue build(JSContext * ctx, JSValue jsobj) ;          \
        static JSValue defineClass(JSContext * ctx) ;                   \
        static JSClassID classID ;                                      \
    friend class NativeClass ;                                          \
    private:

#define DEFINE_NCLASS_META_STATIC(CLASS)                                \
    const char * CLASS::className = #CLASS ;                            \
    JSClassID CLASS::classID = 0 ;

#define DEFINE_NCLASS_META_BUILD(CLASS)                                 \
    JSValue CLASS::build(JSContext * ctx) {                             \
        CLASS::defineClass(ctx) ;                                       \
        return JS_NewObjectClass(ctx, CLASS::classID) ;                 \
    }                                                                   \
    JSValue CLASS::build(JSContext * ctx, JSValue jsobj) {              \
        if(!JS_IsNone(jsobj)) {                                         \
            return jsobj ;                                              \
        }                                                               \
        return CLASS::build(ctx) ;                                      \
    }                                                                   \

#define DEFINE_NCLASS_META_DEFINECLASS(CLASS,PARENT_CLASS)              \
    JSValue CLASS::defineClass(JSContext * ctx) {                       \
        if(PARENT_CLASS::classID==0) {                                  \
            PARENT_CLASS::defineClass(ctx) ;                            \
        }                                                               \
        return NativeClass::defineClass(                                \
                ctx,CLASS::classID,CLASS::className                     \
                , CLASS::methods, CLASS::staticMethods                  \
                , CLASS::constructor                                    \
                , CLASS::finalizer                                      \
                , PARENT_CLASS::classID                                 \
            ) ;                                                         \
    }

#define DEFINE_NCLASS_META(CLASS,PARENT_CLASS)                          \
    DEFINE_NCLASS_META_STATIC(CLASS)                                    \
    DEFINE_NCLASS_META_BUILD(CLASS)                                     \
    DEFINE_NCLASS_META_DEFINECLASS(CLASS,PARENT_CLASS)

#define CHECK_NCLASS_ARGV(CLASS,jsval)                                  \
    if( !be::NativeClass::instanceOf<CLASS>(ctx,jsval) ) {              \
        JSTHROW("value is not a %s object", #CLASS)                     \
    }

#define JSVALUE_TO_NCLASS(CLASS, jsval, var)                            \
    CHECK_NCLASS_ARGV(CLASS,jsval)                                      \
    CLASS * var = (CLASS *) be::NativeClass::fromJS(jsval) ;            \
    if(!var) {                                                          \
        JSTHROW("invalid native pointer")                               \
    }
#define JSVALUE_TO_NCLASS_VAR(CLASS, jsval, var)                        \
    CHECK_NCLASS_ARGV(CLASS,jsval)                                      \
    var = (CLASS *) be::NativeClass::fromJS(jsval) ;                    \
    if(!var) {                                                          \
        JSTHROW("invalid native pointer")                               \
    }

#define THIS_NCLASS(CLASS,var)                                          \
    JSVALUE_TO_NCLASS(CLASS,this_val,var)
