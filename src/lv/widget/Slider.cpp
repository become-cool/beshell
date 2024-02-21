#include "Slider.hpp"

namespace be::lv {
    DEFINE_NCLASS_META(Slider, Obj)
    std::vector<JSCFunctionListEntry> Slider::methods = {
// AUTO GENERATE CODE START [GETSET LIST] --------
        JS_CGETSET_DEF("mode",Slider::getMode,Slider::setMode) ,
        JS_CGETSET_DEF("value",Slider::getValue,be::lv::Obj::invalidSetter) ,
        JS_CGETSET_DEF("leftValue",Slider::getLeftValue,be::lv::Obj::invalidSetter) ,
        JS_CGETSET_DEF("minValue",Slider::getMinValue,be::lv::Obj::invalidSetter) ,
        JS_CGETSET_DEF("maxValue",Slider::getMaxValue,be::lv::Obj::invalidSetter) ,
// AUTO GENERATE CODE END [GETSET LIST] --------
// AUTO GENERATE CODE START [METHOD LIST] --------
        JS_CFUNC_DEF("setValue", 2, Slider::setValue),
        JS_CFUNC_DEF("setLeftValue", 2, Slider::setLeftValue),
        JS_CFUNC_DEF("setRange", 2, Slider::setRange),
        JS_CFUNC_DEF("isDragged", 0, Slider::isDragged),
        JS_CFUNC_DEF("isSymmetrical", 0, Slider::isSymmetrical),
        // Unsupported arg type:
        // lv_observer_t * lv_slider_bind_value(lv_obj_t * obj, lv_subject_t * subject)
// AUTO GENERATE CODE END [METHOD LIST] --------
    } ;

    Slider::Slider(JSContext * ctx, JSValue jsobj, lv_obj_t * lvobj)
        : Obj(ctx, Slider::build(ctx,jsobj), lvobj)
    {}

    Slider::Slider(JSContext * ctx, lv_obj_t * parent)
        : Slider(ctx, JS_NULL, lv_slider_create(parent))
    {}
        
    JSValue Slider::constructor(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        lv_obj_t * lvparent = nullptr ;
        if(argc>0) {
            JSVALUE_TO_LVOBJ_VAR(argv[0], lvparent)
        }
        Slider * widget = new Slider(ctx,lvparent) ;
        return widget->jsobj ;
    }

// AUTO GENERATE CODE START [GETSETS] --------
    #ifndef GETTER_Slider_Mode
    JSValue Slider::getMode(JSContext *ctx, JSValueConst this_val){
        THIS_NCLASS(Slider,thisobj)
        lv_slider_mode_t value = lv_slider_get_mode(thisobj->lvobj()) ;
        JSValue retval = JS_NewUint32(ctx, value) ;
        return retval ;
    }
    #endif
    #ifndef SETTER_Slider_Mode
    JSValue Slider::setMode(JSContext *ctx, JSValueConst this_val, JSValueConst val){
        THIS_NCLASS(Slider,thisobj)
        // argv mode
        const char * cstr_val = JS_ToCString(ctx, val) ;
        lv_slider_mode_t mode;
        if(!lv_slider_mode_str_to_const(cstr_val,&mode)) {
            JS_ThrowReferenceError(ctx,"unknow %s value: %s","lv_slider_mode_t",cstr_val) ;
            JS_FreeCString(ctx, cstr_val) ;
            return JS_EXCEPTION ;
        }
        JS_FreeCString(ctx, cstr_val) ;
        lv_slider_set_mode(thisobj->lvobj(), mode) ;
        return JS_UNDEFINED ;
    }
    #endif
    #ifndef GETTER_Slider_Value
    JSValue Slider::getValue(JSContext *ctx, JSValueConst this_val){
        THIS_NCLASS(Slider,thisobj)
        int32_t value = lv_slider_get_value(thisobj->lvobj()) ;
        JSValue retval = JS_NewInt32(ctx, value) ;
        return retval ;
    }
    #endif
    #ifndef GETTER_Slider_LeftValue
    JSValue Slider::getLeftValue(JSContext *ctx, JSValueConst this_val){
        THIS_NCLASS(Slider,thisobj)
        int32_t value = lv_slider_get_left_value(thisobj->lvobj()) ;
        JSValue retval = JS_NewInt32(ctx, value) ;
        return retval ;
    }
    #endif
    #ifndef GETTER_Slider_MinValue
    JSValue Slider::getMinValue(JSContext *ctx, JSValueConst this_val){
        THIS_NCLASS(Slider,thisobj)
        int32_t value = lv_slider_get_min_value(thisobj->lvobj()) ;
        JSValue retval = JS_NewInt32(ctx, value) ;
        return retval ;
    }
    #endif
    #ifndef GETTER_Slider_MaxValue
    JSValue Slider::getMaxValue(JSContext *ctx, JSValueConst this_val){
        THIS_NCLASS(Slider,thisobj)
        int32_t value = lv_slider_get_max_value(thisobj->lvobj()) ;
        JSValue retval = JS_NewInt32(ctx, value) ;
        return retval ;
    }
    #endif
// AUTO GENERATE CODE END [GETSETS] --------

// AUTO GENERATE CODE START [METHODS] --------
    // Unsupported arg type: lv_subject_t *
    // lv_observer_t * lv_slider_bind_value(lv_obj_t * obj, lv_subject_t * subject)

    #ifndef METHOD_Slider_setValue
    JSValue Slider::setValue(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        THIS_NCLASS(Obj,thisobj)
        CHECK_ARGC(2)
        int32_t value ;
        if(JS_ToInt32(ctx, (int32_t *) &value, argv[0])!=0){
            JSTHROW("arg %s of method %s.%s() must be a %s","value","Slider","setValue","number")
        }
        // argv anim
        const char * cstr_argv_1_ = JS_ToCString(ctx, argv[1]) ;
        lv_anim_enable_t anim;
        if(!lv_anim_enable_str_to_const(cstr_argv_1_,&anim)) {
            JS_ThrowReferenceError(ctx,"unknow %s value: %s","lv_anim_enable_t",cstr_argv_1_) ;
            JS_FreeCString(ctx, cstr_argv_1_) ;
            return JS_EXCEPTION ;
        }
        JS_FreeCString(ctx, cstr_argv_1_) ;
        lv_slider_set_value( thisobj->lvobj(), value, anim ) ;
        return JS_UNDEFINED ;
    }
    #endif

    #ifndef METHOD_Slider_setLeftValue
    JSValue Slider::setLeftValue(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        THIS_NCLASS(Obj,thisobj)
        CHECK_ARGC(2)
        int32_t value ;
        if(JS_ToInt32(ctx, (int32_t *) &value, argv[0])!=0){
            JSTHROW("arg %s of method %s.%s() must be a %s","value","Slider","setLeftValue","number")
        }
        // argv anim
        const char * cstr_argv_1_ = JS_ToCString(ctx, argv[1]) ;
        lv_anim_enable_t anim;
        if(!lv_anim_enable_str_to_const(cstr_argv_1_,&anim)) {
            JS_ThrowReferenceError(ctx,"unknow %s value: %s","lv_anim_enable_t",cstr_argv_1_) ;
            JS_FreeCString(ctx, cstr_argv_1_) ;
            return JS_EXCEPTION ;
        }
        JS_FreeCString(ctx, cstr_argv_1_) ;
        lv_slider_set_left_value( thisobj->lvobj(), value, anim ) ;
        return JS_UNDEFINED ;
    }
    #endif

    #ifndef METHOD_Slider_setRange
    JSValue Slider::setRange(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        THIS_NCLASS(Obj,thisobj)
        CHECK_ARGC(2)
        int32_t min ;
        if(JS_ToInt32(ctx, (int32_t *) &min, argv[0])!=0){
            JSTHROW("arg %s of method %s.%s() must be a %s","min","Slider","setRange","number")
        }
        int32_t max ;
        if(JS_ToInt32(ctx, (int32_t *) &max, argv[1])!=0){
            JSTHROW("arg %s of method %s.%s() must be a %s","max","Slider","setRange","number")
        }
        lv_slider_set_range( thisobj->lvobj(), min, max ) ;
        return JS_UNDEFINED ;
    }
    #endif

    #ifndef METHOD_Slider_isDragged
    JSValue Slider::isDragged(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        THIS_NCLASS(Obj,thisobj)
        bool retval = lv_slider_is_dragged( thisobj->lvobj() ) ;
        JSValue jsretval = JS_NewBool(ctx, retval) ;
        return jsretval ;
    }
    #endif

    #ifndef METHOD_Slider_isSymmetrical
    JSValue Slider::isSymmetrical(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        THIS_NCLASS(Obj,thisobj)
        bool retval = lv_slider_is_symmetrical( thisobj->lvobj() ) ;
        JSValue jsretval = JS_NewBool(ctx, retval) ;
        return jsretval ;
    }
    #endif
// AUTO GENERATE CODE END [METHODS] --------

}
