#include "NativeClass.hpp"
#include <string.h>

using namespace std;
namespace be {

    const JSClassID NativeClass::classID = 0 ;
    std::map<JSContext*, std::map<JSClassID, JSValue>> NativeClass::mapCtxClassID2Constructor ;
    std::vector<JSCFunctionListEntry> NativeClass::methods ;
    std::vector<JSCFunctionListEntry> NativeClass::staticMethods ;

    NativeClass::NativeClass(JSContext * _ctx, JSValue _jsobj)
        : ctx(_ctx)
        , jsobj(_jsobj)
    {
        assert(JS_IsObject(jsobj)) ;
        JS_SetOpaque(jsobj, this) ;
        // JS_DupValue(ctx,jsobj) ;
    }

    NativeClass::~NativeClass() {
        if(!JS_IsUndefined(jsobj)) {
            JS_SetOpaque(jsobj, nullptr) ;
            JS_FreeValue(ctx,jsobj) ;
        }
    }

    NativeClass * NativeClass::fromJS(JSValue jsObj) {
        NativeClass * obj = (NativeClass *) JS_GetOpaqueInternal(jsObj) ;
        if(!obj) {
            return nullptr ;
        }
        return obj ;
    }
    
    JSValue NativeClass::defineClass(
            JSContext * ctx
            , JSClassID & classID
            , const char * className
            , const std::vector<JSCFunctionListEntry>& methods
            , const std::vector<JSCFunctionListEntry>& staticMethods
            , NClassConstructorFunc constructor
            , NClassFinalizerFunc finalizer
            , JSClassID parentClassID
    ) {
        JS_NewClassID(&classID);

        if(mapCtxClassID2Constructor[ctx].count(classID)>0) {
            return mapCtxClassID2Constructor[ctx][classID] ;
        }

        JSClassDef jsClassDef ;
        memset(&jsClassDef, 0, sizeof(JSClassDef)) ;

        jsClassDef.class_name = className ;
        jsClassDef.finalizer = finalizer ;
        JS_NewClass(JS_GetRuntime(ctx), classID, &jsClassDef);

        JSValue proto = JS_NewObject(ctx);
        JS_SetClassProto(ctx, classID, proto);

        if(methods.size()) {
            JS_SetPropertyFunctionList(ctx, proto, methods.data(), methods.size());
        }

        JSValue jscotr = JS_NewCFunction2(ctx, constructor, className, 1, JS_CFUNC_constructor, 0) ;
        JS_SetConstructor(ctx, jscotr, proto) ;

        if(staticMethods.size()) {
            JS_SetPropertyFunctionList(ctx, jscotr, staticMethods.data(), staticMethods.size());
        }
        
        JS_DupValue(ctx, jscotr) ;
        mapCtxClassID2Constructor[ctx][classID] = jscotr ;
        
        // printf("classname: %s, classid: %d\n", className, classID) ;

        if(parentClassID>0) {
            // printf("classid: %d -> parentclassid: %d\n", classID, parentClassID) ;
            if(mapCtxClassID2Constructor[ctx].count(parentClassID)>0) {
                JSValue parent = mapCtxClassID2Constructor[ctx][parentClassID] ;
                JSValue parentProto = JS_GetPropertyStr(ctx,parent,"prototype") ;
                JS_DupValue(ctx, parentProto) ;
                JS_SetPropertyStr(ctx, proto, "__proto__", parentProto);
            }
        }

        return jscotr ;
    }
    JSValue NativeClass::defineClass(JSContext * ctx){
        return JS_UNDEFINED ;
    }

    JSValue NativeClass::constructor(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        return JS_UNDEFINED ;
    }

    void NativeClass::finalizer(JSRuntime *rt, JSValue val) {
        NativeClass * obj = fromJS(val) ;
        if(obj) {
            if(obj->jsobj==val) {
                obj->jsobj = JS_UNDEFINED ;
            }
            obj->self = nullptr ;
        }
    }

    JSValue NativeClass::getClass(JSContext * ctx, const JSClassID & classID) {
        if(mapCtxClassID2Constructor.count(ctx)<1 || mapCtxClassID2Constructor[ctx].count(classID)<1) {
            return JS_NULL ;
        }
        return mapCtxClassID2Constructor[ctx][classID] ;
    }
}
