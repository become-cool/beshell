#pragma once

#include "deps/quickjs/quickjs-libc.h"
#include <map>
#include <vector>
#include <cassert>
#include <iostream>
#include <memory>
#include <quickjs_private.h>
#include <string.h>
#include "utils.h"

#define DECLARE_NCLASS_META                                 \
    protected:                                              \
        static JSClassID classID ;                          \
        static const char * className ;                     \
        static std::map<JSContext*, JSValue> mapCtxProtos ; \
    friend class NativeClass ;

#define DEFINE_NCLASS_META(CLASS)               \
    const char * CLASS::className = #CLASS ;    \
    JSClassID CLASS::classID = 0 ;              \
    std::map<JSContext*, JSValue> CLASS::mapCtxProtos ;

namespace be {

    template <typename T>
    class NativeClass {
    private:
        JSContext * ctx ;
        std::shared_ptr<T> self ;

    protected:

        static std::vector<JSCFunctionListEntry> methods ;
        static std::vector<JSCFunctionListEntry> staticMethods ;
        static JSValue constructor(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
            auto obj = new T(ctx) ;
            obj->self = std::shared_ptr<T> (obj) ;
            return JS_UNDEFINED ;
        }
        static void finalizer(JSRuntime *rt, JSValue val) {
            NativeClass<T> * obj = fromJSObject(val) ;
            if(obj) {
                obj->self = nullptr ;
                obj->jsobj = JS_UNDEFINED ;
            }
        }

    public:
        JSValue jsobj ;
        
        // for JS
        NativeClass(JSContext * _ctx, JSValue _jsobj)
            : ctx(_ctx)
            , jsobj(_jsobj)
        {
            JS_DupValue(ctx, jsobj) ;
        }
        // for c++
        NativeClass(JSContext * ctx) {
            if( T::classID<1 ) {
                defineClass(ctx) ;
                assert(T::classID>0) ;
            }
            NativeClass( ctx, JS_NewObjectClass(ctx, T::classID) ) ;
        }

        ~NativeClass() {
            if(!JS_IsUndefined(jsobj)) {
                JS_SetOpaque(jsobj, nullptr) ;
                JS_FreeValue(ctx,jsobj) ;
            }
        }

        static NativeClass<T> * fromJSObject(JSValue jsObj) {
            NativeClass * obj = (NativeClass *) JS_GetOpaqueInternal(jsObj) ;
            if(!obj) {
                return nullptr ;
            }
            if( JS_GetOpaqueClassID(jsObj)!=T::classID ) {
                return nullptr ;
            }
            return obj ;
        }

        static JSValue defineClass(JSContext * ctx) {
            if(T::mapCtxProtos.count(ctx)>0) {
                return T::mapCtxProtos[ctx] ;
            }
            
            JS_NewClassID(&T::classID);

            JSClassDef jsClassDef ;
            memset(&jsClassDef, 0, sizeof(JSClassDef)) ;

            jsClassDef.class_name = T::className ;
            jsClassDef.finalizer = T::finalizer ;
            JS_NewClass(JS_GetRuntime(ctx), T::classID, &jsClassDef);

            JSValue proto = JS_NewObject(ctx);
            JS_SetClassProto(ctx, T::classID, proto);

            if(T::methods.size()) {
                JS_SetPropertyFunctionList(ctx, proto, T::methods.data(), T::methods.size());
            }

            JSValue jscotr = JS_NewCFunction2(ctx, T::constructor, T::className, 1, JS_CFUNC_constructor, 0) ;
            JS_SetConstructor(ctx, jscotr, proto) ;

            if(T::staticMethods.size()) {
                JS_SetPropertyFunctionList(ctx, jscotr, T::staticMethods.data(), T::staticMethods.size());
            }
            
            JS_DupValue(ctx, proto) ;
            T::mapCtxProtos[ctx] = proto ;

            return proto ;
        }
        
        template <typename P>
        void setParent(JSContext * ctx, NativeClass<P> * parent) {            
            if(T::mapCtxProtos.count(ctx)<1 || P::mapCtxProtos.count(ctx)<1) {
                return ;
            }
            JSValue parentProto = P::mapCtxProtos[ctx] ;
            JS_DupValue(ctx, parentProto) ;

            JSValue proto = T::mapCtxProtos[ctx] ;
            JS_SetPropertyStr(ctx, proto, "__proto__", parentProto);
        }
    } ;

    
    template <typename T>
    std::vector<JSCFunctionListEntry> NativeClass<T>::methods ;
    
    template <typename T>
    std::vector<JSCFunctionListEntry> NativeClass<T>::staticMethods ;
}