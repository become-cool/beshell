#pragma once

#include "Obj.hpp"
#include "lvgl.h"

namespace be::lv {

    class Calendar: public Obj {
        DECLARE_NCLASS_META
    private:
        static std::vector<JSCFunctionListEntry> methods ;

    public:
        Calendar(JSContext * ctx, lv_obj_t * parent=nullptr) ;
        Calendar(JSContext * ctx, JSValue jsobj, lv_obj_t * lvobj) ;
        static JSValue constructor(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;


// AUTO GENERATE CODE START [GETSETS] --------
        static JSValue getBtnmatrix(JSContext *ctx, JSValueConst this_val) ;
// AUTO GENERATE CODE END [GETSETS] --------

// AUTO GENERATE CODE START [METHODS] --------
        static JSValue setTodayDate(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue setShowedDate(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue setHighlightedDates(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue getPressedDate(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue headerArrowCreate(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue headerDropdownCreate(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue headerDropdownSetYearList(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
// AUTO GENERATE CODE END [METHODS] --------
    } ;

}
