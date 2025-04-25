#include "NativeClass.hpp"
#include <string.h>
#include "JSEngine.hpp"

using namespace std;
namespace be {

    std::map<JSContext*, std::map<JSClassID, JSValue>> NativeClass::mapCtxClassID2Constructor ;

    DEFINE_NCLASS_META_STATIC(NativeClass)
    DEFINE_NCLASS_META_BUILD(NativeClass)

    std::vector<JSCFunctionListEntry> NativeClass::methods = {
        JS_CFUNC_DEF("setDestructor", 0, NativeClass::setDestructor),
    };
    std::vector<JSCFunctionListEntry> NativeClass::staticMethods = {
    } ;

    NativeClass::NativeClass(JSContext * _ctx, JSValue _jsobj)
        : ctx(_ctx)
        , jsobj(build(_ctx, _jsobj))
    {
        assert(JS_IsObject(jsobj)) ;
        JS_SetOpaque(jsobj, this) ;
    }

    NativeClass::~NativeClass() {
        if(!JS_IsUndefined(jsobj)) {
            JS_SetOpaque(jsobj, nullptr) ;
            JS_FreeValue(ctx,jsobj) ;
        }
        JS_FreeValue(ctx, destructor) ;
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
        // JSCFunctionListEntry settergetters[] = {
        //     JS_CGETSET_DEF("printOnDestruct",printOnDestructGetter,printOnDestructSetter) ,
        // } ;
        // JS_SetPropertyFunctionList(ctx, proto, settergetters, sizeof(settergetters)/sizeof(JSCFunctionListEntry));


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

    JSValue NativeClass::defineClass(JSContext * ctx) {
        JSValue jscotr = NativeClass::defineClass(
            ctx,NativeClass::classID,NativeClass::className
            , NativeClass::methods, NativeClass::staticMethods
            , NativeClass::constructor
            , NativeClass::finalizer
            , NativeClass::classID
        ) ;
        return jscotr ;
    }

    JSValue NativeClass::constructor(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        auto obj = new NativeClass(ctx, this_val) ;
        obj->shared() ;
        return obj->jsobj ;
    }

    void NativeClass::finalizer(JSRuntime *rt, JSValue val) {
        NativeClass * obj = fromJS(val) ;
        if(obj) {
            if( JS_VALUE_GET_PTR(obj->jsobj)==JS_VALUE_GET_PTR(val) ) {

                // 调用 JS destructor
                if(JS_IsFunction(obj->ctx, obj->destructor)) {
                    JSValue ret = JS_Call(obj->ctx, obj->destructor, obj->jsobj, 0, nullptr) ;
                    if(JS_IsException(ret)) {
                        JSEngine::fromJSContext(obj->ctx)->dumpError() ;
                    }
                    JS_FreeValue(obj->ctx, ret) ;
                }

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

    const std::shared_ptr<NativeClass> & NativeClass::shared() {
        if(!self) {
            self = std::shared_ptr<NativeClass>(this) ;
        }
        return self ;
    }

    JSValue NativeClass::newObject(JSContext * ctx, JSValue constructor) {
        JSValue proto = JS_GetPropertyStr(ctx,constructor,"prototype") ;
        if( !JS_IsObject(proto) ){
            JSTHROW("arg is not a JS class constructor")
        }
        JS_FreeValue(ctx,proto) ;

        JSClassID classId = 0 ;
        if(!JS_GetClassIDFromProto(ctx,proto,&classId)){
            JSValue obj = JS_NewObject(ctx) ;
            JS_SetPropertyStr(ctx,obj,"__proto__",JS_DupValue(ctx,proto)) ;
            return obj ;
        } else {
            return JS_NewObjectClass(ctx, classId) ;
        }
    }
    
    JSValue NativeClass::setDestructor(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        THIS_NCLASS(NativeClass, that) ;
        CHECK_ARGC(1)

        if(!JS_IsFunction(ctx, argv[0])) {
            JSTHROW("arg must be a function")
        }
        JS_FreeValue(ctx, that->destructor) ;
        that->destructor = JS_DupValue(ctx, argv[0]) ;
        
        return JS_UNDEFINED ;
    }
}
