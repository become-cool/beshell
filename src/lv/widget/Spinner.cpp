#include "Spinner.hpp"

namespace be::lv {
    DEFINE_NCLASS_META(Spinner, Obj)
    std::vector<JSCFunctionListEntry> Spinner::methods = {
// AUTO GENERATE CODE START [GETSET LIST] --------

// AUTO GENERATE CODE END [GETSET LIST] --------
// AUTO GENERATE CODE START [METHOD LIST] --------
        JS_CFUNC_DEF("setAnimParams", 2, Spinner::setAnimParams),
// AUTO GENERATE CODE END [METHOD LIST] --------
    } ;

    Spinner::Spinner(JSContext * ctx, JSValue jsobj, lv_obj_t * lvobj)
        : Obj(ctx, Spinner::build(ctx,jsobj), lvobj)
    {}

    Spinner::Spinner(JSContext * ctx, lv_obj_t * parent)
        : Spinner(ctx, JS_NULL, lv_spinner_create(parent))
    {}

    JSValue Spinner::constructor(JSContext *ctx, JSValueConst ctor, int argc, JSValueConst *argv) {
        lv_obj_t * lvparent = nullptr ;
        if(argc>0) {
            JSVALUE_TO_LVOBJ_VAR(argv[0], lvparent)
        }
        JSValue obj = newObject(ctx, ctor) ;
        if( JS_IsException(obj) ) {
            return obj ;
        }
        Spinner * widget = new Spinner(ctx, obj, lv_spinner_create(lvparent)) ;
        return widget->jsobj ;
    }

// AUTO GENERATE CODE START [GETSETS] --------

// AUTO GENERATE CODE END [GETSETS] --------

// AUTO GENERATE CODE START [METHODS] --------
    #ifndef METHOD_Spinner_setAnimParams
    JSValue Spinner::setAnimParams(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        THIS_NCLASS(Obj,thisobj)
        CHECK_ARGC(2)
        uint32_t t ;
        if(JS_ToUint32(ctx, (uint32_t *) &t, argv[0])!=0){
            JSTHROW("arg %s of method %s.%s() must be a %s","t","Spinner","setAnimParams","number")
        }
        uint32_t angle ;
        if(JS_ToUint32(ctx, (uint32_t *) &angle, argv[1])!=0){
            JSTHROW("arg %s of method %s.%s() must be a %s","angle","Spinner","setAnimParams","number")
        }
        lv_spinner_set_anim_params( thisobj->lvobj(), t, angle ) ;
        return JS_UNDEFINED ;
    }
    #endif
// AUTO GENERATE CODE END [METHODS] --------

}
