#pragma once

#include "Obj.hpp"
#include "lvgl.h"

namespace be::lv {

    class MsgBox: public Obj {
        DECLARE_NCLASS_META
    private:
        static std::vector<JSCFunctionListEntry> methods ;

    public:
        MsgBox(JSContext * ctx, lv_obj_t * parent=nullptr) ;
        MsgBox(JSContext * ctx, JSValue jsobj, lv_obj_t * lvobj) ;
        static JSValue constructor(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;


// AUTO GENERATE CODE START [GETSETS] --------
        static JSValue getHeader(JSContext *ctx, JSValueConst this_val) ;
        static JSValue getFooter(JSContext *ctx, JSValueConst this_val) ;
        static JSValue getContent(JSContext *ctx, JSValueConst this_val) ;
        static JSValue getTitle(JSContext *ctx, JSValueConst this_val) ;
// AUTO GENERATE CODE END [GETSETS] --------

// AUTO GENERATE CODE START [METHODS] --------
        static JSValue addTitle(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue addHeaderButton(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue addText(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue addFooterButton(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue addCloseButton(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue close(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue closeAsync(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
// AUTO GENERATE CODE END [METHODS] --------
    } ;

}
