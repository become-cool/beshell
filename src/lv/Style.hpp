#pragma once

#include <NativeClass.hpp>
#include "deps/lvgl/lvgl.h"

namespace be::lv {

    class Style: public be::NativeClass {
        DECLARE_NCLASS_META
    private:
        static std::vector<JSCFunctionListEntry> methods ;

        lv_style_t * lvstyle = nullptr ;

    public:
        Style(JSContext * ctx) ;

        static JSValue constructor(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;

        static JSValue propToJS(JSContext * ctx, lv_style_prop_t prop, lv_style_value_t value) ;
        static bool propToValue(JSContext * ctx, lv_style_prop_t prop, JSValue jsval, lv_style_value_t * value) ;

// AUTO GENERATE CODE START [PROPS] --------
// AUTO GENERATE CODE END [PROPS] --------
    } ;

}