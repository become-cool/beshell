#include "Scale.hpp"

namespace be::lv {
    DEFINE_NCLASS_META(Scale, Obj)
    std::vector<JSCFunctionListEntry> Scale::methods = {
// AUTO GENERATE CODE START [GETSET LIST] --------
        JS_CGETSET_DEF("mode",Scale::getMode,Scale::setMode) ,
        JS_CGETSET_DEF("totalTickCount",Scale::getTotalTickCount,Scale::setTotalTickCount) ,
        JS_CGETSET_DEF("majorTickEvery",Scale::getMajorTickEvery,Scale::setMajorTickEvery) ,
        JS_CGETSET_DEF("labelShow",Scale::getLabelShow,Scale::setLabelShow) ,
        JS_CGETSET_DEF("angleRange",Scale::getAngleRange,Scale::setAngleRange) ,
        JS_CGETSET_DEF("rotation",be::lv::Obj::invalidGetter,Scale::setRotation) ,
        JS_CGETSET_DEF("postDraw",be::lv::Obj::invalidGetter,Scale::setPostDraw) ,
        JS_CGETSET_DEF("rangeMinValue",Scale::getRangeMinValue,be::lv::Obj::invalidSetter) ,
        JS_CGETSET_DEF("rangeMaxValue",Scale::getRangeMaxValue,be::lv::Obj::invalidSetter) ,
// AUTO GENERATE CODE END [GETSET LIST] --------
// AUTO GENERATE CODE START [METHOD LIST] --------
        JS_CFUNC_DEF("setRange", 2, Scale::setRange),
        JS_CFUNC_DEF("setLineNeedleValue", 3, Scale::setLineNeedleValue),
        JS_CFUNC_DEF("setImageNeedleValue", 2, Scale::setImageNeedleValue),
        // Unsupported arg type:
        // lv_scale_section_t * lv_scale_add_section(lv_obj_t * obj)
        // void lv_scale_section_set_range(lv_scale_section_t * section, int32_t minor_range, int32_t major_range)
        // void lv_scale_section_set_style(lv_scale_section_t * section, uint32_t part, lv_style_t * section_part_style)
// AUTO GENERATE CODE END [METHOD LIST] --------
    } ;

    Scale::Scale(JSContext * ctx, JSValue jsobj, lv_obj_t * lvobj)
        : Obj(ctx, Scale::build(ctx,jsobj), lvobj)
    {}

    Scale::Scale(JSContext * ctx, lv_obj_t * parent)
        : Scale(ctx, JS_NULL, lv_scale_create(parent))
    {}
        
    JSValue Scale::constructor(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        lv_obj_t * lvparent = nullptr ;
        if(argc>0) {
            JSVALUE_TO_LVOBJ_VAR(argv[0], lvparent)
        }
        Scale * widget = new Scale(ctx,lvparent) ;
        return widget->jsobj ;
    }

// AUTO GENERATE CODE START [GETSETS] --------
    #ifndef GETTER_Scale_Mode
    JSValue Scale::getMode(JSContext *ctx, JSValueConst this_val){
        THIS_NCLASS(Scale,thisobj)
        lv_scale_mode_t value = lv_scale_get_mode(thisobj->lvobj()) ;
        JSValue retval = lv_scale_mode_const_to_jsstr(ctx, value) ;
        return retval ;
    }
    #endif
    #ifndef SETTER_Scale_Mode
    JSValue Scale::setMode(JSContext *ctx, JSValueConst this_val, JSValueConst val){
        THIS_NCLASS(Scale,thisobj)
        // argv mode
        const char * cstr_val = JS_ToCString(ctx, val) ;
        lv_scale_mode_t mode;
        if(!lv_scale_mode_str_to_const(cstr_val,&mode)) {
            JS_ThrowReferenceError(ctx,"unknow %s value: %s","lv_scale_mode_t",cstr_val) ;
            JS_FreeCString(ctx, cstr_val) ;
            return JS_EXCEPTION ;
        }
        JS_FreeCString(ctx, cstr_val) ;
        lv_scale_set_mode(thisobj->lvobj(), mode) ;
        return JS_UNDEFINED ;
    }
    #endif
    #ifndef GETTER_Scale_TotalTickCount
    JSValue Scale::getTotalTickCount(JSContext *ctx, JSValueConst this_val){
        THIS_NCLASS(Scale,thisobj)
        int32_t value = lv_scale_get_total_tick_count(thisobj->lvobj()) ;
        JSValue retval = JS_NewInt32(ctx, value) ;
        return retval ;
    }
    #endif
    #ifndef SETTER_Scale_TotalTickCount
    JSValue Scale::setTotalTickCount(JSContext *ctx, JSValueConst this_val, JSValueConst val){
        THIS_NCLASS(Scale,thisobj)
        uint32_t totalTickCount ;
        if(JS_ToUint32(ctx, (uint32_t *) &totalTickCount, val)!=0){
            JSTHROW("arg %s of method %s.%s() must be a %s","totalTickCount","Scale","setTotalTickCount","number")
        }
        lv_scale_set_total_tick_count(thisobj->lvobj(), totalTickCount) ;
        return JS_UNDEFINED ;
    }
    #endif
    #ifndef GETTER_Scale_MajorTickEvery
    JSValue Scale::getMajorTickEvery(JSContext *ctx, JSValueConst this_val){
        THIS_NCLASS(Scale,thisobj)
        int32_t value = lv_scale_get_major_tick_every(thisobj->lvobj()) ;
        JSValue retval = JS_NewInt32(ctx, value) ;
        return retval ;
    }
    #endif
    #ifndef SETTER_Scale_MajorTickEvery
    JSValue Scale::setMajorTickEvery(JSContext *ctx, JSValueConst this_val, JSValueConst val){
        THIS_NCLASS(Scale,thisobj)
        uint32_t majorTickEvery ;
        if(JS_ToUint32(ctx, (uint32_t *) &majorTickEvery, val)!=0){
            JSTHROW("arg %s of method %s.%s() must be a %s","majorTickEvery","Scale","setMajorTickEvery","number")
        }
        lv_scale_set_major_tick_every(thisobj->lvobj(), majorTickEvery) ;
        return JS_UNDEFINED ;
    }
    #endif
    #ifndef GETTER_Scale_LabelShow
    JSValue Scale::getLabelShow(JSContext *ctx, JSValueConst this_val){
        THIS_NCLASS(Scale,thisobj)
        bool value = lv_scale_get_label_show(thisobj->lvobj()) ;
        JSValue retval = JS_NewBool(ctx, value) ;
        return retval ;
    }
    #endif
    #ifndef SETTER_Scale_LabelShow
    JSValue Scale::setLabelShow(JSContext *ctx, JSValueConst this_val, JSValueConst val){
        THIS_NCLASS(Scale,thisobj)
        bool labelShow = JS_ToBool(ctx, val) ;
        lv_scale_set_label_show(thisobj->lvobj(), labelShow) ;
        return JS_UNDEFINED ;
    }
    #endif
    #ifndef GETTER_Scale_AngleRange
    JSValue Scale::getAngleRange(JSContext *ctx, JSValueConst this_val){
        THIS_NCLASS(Scale,thisobj)
        uint32_t value = lv_scale_get_angle_range(thisobj->lvobj()) ;
        JSValue retval = JS_NewUint32(ctx, value) ;
        return retval ;
    }
    #endif
    #ifndef SETTER_Scale_AngleRange
    JSValue Scale::setAngleRange(JSContext *ctx, JSValueConst this_val, JSValueConst val){
        THIS_NCLASS(Scale,thisobj)
        uint32_t angleRange ;
        if(JS_ToUint32(ctx, (uint32_t *) &angleRange, val)!=0){
            JSTHROW("arg %s of method %s.%s() must be a %s","angleRange","Scale","setAngleRange","number")
        }
        lv_scale_set_angle_range(thisobj->lvobj(), angleRange) ;
        return JS_UNDEFINED ;
    }
    #endif
    #ifndef SETTER_Scale_Rotation
    JSValue Scale::setRotation(JSContext *ctx, JSValueConst this_val, JSValueConst val){
        THIS_NCLASS(Scale,thisobj)
        int32_t rotation ;
        if(JS_ToInt32(ctx, (int32_t *) &rotation, val)!=0){
            JSTHROW("arg %s of method %s.%s() must be a %s","rotation","Scale","setRotation","number")
        }
        lv_scale_set_rotation(thisobj->lvobj(), rotation) ;
        return JS_UNDEFINED ;
    }
    #endif
    // unspported type: const char **
    // JSValue Scale::setTextSrc(JSContext *ctx, JSValueConst this_val, JSValueConst value){}
    // void lv_scale_set_text_src(lv_obj_t * obj, const char * txt_src[])
    #ifndef SETTER_Scale_PostDraw
    JSValue Scale::setPostDraw(JSContext *ctx, JSValueConst this_val, JSValueConst val){
        THIS_NCLASS(Scale,thisobj)
        bool postDraw = JS_ToBool(ctx, val) ;
        lv_scale_set_post_draw(thisobj->lvobj(), postDraw) ;
        return JS_UNDEFINED ;
    }
    #endif
    #ifndef GETTER_Scale_RangeMinValue
    JSValue Scale::getRangeMinValue(JSContext *ctx, JSValueConst this_val){
        THIS_NCLASS(Scale,thisobj)
        int32_t value = lv_scale_get_range_min_value(thisobj->lvobj()) ;
        JSValue retval = JS_NewInt32(ctx, value) ;
        return retval ;
    }
    #endif
    #ifndef GETTER_Scale_RangeMaxValue
    JSValue Scale::getRangeMaxValue(JSContext *ctx, JSValueConst this_val){
        THIS_NCLASS(Scale,thisobj)
        int32_t value = lv_scale_get_range_max_value(thisobj->lvobj()) ;
        JSValue retval = JS_NewInt32(ctx, value) ;
        return retval ;
    }
    #endif
// AUTO GENERATE CODE END [GETSETS] --------

// AUTO GENERATE CODE START [METHODS] --------
    #ifndef METHOD_Scale_setRange
    JSValue Scale::setRange(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        THIS_NCLASS(Obj,thisobj)
        CHECK_ARGC(2)
        int32_t min ;
        if(JS_ToInt32(ctx, (int32_t *) &min, argv[0])!=0){
            JSTHROW("arg %s of method %s.%s() must be a %s","min","Scale","setRange","number")
        }
        int32_t max ;
        if(JS_ToInt32(ctx, (int32_t *) &max, argv[1])!=0){
            JSTHROW("arg %s of method %s.%s() must be a %s","max","Scale","setRange","number")
        }
        lv_scale_set_range( thisobj->lvobj(), min, max ) ;
        return JS_UNDEFINED ;
    }
    #endif

    #ifndef METHOD_Scale_setLineNeedleValue
    JSValue Scale::setLineNeedleValue(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        THIS_NCLASS(Obj,thisobj)
        CHECK_ARGC(3)
        JSVALUE_TO_LVOBJ(argv[0],needle_line)
        int32_t needle_length ;
        if(JS_ToInt32(ctx, (int32_t *) &needle_length, argv[1])!=0){
            JSTHROW("arg %s of method %s.%s() must be a %s","needle_length","Scale","setLineNeedleValue","number")
        }
        int32_t value ;
        if(JS_ToInt32(ctx, (int32_t *) &value, argv[2])!=0){
            JSTHROW("arg %s of method %s.%s() must be a %s","value","Scale","setLineNeedleValue","number")
        }
        lv_scale_set_line_needle_value( thisobj->lvobj(), needle_line, needle_length, value ) ;
        return JS_UNDEFINED ;
    }
    #endif

    #ifndef METHOD_Scale_setImageNeedleValue
    JSValue Scale::setImageNeedleValue(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        THIS_NCLASS(Obj,thisobj)
        CHECK_ARGC(2)
        JSVALUE_TO_LVOBJ(argv[0],needle_img)
        int32_t value ;
        if(JS_ToInt32(ctx, (int32_t *) &value, argv[1])!=0){
            JSTHROW("arg %s of method %s.%s() must be a %s","value","Scale","setImageNeedleValue","number")
        }
        lv_scale_set_image_needle_value( thisobj->lvobj(), needle_img, value ) ;
        return JS_UNDEFINED ;
    }
    #endif

        // Unsupported return type: lv_scale_section_t *
        // lv_scale_section_t * lv_scale_add_section(lv_obj_t * obj)

    // Unsupported arg type: lv_scale_section_t *
    // void lv_scale_section_set_range(lv_scale_section_t * section, int32_t minor_range, int32_t major_range)

    // Unsupported arg type: lv_scale_section_t *
    // void lv_scale_section_set_style(lv_scale_section_t * section, uint32_t part, lv_style_t * section_part_style)
// AUTO GENERATE CODE END [METHODS] --------

}
