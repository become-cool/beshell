#include "NativeClass.hpp"
#include "utils.h"
#include <string.h>

using namespace std;


namespace be {

    // std::map<JSContext*, std::map<JSClassID, NativeClassDef *>> NativeClassDef::mapClasses ;

    NativeClassDef::NativeClassDef(
        JSContext * _ctx
        , JSClassID & _classID
        , const char * name
        , JSCFunction * constructor
        , const std::vector<JSCFunctionListEntry> & methods
        , const std::vector<JSCFunctionListEntry> & staticMethods
        , JSClassFinalizer * finalizer
    )
        : classID(_classID)
        , ctx(_ctx)
    {
        JS_NewClassID(&classID);

        JSClassDef jsClassDef ;
        memset(&jsClassDef, 0, sizeof(JSClassDef)) ;

        jsClassDef.class_name = name ;
        jsClassDef.finalizer = finalizer ;
        JS_NewClass(JS_GetRuntime(ctx), classID, &jsClassDef);

        proto = JS_NewObject(ctx);
        JS_SetClassProto(ctx, classID, proto);

        if(methods.size()) {
            JS_SetPropertyFunctionList(ctx, proto, methods.data(), methods.size());
        }

        if(constructor) {
            JSValue jscotr = JS_NewCFunction2(_ctx, constructor, name, 1, JS_CFUNC_constructor, 0) ;
            JS_SetConstructor(_ctx, jscotr, proto) ;

            if(staticMethods.size()) {
                JS_SetPropertyFunctionList(ctx, jscotr, staticMethods.data(), staticMethods.size());
            }
        }

        // mapClasses[ctx][classID] = this ;
    }

    void NativeClassDef::setParent(JSContext * ctx, NativeClassDef * parent) {
        // @todo 解除并 free 原有 __proto__
        JS_DupValue(ctx, parent->proto) ;
        JS_SetPropertyStr(ctx, proto, "__proto__", parent->proto);
    }

    NativeClassDef::~NativeClassDef() {
        if(ctx) {
            if(!JS_IsNull(proto)) {
                JS_FreeValue(ctx, proto) ;
                proto = JS_NULL ;
            }
            ctx = nullptr ;
        }
    }

    JSValue NativeClassDef::newJSObject(JSContext * ctx){
        return  JS_NewObjectClass(ctx, classID) ;
    }

    // 在 c++ 中创建对象
    // NativeClass::NativeClass(JSContext * ctx, NativeClassDef * _nclass)
    //     : ctx(ctx)
    //     , nclass(_nclass)
    // {
    //     jsobj = nclass->newJSObject(ctx) ;
    // }

    // 在 JS 中创建
    // NativeClass::NativeClass(JSContext * ctx, NativeClassDef * _nclass, JSValue _jsobj)
    //     : ctx(ctx)
    //     , nclass(_nclass)
    //     , jsobj(_jsobj)
    // {}

    template <typename T>
    NativeClass<T>::~NativeClass() {
        JS_FreeValue(ctx, jsobj) ;
    }

    template <typename T>
    NativeClass<T> * NativeClass<T>::fromJSObject(JSValue jsObj) {
        NativeClass * obj = (NativeClass *) JS_GetOpaqueInternal(jsObj) ;
        if(!obj) {
            return nullptr ;
        }
        if( JS_GetOpaqueClassID(jsObj)!=obj->nclass->classID ) {
            return nullptr ;
        }
        return obj ;
    }

    template <typename T>
    JSValue NativeClass<T>::constructor(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        return JS_UNDEFINED ;
    }
    
    template <typename T>
    void NativeClass<T>::finalizer(JSRuntime *rt, JSValue val) {
        NativeClass * obj = fromJSObject(val) ;
        if(obj) {
            obj->jsobj = JS_UNDEFINED ;
        }
    }
    // void NativeClass::finalize(JSRuntime *rt, JSValue val) {}
}