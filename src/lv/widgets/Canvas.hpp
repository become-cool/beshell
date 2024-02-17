#pragma once

#include "Obj.hpp"
#include "lvgl.h"

namespace be::lv {

    class Canvas: public Obj {
        DECLARE_NCLASS_META
    private:
        static std::vector<JSCFunctionListEntry> methods ;

    public:
        Canvas(JSContext * ctx, lv_obj_t * parent=nullptr) ;
        Canvas(JSContext * ctx, JSValue jsobj, lv_obj_t * lvobj) ;
        static JSValue constructor(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;


// AUTO GENERATE CODE START [GETSETS] --------
// AUTO GENERATE CODE END [GETSETS] --------

// AUTO GENERATE CODE START [METHODS] --------
        static JSValue jsCopyBuf(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue jsFillBg(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue jsInitLayer(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue jsFinishLayer(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
// AUTO GENERATE CODE END [METHODS] --------
    } ;

}
