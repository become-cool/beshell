#pragma once

#include "Obj.hpp"
#include "lvgl.h"

namespace be::lv {

    class TabView: public Obj {
        DECLARE_NCLASS_META
    private:
        static std::vector<JSCFunctionListEntry> methods ;

    public:
        TabView(JSContext * ctx, lv_obj_t * parent=nullptr) ;
        TabView(JSContext * ctx, JSValue jsobj, lv_obj_t * lvobj) ;
        static JSValue constructor(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;


// AUTO GENERATE CODE START [GETSETS] --------
        static JSValue setTabBarPosition(JSContext *ctx, JSValueConst this_val, JSValueConst value) ;
        static JSValue setTabBarSize(JSContext *ctx, JSValueConst this_val, JSValueConst value) ;
        static JSValue getTabCount(JSContext *ctx, JSValueConst this_val) ;
        static JSValue getTabActive(JSContext *ctx, JSValueConst this_val) ;
        static JSValue getContent(JSContext *ctx, JSValueConst this_val) ;
        static JSValue getTabBar(JSContext *ctx, JSValueConst this_val) ;
// AUTO GENERATE CODE END [GETSETS] --------

// AUTO GENERATE CODE START [METHODS] --------
        static JSValue jsAddTab(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue jsRenameTab(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
// AUTO GENERATE CODE END [METHODS] --------
    } ;

}