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

#define DECLARE_NCLASS_META                                 \
    public:                                                 \
        static const char * className ;                     \
    protected:                                              \
        static JSClassID classID ;                          \
        static std::map<JSContext*, JSValue> mapCtxConstructors ; \
    friend class NativeClass ;

#define DEFINE_NCLASS_META(CLASS)               \
    const char * CLASS::className = #CLASS ;    \
    JSClassID CLASS::classID = 0 ;              \
    std::map<JSContext*, JSValue> CLASS::mapCtxConstructors ;


namespace be {

    template <typename T, typename P=void>
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

    protected :
        NativeClass(JSContext * _ctx, JSValue _jsobj=JS_UNDEFINED)
            : ctx(_ctx)
        {
            if( JS_IsNone(_jsobj) ) {
                if( T::classID<1 ) {
                    defineClass(ctx) ;
                    assert(T::classID>0) ;
                }
                _jsobj = JS_NewObjectClass(ctx, T::classID) ;
            }

            JS_DupValue(ctx, jsobj) ;
            jsobj = _jsobj ;
        }

    public:
        JSValue jsobj ;

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
            if(T::mapCtxConstructors.count(ctx)>0) {
                return T::mapCtxConstructors[ctx] ;
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
            
            JS_DupValue(ctx, jscotr) ;
            T::mapCtxConstructors[ctx] = jscotr ;

            return jscotr ;
        }
        
        void setParent(JSContext * ctx, NativeClass<T> * parent) {            
            if(T::mapCtxConstructors.count(ctx)<1 || T::mapCtxConstructors.count(ctx)<1) {
                return ;
            }
            JSValue parentProto = T::mapCtxConstructors[ctx] ;
            JS_DupValue(ctx, parentProto) ;

            JSValue proto = T::mapCtxConstructors[ctx] ;
            JS_SetPropertyStr(ctx, proto, "__proto__", parentProto);
        }
    } ;

    
    template <typename T,typename P>
    std::vector<JSCFunctionListEntry> NativeClass<T,P>::methods ;
    
    template <typename T,typename P>
    std::vector<JSCFunctionListEntry> NativeClass<T,P>::staticMethods ;
}