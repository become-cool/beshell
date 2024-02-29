#pragma once

#include "Obj.hpp"
#include "lvgl.h"

namespace be::lv {

    class Chart: public Obj {
        DECLARE_NCLASS_META
    private:
        static std::vector<JSCFunctionListEntry> methods ;
    
    protected:
        Chart(JSContext * ctx, JSValue jsobj, lv_obj_t * lvobj) ;
    public:
        Chart(JSContext * ctx, lv_obj_t * parent=nullptr) ;
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
        static JSValue setRange(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue setDivLineCount(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue getXStartPoint(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue getPointPosById(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue refresh(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue addSeries(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue removeSeries(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue hideSeries(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue setSeriesColor(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue setXStartPoint(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue getSeriesNext(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue addCursor(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue setCursorPos(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue setCursorPoint(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue getCursorPoint(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue setAllValue(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue setNextValue(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue setValueById(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue setExtYArray(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue setExtXArray(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue getYArray(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue getXArray(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;

// AUTO GENERATE CODE END [METHODS] --------

    friend class Obj ;
    } ;

}
