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
        JS_CFUNC_DEF("stepNext", 0, SpinBox::jsStepNext),
        JS_CFUNC_DEF("stepPrev", 0, SpinBox::jsStepPrev),
        JS_CFUNC_DEF("increment", 0, SpinBox::jsIncrement),
        JS_CFUNC_DEF("decrement", 0, SpinBox::jsDecrement),

// AUTO GENERATE CODE END [METHOD LIST] --------
    } ;

    SpinBox::SpinBox(JSContext * ctx, JSValue jsobj, lv_obj_t * lvobj)
        : Obj(ctx, jsobj, lvobj)
    {}

    SpinBox::SpinBox(JSContext * ctx, lv_obj_t * parent)
        : Obj(ctx, JS_NULL, lv_spinbox_create(parent))
    {}
        
    JSValue SpinBox::constructor(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        lv_obj_t * lvparent = nullptr ;
        if(argc>0) {
            JSVALUE_TO_LVOBJ_VAR(argv[0], lvparent)
        }
        SpinBox * widget = new SpinBox(ctx,lvparent) ;
        return widget->jsobj ;
    }

// AUTO GENERATE CODE START [GETSETS] --------
    JSValue SpinBox::getValue(JSContext *ctx, JSValueConst this_val){
        THIS_NCLASS(SpinBox,thisobj)
        int32_t value = lv_spinbox_get_value(thisobj->lvobj()) ;
        JSValue retval = JS_NewInt32(ctx, value) ;
        return retval ;
    }
    JSValue SpinBox::setValue(JSContext *ctx, JSValueConst this_val, JSValueConst val){
        THIS_NCLASS(SpinBox,thisobj)
        int32_t value ;
        if(JS_ToInt32(ctx, (int32_t *) &value, val)!=0){
            JSTHROW("arg %s of method %s.%s() must be a %s","value","SpinBox","setValue","number")
        }
        lv_spinbox_set_value(thisobj->lvobj(), value) ;
        return JS_UNDEFINED ;
    }
    JSValue SpinBox::getRollover(JSContext *ctx, JSValueConst this_val){
        THIS_NCLASS(SpinBox,thisobj)
        bool value = lv_spinbox_get_rollover(thisobj->lvobj()) ;
        JSValue retval = JS_NewBool(ctx, value) ;
        return retval ;
    }
    JSValue SpinBox::setRollover(JSContext *ctx, JSValueConst this_val, JSValueConst val){
        THIS_NCLASS(SpinBox,thisobj)
        bool rollover = JS_ToBool(ctx, val) ;
        lv_spinbox_set_rollover(thisobj->lvobj(), rollover) ;
        return JS_UNDEFINED ;
    }
    JSValue SpinBox::getStep(JSContext *ctx, JSValueConst this_val){
        THIS_NCLASS(SpinBox,thisobj)
        int32_t value = lv_spinbox_get_step(thisobj->lvobj()) ;
        JSValue retval = JS_NewInt32(ctx, value) ;
        return retval ;
    }
    JSValue SpinBox::setStep(JSContext *ctx, JSValueConst this_val, JSValueConst val){
        THIS_NCLASS(SpinBox,thisobj)
        uint32_t step ;
        if(JS_ToUint32(ctx, (uint32_t *) &step, val)!=0){
            JSTHROW("arg %s of method %s.%s() must be a %s","step","SpinBox","setStep","number")
        }
        lv_spinbox_set_step(thisobj->lvobj(), step) ;
        return JS_UNDEFINED ;
    }
    JSValue SpinBox::setCursorPos(JSContext *ctx, JSValueConst this_val, JSValueConst val){
        THIS_NCLASS(SpinBox,thisobj)
        uint32_t cursorPos ;
        if(JS_ToUint32(ctx, (uint32_t *) &cursorPos, val)!=0){
            JSTHROW("arg %s of method %s.%s() must be a %s","cursorPos","SpinBox","setCursorPos","number")
        }
        lv_spinbox_set_cursor_pos(thisobj->lvobj(), cursorPos) ;
        return JS_UNDEFINED ;
    }
    JSValue SpinBox::setDigitStepDirection(JSContext *ctx, JSValueConst this_val, JSValueConst val){
        THIS_NCLASS(SpinBox,thisobj)
        // argv digitStepDirection
        const char * cstr_val = JS_ToCString(ctx, val) ;
        lv_dir_t digitStepDirection;
        if(lv_dir_str_to_const(cstr_val,&digitStepDirection)) {
            JS_ThrowReferenceError(ctx,"unknow %s value: %s","lv_dir_t",cstr_val) ;
            JS_FreeCString(ctx, cstr_val) ;
            return JS_EXCEPTION ;
        }
        JS_FreeCString(ctx, cstr_val) ;
        lv_spinbox_set_digit_step_direction(thisobj->lvobj(), digitStepDirection) ;
        return JS_UNDEFINED ;
    }

// AUTO GENERATE CODE END [GETSETS] --------

// AUTO GENERATE CODE START [METHODS] --------
        JSValue SpinBox::jsStepNext(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
            THIS_NCLASS(Obj,thisobj)
            lv_spinbox_step_next( thisobj->lvobj() ) ;
            return JS_UNDEFINED ;
        }

        JSValue SpinBox::jsStepPrev(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
            THIS_NCLASS(Obj,thisobj)
            lv_spinbox_step_prev( thisobj->lvobj() ) ;
            return JS_UNDEFINED ;
        }

        JSValue SpinBox::jsIncrement(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
            THIS_NCLASS(Obj,thisobj)
            lv_spinbox_increment( thisobj->lvobj() ) ;
            return JS_UNDEFINED ;
        }

        JSValue SpinBox::jsDecrement(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
            THIS_NCLASS(Obj,thisobj)
            lv_spinbox_decrement( thisobj->lvobj() ) ;
            return JS_UNDEFINED ;
        }

// AUTO GENERATE CODE END [METHODS] --------

}
