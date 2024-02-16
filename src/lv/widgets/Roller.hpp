#pragma once

#include "Obj.hpp"
#include "lvgl.h"

namespace be::lv {

    class Roller: public Obj {
        DECLARE_NCLASS_META
    private:
        static std::vector<JSCFunctionListEntry> methods ;

    public:
        Roller(JSContext * ctx, lv_obj_t * parent=nullptr) ;
        Roller(JSContext * ctx, JSValue jsobj, lv_obj_t * lvobj) ;
        static JSValue constructor(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;


// AUTO GENERATE CODE START [GETSETS] --------
        static JSValue setVisibleRowCount(JSContext *ctx, JSValueConst this_val, JSValueConst value) ;
        static JSValue getSelected(JSContext *ctx, JSValueConst this_val) ;
        static JSValue getOptionCount(JSContext *ctx, JSValueConst this_val) ;

// AUTO GENERATE CODE END [GETSETS] --------

// AUTO GENERATE CODE START [METHODS] --------
        static JSValue jsBindValue(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;

// AUTO GENERATE CODE END [METHODS] --------
    } ;

}
