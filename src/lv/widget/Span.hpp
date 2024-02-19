#pragma once

#include "Obj.hpp"
#include "lvgl.h"

namespace be::lv {

    class Span: public Obj {
        DECLARE_NCLASS_META
    private:
        static std::vector<JSCFunctionListEntry> methods ;

    public:
        Span(JSContext * ctx, lv_obj_t * parent=nullptr) ;
        Span(JSContext * ctx, JSValue jsobj, lv_obj_t * lvobj) ;
        static JSValue constructor(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;


// AUTO GENERATE CODE START [GETSETS] --------
        static JSValue getAlign(JSContext *ctx, JSValueConst this_val) ;
        static JSValue setAlign(JSContext *ctx, JSValueConst this_val, JSValueConst value) ;
        static JSValue getOverflow(JSContext *ctx, JSValueConst this_val) ;
        static JSValue setOverflow(JSContext *ctx, JSValueConst this_val, JSValueConst value) ;
        static JSValue getIndent(JSContext *ctx, JSValueConst this_val) ;
        static JSValue setIndent(JSContext *ctx, JSValueConst this_val, JSValueConst value) ;
        static JSValue getMode(JSContext *ctx, JSValueConst this_val) ;
        static JSValue setMode(JSContext *ctx, JSValueConst this_val, JSValueConst value) ;
        static JSValue getMaxLines(JSContext *ctx, JSValueConst this_val) ;
        static JSValue setMaxLines(JSContext *ctx, JSValueConst this_val, JSValueConst value) ;
        static JSValue getSpanCount(JSContext *ctx, JSValueConst this_val) ;
        static JSValue getMaxLineHeight(JSContext *ctx, JSValueConst this_val) ;
// AUTO GENERATE CODE END [GETSETS] --------

// AUTO GENERATE CODE START [METHODS] --------
        static JSValue newSpan(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue deleteSpan(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue getChild(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue getExpandWidth(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue getExpandHeight(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue refrMode(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
// AUTO GENERATE CODE END [METHODS] --------
    } ;

}
