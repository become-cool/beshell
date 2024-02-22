#pragma once

#include "Obj.hpp"
#include "lvgl.h"

namespace be::lv {

    class ImgBtn: public Obj {
        DECLARE_NCLASS_META
    private:
        static std::vector<JSCFunctionListEntry> methods ;

    public:
        ImgBtn(JSContext * ctx, lv_obj_t * parent=nullptr) ;
        ImgBtn(JSContext * ctx, JSValue jsobj, lv_obj_t * lvobj) ;
        static JSValue constructor(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;


// AUTO GENERATE CODE START [GETSETS] --------
        static JSValue setState(JSContext *ctx, JSValueConst this_val, JSValueConst value) ;

// AUTO GENERATE CODE END [GETSETS] --------

// AUTO GENERATE CODE START [METHODS] --------
        static JSValue setSrc(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue getSrcLeft(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue getSrcMiddle(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue getSrcRight(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;

// AUTO GENERATE CODE END [METHODS] --------
    } ;

}
