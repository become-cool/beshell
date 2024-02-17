#pragma once

#include "Obj.hpp"
#include "lvgl.h"

namespace be::lv {

    class Chart: public Obj {
        DECLARE_NCLASS_META
    private:
        static std::vector<JSCFunctionListEntry> methods ;

    public:
        Chart(JSContext * ctx, lv_obj_t * parent=nullptr) ;
        Chart(JSContext * ctx, JSValue jsobj, lv_obj_t * lvobj) ;
        static JSValue constructor(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;


// AUTO GENERATE CODE START [GETSETS] --------
        static JSValue getType(JSContext *ctx, JSValueConst this_val) ;
        static JSValue setType(JSContext *ctx, JSValueConst this_val, JSValueConst value) ;
        static JSValue getPointCount(JSContext *ctx, JSValueConst this_val) ;
        static JSValue setPointCount(JSContext *ctx, JSValueConst this_val, JSValueConst value) ;
        static JSValue setUpdateMode(JSContext *ctx, JSValueConst this_val, JSValueConst value) ;
        static JSValue getPressedPoint(JSContext *ctx, JSValueConst this_val) ;
        static JSValue getFirstPointCenterOffset(JSContext *ctx, JSValueConst this_val) ;

// AUTO GENERATE CODE END [GETSETS] --------

// AUTO GENERATE CODE START [METHODS] --------
        static JSValue jsRefresh(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue jsAddSeries(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue jsRemoveSeries(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue jsHideSeries(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue jsAddCursor(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;

// AUTO GENERATE CODE END [METHODS] --------
    } ;

}
