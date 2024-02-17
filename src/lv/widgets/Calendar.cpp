#include "Calendar.hpp"

namespace be::lv {
    DEFINE_NCLASS_META(Calendar, Obj)
    std::vector<JSCFunctionListEntry> Calendar::methods = {
// AUTO GENERATE CODE START [GETSET LIST] --------
        JS_CGETSET_DEF("btnmatrix",Calendar::getBtnmatrix,be::lv::Obj::invalidSetter) ,
// AUTO GENERATE CODE END [GETSET LIST] --------
// AUTO GENERATE CODE START [METHOD LIST] --------
        JS_CFUNC_DEF("headerArrowCreate", 0, Calendar::jsHeaderArrowCreate),
        JS_CFUNC_DEF("headerDropdownCreate", 0, Calendar::jsHeaderDropdownCreate),
        JS_CFUNC_DEF("headerDropdownSetYearList", 1, Calendar::jsHeaderDropdownSetYearList),
// AUTO GENERATE CODE END [METHOD LIST] --------
    } ;

    Calendar::Calendar(JSContext * ctx, JSValue jsobj, lv_obj_t * lvobj)
        : Obj(ctx, Calendar::build(ctx,jsobj), lvobj)
    {}

    Calendar::Calendar(JSContext * ctx, lv_obj_t * parent)
        : Calendar(ctx, JS_NULL, lv_calendar_create(parent))
    {}
        
    JSValue Calendar::constructor(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        lv_obj_t * lvparent = nullptr ;
        if(argc>0) {
            JSVALUE_TO_LVOBJ_VAR(argv[0], lvparent)
        }
        Calendar * widget = new Calendar(ctx,lvparent) ;
        return widget->jsobj ;
    }

// AUTO GENERATE CODE START [GETSETS] --------
    // unspported type: const char **
    // JSValue Calendar::setDayNames(JSContext *ctx, JSValueConst this_val, JSValueConst value){}
    // void lv_calendar_set_day_names(lv_obj_t * obj, const char ** day_names)
    JSValue Calendar::getBtnmatrix(JSContext *ctx, JSValueConst this_val){
        THIS_NCLASS(Calendar,thisobj)
        lv_obj_t * value = lv_calendar_get_btnmatrix(thisobj->lvobj()) ;
        JSValue retval = value? be::lv::Obj::wrap(ctx, (lv_obj_t*)value)->jsobj: JS_NULL ;
        return retval ;
    }
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
        JSValue Calendar::jsHeaderArrowCreate(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
            THIS_NCLASS(Obj,thisobj)
            lv_obj_t * retval = lv_calendar_header_arrow_create( thisobj->lvobj() ) ;
            JSValue jsretval = retval? be::lv::Obj::wrap(ctx, (lv_obj_t*)retval)->jsobj: JS_NULL ;
            return jsretval ;
        }

        JSValue Calendar::jsHeaderDropdownCreate(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
            THIS_NCLASS(Obj,thisobj)
            lv_obj_t * retval = lv_calendar_header_dropdown_create( thisobj->lvobj() ) ;
            JSValue jsretval = retval? be::lv::Obj::wrap(ctx, (lv_obj_t*)retval)->jsobj: JS_NULL ;
            return jsretval ;
        }

        JSValue Calendar::jsHeaderDropdownSetYearList(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
            THIS_NCLASS(Obj,thisobj)
            CHECK_ARGC(1)
            char * years_list = (char *)JS_ToCString(ctx, argv[0]) ;
            lv_calendar_header_dropdown_set_year_list( thisobj->lvobj(), years_list ) ;
            return JS_UNDEFINED ;
        }
// AUTO GENERATE CODE END [METHODS] --------

}
