#pragma once

#include "Obj.hpp"
#include "lvgl.h"

namespace be::lv {

    class BtnMatrix: public Obj {
        DECLARE_NCLASS_META
    private:
        static std::vector<JSCFunctionListEntry> methods ;

    public:
        BtnMatrix(JSContext * ctx, lv_obj_t * parent=nullptr) ;
        BtnMatrix(JSContext * ctx, JSValue jsobj, lv_obj_t * lvobj) ;
        static JSValue constructor(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;


// AUTO GENERATE CODE START [GETSETS] --------
        static JSValue getSelectedButton(JSContext *ctx, JSValueConst this_val) ;
        static JSValue setSelectedButton(JSContext *ctx, JSValueConst this_val, JSValueConst value) ;
        static JSValue setButtonCtrlAll(JSContext *ctx, JSValueConst this_val, JSValueConst value) ;
        static JSValue getOneChecked(JSContext *ctx, JSValueConst this_val) ;
        static JSValue setOneChecked(JSContext *ctx, JSValueConst this_val, JSValueConst value) ;
        static JSValue getPopovers(JSContext *ctx, JSValueConst this_val) ;
// AUTO GENERATE CODE END [GETSETS] --------

// AUTO GENERATE CODE START [METHODS] --------
        static JSValue setButtonCtrl(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue clearButtonCtrl(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue clearButtonCtrlAll(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue setButtonWidth(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue getButtonText(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue hasButtonCtrl(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
// AUTO GENERATE CODE END [METHODS] --------
    } ;

}
