#pragma once

#include "Obj.hpp"
#include "deps/lvgl/lvgl.h"

namespace be::lv {

    class Roller: public Obj {
        DECLARE_NCLASS_META
    private:
        static std::vector<JSCFunctionListEntry> methods ;
    
    protected:
        Roller(JSContext * ctx, JSValue jsobj, lv_obj_t * lvobj) ;
    public:
        Roller(JSContext * ctx, lv_obj_t * parent=nullptr) ;
        static JSValue constructor(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;


// AUTO GENERATE CODE START [GETSETS] --------
        static JSValue setVisibleRowCount(JSContext *ctx, JSValueConst this_val, JSValueConst value) ;
        static JSValue getSelected(JSContext *ctx, JSValueConst this_val) ;
        static JSValue getOptionCount(JSContext *ctx, JSValueConst this_val) ;
// AUTO GENERATE CODE END [GETSETS] --------

// AUTO GENERATE CODE START [METHODS] --------
        static JSValue bindValue(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue setOptions(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue setSelected(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue getSelectedStr(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
// AUTO GENERATE CODE END [METHODS] --------

    friend class Obj ;
    } ;

}
