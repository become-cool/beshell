#pragma once

#include "Obj.hpp"
#include "lvgl.h"

namespace be::lv {

    class Line: public Obj {
        DECLARE_NCLASS_META
    private:
        static std::vector<JSCFunctionListEntry> methods ;
    
    protected:
        Line(JSContext * ctx, JSValue jsobj, lv_obj_t * lvobj) ;
    public:
        Line(JSContext * ctx, lv_obj_t * parent=nullptr) ;
        static JSValue constructor(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;


// AUTO GENERATE CODE START [GETSETS] --------
        static JSValue getYInvert(JSContext *ctx, JSValueConst this_val) ;
        static JSValue setYInvert(JSContext *ctx, JSValueConst this_val, JSValueConst value) ;

// AUTO GENERATE CODE END [GETSETS] --------

// AUTO GENERATE CODE START [METHODS] --------
        static JSValue setPoints(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;

// AUTO GENERATE CODE END [METHODS] --------

    friend class Obj ;
    } ;

}
