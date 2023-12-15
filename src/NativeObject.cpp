#include "NativeObject.hpp"
using namespace std;


namespace be {
    NativeClass::NativeClass(
        JSContext * _ctx
        , const char * name
        , JSClassID & _classID
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

        JSValue proto = JS_NewObject(ctx);

        JS_SetClassProto(ctx, classID, proto);

        mapClasses[ctx][classID] = this ;
    }

    void NativeClass::setParent(JSContext * ctx, NativeClass * parent) {
        // @todo 解除并 free 原有 __proto__
        JS_DupValue(ctx, parent->proto) ;
        JS_SetPropertyStr(ctx, proto, "__proto__", parent->proto);
    }

    NativeClass::~NativeClass() {
        if(ctx) {
            if(!JS_IsNull(constructor)) {
                JS_FreeValue(ctx, constructor) ;
                constructor = JS_NULL ;
            }
            if(!JS_IsNull(proto)) {
                JS_FreeValue(ctx, proto) ;
                proto = JS_NULL ;
            }
            if(!JS_IsNull(parentProto)) {
                JS_FreeValue(ctx, parentProto) ;
                parentProto = JS_NULL ;
            }
            ctx = nullptr ;
        }
    }
}