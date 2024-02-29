#pragma once

#include "Obj.hpp"
#include "lvgl.h"

namespace be::lv {

    class Label: public Obj {
        DECLARE_NCLASS_META
    private:
        static std::vector<JSCFunctionListEntry> methods ;

    protected:
        Label(JSContext * ctx, JSValue jsobj, lv_obj_t * lvobj) ;
    public:
        Label(JSContext * ctx, lv_obj_t * parent=nullptr) ;
        static JSValue constructor(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;

        static JSValue getFont(JSContext *ctx, JSValueConst this_val) ;
        static JSValue setFont(JSContext *ctx, JSValueConst this_val, JSValueConst val) ;

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
        static JSValue bindText(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue setTextFmt(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue getLetterPos(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue getLetterOn(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue isCharUnderPos(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue insText(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue cutText(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
// AUTO GENERATE CODE END [METHODS] --------
    friend class Obj ;
    } ;

}
