#pragma once

#include "Obj.hpp"
#include "lvgl.h"

namespace be::lv {

    class AnimImg: public Obj {
        DECLARE_NCLASS_META
    private:
        static std::vector<JSCFunctionListEntry> methods ;

    public:
        AnimImg(JSContext * ctx, lv_obj_t * parent=nullptr) ;
        AnimImg(JSContext * ctx, JSValue jsobj, lv_obj_t * lvobj) ;
        static JSValue constructor(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;


// AUTO GENERATE CODE START [GETSETS] --------
        static JSValue getDuration(JSContext *ctx, JSValueConst this_val) ;
        static JSValue setDuration(JSContext *ctx, JSValueConst this_val, JSValueConst value) ;
        static JSValue getRepeatCount(JSContext *ctx, JSValueConst this_val) ;
        static JSValue setRepeatCount(JSContext *ctx, JSValueConst this_val, JSValueConst value) ;
        static JSValue getSrcCount(JSContext *ctx, JSValueConst this_val) ;
// AUTO GENERATE CODE END [GETSETS] --------

// AUTO GENERATE CODE START [METHODS] --------
        static JSValue jsStart(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
// AUTO GENERATE CODE END [METHODS] --------
    } ;

}
