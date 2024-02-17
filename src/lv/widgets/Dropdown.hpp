#pragma once

#include "Obj.hpp"
#include "lvgl.h"

namespace be::lv {

    class Dropdown: public Obj {
        DECLARE_NCLASS_META
    private:
        static std::vector<JSCFunctionListEntry> methods ;

    public:
        Dropdown(JSContext * ctx, lv_obj_t * parent=nullptr) ;
        Dropdown(JSContext * ctx, JSValue jsobj, lv_obj_t * lvobj) ;
        static JSValue constructor(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;


// AUTO GENERATE CODE START [GETSETS] --------
        static JSValue setText(JSContext *ctx, JSValueConst this_val, JSValueConst value) ;
        static JSValue setOptions(JSContext *ctx, JSValueConst this_val, JSValueConst value) ;
        static JSValue setOptionsStatic(JSContext *ctx, JSValueConst this_val, JSValueConst value) ;
        static JSValue getSelected(JSContext *ctx, JSValueConst this_val) ;
        static JSValue setSelected(JSContext *ctx, JSValueConst this_val, JSValueConst value) ;
        static JSValue getDir(JSContext *ctx, JSValueConst this_val) ;
        static JSValue setDir(JSContext *ctx, JSValueConst this_val, JSValueConst value) ;
        static JSValue getSelectedHighlight(JSContext *ctx, JSValueConst this_val) ;
        static JSValue setSelectedHighlight(JSContext *ctx, JSValueConst this_val, JSValueConst value) ;
        static JSValue getList(JSContext *ctx, JSValueConst this_val) ;
        static JSValue getOptionCount(JSContext *ctx, JSValueConst this_val) ;

// AUTO GENERATE CODE END [GETSETS] --------

// AUTO GENERATE CODE START [METHODS] --------
        static JSValue jsBindValue(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue jsAddOption(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue jsClearOptions(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue jsOpen(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue jsClose(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue jsIsOpen(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;

// AUTO GENERATE CODE END [METHODS] --------
    } ;

}
