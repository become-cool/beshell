#pragma once

#include "Obj.hpp"
#include "lvgl.h"

namespace be::lv {

    class Label: public Obj {
        DECLARE_NCLASS_META
    private:
        static std::vector<JSCFunctionListEntry> methods ;

    public:
        Label(JSContext * ctx, lv_obj_t * parent=nullptr) ;
        Label(JSContext * ctx, JSValue jsobj, lv_obj_t * lvobj) ;
        static JSValue constructor(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;


// AUTO GENERATE CODE START [GETSETS] --------
        static JSValue getText(JSContext *ctx, JSValueConst this_val) ;
        static JSValue setText(JSContext *ctx, JSValueConst this_val, JSValueConst value) ;
        static JSValue setTextStatic(JSContext *ctx, JSValueConst this_val, JSValueConst value) ;
        static JSValue getLongMode(JSContext *ctx, JSValueConst this_val) ;
        static JSValue setLongMode(JSContext *ctx, JSValueConst this_val, JSValueConst value) ;
        static JSValue getTextSelectionStart(JSContext *ctx, JSValueConst this_val) ;
        static JSValue setTextSelectionStart(JSContext *ctx, JSValueConst this_val, JSValueConst value) ;
        static JSValue getTextSelectionEnd(JSContext *ctx, JSValueConst this_val) ;
        static JSValue setTextSelectionEnd(JSContext *ctx, JSValueConst this_val, JSValueConst value) ;

// AUTO GENERATE CODE END [GETSETS] --------

// AUTO GENERATE CODE START [METHODS] --------
        static JSValue jsBindText(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue jsIsCharUnderPos(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue jsInsText(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue jsCutText(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;

// AUTO GENERATE CODE END [METHODS] --------
    } ;

}
