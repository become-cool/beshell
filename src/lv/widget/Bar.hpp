#pragma once

#include "Obj.hpp"
#include "lvgl.h"

namespace be::lv {

    class Bar: public Obj {
        DECLARE_NCLASS_META
    private:
        static std::vector<JSCFunctionListEntry> methods ;
    
    protected:
        Bar(JSContext * ctx, JSValue jsobj, lv_obj_t * lvobj) ;
    public:
        Bar(JSContext * ctx, lv_obj_t * parent=nullptr) ;
        static JSValue constructor(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;


// AUTO GENERATE CODE START [GETSETS] --------
        static JSValue getMode(JSContext *ctx, JSValueConst this_val) ;
        static JSValue setMode(JSContext *ctx, JSValueConst this_val, JSValueConst value) ;
        static JSValue getValue(JSContext *ctx, JSValueConst this_val) ;
        static JSValue getStartValue(JSContext *ctx, JSValueConst this_val) ;
        static JSValue getMinValue(JSContext *ctx, JSValueConst this_val) ;
        static JSValue getMaxValue(JSContext *ctx, JSValueConst this_val) ;
// AUTO GENERATE CODE END [GETSETS] --------

// AUTO GENERATE CODE START [METHODS] --------
        static JSValue setValue(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue setStartValue(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue setRange(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue isSymmetrical(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
// AUTO GENERATE CODE END [METHODS] --------

    friend class Obj ;
    } ;

}
