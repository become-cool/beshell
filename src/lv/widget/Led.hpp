#pragma once

#include "Obj.hpp"
#include "lvgl.h"

namespace be::lv {

    class Led: public Obj {
        DECLARE_NCLASS_META
    private:
        static std::vector<JSCFunctionListEntry> methods ;

    public:
        Led(JSContext * ctx, lv_obj_t * parent=nullptr) ;
        Led(JSContext * ctx, JSValue jsobj, lv_obj_t * lvobj) ;
        static JSValue constructor(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;


// AUTO GENERATE CODE START [GETSETS] --------
        static JSValue setColor(JSContext *ctx, JSValueConst this_val, JSValueConst value) ;
        static JSValue getBrightness(JSContext *ctx, JSValueConst this_val) ;
        static JSValue setBrightness(JSContext *ctx, JSValueConst this_val, JSValueConst value) ;
// AUTO GENERATE CODE END [GETSETS] --------

// AUTO GENERATE CODE START [METHODS] --------
        static JSValue on(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue off(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue toggle(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
// AUTO GENERATE CODE END [METHODS] --------
    } ;

}
