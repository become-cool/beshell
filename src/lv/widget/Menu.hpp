#pragma once

#include "Obj.hpp"
#include "deps/lvgl/lvgl.h"

namespace be::lv {

    class Menu: public Obj {
        DECLARE_NCLASS_META
    private:
        static std::vector<JSCFunctionListEntry> methods ;
    
    protected:
        Menu(JSContext * ctx, JSValue jsobj, lv_obj_t * lvobj) ;
    public:
        Menu(JSContext * ctx, lv_obj_t * parent=nullptr) ;
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
        static JSValue pageCreate(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue contCreate(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue sectionCreate(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue separatorCreate(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue setLoadPageEvent(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue backButtonIsRoot(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue clearHistory(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
// AUTO GENERATE CODE END [METHODS] --------

    friend class Obj ;
    } ;

}
