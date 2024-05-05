#include "Checkbox.hpp"

namespace be::lv {
    DEFINE_NCLASS_META(Checkbox, Obj)
    std::vector<JSCFunctionListEntry> Checkbox::methods = {
// AUTO GENERATE CODE START [GETSET LIST] --------
        JS_CGETSET_DEF("text",be::lv::Obj::invalidGetter,Checkbox::setText) ,
        JS_CGETSET_DEF("textStatic",be::lv::Obj::invalidGetter,Checkbox::setTextStatic) ,
// AUTO GENERATE CODE END [GETSET LIST] --------
// AUTO GENERATE CODE START [METHOD LIST] --------

// AUTO GENERATE CODE END [METHOD LIST] --------
    } ;

    Checkbox::Checkbox(JSContext * ctx, JSValue jsobj, lv_obj_t * lvobj)
        : Obj(ctx, Checkbox::build(ctx,jsobj), lvobj)
    {}

    Checkbox::Checkbox(JSContext * ctx, lv_obj_t * parent)
        : Checkbox(ctx, JS_NULL, lv_checkbox_create(parent))
    {}

    JSValue Checkbox::constructor(JSContext *ctx, JSValueConst ctor, int argc, JSValueConst *argv) {
        lv_obj_t * lvparent = nullptr ;
        if(argc>0) {
            JSVALUE_TO_LVOBJ_VAR(argv[0], lvparent)
        }
        JSValue obj = newObject(ctx, ctor) ;
        if( JS_IsException(obj) ) {
            return obj ;
        }
        Checkbox * widget = new Checkbox(ctx, obj, lv_checkbox_create(lvparent)) ;
        return widget->jsobj ;
    }

// AUTO GENERATE CODE START [GETSETS] --------
    /**
     * 访问: 只写
     *
     * > 该属性是对 LVGL C API `lv_checkbox_set_text` 的封装，
     * > 具体用法请参考  [LVGL文档](https://docs.lvgl.io/9.0/API/index.html)
     *
     * @module lv
     * @class Checkbox
     * @property text:
     *
     */
    // unspported type: const char *
    // JSValue Checkbox::getText(JSContext *ctx, JSValueConst this_val){}
    // const char * lv_checkbox_get_text(const lv_obj_t * obj)
    #ifndef SETTER_Checkbox_Text
    JSValue Checkbox::setText(JSContext *ctx, JSValueConst this_val, JSValueConst val){
        THIS_NCLASS(Checkbox,thisobj)
        char * text = (char *)JS_ToCString(ctx, val) ;
        lv_checkbox_set_text(thisobj->lvobj(), text) ;
        JS_FreeCString(ctx, text) ;
        return JS_UNDEFINED ;
    }
    #endif
    /**
     * 访问: 只写
     *
     * > 该属性是对 LVGL C API `lv_checkbox_set_text_static` 的封装，
     * > 具体用法请参考  [LVGL文档](https://docs.lvgl.io/9.0/API/index.html)
     *
     * @module lv
     * @class Checkbox
     * @property textStatic:
     *
     */
    #ifndef SETTER_Checkbox_TextStatic
    JSValue Checkbox::setTextStatic(JSContext *ctx, JSValueConst this_val, JSValueConst val){
        THIS_NCLASS(Checkbox,thisobj)
        char * textStatic = (char *)JS_ToCString(ctx, val) ;
        lv_checkbox_set_text_static(thisobj->lvobj(), textStatic) ;
        JS_FreeCString(ctx, textStatic) ;
        return JS_UNDEFINED ;
    }
    #endif
// AUTO GENERATE CODE END [GETSETS] --------

// AUTO GENERATE CODE START [METHODS] --------

// AUTO GENERATE CODE END [METHODS] --------

}
