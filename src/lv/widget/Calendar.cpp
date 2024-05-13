#include "Calendar.hpp"

namespace be::lv {
    DEFINE_NCLASS_META(Calendar, Obj)
    std::vector<JSCFunctionListEntry> Calendar::methods = {
// AUTO GENERATE CODE START [GETSET LIST] --------
        JS_CGETSET_DEF("btnmatrix",Calendar::getBtnmatrix,be::lv::Obj::invalidSetter) ,
// AUTO GENERATE CODE END [GETSET LIST] --------
// AUTO GENERATE CODE START [METHOD LIST] --------
        JS_CFUNC_DEF("setTodayDate", 3, Calendar::setTodayDate),
        JS_CFUNC_DEF("setShowedDate", 2, Calendar::setShowedDate),
        JS_CFUNC_DEF("headerArrowCreate", 0, Calendar::headerArrowCreate),
        JS_CFUNC_DEF("headerDropdownCreate", 0, Calendar::headerDropdownCreate),
        JS_CFUNC_DEF("headerDropdownSetYearList", 1, Calendar::headerDropdownSetYearList),
        // Unsupported arg type:
        // void lv_calendar_set_highlighted_dates(lv_obj_t * obj, lv_calendar_date_t highlighted[], size_t date_num)
        // lv_result_t lv_calendar_get_pressed_date(const lv_obj_t * calendar, lv_calendar_date_t * date)
// AUTO GENERATE CODE END [METHOD LIST] --------
    } ;

    Calendar::Calendar(JSContext * ctx, JSValue jsobj, lv_obj_t * lvobj)
        : Obj(ctx, Calendar::build(ctx,jsobj), lvobj)
    {}

    Calendar::Calendar(JSContext * ctx, lv_obj_t * parent)
        : Calendar(ctx, JS_NULL, lv_calendar_create(parent))
    {}

    JSValue Calendar::constructor(JSContext *ctx, JSValueConst ctor, int argc, JSValueConst *argv) {
        lv_obj_t * lvparent = nullptr ;
        if(argc>0) {
            JSVALUE_TO_LVOBJ_VAR(argv[0], lvparent)
        }
        JSValue obj = newObject(ctx, ctor) ;
        if( JS_IsException(obj) ) {
            return obj ;
        }
        Calendar * widget = new Calendar(ctx, obj, lv_calendar_create(lvparent)) ;
        return widget->jsobj ;
    }

// AUTO GENERATE CODE START [GETSETS] --------
    // unspported type: const char **
    // JSValue Calendar::setDayNames(JSContext *ctx, JSValueConst this_val, JSValueConst value){}
    // void lv_calendar_set_day_names(lv_obj_t * obj, const char ** day_names)
    /**
     * 访问: 只读
     *
     * > 该属性是对 LVGL C API `lv_calendar_get_btnmatrix` 的封装，
     * > 具体用法请参考  [LVGL文档](https://docs.lvgl.io/9.0/API/index.html)
     *
     * @module lv
     * @class Calendar
     * @property btnmatrix:[obj](obj.html)
     *
     */
    #ifndef GETTER_Calendar_Btnmatrix
    JSValue Calendar::getBtnmatrix(JSContext *ctx, JSValueConst this_val){
        THIS_NCLASS(Calendar,thisobj)
        lv_obj_t * value = lv_calendar_get_btnmatrix(thisobj->lvobj()) ;
        JSValue retval = value? be::lv::Obj::wrap(ctx, (lv_obj_t*)value)->jsobj: JS_NULL ;
        return retval ;
    }
    #endif
    // unspported type: const lv_calendar_date_t *
    // JSValue Calendar::getTodayDate(JSContext *ctx, JSValueConst this_val){}
    // const lv_calendar_date_t * lv_calendar_get_today_date(const lv_obj_t * calendar)
    // unspported type: const lv_calendar_date_t *
    // JSValue Calendar::getShowedDate(JSContext *ctx, JSValueConst this_val){}
    // const lv_calendar_date_t * lv_calendar_get_showed_date(const lv_obj_t * calendar)
    // unspported type: lv_calendar_date_t *
    // JSValue Calendar::getHighlightedDates(JSContext *ctx, JSValueConst this_val){}
    // lv_calendar_date_t * lv_calendar_get_highlighted_dates(const lv_obj_t * calendar)
    // unspported type: size_t
    // JSValue Calendar::getHighlightedDatesNum(JSContext *ctx, JSValueConst this_val){}
    // size_t lv_calendar_get_highlighted_dates_num(const lv_obj_t * calendar)
// AUTO GENERATE CODE END [GETSETS] --------

// AUTO GENERATE CODE START [METHODS] --------
    #ifndef METHOD_Calendar_setTodayDate
    /**
     * > 该方法是对 LVGL C API `lv_calendar_set_today_date` 的封装，具体用法请参考  [LVGL文档](https://docs.lvgl.io/9.0/API/index.html)
     *
     * @module lv
     * @class Calendar
     * @method setTodayDate
     *
     */
    JSValue Calendar::setTodayDate(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        THIS_NCLASS(Obj,thisobj)
        ASSERT_ARGC(3)
        uint32_t year ;
        if(JS_ToUint32(ctx, (uint32_t *) &year, argv[0])!=0){
            JSTHROW("arg %s of method %s.%s() must be a %s","year","Calendar","setTodayDate","number")
        }
        uint32_t month ;
        if(JS_ToUint32(ctx, (uint32_t *) &month, argv[1])!=0){
            JSTHROW("arg %s of method %s.%s() must be a %s","month","Calendar","setTodayDate","number")
        }
        uint32_t day ;
        if(JS_ToUint32(ctx, (uint32_t *) &day, argv[2])!=0){
            JSTHROW("arg %s of method %s.%s() must be a %s","day","Calendar","setTodayDate","number")
        }
        lv_calendar_set_today_date( thisobj->lvobj(), year, month, day ) ;
        return JS_UNDEFINED ;
    }
    #endif

    #ifndef METHOD_Calendar_setShowedDate
    /**
     * > 该方法是对 LVGL C API `lv_calendar_set_showed_date` 的封装，具体用法请参考  [LVGL文档](https://docs.lvgl.io/9.0/API/index.html)
     *
     * @module lv
     * @class Calendar
     * @method setShowedDate
     *
     */
    JSValue Calendar::setShowedDate(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        THIS_NCLASS(Obj,thisobj)
        ASSERT_ARGC(2)
        uint32_t year ;
        if(JS_ToUint32(ctx, (uint32_t *) &year, argv[0])!=0){
            JSTHROW("arg %s of method %s.%s() must be a %s","year","Calendar","setShowedDate","number")
        }
        uint32_t month ;
        if(JS_ToUint32(ctx, (uint32_t *) &month, argv[1])!=0){
            JSTHROW("arg %s of method %s.%s() must be a %s","month","Calendar","setShowedDate","number")
        }
        lv_calendar_set_showed_date( thisobj->lvobj(), year, month ) ;
        return JS_UNDEFINED ;
    }
    #endif

    // Unsupported arg type: lv_calendar_date_t*
    // void lv_calendar_set_highlighted_dates(lv_obj_t * obj, lv_calendar_date_t highlighted[], size_t date_num)

    // Unsupported arg type: lv_calendar_date_t *
    // lv_result_t lv_calendar_get_pressed_date(const lv_obj_t * calendar, lv_calendar_date_t * date)

    #ifndef METHOD_Calendar_headerArrowCreate
    /**
     * > 该方法是对 LVGL C API `lv_calendar_header_arrow_create` 的封装，具体用法请参考  [LVGL文档](https://docs.lvgl.io/9.0/API/index.html)
     *
     * @module lv
     * @class Calendar
     * @method headerArrowCreate
     *
     */
    JSValue Calendar::headerArrowCreate(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        THIS_NCLASS(Obj,thisobj)
        lv_obj_t * retval = lv_calendar_header_arrow_create( thisobj->lvobj() ) ;
        JSValue jsretval = retval? be::lv::Obj::wrap(ctx, (lv_obj_t*)retval)->jsobj: JS_NULL ;
        return jsretval ;
    }
    #endif

    #ifndef METHOD_Calendar_headerDropdownCreate
    /**
     * > 该方法是对 LVGL C API `lv_calendar_header_dropdown_create` 的封装，具体用法请参考  [LVGL文档](https://docs.lvgl.io/9.0/API/index.html)
     *
     * @module lv
     * @class Calendar
     * @method headerDropdownCreate
     *
     */
    JSValue Calendar::headerDropdownCreate(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        THIS_NCLASS(Obj,thisobj)
        lv_obj_t * retval = lv_calendar_header_dropdown_create( thisobj->lvobj() ) ;
        JSValue jsretval = retval? be::lv::Obj::wrap(ctx, (lv_obj_t*)retval)->jsobj: JS_NULL ;
        return jsretval ;
    }
    #endif

    #ifndef METHOD_Calendar_headerDropdownSetYearList
    /**
     * > 该方法是对 LVGL C API `lv_calendar_header_dropdown_set_year_list` 的封装，具体用法请参考  [LVGL文档](https://docs.lvgl.io/9.0/API/index.html)
     *
     * @module lv
     * @class Calendar
     * @method headerDropdownSetYearList
     *
     */
    JSValue Calendar::headerDropdownSetYearList(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        THIS_NCLASS(Obj,thisobj)
        ASSERT_ARGC(1)
        char * years_list = (char *)JS_ToCString(ctx, argv[0]) ;
        lv_calendar_header_dropdown_set_year_list( thisobj->lvobj(), years_list ) ;
        return JS_UNDEFINED ;
    }
    #endif
// AUTO GENERATE CODE END [METHODS] --------

}
