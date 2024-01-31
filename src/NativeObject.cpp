#include "NativeObject.hpp"
#include "utils.h"
#include <string.h>

using namespace std;


namespace be {

    // std::map<JSContext*, std::map<JSClassID, NativeClass *>> NativeClass::mapClasses ;

    NativeClass::NativeClass(
        JSContext * _ctx
        , JSClassID & _classID
        , const char * name
        , JSCFunction * constructor
        , NativeClass * parent
        , const std::vector<JSCFunctionListEntry> && _methods
        , const std::vector<JSCFunctionListEntry> && _staticMethods
        , JSClassFinalizer * finalizer
    )
        : classID(_classID)
        , ctx(_ctx)
        , methods(move(_methods))
        , staticMethods(move(_staticMethods))
    {
        JS_NewClassID(&classID);
        dn(classID)

        JSClassDef jsClassDef ;
        memset(&jsClassDef, 0, sizeof(JSClassDef)) ;

        jsClassDef.class_name = name ;
        jsClassDef.finalizer = finalizer ;
        JS_NewClass(JS_GetRuntime(ctx), classID, &jsClassDef);

        proto = JS_NewObject(ctx);
        JS_SetClassProto(ctx, classID, proto);
        dn(classID)

        if(methods.size()) {
            dd
            JS_SetPropertyFunctionList(ctx, proto, methods.data(), methods.size());
        }

        if(constructor) {
            ds(name)
            dp(constructor)
            JSValue jscotr = JS_NewCFunction2(_ctx, constructor, name, 1, JS_CFUNC_constructor, 0) ;
            JS_SetConstructor(_ctx, jscotr, proto) ;

            if(staticMethods.size()) {
                JS_SetPropertyFunctionList(ctx, jscotr, staticMethods.data(), staticMethods.size());
            }
        }

        // mapClasses[ctx][classID] = this ;
    }

    void NativeClass::setParent(JSContext * ctx, NativeClass * parent) {
        // @todo 解除并 free 原有 __proto__
        JS_DupValue(ctx, parent->proto) ;
        JS_SetPropertyStr(ctx, proto, "__proto__", parent->proto);
    }

    NativeClass::~NativeClass() {
        if(ctx) {
            if(!JS_IsNull(proto)) {
                JS_FreeValue(ctx, proto) ;
                proto = JS_NULL ;
            }
            ctx = nullptr ;
        }
    }

    JSValue NativeClass::newJSObject(JSContext * ctx){
        dd
        dn(classID)
        return  JS_NewObjectClass(ctx, classID) ;
    }

    // 在 c++ 中创建对象
    NativeObject::NativeObject(JSContext * ctx, NativeClass * _nclass)
        : ctx(ctx)
        , nclass(_nclass)
    {
        jsobj = nclass->newJSObject(ctx) ;
    }

    // 在 JS 中创建
    NativeObject::NativeObject(JSContext * ctx, NativeClass * _nclass, JSValue _jsobj)
        : ctx(ctx)
        , nclass(_nclass)
        , jsobj(_jsobj)
    {}

    NativeObject::~NativeObject() {
        JS_FreeValue(ctx, jsobj) ;
    }

    NativeObject * NativeObject::fromJSObject(JSValue jsObj) {
        NativeObject * obj = (NativeObject *) JS_GetOpaqueInternal(jsObj) ;
        if(!obj) {
            return nullptr ;
        }
        if( JS_GetOpaqueClassID(jsObj)!=obj->nclass->classID ) {
            return nullptr ;
        }
        return obj ;
    }

    JSValue NativeObject::constructor(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {}
    
    void NativeObject::finalizer(JSRuntime *rt, JSValue val) {
        NativeObject * obj = fromJSObject(val) ;
        if(obj) {
            obj->jsobj = JS_UNDEFINED ;
        }
    }
    // void NativeObject::finalize(JSRuntime *rt, JSValue val) {}
}