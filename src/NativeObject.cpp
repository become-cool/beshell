#include "NativeObject.hpp"
#include "utils.h"

using namespace std;



namespace be {
    
    std::map<JSContext*, std::map<JSClassID, NativeClass *>> NativeClass::mapClasses ;
    
    NativeClass::NativeClass(
        JSContext * _ctx
        , JSClassID & _classID
        , const char * name
        , JSCFunction * constructor
        , JSClassFinalizer * finalizer
    )
        : classID(_classID)
        , ctx(_ctx)
    {
        JS_NewClassID(&classID);

        JSClassDef jsClassDef ;
        jsClassDef.class_name = name ;
        jsClassDef.finalizer = finalizer ;
        JS_NewClass(JS_GetRuntime(ctx), classID, &jsClassDef);

        proto = JS_NewObject(ctx);
        JS_SetClassProto(ctx, classID, proto);

        if(constructor) {
            JSValue jscotr = JS_NewCFunction2(_ctx, constructor, name, 1, JS_CFUNC_constructor, 0) ;
            JS_SetConstructor(_ctx, jscotr, proto) ;
        }

        mapClasses[ctx][classID] = this ;
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
    
    NativeClass * NativeClass::fromClassID(JSContext * ctx, JSClassID & classID) {
        JS_NewClassID(&classID);

        if (mapClasses.count(ctx) < 1) {
            std::cout << "new ctx nclass pool" << std::endl ;
            mapClasses[ctx] = std::map<JSClassID, NativeClass *>() ;
        }

        if (mapClasses.count(ctx)<0) {
            return nullptr ;
        }

        return mapClasses[ctx][classID] ;
    }

    void NativeClass::registerClass(JSContext * ctx, JSClassID classID , const char * name , NativeClass * parent) {

    }

    JSValue NativeClass::newJSObject(JSContext * ctx){
        return  JS_NewObjectClass(ctx, classID) ;
    }


    NativeObject::NativeObject(
        JSContext * _ctx
        , JSClassID classID
        , const char * name
        , JSCFunction * constructor
        , JSClassFinalizer * finalizer
        , NativeObject * parent
        , NativeClassDefineFunc funcDefineClass
    )
        : nclass( NativeClass::fromClassID(_ctx, classID) )
        , ctx(_ctx)
    {
        if(!nclass) {

            nclass = new NativeClass(_ctx, classID, name, constructor, finalizer) ;

            JSValue jscotr = JS_NewCFunction2(_ctx, jsConstructor, name, 1, JS_CFUNC_constructor, 0) ;
            JS_SetConstructor(_ctx, jscotr, nclass->proto) ;

            if(parent) {
                nclass->setParent(ctx,parent->nclass) ;
            }
            
            if(funcDefineClass) {
                funcDefineClass(nclass) ;
            }
        }

        jsobj = nclass->newJSObject(ctx) ;
        JS_DupValue(ctx, jsobj) ;

        JS_SetOpaque(jsobj, this) ;
    }
    
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
    JSValue NativeObject::jsConstructor(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        NativeObject * obj = fromJSObject(this_val) ;
        if(obj) {
            obj->constructor(ctx, this_val, argc, argv) ;
            return JS_UNDEFINED ;
        } else {
            THROW_EXCEPTION("Is not a NativeObject object") ;
        }
    }
    void NativeObject::jsFinalizer(JSRuntime *rt, JSValue val) {
        NativeObject * obj = fromJSObject(val) ;
        obj->finalize(rt,val) ;
    }

    void NativeObject::constructor(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst * argv) {}
    void NativeObject::finalize(JSRuntime *rt, JSValue val) {}
}