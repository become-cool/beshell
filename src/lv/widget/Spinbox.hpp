#pragma once

#include "Obj.hpp"
#include "lvgl.h"

namespace be::lv {

    class SpinBox: public Obj {
        DECLARE_NCLASS_META
    private:
        static std::vector<JSCFunctionListEntry> methods ;

    public:
        SpinBox(JSContext * ctx, lv_obj_t * parent=nullptr) ;
        SpinBox(JSContext * ctx, JSValue jsobj, lv_obj_t * lvobj) ;
        static JSValue constructor(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;


// AUTO GENERATE CODE START [GETSETS] --------
        static JSValue getValue(JSContext *ctx, JSValueConst this_val) ;
        static JSValue setValue(JSContext *ctx, JSValueConst this_val, JSValueConst value) ;
        static JSValue getRollover(JSContext *ctx, JSValueConst this_val) ;
        static JSValue setRollover(JSContext *ctx, JSValueConst this_val, JSValueConst value) ;
        static JSValue getStep(JSContext *ctx, JSValueConst this_val) ;
        static JSValue setStep(JSContext *ctx, JSValueConst this_val, JSValueConst value) ;
        static JSValue setCursorPos(JSContext *ctx, JSValueConst this_val, JSValueConst value) ;
        static JSValue setDigitStepDirection(JSContext *ctx, JSValueConst this_val, JSValueConst value) ;

// AUTO GENERATE CODE END [GETSETS] --------

// AUTO GENERATE CODE START [METHODS] --------
        static JSValue setDigitFormat(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue setRange(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue stepNext(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue stepPrev(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue increment(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue decrement(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;

// AUTO GENERATE CODE END [METHODS] --------
    } ;

}
