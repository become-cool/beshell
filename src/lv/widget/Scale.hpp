#pragma once

#include "Obj.hpp"
#include "lvgl.h"

namespace be::lv {

    class Scale: public Obj {
        DECLARE_NCLASS_META
    private:
        static std::vector<JSCFunctionListEntry> methods ;
    
    protected:
        Scale(JSContext * ctx, JSValue jsobj, lv_obj_t * lvobj) ;
    public:
        Scale(JSContext * ctx, lv_obj_t * parent=nullptr) ;
        static JSValue constructor(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;


// AUTO GENERATE CODE START [GETSETS] --------
        static JSValue getMode(JSContext *ctx, JSValueConst this_val) ;
        static JSValue setMode(JSContext *ctx, JSValueConst this_val, JSValueConst value) ;
        static JSValue getTotalTickCount(JSContext *ctx, JSValueConst this_val) ;
        static JSValue setTotalTickCount(JSContext *ctx, JSValueConst this_val, JSValueConst value) ;
        static JSValue getMajorTickEvery(JSContext *ctx, JSValueConst this_val) ;
        static JSValue setMajorTickEvery(JSContext *ctx, JSValueConst this_val, JSValueConst value) ;
        static JSValue getLabelShow(JSContext *ctx, JSValueConst this_val) ;
        static JSValue setLabelShow(JSContext *ctx, JSValueConst this_val, JSValueConst value) ;
        static JSValue getAngleRange(JSContext *ctx, JSValueConst this_val) ;
        static JSValue setAngleRange(JSContext *ctx, JSValueConst this_val, JSValueConst value) ;
        static JSValue setRotation(JSContext *ctx, JSValueConst this_val, JSValueConst value) ;
        static JSValue setPostDraw(JSContext *ctx, JSValueConst this_val, JSValueConst value) ;
        static JSValue getRangeMinValue(JSContext *ctx, JSValueConst this_val) ;
        static JSValue getRangeMaxValue(JSContext *ctx, JSValueConst this_val) ;
// AUTO GENERATE CODE END [GETSETS] --------

// AUTO GENERATE CODE START [METHODS] --------
        static JSValue setRange(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue setLineNeedleValue(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue setImageNeedleValue(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue addSection(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue sectionSetRange(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue sectionSetStyle(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
// AUTO GENERATE CODE END [METHODS] --------

    friend class Obj ;
    } ;

}
