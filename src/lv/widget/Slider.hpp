#pragma once

#include "Bar.hpp"
#include "lvgl.h"

namespace be::lv {

    class Slider: public Bar {
        DECLARE_NCLASS_META
    private:
        static std::vector<JSCFunctionListEntry> methods ;

    public:
        Slider(JSContext * ctx, lv_obj_t * parent=nullptr) ;
        Slider(JSContext * ctx, JSValue jsobj, lv_obj_t * lvobj) ;
        static JSValue constructor(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;


// AUTO GENERATE CODE START [GETSETS] --------
// AUTO GENERATE CODE END [GETSETS] --------

// AUTO GENERATE CODE START [METHODS] --------
        static JSValue bindValue(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue isDragged(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
// AUTO GENERATE CODE END [METHODS] --------
    } ;

}
