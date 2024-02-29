#include "Slider.hpp"

namespace be::lv {
    DEFINE_NCLASS_META(Slider, Bar)
    std::vector<JSCFunctionListEntry> Slider::methods = {
// AUTO GENERATE CODE START [GETSET LIST] --------


// AUTO GENERATE CODE END [GETSET LIST] --------
// AUTO GENERATE CODE START [METHOD LIST] --------
        JS_CFUNC_DEF("isDragged", 0, Slider::isDragged),
        // Unsupported arg type:
        // lv_observer_t * lv_slider_bind_value(lv_obj_t * obj, lv_subject_t * subject)

// AUTO GENERATE CODE END [METHOD LIST] --------
    } ;

    Slider::Slider(JSContext * ctx, JSValue jsobj, lv_obj_t * lvobj)
        : Bar(ctx, Slider::build(ctx,jsobj), lvobj)
    {}

    Slider::Slider(JSContext * ctx, lv_obj_t * parent)
        : Slider(ctx, JS_NULL, lv_slider_create(parent))
    {}

    JSValue Slider::constructor(JSContext *ctx, JSValueConst ctor, int argc, JSValueConst *argv) {
        lv_obj_t * lvparent = nullptr ;
        if(argc>0) {
            JSVALUE_TO_LVOBJ_VAR(argv[0], lvparent)
        }
        JSValue obj = newObject(ctx, ctor) ;
        if( JS_IsException(obj) ) {
            return obj ;
        }
        Slider * widget = new Slider(ctx, obj, lv_slider_create(lvparent)) ;
        return widget->jsobj ;
    }

// AUTO GENERATE CODE START [GETSETS] --------


// AUTO GENERATE CODE END [GETSETS] --------

// AUTO GENERATE CODE START [METHODS] --------
    // Unsupported arg type: lv_subject_t *
    // lv_observer_t * lv_slider_bind_value(lv_obj_t * obj, lv_subject_t * subject)

    #ifndef METHOD_Slider_isDragged
    JSValue Slider::isDragged(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        THIS_NCLASS(Obj,thisobj)
        bool retval = lv_slider_is_dragged( thisobj->lvobj() ) ;
        JSValue jsretval = JS_NewBool(ctx, retval) ;
        return jsretval ;
    }
    #endif

// AUTO GENERATE CODE END [METHODS] --------

}
