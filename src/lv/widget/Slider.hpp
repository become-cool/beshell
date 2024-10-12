#pragma once

#include "Bar.hpp"
#include "deps/lvgl/lvgl.h"

namespace be::lv {

    class Slider: public Bar {
        DECLARE_NCLASS_META
    private:
        static std::vector<JSCFunctionListEntry> methods ;
    
    protected:
        Slider(JSContext * ctx, JSValue jsobj, lv_obj_t * lvobj) ;
    public:
        Slider(JSContext * ctx, lv_obj_t * parent=nullptr) ;
        static JSValue constructor(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;


        // static JSValue setRange(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        // static JSValue getMin(JSContext *ctx, JSValueConst this_val) ;
        // static JSValue getMax(JSContext *ctx, JSValueConst this_val) ;
        static JSValue setLeft(JSContext *ctx, JSValueConst this_val, JSValueConst value) ;
        static JSValue getLeft(JSContext *ctx, JSValueConst this_val) ;


// AUTO GENERATE CODE START [GETSETS] --------
// AUTO GENERATE CODE END [GETSETS] --------

// AUTO GENERATE CODE START [METHODS] --------
        static JSValue bindValue(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue isDragged(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
// AUTO GENERATE CODE END [METHODS] --------

    friend class Obj ;
    } ;

}
