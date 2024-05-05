#include "Roller.hpp"

namespace be::lv {
    DEFINE_NCLASS_META(Roller, Obj)
    std::vector<JSCFunctionListEntry> Roller::methods = {
// AUTO GENERATE CODE START [GETSET LIST] --------
        JS_CGETSET_DEF("visibleRowCount",be::lv::Obj::invalidGetter,Roller::setVisibleRowCount) ,
        JS_CGETSET_DEF("selected",Roller::getSelected,be::lv::Obj::invalidSetter) ,
        JS_CGETSET_DEF("optionCount",Roller::getOptionCount,be::lv::Obj::invalidSetter) ,
// AUTO GENERATE CODE END [GETSET LIST] --------
// AUTO GENERATE CODE START [METHOD LIST] --------
        JS_CFUNC_DEF("setOptions", 2, Roller::setOptions),
        JS_CFUNC_DEF("setSelected", 2, Roller::setSelected),
        JS_CFUNC_DEF("getSelectedStr", 2, Roller::getSelectedStr),
        // Unsupported arg type:
        // lv_observer_t * lv_roller_bind_value(lv_obj_t * obj, lv_subject_t * subject)
// AUTO GENERATE CODE END [METHOD LIST] --------
    } ;

    Roller::Roller(JSContext * ctx, JSValue jsobj, lv_obj_t * lvobj)
        : Obj(ctx, Roller::build(ctx,jsobj), lvobj)
    {}

    Roller::Roller(JSContext * ctx, lv_obj_t * parent)
        : Roller(ctx, JS_NULL, lv_roller_create(parent))
    {}

    JSValue Roller::constructor(JSContext *ctx, JSValueConst ctor, int argc, JSValueConst *argv) {
        lv_obj_t * lvparent = nullptr ;
        if(argc>0) {
            JSVALUE_TO_LVOBJ_VAR(argv[0], lvparent)
        }
        JSValue obj = newObject(ctx, ctor) ;
        if( JS_IsException(obj) ) {
            return obj ;
        }
        Roller * widget = new Roller(ctx, obj, lv_roller_create(lvparent)) ;
        return widget->jsobj ;
    }

// AUTO GENERATE CODE START [GETSETS] --------
    /**
     * 访问: 只写
     *
     * > 该属性是对 LVGL C API `lv_roller_set_visible_row_count` 的封装，
     * > 具体用法请参考  [LVGL文档](https://docs.lvgl.io/9.0/API/index.html)
     *
     * @module lv
     * @class Roller
     * @property visibleRowCount:number
     *
     */
    #ifndef SETTER_Roller_VisibleRowCount
    JSValue Roller::setVisibleRowCount(JSContext *ctx, JSValueConst this_val, JSValueConst val){
        THIS_NCLASS(Roller,thisobj)
        uint32_t visibleRowCount ;
        if(JS_ToUint32(ctx, (uint32_t *) &visibleRowCount, val)!=0){
            JSTHROW("arg %s of method %s.%s() must be a %s","visibleRowCount","Roller","setVisibleRowCount","number")
        }
        lv_roller_set_visible_row_count(thisobj->lvobj(), visibleRowCount) ;
        return JS_UNDEFINED ;
    }
    #endif
    /**
     * 访问: 只读
     *
     * > 该属性是对 LVGL C API `lv_roller_get_selected` 的封装，
     * > 具体用法请参考  [LVGL文档](https://docs.lvgl.io/9.0/API/index.html)
     *
     * @module lv
     * @class Roller
     * @property selected:number
     *
     */
    #ifndef GETTER_Roller_Selected
    JSValue Roller::getSelected(JSContext *ctx, JSValueConst this_val){
        THIS_NCLASS(Roller,thisobj)
        uint32_t value = lv_roller_get_selected(thisobj->lvobj()) ;
        JSValue retval = JS_NewUint32(ctx, value) ;
        return retval ;
    }
    #endif
    // unspported type: const char *
    // JSValue Roller::getOptions(JSContext *ctx, JSValueConst this_val){}
    // const char * lv_roller_get_options(const lv_obj_t * obj)
    /**
     * 访问: 只读
     *
     * > 该属性是对 LVGL C API `lv_roller_get_option_count` 的封装，
     * > 具体用法请参考  [LVGL文档](https://docs.lvgl.io/9.0/API/index.html)
     *
     * @module lv
     * @class Roller
     * @property optionCount:number
     *
     */
    #ifndef GETTER_Roller_OptionCount
    JSValue Roller::getOptionCount(JSContext *ctx, JSValueConst this_val){
        THIS_NCLASS(Roller,thisobj)
        uint32_t value = lv_roller_get_option_count(thisobj->lvobj()) ;
        JSValue retval = JS_NewUint32(ctx, value) ;
        return retval ;
    }
    #endif
// AUTO GENERATE CODE END [GETSETS] --------

// AUTO GENERATE CODE START [METHODS] --------
    // Unsupported arg type: lv_subject_t *
    // lv_observer_t * lv_roller_bind_value(lv_obj_t * obj, lv_subject_t * subject)

    #ifndef METHOD_Roller_setOptions
    /**
     * > 该方法是对 LVGL C API `lv_roller_set_options` 的封装，具体用法请参考  [LVGL文档](https://docs.lvgl.io/9.0/API/index.html)
     *
     * @module lv
     * @class Roller
     * @method setOptions
     *
     */
    JSValue Roller::setOptions(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        THIS_NCLASS(Obj,thisobj)
        ASSERT_ARGC(2)
        char * options = (char *)JS_ToCString(ctx, argv[0]) ;
        // argv mode
        const char * cstr_argv_1_ = JS_ToCString(ctx, argv[1]) ;
        lv_roller_mode_t mode;
        if(!lv_roller_mode_str_to_const(cstr_argv_1_,&mode)) {
            JS_ThrowReferenceError(ctx,"unknow %s value: %s","lv_roller_mode_t",cstr_argv_1_) ;
            JS_FreeCString(ctx, cstr_argv_1_) ;
            return JS_EXCEPTION ;
        }
        JS_FreeCString(ctx, cstr_argv_1_) ;
        lv_roller_set_options( thisobj->lvobj(), options, mode ) ;
        return JS_UNDEFINED ;
    }
    #endif

    #ifndef METHOD_Roller_setSelected
    /**
     * > 该方法是对 LVGL C API `lv_roller_set_selected` 的封装，具体用法请参考  [LVGL文档](https://docs.lvgl.io/9.0/API/index.html)
     *
     * @module lv
     * @class Roller
     * @method setSelected
     *
     */
    JSValue Roller::setSelected(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        THIS_NCLASS(Obj,thisobj)
        ASSERT_ARGC(2)
        uint32_t sel_opt ;
        if(JS_ToUint32(ctx, (uint32_t *) &sel_opt, argv[0])!=0){
            JSTHROW("arg %s of method %s.%s() must be a %s","sel_opt","Roller","setSelected","number")
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
        lv_roller_set_selected( thisobj->lvobj(), sel_opt, anim ) ;
        return JS_UNDEFINED ;
    }
    #endif

    #ifndef METHOD_Roller_getSelectedStr
    /**
     * > 该方法是对 LVGL C API `lv_roller_get_selected_str` 的封装，具体用法请参考  [LVGL文档](https://docs.lvgl.io/9.0/API/index.html)
     *
     * @module lv
     * @class Roller
     * @method getSelectedStr
     *
     */
    JSValue Roller::getSelectedStr(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        THIS_NCLASS(Obj,thisobj)
        ASSERT_ARGC(2)
        char * buf = (char *)JS_ToCString(ctx, argv[0]) ;
        uint32_t buf_size ;
        if(JS_ToUint32(ctx, (uint32_t *) &buf_size, argv[1])!=0){
            JSTHROW("arg %s of method %s.%s() must be a %s","buf_size","Roller","getSelectedStr","number")
        }
        lv_roller_get_selected_str( thisobj->lvobj(), buf, buf_size ) ;
        return JS_UNDEFINED ;
    }
    #endif
// AUTO GENERATE CODE END [METHODS] --------

}
