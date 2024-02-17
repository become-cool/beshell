#pragma once

#include "Obj.hpp"
#include "lvgl.h"

namespace be::lv {

    class Arc: public Obj {
        DECLARE_NCLASS_META
    private:
        static std::vector<JSCFunctionListEntry> methods ;

    public:
        Arc(JSContext * ctx, lv_obj_t * parent=nullptr) ;
        Arc(JSContext * ctx, JSValue jsobj, lv_obj_t * lvobj) ;
        static JSValue constructor(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;


// AUTO GENERATE CODE START [GETSETS] --------
        static JSValue getRotation(JSContext *ctx, JSValueConst this_val) ;
        static JSValue setRotation(JSContext *ctx, JSValueConst this_val, JSValueConst value) ;
        static JSValue getMode(JSContext *ctx, JSValueConst this_val) ;
        static JSValue setMode(JSContext *ctx, JSValueConst this_val, JSValueConst value) ;
        static JSValue getValue(JSContext *ctx, JSValueConst this_val) ;
        static JSValue setValue(JSContext *ctx, JSValueConst this_val, JSValueConst value) ;
        static JSValue setChangeRate(JSContext *ctx, JSValueConst this_val, JSValueConst value) ;
        static JSValue getKnobOffset(JSContext *ctx, JSValueConst this_val) ;
        static JSValue setKnobOffset(JSContext *ctx, JSValueConst this_val, JSValueConst value) ;
        static JSValue getMinValue(JSContext *ctx, JSValueConst this_val) ;
        static JSValue getMaxValue(JSContext *ctx, JSValueConst this_val) ;
// AUTO GENERATE CODE END [GETSETS] --------

// AUTO GENERATE CODE START [METHODS] --------
        static JSValue jsBindValue(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue jsAlignObjToAngle(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue jsRotateObjToAngle(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
// AUTO GENERATE CODE END [METHODS] --------
    } ;

}
