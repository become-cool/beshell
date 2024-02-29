#pragma once

#include "Obj.hpp"
#include "lvgl.h"

namespace be::lv {

    class Checkbox: public Obj {
        DECLARE_NCLASS_META
    private:
        static std::vector<JSCFunctionListEntry> methods ;
    
    protected:
        Checkbox(JSContext * ctx, JSValue jsobj, lv_obj_t * lvobj) ;
    public:
        Checkbox(JSContext * ctx, lv_obj_t * parent=nullptr) ;
        static JSValue constructor(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;


// AUTO GENERATE CODE START [GETSETS] --------
        static JSValue setText(JSContext *ctx, JSValueConst this_val, JSValueConst value) ;
        static JSValue setTextStatic(JSContext *ctx, JSValueConst this_val, JSValueConst value) ;

// AUTO GENERATE CODE END [GETSETS] --------

// AUTO GENERATE CODE START [METHODS] --------


// AUTO GENERATE CODE END [METHODS] --------

    friend class Obj ;
    } ;

}
