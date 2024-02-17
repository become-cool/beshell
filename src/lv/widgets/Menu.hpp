#pragma once

#include "Obj.hpp"
#include "lvgl.h"

namespace be::lv {

    class Menu: public Obj {
        DECLARE_NCLASS_META
    private:
        static std::vector<JSCFunctionListEntry> methods ;

    public:
        Menu(JSContext * ctx, lv_obj_t * parent=nullptr) ;
        Menu(JSContext * ctx, JSValue jsobj, lv_obj_t * lvobj) ;
        static JSValue constructor(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;


// AUTO GENERATE CODE START [GETSETS] --------
        static JSValue setPage(JSContext *ctx, JSValueConst this_val, JSValueConst value) ;
        static JSValue setSidebarPage(JSContext *ctx, JSValueConst this_val, JSValueConst value) ;
        static JSValue setModeHeader(JSContext *ctx, JSValueConst this_val, JSValueConst value) ;
        static JSValue setModeRootBackButton(JSContext *ctx, JSValueConst this_val, JSValueConst value) ;
        static JSValue getCurMainPage(JSContext *ctx, JSValueConst this_val) ;
        static JSValue getCurSidebarPage(JSContext *ctx, JSValueConst this_val) ;
        static JSValue getMainHeader(JSContext *ctx, JSValueConst this_val) ;
        static JSValue getMainHeaderBackButton(JSContext *ctx, JSValueConst this_val) ;
        static JSValue getSidebarHeader(JSContext *ctx, JSValueConst this_val) ;
        static JSValue getSidebarHeaderBackButton(JSContext *ctx, JSValueConst this_val) ;
// AUTO GENERATE CODE END [GETSETS] --------

// AUTO GENERATE CODE START [METHODS] --------
        static JSValue jsPageCreate(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue jsContCreate(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue jsSectionCreate(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue jsSeparatorCreate(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue jsBackButtonIsRoot(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue jsClearHistory(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
// AUTO GENERATE CODE END [METHODS] --------
    } ;

}
