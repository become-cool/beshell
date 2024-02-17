#include "Chart.hpp"

namespace be::lv {
    DEFINE_NCLASS_META(Chart, Obj)
    std::vector<JSCFunctionListEntry> Chart::methods = {
// AUTO GENERATE CODE START [GETSET LIST] --------
        JS_CGETSET_DEF("type",Chart::getType,Chart::setType) ,
        JS_CGETSET_DEF("pointCount",Chart::getPointCount,Chart::setPointCount) ,
        JS_CGETSET_DEF("updateMode",be::lv::Obj::invalidGetter,Chart::setUpdateMode) ,
        JS_CGETSET_DEF("pressedPoint",Chart::getPressedPoint,be::lv::Obj::invalidSetter) ,
        JS_CGETSET_DEF("firstPointCenterOffset",Chart::getFirstPointCenterOffset,be::lv::Obj::invalidSetter) ,
// AUTO GENERATE CODE END [GETSET LIST] --------
// AUTO GENERATE CODE START [METHOD LIST] --------
        JS_CFUNC_DEF("refresh", 0, Chart::jsRefresh),
        // Unsupported arg type:
        // lv_chart_series_t * lv_chart_add_series(lv_obj_t * obj, lv_color_t color, lv_chart_axis_t axis)
        // void lv_chart_remove_series(lv_obj_t * obj, lv_chart_series_t * series)
        // void lv_chart_hide_series(lv_obj_t * chart, lv_chart_series_t * series, bool hide)
        // lv_chart_cursor_t  * lv_chart_add_cursor(lv_obj_t * obj, lv_color_t color, lv_dir_t dir)
// AUTO GENERATE CODE END [METHOD LIST] --------
    } ;

    Chart::Chart(JSContext * ctx, JSValue jsobj, lv_obj_t * lvobj)
        : Obj(ctx, jsobj, lvobj)
    {}

    Chart::Chart(JSContext * ctx, lv_obj_t * parent)
        : Obj(ctx, JS_NULL, lv_chart_create(parent))
    {}
        
    JSValue Chart::constructor(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        lv_obj_t * lvparent = nullptr ;
        if(argc>0) {
            JSVALUE_TO_LVOBJ_VAR(argv[0], lvparent)
        }
        Chart * widget = new Chart(ctx,lvparent) ;
        return widget->jsobj ;
    }

// AUTO GENERATE CODE START [GETSETS] --------
    JSValue Chart::getType(JSContext *ctx, JSValueConst this_val){
        THIS_NCLASS(Chart,thisobj)
        lv_chart_type_t value = lv_chart_get_type(thisobj->lvobj()) ;
        JSValue retval = lv_chart_type_const_to_jsstr(ctx, value) ;
        return retval ;
    }
    JSValue Chart::setType(JSContext *ctx, JSValueConst this_val, JSValueConst val){
        THIS_NCLASS(Chart,thisobj)
        // argv type
        const char * cstr_val = JS_ToCString(ctx, val) ;
        lv_chart_type_t type;
        if(lv_chart_type_str_to_const(cstr_val,&type)) {
            JS_ThrowReferenceError(ctx,"unknow %s value: %s","lv_chart_type_t",cstr_val) ;
            JS_FreeCString(ctx, cstr_val) ;
            return JS_EXCEPTION ;
        }
        JS_FreeCString(ctx, cstr_val) ;
        lv_chart_set_type(thisobj->lvobj(), type) ;
        return JS_UNDEFINED ;
    }
    JSValue Chart::getPointCount(JSContext *ctx, JSValueConst this_val){
        THIS_NCLASS(Chart,thisobj)
        uint32_t value = lv_chart_get_point_count(thisobj->lvobj()) ;
        JSValue retval = JS_NewUint32(ctx, value) ;
        return retval ;
    }
    JSValue Chart::setPointCount(JSContext *ctx, JSValueConst this_val, JSValueConst val){
        THIS_NCLASS(Chart,thisobj)
        uint32_t pointCount ;
        if(JS_ToUint32(ctx, (uint32_t *) &pointCount, val)!=0){
            JSTHROW("arg %s of method %s.%s() must be a %s","pointCount","Chart","setPointCount","number")
        }
        lv_chart_set_point_count(thisobj->lvobj(), pointCount) ;
        return JS_UNDEFINED ;
    }
    JSValue Chart::setUpdateMode(JSContext *ctx, JSValueConst this_val, JSValueConst val){
        THIS_NCLASS(Chart,thisobj)
        // argv updateMode
        const char * cstr_val = JS_ToCString(ctx, val) ;
        lv_chart_update_mode_t updateMode;
        if(lv_chart_update_mode_str_to_const(cstr_val,&updateMode)) {
            JS_ThrowReferenceError(ctx,"unknow %s value: %s","lv_chart_update_mode_t",cstr_val) ;
            JS_FreeCString(ctx, cstr_val) ;
            return JS_EXCEPTION ;
        }
        JS_FreeCString(ctx, cstr_val) ;
        lv_chart_set_update_mode(thisobj->lvobj(), updateMode) ;
        return JS_UNDEFINED ;
    }
    JSValue Chart::getPressedPoint(JSContext *ctx, JSValueConst this_val){
        THIS_NCLASS(Chart,thisobj)
        uint32_t value = lv_chart_get_pressed_point(thisobj->lvobj()) ;
        JSValue retval = JS_NewUint32(ctx, value) ;
        return retval ;
    }
    JSValue Chart::getFirstPointCenterOffset(JSContext *ctx, JSValueConst this_val){
        THIS_NCLASS(Chart,thisobj)
        int32_t value = lv_chart_get_first_point_center_offset(thisobj->lvobj()) ;
        JSValue retval = JS_NewInt32(ctx, value) ;
        return retval ;
    }
// AUTO GENERATE CODE END [GETSETS] --------

// AUTO GENERATE CODE START [METHODS] --------
        JSValue Chart::jsRefresh(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
            THIS_NCLASS(Obj,thisobj)
            lv_chart_refresh( thisobj->lvobj() ) ;
            return JS_UNDEFINED ;
        }

        // Unsupported return type: lv_chart_series_t *
        // lv_chart_series_t * lv_chart_add_series(lv_obj_t * obj, lv_color_t color, lv_chart_axis_t axis)

        // Unsupported arg type: lv_chart_series_t *
        // void lv_chart_remove_series(lv_obj_t * obj, lv_chart_series_t * series)

        // Unsupported arg type: lv_chart_series_t *
        // void lv_chart_hide_series(lv_obj_t * chart, lv_chart_series_t * series, bool hide)

        // Unsupported return type: lv_chart_cursor_t  *
        // lv_chart_cursor_t  * lv_chart_add_cursor(lv_obj_t * obj, lv_color_t color, lv_dir_t dir)
// AUTO GENERATE CODE END [METHODS] --------

}
