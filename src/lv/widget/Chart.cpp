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
        JS_CFUNC_DEF("setRange", 3, Chart::setRange),
        JS_CFUNC_DEF("setDivLineCount", 2, Chart::setDivLineCount),
        JS_CFUNC_DEF("refresh", 0, Chart::refresh),
        // Unsupported arg type:
        // uint32_t lv_chart_get_x_start_point(const lv_obj_t * obj, lv_chart_series_t * ser)
        // void lv_chart_get_point_pos_by_id(lv_obj_t * obj, lv_chart_series_t * ser, uint32_t id, lv_point_t * p_out)
        // lv_chart_series_t * lv_chart_add_series(lv_obj_t * obj, lv_color_t color, lv_chart_axis_t axis)
        // void lv_chart_remove_series(lv_obj_t * obj, lv_chart_series_t * series)
        // void lv_chart_hide_series(lv_obj_t * chart, lv_chart_series_t * series, bool hide)
        // void lv_chart_set_series_color(lv_obj_t * chart, lv_chart_series_t * series, lv_color_t color)
        // void lv_chart_set_x_start_point(lv_obj_t * obj, lv_chart_series_t * ser, uint32_t id)
        // lv_chart_series_t * lv_chart_get_series_next(const lv_obj_t * chart, const lv_chart_series_t * ser)
        // lv_chart_cursor_t  * lv_chart_add_cursor(lv_obj_t * obj, lv_color_t color, lv_dir_t dir)
        // void lv_chart_set_cursor_pos(lv_obj_t * chart, lv_chart_cursor_t * cursor, lv_point_t * pos)
        // void lv_chart_set_cursor_point(lv_obj_t * chart, lv_chart_cursor_t * cursor, lv_chart_series_t * ser, uint32_t point_id)
        // lv_point_t lv_chart_get_cursor_point(lv_obj_t * chart, lv_chart_cursor_t * cursor)
        // void lv_chart_set_all_value(lv_obj_t * obj, lv_chart_series_t * ser, int32_t value)
        // void lv_chart_set_next_value(lv_obj_t * obj, lv_chart_series_t * ser, int32_t value)
        // void lv_chart_set_value_by_id(lv_obj_t * obj, lv_chart_series_t * ser, uint32_t id, int32_t value)
        // void lv_chart_set_ext_y_array(lv_obj_t * obj, lv_chart_series_t * ser, int32_t array[])
        // void lv_chart_set_ext_x_array(lv_obj_t * obj, lv_chart_series_t * ser, int32_t array[])
        // int32_t * lv_chart_get_y_array(const lv_obj_t * obj, lv_chart_series_t * ser)
        // int32_t * lv_chart_get_x_array(const lv_obj_t * obj, lv_chart_series_t * ser)
// AUTO GENERATE CODE END [METHOD LIST] --------
    } ;

    Chart::Chart(JSContext * ctx, JSValue jsobj, lv_obj_t * lvobj)
        : Obj(ctx, Chart::build(ctx,jsobj), lvobj)
    {}

    Chart::Chart(JSContext * ctx, lv_obj_t * parent)
        : Chart(ctx, JS_NULL, lv_chart_create(parent))
    {}

    JSValue Chart::constructor(JSContext *ctx, JSValueConst ctor, int argc, JSValueConst *argv) {
        lv_obj_t * lvparent = nullptr ;
        if(argc>0) {
            JSVALUE_TO_LVOBJ_VAR(argv[0], lvparent)
        }
        JSValue obj = newObject(ctx, ctor) ;
        if( JS_IsException(obj) ) {
            return obj ;
        }
        Chart * widget = new Chart(ctx, obj, lv_chart_create(lvparent)) ;
        return widget->jsobj ;
    }

// AUTO GENERATE CODE START [GETSETS] --------
    /**
     * 访问: 可读, 可写
     *
     * > 该属性是对 LVGL C API `lv_chart_get_type`, `lv_chart_set_type` 的封装，
     * > 具体用法请参考  [LVGL文档](https://docs.lvgl.io/9.0/API/index.html)
     *
     * @module lv
     * @class Chart
     * @property type:LV-CONST
     *
     */
    #ifndef GETTER_Chart_Type
    JSValue Chart::getType(JSContext *ctx, JSValueConst this_val){
        THIS_NCLASS(Chart,thisobj)
        lv_chart_type_t value = lv_chart_get_type(thisobj->lvobj()) ;
        JSValue retval = lv_chart_type_const_to_jsstr(ctx, value) ;
        return retval ;
    }
    #endif
    #ifndef SETTER_Chart_Type
    JSValue Chart::setType(JSContext *ctx, JSValueConst this_val, JSValueConst val){
        THIS_NCLASS(Chart,thisobj)
        // argv type
        const char * cstr_val = JS_ToCString(ctx, val) ;
        lv_chart_type_t type;
        if(!lv_chart_type_str_to_const(cstr_val,&type)) {
            JS_ThrowReferenceError(ctx,"unknow %s value: %s","lv_chart_type_t",cstr_val) ;
            JS_FreeCString(ctx, cstr_val) ;
            return JS_EXCEPTION ;
        }
        JS_FreeCString(ctx, cstr_val) ;
        lv_chart_set_type(thisobj->lvobj(), type) ;
        return JS_UNDEFINED ;
    }
    #endif
    /**
     * 访问: 可读, 可写
     *
     * > 该属性是对 LVGL C API `lv_chart_get_point_count`, `lv_chart_set_point_count` 的封装，
     * > 具体用法请参考  [LVGL文档](https://docs.lvgl.io/9.0/API/index.html)
     *
     * @module lv
     * @class Chart
     * @property pointCount:number
     *
     */
    #ifndef GETTER_Chart_PointCount
    JSValue Chart::getPointCount(JSContext *ctx, JSValueConst this_val){
        THIS_NCLASS(Chart,thisobj)
        uint32_t value = lv_chart_get_point_count(thisobj->lvobj()) ;
        JSValue retval = JS_NewUint32(ctx, value) ;
        return retval ;
    }
    #endif
    #ifndef SETTER_Chart_PointCount
    JSValue Chart::setPointCount(JSContext *ctx, JSValueConst this_val, JSValueConst val){
        THIS_NCLASS(Chart,thisobj)
        uint32_t pointCount ;
        if(JS_ToUint32(ctx, (uint32_t *) &pointCount, val)!=0){
            JSTHROW("arg %s of method %s.%s() must be a %s","pointCount","Chart","setPointCount","number")
        }
        lv_chart_set_point_count(thisobj->lvobj(), pointCount) ;
        return JS_UNDEFINED ;
    }
    #endif
    /**
     * 访问: 只写
     *
     * > 该属性是对 LVGL C API `lv_chart_set_update_mode` 的封装，
     * > 具体用法请参考  [LVGL文档](https://docs.lvgl.io/9.0/API/index.html)
     *
     * @module lv
     * @class Chart
     * @property updateMode:LV-CONST
     *
     */
    #ifndef SETTER_Chart_UpdateMode
    JSValue Chart::setUpdateMode(JSContext *ctx, JSValueConst this_val, JSValueConst val){
        THIS_NCLASS(Chart,thisobj)
        // argv updateMode
        const char * cstr_val = JS_ToCString(ctx, val) ;
        lv_chart_update_mode_t updateMode;
        if(!lv_chart_update_mode_str_to_const(cstr_val,&updateMode)) {
            JS_ThrowReferenceError(ctx,"unknow %s value: %s","lv_chart_update_mode_t",cstr_val) ;
            JS_FreeCString(ctx, cstr_val) ;
            return JS_EXCEPTION ;
        }
        JS_FreeCString(ctx, cstr_val) ;
        lv_chart_set_update_mode(thisobj->lvobj(), updateMode) ;
        return JS_UNDEFINED ;
    }
    #endif
    /**
     * 访问: 只读
     *
     * > 该属性是对 LVGL C API `lv_chart_get_pressed_point` 的封装，
     * > 具体用法请参考  [LVGL文档](https://docs.lvgl.io/9.0/API/index.html)
     *
     * @module lv
     * @class Chart
     * @property pressedPoint:number
     *
     */
    #ifndef GETTER_Chart_PressedPoint
    JSValue Chart::getPressedPoint(JSContext *ctx, JSValueConst this_val){
        THIS_NCLASS(Chart,thisobj)
        uint32_t value = lv_chart_get_pressed_point(thisobj->lvobj()) ;
        JSValue retval = JS_NewUint32(ctx, value) ;
        return retval ;
    }
    #endif
    /**
     * 访问: 只读
     *
     * > 该属性是对 LVGL C API `lv_chart_get_first_point_center_offset` 的封装，
     * > 具体用法请参考  [LVGL文档](https://docs.lvgl.io/9.0/API/index.html)
     *
     * @module lv
     * @class Chart
     * @property firstPointCenterOffset:number
     *
     */
    #ifndef GETTER_Chart_FirstPointCenterOffset
    JSValue Chart::getFirstPointCenterOffset(JSContext *ctx, JSValueConst this_val){
        THIS_NCLASS(Chart,thisobj)
        int32_t value = lv_chart_get_first_point_center_offset(thisobj->lvobj()) ;
        JSValue retval = JS_NewInt32(ctx, value) ;
        return retval ;
    }
    #endif
// AUTO GENERATE CODE END [GETSETS] --------

// AUTO GENERATE CODE START [METHODS] --------
    #ifndef METHOD_Chart_setRange
    /**
     * > 该方法是对 LVGL C API `lv_chart_set_range` 的封装，具体用法请参考  [LVGL文档](https://docs.lvgl.io/9.0/API/index.html)
     *
     * @module lv
     * @class Chart
     * @method setRange
     *
     */
    JSValue Chart::setRange(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        THIS_NCLASS(Obj,thisobj)
        ASSERT_ARGC(3)
        // argv axis
        const char * cstr_argv_0_ = JS_ToCString(ctx, argv[0]) ;
        lv_chart_axis_t axis;
        if(!lv_chart_axis_str_to_const(cstr_argv_0_,&axis)) {
            JS_ThrowReferenceError(ctx,"unknow %s value: %s","lv_chart_axis_t",cstr_argv_0_) ;
            JS_FreeCString(ctx, cstr_argv_0_) ;
            return JS_EXCEPTION ;
        }
        JS_FreeCString(ctx, cstr_argv_0_) ;
        int32_t min ;
        if(JS_ToInt32(ctx, (int32_t *) &min, argv[1])!=0){
            JSTHROW("arg %s of method %s.%s() must be a %s","min","Chart","setRange","number")
        }
        int32_t max ;
        if(JS_ToInt32(ctx, (int32_t *) &max, argv[2])!=0){
            JSTHROW("arg %s of method %s.%s() must be a %s","max","Chart","setRange","number")
        }
        lv_chart_set_range( thisobj->lvobj(), axis, min, max ) ;
        return JS_UNDEFINED ;
    }
    #endif

    #ifndef METHOD_Chart_setDivLineCount
    /**
     * > 该方法是对 LVGL C API `lv_chart_set_div_line_count` 的封装，具体用法请参考  [LVGL文档](https://docs.lvgl.io/9.0/API/index.html)
     *
     * @module lv
     * @class Chart
     * @method setDivLineCount
     *
     */
    JSValue Chart::setDivLineCount(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        THIS_NCLASS(Obj,thisobj)
        ASSERT_ARGC(2)
        uint8_t hdiv ;
        if(JS_ToUint32(ctx, (uint32_t *) &hdiv, argv[0])!=0){
            JSTHROW("arg %s of method %s.%s() must be a %s","hdiv","Chart","setDivLineCount","number")
        }
        uint8_t vdiv ;
        if(JS_ToUint32(ctx, (uint32_t *) &vdiv, argv[1])!=0){
            JSTHROW("arg %s of method %s.%s() must be a %s","vdiv","Chart","setDivLineCount","number")
        }
        lv_chart_set_div_line_count( thisobj->lvobj(), hdiv, vdiv ) ;
        return JS_UNDEFINED ;
    }
    #endif

    // Unsupported arg type: lv_chart_series_t *
    // uint32_t lv_chart_get_x_start_point(const lv_obj_t * obj, lv_chart_series_t * ser)

    // Unsupported arg type: lv_chart_series_t *
    // void lv_chart_get_point_pos_by_id(lv_obj_t * obj, lv_chart_series_t * ser, uint32_t id, lv_point_t * p_out)

    #ifndef METHOD_Chart_refresh
    /**
     * > 该方法是对 LVGL C API `lv_chart_refresh` 的封装，具体用法请参考  [LVGL文档](https://docs.lvgl.io/9.0/API/index.html)
     *
     * @module lv
     * @class Chart
     * @method refresh
     *
     */
    JSValue Chart::refresh(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        THIS_NCLASS(Obj,thisobj)
        lv_chart_refresh( thisobj->lvobj() ) ;
        return JS_UNDEFINED ;
    }
    #endif

        // Unsupported return type: lv_chart_series_t *
        // lv_chart_series_t * lv_chart_add_series(lv_obj_t * obj, lv_color_t color, lv_chart_axis_t axis)

    // Unsupported arg type: lv_chart_series_t *
    // void lv_chart_remove_series(lv_obj_t * obj, lv_chart_series_t * series)

    // Unsupported arg type: lv_chart_series_t *
    // void lv_chart_hide_series(lv_obj_t * chart, lv_chart_series_t * series, bool hide)

    // Unsupported arg type: lv_chart_series_t *
    // void lv_chart_set_series_color(lv_obj_t * chart, lv_chart_series_t * series, lv_color_t color)

    // Unsupported arg type: lv_chart_series_t *
    // void lv_chart_set_x_start_point(lv_obj_t * obj, lv_chart_series_t * ser, uint32_t id)

    // Unsupported arg type: const lv_chart_series_t *
    // lv_chart_series_t * lv_chart_get_series_next(const lv_obj_t * chart, const lv_chart_series_t * ser)

        // Unsupported return type: lv_chart_cursor_t  *
        // lv_chart_cursor_t  * lv_chart_add_cursor(lv_obj_t * obj, lv_color_t color, lv_dir_t dir)

    // Unsupported arg type: lv_chart_cursor_t *
    // void lv_chart_set_cursor_pos(lv_obj_t * chart, lv_chart_cursor_t * cursor, lv_point_t * pos)

    // Unsupported arg type: lv_chart_cursor_t *
    // void lv_chart_set_cursor_point(lv_obj_t * chart, lv_chart_cursor_t * cursor, lv_chart_series_t * ser, uint32_t point_id)

    // Unsupported arg type: lv_chart_cursor_t *
    // lv_point_t lv_chart_get_cursor_point(lv_obj_t * chart, lv_chart_cursor_t * cursor)

    // Unsupported arg type: lv_chart_series_t *
    // void lv_chart_set_all_value(lv_obj_t * obj, lv_chart_series_t * ser, int32_t value)

    // Unsupported arg type: lv_chart_series_t *
    // void lv_chart_set_next_value(lv_obj_t * obj, lv_chart_series_t * ser, int32_t value)

    // Unsupported arg type: lv_chart_series_t *
    // void lv_chart_set_value_by_id(lv_obj_t * obj, lv_chart_series_t * ser, uint32_t id, int32_t value)

    // Unsupported arg type: lv_chart_series_t *
    // void lv_chart_set_ext_y_array(lv_obj_t * obj, lv_chart_series_t * ser, int32_t array[])

    // Unsupported arg type: lv_chart_series_t *
    // void lv_chart_set_ext_x_array(lv_obj_t * obj, lv_chart_series_t * ser, int32_t array[])

    // Unsupported arg type: lv_chart_series_t *
    // int32_t * lv_chart_get_y_array(const lv_obj_t * obj, lv_chart_series_t * ser)

    // Unsupported arg type: lv_chart_series_t *
    // int32_t * lv_chart_get_x_array(const lv_obj_t * obj, lv_chart_series_t * ser)
// AUTO GENERATE CODE END [METHODS] --------

}
