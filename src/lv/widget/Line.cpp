#include "Line.hpp"

namespace be::lv {
    DEFINE_NCLASS_META(Line, Obj)
    std::vector<JSCFunctionListEntry> Line::methods = {
// AUTO GENERATE CODE START [GETSET LIST] --------
        JS_CGETSET_DEF("yInvert",Line::getYInvert,Line::setYInvert) ,
// AUTO GENERATE CODE END [GETSET LIST] --------
// AUTO GENERATE CODE START [METHOD LIST] --------
        // Unsupported arg type:
        // void lv_line_set_points(lv_obj_t * obj, const lv_point_precise_t points[], uint32_t point_num)
// AUTO GENERATE CODE END [METHOD LIST] --------
    } ;

    Line::Line(JSContext * ctx, JSValue jsobj, lv_obj_t * lvobj)
        : Obj(ctx, Line::build(ctx,jsobj), lvobj)
    {}

    Line::Line(JSContext * ctx, lv_obj_t * parent)
        : Line(ctx, JS_NULL, lv_line_create(parent))
    {}

    JSValue Line::constructor(JSContext *ctx, JSValueConst ctor, int argc, JSValueConst *argv) {
        lv_obj_t * lvparent = nullptr ;
        if(argc>0) {
            JSVALUE_TO_LVOBJ_VAR(argv[0], lvparent)
        }
        JSValue obj = newObject(ctx, ctor) ;
        if( JS_IsException(obj) ) {
            return obj ;
        }
        Line * widget = new Line(ctx, obj, lv_line_create(lvparent)) ;
        return widget->jsobj ;
    }

// AUTO GENERATE CODE START [GETSETS] --------
    /**
     * 访问: 可读, 可写
     *
     * > 该属性是对 LVGL C API `lv_line_get_y_invert`, `lv_line_set_y_invert` 的封装，
     * > 具体用法请参考  [LVGL文档](https://docs.lvgl.io/9.0/API/index.html)
     *
     * @module lv
     * @class Line
     * @property yInvert:number
     *
     */
    #ifndef GETTER_Line_YInvert
    JSValue Line::getYInvert(JSContext *ctx, JSValueConst this_val){
        THIS_NCLASS(Line,thisobj)
        bool value = lv_line_get_y_invert(thisobj->lvobj()) ;
        JSValue retval = JS_NewBool(ctx, value) ;
        return retval ;
    }
    #endif
    #ifndef SETTER_Line_YInvert
    JSValue Line::setYInvert(JSContext *ctx, JSValueConst this_val, JSValueConst val){
        THIS_NCLASS(Line,thisobj)
        bool yInvert = JS_ToBool(ctx, val) ;
        lv_line_set_y_invert(thisobj->lvobj(), yInvert) ;
        return JS_UNDEFINED ;
    }
    #endif
// AUTO GENERATE CODE END [GETSETS] --------

// AUTO GENERATE CODE START [METHODS] --------
    // Unsupported arg type: const lv_point_precise_t*
    // void lv_line_set_points(lv_obj_t * obj, const lv_point_precise_t points[], uint32_t point_num)
// AUTO GENERATE CODE END [METHODS] --------

}
