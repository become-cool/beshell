#include "Arc.hpp"

namespace be::lv {
    DEFINE_NCLASS_META(Arc, Obj)
    std::vector<JSCFunctionListEntry> Arc::methods = {
// AUTO GENERATE CODE START [GETSET LIST] --------
        JS_CGETSET_DEF("startAngle",be::lv::Obj::invalidGetter,Arc::setStartAngle) ,
        JS_CGETSET_DEF("endAngle",be::lv::Obj::invalidGetter,Arc::setEndAngle) ,
        JS_CGETSET_DEF("bgStartAngle",be::lv::Obj::invalidGetter,Arc::setBgStartAngle) ,
        JS_CGETSET_DEF("bgEndAngle",be::lv::Obj::invalidGetter,Arc::setBgEndAngle) ,
        JS_CGETSET_DEF("rotation",Arc::getRotation,Arc::setRotation) ,
        JS_CGETSET_DEF("mode",Arc::getMode,Arc::setMode) ,
        JS_CGETSET_DEF("value",Arc::getValue,Arc::setValue) ,
        JS_CGETSET_DEF("changeRate",be::lv::Obj::invalidGetter,Arc::setChangeRate) ,
        JS_CGETSET_DEF("knobOffset",Arc::getKnobOffset,Arc::setKnobOffset) ,
        JS_CGETSET_DEF("angleStart",Arc::getAngleStart,be::lv::Obj::invalidSetter) ,
        JS_CGETSET_DEF("angleEnd",Arc::getAngleEnd,be::lv::Obj::invalidSetter) ,
        JS_CGETSET_DEF("bgAngleStart",Arc::getBgAngleStart,be::lv::Obj::invalidSetter) ,
        JS_CGETSET_DEF("bgAngleEnd",Arc::getBgAngleEnd,be::lv::Obj::invalidSetter) ,
        JS_CGETSET_DEF("minValue",Arc::getMinValue,be::lv::Obj::invalidSetter) ,
        JS_CGETSET_DEF("maxValue",Arc::getMaxValue,be::lv::Obj::invalidSetter) ,
// AUTO GENERATE CODE END [GETSET LIST] --------
// AUTO GENERATE CODE START [METHOD LIST] --------
        JS_CFUNC_DEF("setAngles", 2, Arc::setAngles),
        JS_CFUNC_DEF("setBgAngles", 2, Arc::setBgAngles),
        JS_CFUNC_DEF("setRange", 2, Arc::setRange),
        JS_CFUNC_DEF("alignObjToAngle", 2, Arc::alignObjToAngle),
        JS_CFUNC_DEF("rotateObjToAngle", 2, Arc::rotateObjToAngle),
        // Unsupported arg type:
        // lv_observer_t * lv_arc_bind_value(lv_obj_t * obj, lv_subject_t * subject)
// AUTO GENERATE CODE END [METHOD LIST] --------
    } ;

    Arc::Arc(JSContext * ctx, JSValue jsobj, lv_obj_t * lvobj)
        : Obj(ctx, Arc::build(ctx,jsobj), lvobj)
    {}

    Arc::Arc(JSContext * ctx, lv_obj_t * parent)
        : Arc(ctx, JS_NULL, lv_arc_create(parent))
    {}

    JSValue Arc::constructor(JSContext *ctx, JSValueConst ctor, int argc, JSValueConst *argv) {
        lv_obj_t * lvparent = nullptr ;
        if(argc>0) {
            JSVALUE_TO_LVOBJ_VAR(argv[0], lvparent)
        }
        JSValue obj = newObject(ctx, ctor) ;
        if( JS_IsException(obj) ) {
            return obj ;
        }
        Arc * widget = new Arc(ctx, obj, lv_arc_create(lvparent)) ;
        return widget->jsobj ;
    }

// AUTO GENERATE CODE START [GETSETS] --------
    #ifndef SETTER_Arc_StartAngle
    JSValue Arc::setStartAngle(JSContext *ctx, JSValueConst this_val, JSValueConst val){
        THIS_NCLASS(Arc,thisobj)
        int32_t startAngle ;
        if(JS_ToInt32(ctx, (int32_t *) &startAngle, val)!=0){
            JSTHROW("arg %s of method %s.%s() must be a %s","startAngle","Arc","setStartAngle","number")
        }
        lv_arc_set_start_angle(thisobj->lvobj(), startAngle) ;
        return JS_UNDEFINED ;
    }
    #endif
    #ifndef SETTER_Arc_EndAngle
    JSValue Arc::setEndAngle(JSContext *ctx, JSValueConst this_val, JSValueConst val){
        THIS_NCLASS(Arc,thisobj)
        int32_t endAngle ;
        if(JS_ToInt32(ctx, (int32_t *) &endAngle, val)!=0){
            JSTHROW("arg %s of method %s.%s() must be a %s","endAngle","Arc","setEndAngle","number")
        }
        lv_arc_set_end_angle(thisobj->lvobj(), endAngle) ;
        return JS_UNDEFINED ;
    }
    #endif
    #ifndef SETTER_Arc_BgStartAngle
    JSValue Arc::setBgStartAngle(JSContext *ctx, JSValueConst this_val, JSValueConst val){
        THIS_NCLASS(Arc,thisobj)
        int32_t bgStartAngle ;
        if(JS_ToInt32(ctx, (int32_t *) &bgStartAngle, val)!=0){
            JSTHROW("arg %s of method %s.%s() must be a %s","bgStartAngle","Arc","setBgStartAngle","number")
        }
        lv_arc_set_bg_start_angle(thisobj->lvobj(), bgStartAngle) ;
        return JS_UNDEFINED ;
    }
    #endif
    #ifndef SETTER_Arc_BgEndAngle
    JSValue Arc::setBgEndAngle(JSContext *ctx, JSValueConst this_val, JSValueConst val){
        THIS_NCLASS(Arc,thisobj)
        int32_t bgEndAngle ;
        if(JS_ToInt32(ctx, (int32_t *) &bgEndAngle, val)!=0){
            JSTHROW("arg %s of method %s.%s() must be a %s","bgEndAngle","Arc","setBgEndAngle","number")
        }
        lv_arc_set_bg_end_angle(thisobj->lvobj(), bgEndAngle) ;
        return JS_UNDEFINED ;
    }
    #endif
    #ifndef GETTER_Arc_Rotation
    JSValue Arc::getRotation(JSContext *ctx, JSValueConst this_val){
        THIS_NCLASS(Arc,thisobj)
        int32_t value = lv_arc_get_rotation(thisobj->lvobj()) ;
        JSValue retval = JS_NewInt32(ctx, value) ;
        return retval ;
    }
    #endif
    #ifndef SETTER_Arc_Rotation
    JSValue Arc::setRotation(JSContext *ctx, JSValueConst this_val, JSValueConst val){
        THIS_NCLASS(Arc,thisobj)
        int32_t rotation ;
        if(JS_ToInt32(ctx, (int32_t *) &rotation, val)!=0){
            JSTHROW("arg %s of method %s.%s() must be a %s","rotation","Arc","setRotation","number")
        }
        lv_arc_set_rotation(thisobj->lvobj(), rotation) ;
        return JS_UNDEFINED ;
    }
    #endif
    #ifndef GETTER_Arc_Mode
    JSValue Arc::getMode(JSContext *ctx, JSValueConst this_val){
        THIS_NCLASS(Arc,thisobj)
        lv_arc_mode_t value = lv_arc_get_mode(thisobj->lvobj()) ;
        JSValue retval = JS_NewUint32(ctx, value) ;
        return retval ;
    }
    #endif
    #ifndef SETTER_Arc_Mode
    JSValue Arc::setMode(JSContext *ctx, JSValueConst this_val, JSValueConst val){
        THIS_NCLASS(Arc,thisobj)
        // argv mode
        const char * cstr_val = JS_ToCString(ctx, val) ;
        lv_arc_mode_t mode;
        if(!lv_arc_mode_str_to_const(cstr_val,&mode)) {
            JS_ThrowReferenceError(ctx,"unknow %s value: %s","lv_arc_mode_t",cstr_val) ;
            JS_FreeCString(ctx, cstr_val) ;
            return JS_EXCEPTION ;
        }
        JS_FreeCString(ctx, cstr_val) ;
        lv_arc_set_mode(thisobj->lvobj(), mode) ;
        return JS_UNDEFINED ;
    }
    #endif
    #ifndef GETTER_Arc_Value
    JSValue Arc::getValue(JSContext *ctx, JSValueConst this_val){
        THIS_NCLASS(Arc,thisobj)
        int32_t value = lv_arc_get_value(thisobj->lvobj()) ;
        JSValue retval = JS_NewInt32(ctx, value) ;
        return retval ;
    }
    #endif
    #ifndef SETTER_Arc_Value
    JSValue Arc::setValue(JSContext *ctx, JSValueConst this_val, JSValueConst val){
        THIS_NCLASS(Arc,thisobj)
        int32_t value ;
        if(JS_ToInt32(ctx, (int32_t *) &value, val)!=0){
            JSTHROW("arg %s of method %s.%s() must be a %s","value","Arc","setValue","number")
        }
        lv_arc_set_value(thisobj->lvobj(), value) ;
        return JS_UNDEFINED ;
    }
    #endif
    #ifndef SETTER_Arc_ChangeRate
    JSValue Arc::setChangeRate(JSContext *ctx, JSValueConst this_val, JSValueConst val){
        THIS_NCLASS(Arc,thisobj)
        uint32_t changeRate ;
        if(JS_ToUint32(ctx, (uint32_t *) &changeRate, val)!=0){
            JSTHROW("arg %s of method %s.%s() must be a %s","changeRate","Arc","setChangeRate","number")
        }
        lv_arc_set_change_rate(thisobj->lvobj(), changeRate) ;
        return JS_UNDEFINED ;
    }
    #endif
    #ifndef GETTER_Arc_KnobOffset
    JSValue Arc::getKnobOffset(JSContext *ctx, JSValueConst this_val){
        THIS_NCLASS(Arc,thisobj)
        int32_t value = lv_arc_get_knob_offset(thisobj->lvobj()) ;
        JSValue retval = JS_NewInt32(ctx, value) ;
        return retval ;
    }
    #endif
    #ifndef SETTER_Arc_KnobOffset
    JSValue Arc::setKnobOffset(JSContext *ctx, JSValueConst this_val, JSValueConst val){
        THIS_NCLASS(Arc,thisobj)
        int32_t knobOffset ;
        if(JS_ToInt32(ctx, (int32_t *) &knobOffset, val)!=0){
            JSTHROW("arg %s of method %s.%s() must be a %s","knobOffset","Arc","setKnobOffset","number")
        }
        lv_arc_set_knob_offset(thisobj->lvobj(), knobOffset) ;
        return JS_UNDEFINED ;
    }
    #endif
    #ifndef GETTER_Arc_AngleStart
    JSValue Arc::getAngleStart(JSContext *ctx, JSValueConst this_val){
        THIS_NCLASS(Arc,thisobj)
        lv_value_precise_t value = lv_arc_get_angle_start(thisobj->lvobj()) ;
        JSValue retval = JS_NewInt32(ctx, value) ;
        return retval ;
    }
    #endif
    #ifndef GETTER_Arc_AngleEnd
    JSValue Arc::getAngleEnd(JSContext *ctx, JSValueConst this_val){
        THIS_NCLASS(Arc,thisobj)
        lv_value_precise_t value = lv_arc_get_angle_end(thisobj->lvobj()) ;
        JSValue retval = JS_NewInt32(ctx, value) ;
        return retval ;
    }
    #endif
    #ifndef GETTER_Arc_BgAngleStart
    JSValue Arc::getBgAngleStart(JSContext *ctx, JSValueConst this_val){
        THIS_NCLASS(Arc,thisobj)
        lv_value_precise_t value = lv_arc_get_bg_angle_start(thisobj->lvobj()) ;
        JSValue retval = JS_NewInt32(ctx, value) ;
        return retval ;
    }
    #endif
    #ifndef GETTER_Arc_BgAngleEnd
    JSValue Arc::getBgAngleEnd(JSContext *ctx, JSValueConst this_val){
        THIS_NCLASS(Arc,thisobj)
        lv_value_precise_t value = lv_arc_get_bg_angle_end(thisobj->lvobj()) ;
        JSValue retval = JS_NewInt32(ctx, value) ;
        return retval ;
    }
    #endif
    #ifndef GETTER_Arc_MinValue
    JSValue Arc::getMinValue(JSContext *ctx, JSValueConst this_val){
        THIS_NCLASS(Arc,thisobj)
        int32_t value = lv_arc_get_min_value(thisobj->lvobj()) ;
        JSValue retval = JS_NewInt32(ctx, value) ;
        return retval ;
    }
    #endif
    #ifndef GETTER_Arc_MaxValue
    JSValue Arc::getMaxValue(JSContext *ctx, JSValueConst this_val){
        THIS_NCLASS(Arc,thisobj)
        int32_t value = lv_arc_get_max_value(thisobj->lvobj()) ;
        JSValue retval = JS_NewInt32(ctx, value) ;
        return retval ;
    }
    #endif
// AUTO GENERATE CODE END [GETSETS] --------

// AUTO GENERATE CODE START [METHODS] --------
    // Unsupported arg type: lv_subject_t *
    // lv_observer_t * lv_arc_bind_value(lv_obj_t * obj, lv_subject_t * subject)

    #ifndef METHOD_Arc_setAngles
    JSValue Arc::setAngles(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        THIS_NCLASS(Obj,thisobj)
        CHECK_ARGC(2)
        int32_t start ;
        if(JS_ToInt32(ctx, (int32_t *) &start, argv[0])!=0){
            JSTHROW("arg %s of method %s.%s() must be a %s","start","Arc","setAngles","number")
        }
        int32_t end ;
        if(JS_ToInt32(ctx, (int32_t *) &end, argv[1])!=0){
            JSTHROW("arg %s of method %s.%s() must be a %s","end","Arc","setAngles","number")
        }
        lv_arc_set_angles( thisobj->lvobj(), start, end ) ;
        return JS_UNDEFINED ;
    }
    #endif

    #ifndef METHOD_Arc_setBgAngles
    JSValue Arc::setBgAngles(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        THIS_NCLASS(Obj,thisobj)
        CHECK_ARGC(2)
        int32_t start ;
        if(JS_ToInt32(ctx, (int32_t *) &start, argv[0])!=0){
            JSTHROW("arg %s of method %s.%s() must be a %s","start","Arc","setBgAngles","number")
        }
        int32_t end ;
        if(JS_ToInt32(ctx, (int32_t *) &end, argv[1])!=0){
            JSTHROW("arg %s of method %s.%s() must be a %s","end","Arc","setBgAngles","number")
        }
        lv_arc_set_bg_angles( thisobj->lvobj(), start, end ) ;
        return JS_UNDEFINED ;
    }
    #endif

    #ifndef METHOD_Arc_setRange
    JSValue Arc::setRange(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        THIS_NCLASS(Obj,thisobj)
        CHECK_ARGC(2)
        int32_t min ;
        if(JS_ToInt32(ctx, (int32_t *) &min, argv[0])!=0){
            JSTHROW("arg %s of method %s.%s() must be a %s","min","Arc","setRange","number")
        }
        int32_t max ;
        if(JS_ToInt32(ctx, (int32_t *) &max, argv[1])!=0){
            JSTHROW("arg %s of method %s.%s() must be a %s","max","Arc","setRange","number")
        }
        lv_arc_set_range( thisobj->lvobj(), min, max ) ;
        return JS_UNDEFINED ;
    }
    #endif

    #ifndef METHOD_Arc_alignObjToAngle
    JSValue Arc::alignObjToAngle(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        THIS_NCLASS(Obj,thisobj)
        CHECK_ARGC(2)
        JSVALUE_TO_LVOBJ(argv[0],obj_to_align)
        int32_t r_offset ;
        if(JS_ToInt32(ctx, (int32_t *) &r_offset, argv[1])!=0){
            JSTHROW("arg %s of method %s.%s() must be a %s","r_offset","Arc","alignObjToAngle","number")
        }
        lv_arc_align_obj_to_angle( thisobj->lvobj(), obj_to_align, r_offset ) ;
        return JS_UNDEFINED ;
    }
    #endif

    #ifndef METHOD_Arc_rotateObjToAngle
    JSValue Arc::rotateObjToAngle(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        THIS_NCLASS(Obj,thisobj)
        CHECK_ARGC(2)
        JSVALUE_TO_LVOBJ(argv[0],obj_to_rotate)
        int32_t r_offset ;
        if(JS_ToInt32(ctx, (int32_t *) &r_offset, argv[1])!=0){
            JSTHROW("arg %s of method %s.%s() must be a %s","r_offset","Arc","rotateObjToAngle","number")
        }
        lv_arc_rotate_obj_to_angle( thisobj->lvobj(), obj_to_rotate, r_offset ) ;
        return JS_UNDEFINED ;
    }
    #endif
// AUTO GENERATE CODE END [METHODS] --------

}
