#pragma once

#include "../widget/Obj.hpp"

namespace be::lv {

    class ColorPicker: public Obj {
        DECLARE_NCLASS_META
    private:
        static std::vector<JSCFunctionListEntry> methods ;
        // static std::vector<JSCFunctionListEntry> staticMethods ;

    public:
        ColorPicker(JSContext * ctx, lv_obj_t * parent=nullptr) ;
        ColorPicker(JSContext * ctx, JSValue jsobj, lv_obj_t * lvobj) ;

        static JSValue constructor(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;

        static void draw(lv_event_t * e) ;
    } ;

}