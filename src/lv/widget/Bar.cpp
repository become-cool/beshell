#include "Bar.hpp"

namespace be::lv {
    DEFINE_NCLASS_META(Bar, Obj)
    std::vector<JSCFunctionListEntry> Bar::methods = {
// AUTO GENERATE CODE START [GETSET LIST] --------
        JS_CGETSET_DEF("mode",Bar::getMode,Bar::setMode) ,
        JS_CGETSET_DEF("value",Bar::getValue,be::lv::Obj::invalidSetter) ,
        JS_CGETSET_DEF("startValue",Bar::getStartValue,be::lv::Obj::invalidSetter) ,
        JS_CGETSET_DEF("minValue",Bar::getMinValue,be::lv::Obj::invalidSetter) ,
        JS_CGETSET_DEF("maxValue",Bar::getMaxValue,be::lv::Obj::invalidSetter) ,
// AUTO GENERATE CODE END [GETSET LIST] --------
// AUTO GENERATE CODE START [METHOD LIST] --------
        JS_CFUNC_DEF("setValue", 2, Bar::setValue),
        JS_CFUNC_DEF("setStartValue", 2, Bar::setStartValue),
        JS_CFUNC_DEF("setRange", 2, Bar::setRange),
        JS_CFUNC_DEF("isSymmetrical", 0, Bar::isSymmetrical),
// AUTO GENERATE CODE END [METHOD LIST] --------
    } ;

    Bar::Bar(JSContext * ctx, JSValue jsobj, lv_obj_t * lvobj)
        : Obj(ctx, Bar::build(ctx,jsobj), lvobj)
    {}

    Bar::Bar(JSContext * ctx, lv_obj_t * parent)
        : Bar(ctx, JS_NULL, lv_bar_create(parent))
    {}
        
    JSValue Bar::constructor(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        lv_obj_t * lvparent = nullptr ;
        if(argc>0) {
            JSVALUE_TO_LVOBJ_VAR(argv[0], lvparent)
        }
        Bar * widget = new Bar(ctx,lvparent) ;
        return widget->jsobj ;
    }

// AUTO GENERATE CODE START [GETSETS] --------
    #ifndef GETTER_Bar_Mode
    JSValue Bar::getMode(JSContext *ctx, JSValueConst this_val){
        THIS_NCLASS(Bar,thisobj)
        lv_bar_mode_t value = lv_bar_get_mode(thisobj->lvobj()) ;
        JSValue retval = JS_NewUint32(ctx, value) ;
        return retval ;
    }
    #endif
    #ifndef SETTER_Bar_Mode
    JSValue Bar::setMode(JSContext *ctx, JSValueConst this_val, JSValueConst val){
        THIS_NCLASS(Bar,thisobj)
        // argv mode
        const char * cstr_val = JS_ToCString(ctx, val) ;
        lv_bar_mode_t mode;
        if(!lv_bar_mode_str_to_const(cstr_val,&mode)) {
            JS_ThrowReferenceError(ctx,"unknow %s value: %s","lv_bar_mode_t",cstr_val) ;
            JS_FreeCString(ctx, cstr_val) ;
            return JS_EXCEPTION ;
        }
        JS_FreeCString(ctx, cstr_val) ;
        lv_bar_set_mode(thisobj->lvobj(), mode) ;
        return JS_UNDEFINED ;
    }
    #endif
    #ifndef GETTER_Bar_Value
    JSValue Bar::getValue(JSContext *ctx, JSValueConst this_val){
        THIS_NCLASS(Bar,thisobj)
        int32_t value = lv_bar_get_value(thisobj->lvobj()) ;
        JSValue retval = JS_NewInt32(ctx, value) ;
        return retval ;
    }
    #endif
    #ifndef GETTER_Bar_StartValue
    JSValue Bar::getStartValue(JSContext *ctx, JSValueConst this_val){
        THIS_NCLASS(Bar,thisobj)
        int32_t value = lv_bar_get_start_value(thisobj->lvobj()) ;
        JSValue retval = JS_NewInt32(ctx, value) ;
        return retval ;
    }
    #endif
    #ifndef GETTER_Bar_MinValue
    JSValue Bar::getMinValue(JSContext *ctx, JSValueConst this_val){
        THIS_NCLASS(Bar,thisobj)
        int32_t value = lv_bar_get_min_value(thisobj->lvobj()) ;
        JSValue retval = JS_NewInt32(ctx, value) ;
        return retval ;
    }
    #endif
    #ifndef GETTER_Bar_MaxValue
    JSValue Bar::getMaxValue(JSContext *ctx, JSValueConst this_val){
        THIS_NCLASS(Bar,thisobj)
        int32_t value = lv_bar_get_max_value(thisobj->lvobj()) ;
        JSValue retval = JS_NewInt32(ctx, value) ;
        return retval ;
    }
    #endif
// AUTO GENERATE CODE END [GETSETS] --------

// AUTO GENERATE CODE START [METHODS] --------
    #ifndef METHOD_Bar_setValue
    JSValue Bar::setValue(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        THIS_NCLASS(Obj,thisobj)
        CHECK_ARGC(2)
        int32_t value ;
        if(JS_ToInt32(ctx, (int32_t *) &value, argv[0])!=0){
            JSTHROW("arg %s of method %s.%s() must be a %s","value","Bar","setValue","number")
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
        lv_bar_set_value( thisobj->lvobj(), value, anim ) ;
        return JS_UNDEFINED ;
    }
    #endif

    #ifndef METHOD_Bar_setStartValue
    JSValue Bar::setStartValue(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        THIS_NCLASS(Obj,thisobj)
        CHECK_ARGC(2)
        int32_t start_value ;
        if(JS_ToInt32(ctx, (int32_t *) &start_value, argv[0])!=0){
            JSTHROW("arg %s of method %s.%s() must be a %s","start_value","Bar","setStartValue","number")
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
        lv_bar_set_start_value( thisobj->lvobj(), start_value, anim ) ;
        return JS_UNDEFINED ;
    }
    #endif

    #ifndef METHOD_Bar_setRange
    JSValue Bar::setRange(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        THIS_NCLASS(Obj,thisobj)
        CHECK_ARGC(2)
        int32_t min ;
        if(JS_ToInt32(ctx, (int32_t *) &min, argv[0])!=0){
            JSTHROW("arg %s of method %s.%s() must be a %s","min","Bar","setRange","number")
        }
        int32_t max ;
        if(JS_ToInt32(ctx, (int32_t *) &max, argv[1])!=0){
            JSTHROW("arg %s of method %s.%s() must be a %s","max","Bar","setRange","number")
        }
        lv_bar_set_range( thisobj->lvobj(), min, max ) ;
        return JS_UNDEFINED ;
    }
    #endif

    #ifndef METHOD_Bar_isSymmetrical
    JSValue Bar::isSymmetrical(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        THIS_NCLASS(Obj,thisobj)
        bool retval = lv_bar_is_symmetrical( thisobj->lvobj() ) ;
        JSValue jsretval = JS_NewBool(ctx, retval) ;
        return jsretval ;
    }
    #endif
// AUTO GENERATE CODE END [METHODS] --------

}
