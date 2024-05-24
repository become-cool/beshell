#pragma once

#include "Obj.hpp"
#include "deps/lvgl/lvgl.h"

namespace be::lv {

    class Win: public Obj {
        DECLARE_NCLASS_META
    private:
        static std::vector<JSCFunctionListEntry> methods ;
    
    protected:
        Win(JSContext * ctx, JSValue jsobj, lv_obj_t * lvobj) ;
    public:
        Win(JSContext * ctx, lv_obj_t * parent=nullptr) ;
        static JSValue constructor(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;


// AUTO GENERATE CODE START [GETSETS] --------
        static JSValue getHeader(JSContext *ctx, JSValueConst this_val) ;
        static JSValue getContent(JSContext *ctx, JSValueConst this_val) ;
// AUTO GENERATE CODE END [GETSETS] --------

// AUTO GENERATE CODE START [METHODS] --------
        static JSValue addTitle(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue addButton(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
// AUTO GENERATE CODE END [METHODS] --------

    friend class Obj ;
    } ;

}
