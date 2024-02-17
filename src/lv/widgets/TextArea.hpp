#pragma once

#include "Obj.hpp"
#include "lvgl.h"

namespace be::lv {

    class TextArea: public Obj {
        DECLARE_NCLASS_META
    private:
        static std::vector<JSCFunctionListEntry> methods ;

    public:
        TextArea(JSContext * ctx, lv_obj_t * parent=nullptr) ;
        TextArea(JSContext * ctx, JSValue jsobj, lv_obj_t * lvobj) ;
        static JSValue constructor(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;


// AUTO GENERATE CODE START [GETSETS] --------
        static JSValue setText(JSContext *ctx, JSValueConst this_val, JSValueConst value) ;
        static JSValue setPlaceholderText(JSContext *ctx, JSValueConst this_val, JSValueConst value) ;
        static JSValue getCursorPos(JSContext *ctx, JSValueConst this_val) ;
        static JSValue setCursorPos(JSContext *ctx, JSValueConst this_val, JSValueConst value) ;
        static JSValue getCursorClickPos(JSContext *ctx, JSValueConst this_val) ;
        static JSValue setCursorClickPos(JSContext *ctx, JSValueConst this_val, JSValueConst value) ;
        static JSValue getPasswordMode(JSContext *ctx, JSValueConst this_val) ;
        static JSValue setPasswordMode(JSContext *ctx, JSValueConst this_val, JSValueConst value) ;
        static JSValue setPasswordBullet(JSContext *ctx, JSValueConst this_val, JSValueConst value) ;
        static JSValue getOneLine(JSContext *ctx, JSValueConst this_val) ;
        static JSValue setOneLine(JSContext *ctx, JSValueConst this_val, JSValueConst value) ;
        static JSValue setAcceptedChars(JSContext *ctx, JSValueConst this_val, JSValueConst value) ;
        static JSValue getMaxLength(JSContext *ctx, JSValueConst this_val) ;
        static JSValue setMaxLength(JSContext *ctx, JSValueConst this_val, JSValueConst value) ;
        static JSValue setInsertReplace(JSContext *ctx, JSValueConst this_val, JSValueConst value) ;
        static JSValue getTextSelection(JSContext *ctx, JSValueConst this_val) ;
        static JSValue setTextSelection(JSContext *ctx, JSValueConst this_val, JSValueConst value) ;
        static JSValue getPasswordShowTime(JSContext *ctx, JSValueConst this_val) ;
        static JSValue setPasswordShowTime(JSContext *ctx, JSValueConst this_val, JSValueConst value) ;
        static JSValue setAlign(JSContext *ctx, JSValueConst this_val, JSValueConst value) ;
        static JSValue getLabel(JSContext *ctx, JSValueConst this_val) ;
        static JSValue getCurrentChar(JSContext *ctx, JSValueConst this_val) ;
// AUTO GENERATE CODE END [GETSETS] --------

// AUTO GENERATE CODE START [METHODS] --------
        static JSValue jsAddChar(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue jsAddText(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue jsDeleteChar(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue jsDeleteCharForward(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue jsTextIsSelected(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue jsClearSelection(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue jsCursorRight(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue jsCursorLeft(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue jsCursorDown(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue jsCursorUp(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
// AUTO GENERATE CODE END [METHODS] --------
    } ;

}
