#pragma once

#include "Obj.hpp"
#include "lvgl.h"

namespace be::lv {

    class Switch: public Obj {
        DECLARE_NCLASS_META
    private:
        static std::vector<JSCFunctionListEntry> methods ;
    
    protected:
        Switch(JSContext * ctx, JSValue jsobj, lv_obj_t * lvobj) ;
    public:
        Switch(JSContext * ctx, lv_obj_t * parent=nullptr) ;
        static JSValue constructor(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;


// AUTO GENERATE CODE START [GETSETS] --------

// AUTO GENERATE CODE END [GETSETS] --------

// AUTO GENERATE CODE START [METHODS] --------


// AUTO GENERATE CODE END [METHODS] --------

    friend class Obj ;
    } ;

}
