#include "Slider.hpp"

namespace be::lv {
    DEFINE_NCLASS_META(Slider, Bar)
    std::vector<JSCFunctionListEntry> Slider::methods = {
        // JS_CFUNC_DEF("setRange", 2, Slider::setRange),
        // JS_CGETSET_DEF("min",Slider::getMin,Slider::invalidSetter) ,
        // JS_CGETSET_DEF("max",Slider::getMax,Slider::invalidSetter) ,
        JS_CGETSET_DEF("leftValue",Slider::getLeft,Slider::setLeft) ,
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

    
    // JSValue Slider::setRange(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    //     THIS_NCLASS(Obj,thisobj)
    //     ARGV_TO_INT32(0, min)
    //     ARGV_TO_INT32(1, max)
    //     lv_slider_set_range( thisobj->lvobj(), min, max ) ;
    //     return JS_UNDEFINED ;
    // }
    // JSValue Slider::getMin(JSContext *ctx, JSValueConst this_val) {
    //     THIS_NCLASS(Obj,thisobj)
    //     int32_t value = lv_slider_get_min_value( thisobj->lvobj() ) ;
    //     return JS_NewInt32(ctx, value) ;
    // }
    // JSValue Slider::getMax(JSContext *ctx, JSValueConst this_val) {
    //     THIS_NCLASS(Obj,thisobj)
    //     int32_t value = lv_slider_get_max_value( thisobj->lvobj() ) ;
    //     return JS_NewInt32(ctx, value) ;
    // }
    
    JSValue Slider::setLeft(JSContext *ctx, JSValueConst this_val, JSValueConst value) {
        THIS_NCLASS(Obj,thisobj)
        int32_t val = 0 ;
        if(JS_ToInt32(ctx, &val, value)!=0){
            JSTHROW("Invalid argument")
        }
        lv_slider_set_left_value( thisobj->lvobj(), val, LV_ANIM_OFF ) ;
        return JS_UNDEFINED ;
    }
    JSValue Slider::getLeft(JSContext *ctx, JSValueConst this_val) {
        THIS_NCLASS(Obj,thisobj)
        int32_t value = lv_slider_get_left_value( thisobj->lvobj() ) ;
        return JS_NewInt32(ctx, value) ;
    }

// AUTO GENERATE CODE START [GETSETS] --------

// AUTO GENERATE CODE END [GETSETS] --------

// AUTO GENERATE CODE START [METHODS] --------
    // Unsupported arg type: lv_subject_t *
    // lv_observer_t * lv_slider_bind_value(lv_obj_t * obj, lv_subject_t * subject)

    #ifndef METHOD_Slider_isDragged
    /**
     * > 该方法是对 LVGL C API `lv_slider_is_dragged` 的封装，具体用法请参考  [LVGL文档](https://docs.lvgl.io/9.0/API/index.html)
     *
     * @module lv
     * @class Slider
     * @method isDragged
     *
     */
    JSValue Slider::isDragged(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        THIS_NCLASS(Obj,thisobj)
        bool retval = lv_slider_is_dragged( thisobj->lvobj() ) ;
        JSValue jsretval = JS_NewBool(ctx, retval) ;
        return jsretval ;
    }
    #endif
// AUTO GENERATE CODE END [METHODS] --------

}
