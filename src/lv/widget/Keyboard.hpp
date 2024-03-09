#pragma once

#include "Obj.hpp"
#include "lvgl.h"

namespace be::lv {

    class Keyboard: public Obj {
        DECLARE_NCLASS_META
    private:
        static std::vector<JSCFunctionListEntry> methods ;
    
    protected:
        Keyboard(JSContext * ctx, JSValue jsobj, lv_obj_t * lvobj) ;
    public:
        Keyboard(JSContext * ctx, lv_obj_t * parent=nullptr) ;
        static JSValue constructor(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;


// AUTO GENERATE CODE START [GETSETS] --------
        static JSValue getTextarea(JSContext *ctx, JSValueConst this_val) ;
        static JSValue setTextarea(JSContext *ctx, JSValueConst this_val, JSValueConst value) ;
        static JSValue getMode(JSContext *ctx, JSValueConst this_val) ;
        static JSValue setMode(JSContext *ctx, JSValueConst this_val, JSValueConst value) ;
        static JSValue setPopovers(JSContext *ctx, JSValueConst this_val, JSValueConst value) ;
// AUTO GENERATE CODE END [GETSETS] --------

// AUTO GENERATE CODE START [METHODS] --------
        static JSValue setMap(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue defEventCb(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
// AUTO GENERATE CODE END [METHODS] --------

    friend class Obj ;
    } ;

}
