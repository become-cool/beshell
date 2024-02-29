#pragma once

#include "Obj.hpp"
#include "lvgl.h"

namespace be::lv {

    class Arc: public Obj {
        DECLARE_NCLASS_META
    private:
        static std::vector<JSCFunctionListEntry> methods ;
    
    protected:
        Arc(JSContext * ctx, JSValue jsobj, lv_obj_t * lvobj) ;
    public:
        Arc(JSContext * ctx, lv_obj_t * parent=nullptr) ;
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
        static JSValue bindValue(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue setAngles(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue setBgAngles(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue setRange(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue alignObjToAngle(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue rotateObjToAngle(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
// AUTO GENERATE CODE END [METHODS] --------

    friend class Obj ;
    } ;

}
