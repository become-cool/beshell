#include "NativeClass.hpp"
using namespace std;


namespace be {
    NativeClass::NativeClass() {

    }
    
    JSValue NativeClass::defineClass(JSContext * ctx, JSClassID classId, JSClassDef classDef) {
        
        JSValue proto = JS_NewObject(ctx);
        
        JS_SetPropertyFunctionList(ctx, proto, funcLst, funcs);


        JS_SetClassProto(ctx, classId, proto);

        if(!JS_IsUndefined(parentProto)) {
            JS_SetPropertyStr(ctx, proto, "__proto__", parentProto);
        }

        if(cotr) {
            JSValue jscotr = JS_NewCFunction2(ctx, cotr, cotrName, 1, JS_CFUNC_constructor, 0) ;
            JS_SetConstructor(ctx, jscotr, proto) ;
            
            if(staticFuncs>0) {
                JS_SetPropertyFunctionList(ctx, jscotr, staticFuncLst, staticFuncs);
            }
            
            if(!JS_IsUndefined(pkg)) {
                JS_SetPropertyStr(ctx, pkg, className, jscotr);
            }
        }

        return proto ;
    }
}