#include "SpinBox.hpp"

namespace be::lv {
    DEFINE_NCLASS_META(SpinBox, Obj)
    std::vector<JSCFunctionListEntry> SpinBox::methods = {
// AUTO GENERATE CODE START [GETSET LIST] --------
        JS_CGETSET_DEF("value",SpinBox::getValue,SpinBox::setValue) ,
        JS_CGETSET_DEF("rollover",SpinBox::getRollover,SpinBox::setRollover) ,
        JS_CGETSET_DEF("step",SpinBox::getStep,SpinBox::setStep) ,
        JS_CGETSET_DEF("cursorPos",be::lv::Obj::invalidGetter,SpinBox::setCursorPos) ,
        JS_CGETSET_DEF("digitStepDirection",be::lv::Obj::invalidGetter,SpinBox::setDigitStepDirection) ,
// AUTO GENERATE CODE END [GETSET LIST] --------
// AUTO GENERATE CODE START [METHOD LIST] --------
        JS_CFUNC_DEF("setDigitFormat", 2, SpinBox::setDigitFormat),
        JS_CFUNC_DEF("setRange", 2, SpinBox::setRange),
        JS_CFUNC_DEF("stepNext", 0, SpinBox::stepNext),
        JS_CFUNC_DEF("stepPrev", 0, SpinBox::stepPrev),
        JS_CFUNC_DEF("increment", 0, SpinBox::increment),
        JS_CFUNC_DEF("decrement", 0, SpinBox::decrement),
// AUTO GENERATE CODE END [METHOD LIST] --------
    } ;

    SpinBox::SpinBox(JSContext * ctx, JSValue jsobj, lv_obj_t * lvobj)
        : Obj(ctx, SpinBox::build(ctx,jsobj), lvobj)
    {}

    SpinBox::SpinBox(JSContext * ctx, lv_obj_t * parent)
        : SpinBox(ctx, JS_NULL, lv_spinbox_create(parent))
    {}

    JSValue SpinBox::constructor(JSContext *ctx, JSValueConst ctor, int argc, JSValueConst *argv) {
        lv_obj_t * lvparent = nullptr ;
        if(argc>0) {
            JSVALUE_TO_LVOBJ_VAR(argv[0], lvparent)
        }
        JSValue obj = newObject(ctx, ctor) ;
        if( JS_IsException(obj) ) {
            return obj ;
        }
        SpinBox * widget = new SpinBox(ctx, obj, lv_obj_create(lvparent)) ;
        return widget->jsobj ;
    }

// AUTO GENERATE CODE START [GETSETS] --------
    #ifndef GETTER_SpinBox_Value
    JSValue SpinBox::getValue(JSContext *ctx, JSValueConst this_val){
        THIS_NCLASS(SpinBox,thisobj)
        int32_t value = lv_spinbox_get_value(thisobj->lvobj()) ;
        JSValue retval = JS_NewInt32(ctx, value) ;
        return retval ;
    }
    #endif
    #ifndef SETTER_SpinBox_Value
    JSValue SpinBox::setValue(JSContext *ctx, JSValueConst this_val, JSValueConst val){
        THIS_NCLASS(SpinBox,thisobj)
        int32_t value ;
        if(JS_ToInt32(ctx, (int32_t *) &value, val)!=0){
            JSTHROW("arg %s of method %s.%s() must be a %s","value","SpinBox","setValue","number")
        }
        lv_spinbox_set_value(thisobj->lvobj(), value) ;
        return JS_UNDEFINED ;
    }
    #endif
    #ifndef GETTER_SpinBox_Rollover
    JSValue SpinBox::getRollover(JSContext *ctx, JSValueConst this_val){
        THIS_NCLASS(SpinBox,thisobj)
        bool value = lv_spinbox_get_rollover(thisobj->lvobj()) ;
        JSValue retval = JS_NewBool(ctx, value) ;
        return retval ;
    }
    #endif
    #ifndef SETTER_SpinBox_Rollover
    JSValue SpinBox::setRollover(JSContext *ctx, JSValueConst this_val, JSValueConst val){
        THIS_NCLASS(SpinBox,thisobj)
        bool rollover = JS_ToBool(ctx, val) ;
        lv_spinbox_set_rollover(thisobj->lvobj(), rollover) ;
        return JS_UNDEFINED ;
    }
    #endif
    #ifndef GETTER_SpinBox_Step
    JSValue SpinBox::getStep(JSContext *ctx, JSValueConst this_val){
        THIS_NCLASS(SpinBox,thisobj)
        int32_t value = lv_spinbox_get_step(thisobj->lvobj()) ;
        JSValue retval = JS_NewInt32(ctx, value) ;
        return retval ;
    }
    #endif
    #ifndef SETTER_SpinBox_Step
    JSValue SpinBox::setStep(JSContext *ctx, JSValueConst this_val, JSValueConst val){
        THIS_NCLASS(SpinBox,thisobj)
        uint32_t step ;
        if(JS_ToUint32(ctx, (uint32_t *) &step, val)!=0){
            JSTHROW("arg %s of method %s.%s() must be a %s","step","SpinBox","setStep","number")
        }
        lv_spinbox_set_step(thisobj->lvobj(), step) ;
        return JS_UNDEFINED ;
    }
    #endif
    #ifndef SETTER_SpinBox_CursorPos
    JSValue SpinBox::setCursorPos(JSContext *ctx, JSValueConst this_val, JSValueConst val){
        THIS_NCLASS(SpinBox,thisobj)
        uint32_t cursorPos ;
        if(JS_ToUint32(ctx, (uint32_t *) &cursorPos, val)!=0){
            JSTHROW("arg %s of method %s.%s() must be a %s","cursorPos","SpinBox","setCursorPos","number")
        }
        lv_spinbox_set_cursor_pos(thisobj->lvobj(), cursorPos) ;
        return JS_UNDEFINED ;
    }
    #endif
    #ifndef SETTER_SpinBox_DigitStepDirection
    JSValue SpinBox::setDigitStepDirection(JSContext *ctx, JSValueConst this_val, JSValueConst val){
        THIS_NCLASS(SpinBox,thisobj)
        // argv digitStepDirection
        const char * cstr_val = JS_ToCString(ctx, val) ;
        lv_dir_t digitStepDirection;
        if(!lv_dir_str_to_const(cstr_val,&digitStepDirection)) {
            JS_ThrowReferenceError(ctx,"unknow %s value: %s","lv_dir_t",cstr_val) ;
            JS_FreeCString(ctx, cstr_val) ;
            return JS_EXCEPTION ;
        }
        JS_FreeCString(ctx, cstr_val) ;
        lv_spinbox_set_digit_step_direction(thisobj->lvobj(), digitStepDirection) ;
        return JS_UNDEFINED ;
    }
    #endif
// AUTO GENERATE CODE END [GETSETS] --------

// AUTO GENERATE CODE START [METHODS] --------
    #ifndef METHOD_SpinBox_setDigitFormat
    JSValue SpinBox::setDigitFormat(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        THIS_NCLASS(Obj,thisobj)
        CHECK_ARGC(2)
        uint32_t digit_count ;
        if(JS_ToUint32(ctx, (uint32_t *) &digit_count, argv[0])!=0){
            JSTHROW("arg %s of method %s.%s() must be a %s","digit_count","SpinBox","setDigitFormat","number")
        }
        uint32_t sep_pos ;
        if(JS_ToUint32(ctx, (uint32_t *) &sep_pos, argv[1])!=0){
            JSTHROW("arg %s of method %s.%s() must be a %s","sep_pos","SpinBox","setDigitFormat","number")
        }
        lv_spinbox_set_digit_format( thisobj->lvobj(), digit_count, sep_pos ) ;
        return JS_UNDEFINED ;
    }
    #endif

    #ifndef METHOD_SpinBox_setRange
    JSValue SpinBox::setRange(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        THIS_NCLASS(Obj,thisobj)
        CHECK_ARGC(2)
        int32_t range_min ;
        if(JS_ToInt32(ctx, (int32_t *) &range_min, argv[0])!=0){
            JSTHROW("arg %s of method %s.%s() must be a %s","range_min","SpinBox","setRange","number")
        }
        int32_t range_max ;
        if(JS_ToInt32(ctx, (int32_t *) &range_max, argv[1])!=0){
            JSTHROW("arg %s of method %s.%s() must be a %s","range_max","SpinBox","setRange","number")
        }
        lv_spinbox_set_range( thisobj->lvobj(), range_min, range_max ) ;
        return JS_UNDEFINED ;
    }
    #endif

    #ifndef METHOD_SpinBox_stepNext
    JSValue SpinBox::stepNext(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        THIS_NCLASS(Obj,thisobj)
        lv_spinbox_step_next( thisobj->lvobj() ) ;
        return JS_UNDEFINED ;
    }
    #endif

    #ifndef METHOD_SpinBox_stepPrev
    JSValue SpinBox::stepPrev(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        THIS_NCLASS(Obj,thisobj)
        lv_spinbox_step_prev( thisobj->lvobj() ) ;
        return JS_UNDEFINED ;
    }
    #endif

    #ifndef METHOD_SpinBox_increment
    JSValue SpinBox::increment(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        THIS_NCLASS(Obj,thisobj)
        lv_spinbox_increment( thisobj->lvobj() ) ;
        return JS_UNDEFINED ;
    }
    #endif

    #ifndef METHOD_SpinBox_decrement
    JSValue SpinBox::decrement(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        THIS_NCLASS(Obj,thisobj)
        lv_spinbox_decrement( thisobj->lvobj() ) ;
        return JS_UNDEFINED ;
    }
    #endif
// AUTO GENERATE CODE END [METHODS] --------

}
