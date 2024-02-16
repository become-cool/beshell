#include "Arc.hpp"

namespace be::lv {
    DEFINE_NCLASS_META(Arc, Obj)
    std::vector<JSCFunctionListEntry> Arc::methods = {
// AUTO GENERATE CODE START [GETSET LIST] --------
        JS_CGETSET_DEF("rotation",Arc::getRotation,Arc::setRotation) ,
        JS_CGETSET_DEF("mode",Arc::getMode,Arc::setMode) ,
        JS_CGETSET_DEF("value",Arc::getValue,Arc::setValue) ,
        JS_CGETSET_DEF("changeRate",be::lv::Obj::invalidGetter,Arc::setChangeRate) ,
        JS_CGETSET_DEF("knobOffset",Arc::getKnobOffset,Arc::setKnobOffset) ,
        JS_CGETSET_DEF("minValue",Arc::getMinValue,be::lv::Obj::invalidSetter) ,
        JS_CGETSET_DEF("maxValue",Arc::getMaxValue,be::lv::Obj::invalidSetter) ,

// AUTO GENERATE CODE END [GETSET LIST] --------
// AUTO GENERATE CODE START [METHOD LIST] --------
        // Unsupported arg type:
        // lv_observer_t * lv_arc_bind_value(lv_obj_t * obj, lv_subject_t * subject)
        // void lv_arc_align_obj_to_angle(const lv_obj_t * obj, lv_obj_t * obj_to_align, int32_t r_offset)
        // void lv_arc_rotate_obj_to_angle(const lv_obj_t * obj, lv_obj_t * obj_to_rotate, int32_t r_offset)

// AUTO GENERATE CODE END [METHOD LIST] --------
    } ;

    Arc::Arc(JSContext * ctx, JSValue jsobj, lv_obj_t * lvobj)
        : Obj(ctx, jsobj, lvobj)
    {}

    Arc::Arc(JSContext * ctx, lv_obj_t * parent)
        : Obj(ctx, JS_NULL, lv_arc_create(parent))
    {}
        
    JSValue Arc::constructor(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        lv_obj_t * lvparent = nullptr ;
        if(argc>0) {
            JSVALUE_TO_LVOBJ_VAR(argv[0], lvparent)
        }
        Arc * widget = new Arc(ctx,lvparent) ;
        return widget->jsobj ;
    }

// AUTO GENERATE CODE START [GETSETS] --------
    // unspported type: lv_value_precise_t
    // JSValue Arc::setStartAngle(JSContext *ctx, JSValueConst this_val, JSValueConst value){}
    // void lv_arc_set_start_angle(lv_obj_t * obj, lv_value_precise_t start)
    // unspported type: lv_value_precise_t
    // JSValue Arc::setEndAngle(JSContext *ctx, JSValueConst this_val, JSValueConst value){}
    // void lv_arc_set_end_angle(lv_obj_t * obj, lv_value_precise_t end)
    // unspported type: lv_value_precise_t
    // JSValue Arc::setBgStartAngle(JSContext *ctx, JSValueConst this_val, JSValueConst value){}
    // void lv_arc_set_bg_start_angle(lv_obj_t * obj, lv_value_precise_t start)
    // unspported type: lv_value_precise_t
    // JSValue Arc::setBgEndAngle(JSContext *ctx, JSValueConst this_val, JSValueConst value){}
    // void lv_arc_set_bg_end_angle(lv_obj_t * obj, lv_value_precise_t end)
    JSValue Arc::getRotation(JSContext *ctx, JSValueConst this_val){
        THIS_NCLASS(Arc,thisobj)
        int32_t value = lv_arc_get_rotation(thisobj->lvobj()) ;
        JSValue retval = JS_NewInt32(ctx, value) ;
        return retval ;
    }
    JSValue Arc::setRotation(JSContext *ctx, JSValueConst this_val, JSValueConst val){
        THIS_NCLASS(Arc,thisobj)
        int32_t rotation ;
        if(JS_ToInt32(ctx, (int32_t *) &rotation, val)!=0){
            JSTHROW("arg %s of method %s.%s() must be a %s","rotation","Arc","setRotation","number")
        }
        lv_arc_set_rotation(thisobj->lvobj(), rotation) ;
        return JS_UNDEFINED ;
    }
    JSValue Arc::getMode(JSContext *ctx, JSValueConst this_val){
        THIS_NCLASS(Arc,thisobj)
        lv_arc_mode_t value = lv_arc_get_mode(thisobj->lvobj()) ;
        JSValue retval = JS_NewUint32(ctx, value) ;
        return retval ;
    }
    JSValue Arc::setMode(JSContext *ctx, JSValueConst this_val, JSValueConst val){
        THIS_NCLASS(Arc,thisobj)
        uint8_t mode ;
        if(JS_ToUint32(ctx, (uint32_t *) &mode, val)!=0){
            JSTHROW("arg %s of method %s.%s() must be a %s","mode","Arc","setMode","number")
        }
        lv_arc_set_mode(thisobj->lvobj(), mode) ;
        return JS_UNDEFINED ;
    }
    JSValue Arc::getValue(JSContext *ctx, JSValueConst this_val){
        THIS_NCLASS(Arc,thisobj)
        int32_t value = lv_arc_get_value(thisobj->lvobj()) ;
        JSValue retval = JS_NewInt32(ctx, value) ;
        return retval ;
    }
    JSValue Arc::setValue(JSContext *ctx, JSValueConst this_val, JSValueConst val){
        THIS_NCLASS(Arc,thisobj)
        int32_t value ;
        if(JS_ToInt32(ctx, (int32_t *) &value, val)!=0){
            JSTHROW("arg %s of method %s.%s() must be a %s","value","Arc","setValue","number")
        }
        lv_arc_set_value(thisobj->lvobj(), value) ;
        return JS_UNDEFINED ;
    }
    JSValue Arc::setChangeRate(JSContext *ctx, JSValueConst this_val, JSValueConst val){
        THIS_NCLASS(Arc,thisobj)
        uint32_t changeRate ;
        if(JS_ToUint32(ctx, (uint32_t *) &changeRate, val)!=0){
            JSTHROW("arg %s of method %s.%s() must be a %s","changeRate","Arc","setChangeRate","number")
        }
        lv_arc_set_change_rate(thisobj->lvobj(), changeRate) ;
        return JS_UNDEFINED ;
    }
    JSValue Arc::getKnobOffset(JSContext *ctx, JSValueConst this_val){
        THIS_NCLASS(Arc,thisobj)
        int32_t value = lv_arc_get_knob_offset(thisobj->lvobj()) ;
        JSValue retval = JS_NewInt32(ctx, value) ;
        return retval ;
    }
    JSValue Arc::setKnobOffset(JSContext *ctx, JSValueConst this_val, JSValueConst val){
        THIS_NCLASS(Arc,thisobj)
        int32_t knobOffset ;
        if(JS_ToInt32(ctx, (int32_t *) &knobOffset, val)!=0){
            JSTHROW("arg %s of method %s.%s() must be a %s","knobOffset","Arc","setKnobOffset","number")
        }
        lv_arc_set_knob_offset(thisobj->lvobj(), knobOffset) ;
        return JS_UNDEFINED ;
    }
    // unspported type: lv_value_precise_t
    // JSValue Arc::getAngleStart(JSContext *ctx, JSValueConst this_val){}
    // lv_value_precise_t lv_arc_get_angle_start(lv_obj_t * obj)
    // unspported type: lv_value_precise_t
    // JSValue Arc::getAngleEnd(JSContext *ctx, JSValueConst this_val){}
    // lv_value_precise_t lv_arc_get_angle_end(lv_obj_t * obj)
    // unspported type: lv_value_precise_t
    // JSValue Arc::getBgAngleStart(JSContext *ctx, JSValueConst this_val){}
    // lv_value_precise_t lv_arc_get_bg_angle_start(lv_obj_t * obj)
    // unspported type: lv_value_precise_t
    // JSValue Arc::getBgAngleEnd(JSContext *ctx, JSValueConst this_val){}
    // lv_value_precise_t lv_arc_get_bg_angle_end(lv_obj_t * obj)
    JSValue Arc::getMinValue(JSContext *ctx, JSValueConst this_val){
        THIS_NCLASS(Arc,thisobj)
        int32_t value = lv_arc_get_min_value(thisobj->lvobj()) ;
        JSValue retval = JS_NewInt32(ctx, value) ;
        return retval ;
    }
    JSValue Arc::getMaxValue(JSContext *ctx, JSValueConst this_val){
        THIS_NCLASS(Arc,thisobj)
        int32_t value = lv_arc_get_max_value(thisobj->lvobj()) ;
        JSValue retval = JS_NewInt32(ctx, value) ;
        return retval ;
    }

// AUTO GENERATE CODE END [GETSETS] --------

// AUTO GENERATE CODE START [METHODS] --------
        // Unsupported arg type: lv_subject_t *
        // lv_observer_t * lv_arc_bind_value(lv_obj_t * obj, lv_subject_t * subject)

        // Unsupported arg type: const lv_obj_t *
        // void lv_arc_align_obj_to_angle(const lv_obj_t * obj, lv_obj_t * obj_to_align, int32_t r_offset)

        // Unsupported arg type: const lv_obj_t *
        // void lv_arc_rotate_obj_to_angle(const lv_obj_t * obj, lv_obj_t * obj_to_rotate, int32_t r_offset)

// AUTO GENERATE CODE END [METHODS] --------

}
