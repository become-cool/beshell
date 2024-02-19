#include "Rect.hpp"

using namespace std ;

namespace be::lv {

    DEFINE_NCLASS_META(Rect, Obj)

    std::vector<JSCFunctionListEntry> Rect::methods = {
        JS_CFUNC_DEF("method", 0, Rect::jsMethod),
    } ;

    Rect::Rect(JSContext * ctx, JSValue jsobj, lv_obj_t * lvobj)
        : Obj(ctx, Rect::build(ctx,jsobj), lvobj)
    {
        lv_obj_remove_style(_lvobj, NULL, LV_PART_ANY | LV_STATE_ANY);

        lv_style_selector_t selector = LV_PART_MAIN | LV_STATE_DEFAULT ;
        lv_style_value_t value ;
        value.num = 0 ;
        lv_obj_set_local_style_prop(_lvobj, LV_STYLE_PAD_TOP, value, selector) ;
        lv_obj_set_local_style_prop(_lvobj, LV_STYLE_PAD_BOTTOM, value, selector) ;
        lv_obj_set_local_style_prop(_lvobj, LV_STYLE_PAD_RIGHT, value, selector) ;
        lv_obj_set_local_style_prop(_lvobj, LV_STYLE_PAD_LEFT, value, selector) ;
        lv_obj_set_local_style_prop(_lvobj, LV_STYLE_BORDER_WIDTH, value, selector) ;
        lv_obj_set_local_style_prop(_lvobj, LV_STYLE_BORDER_WIDTH, value, selector) ;
        lv_obj_set_local_style_prop(_lvobj, LV_STYLE_RADIUS, value, selector) ;
        lv_obj_set_local_style_prop(_lvobj, LV_STYLE_BG_OPA, value, selector) ;

        lv_obj_clear_flag(_lvobj, LV_OBJ_FLAG_SCROLLABLE) ;
    }

    Rect::Rect(JSContext * ctx, lv_obj_t * parent)
        : Rect(ctx, JS_NULL, lv_obj_create(parent))
    {}

    JSValue Rect::constructor(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        lv_obj_t * lvparent = nullptr ;
        if(argc>0) {
            JSVALUE_TO_LVOBJ_VAR(argv[0], lvparent)
        }
        Rect * widget = new Rect(ctx,lvparent) ;
        return widget->jsobj ;
    }

    JSValue Rect::jsMethod(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        return JS_UNDEFINED ;
    }

}