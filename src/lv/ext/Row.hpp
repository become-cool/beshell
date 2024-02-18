#pragma once

#include "../widget/Obj.hpp"

namespace be::lv {

    class Row: public Obj {
        DECLARE_NCLASS_META
    private:
        static std::vector<JSCFunctionListEntry> methods ;
        // static std::vector<JSCFunctionListEntry> staticMethods ;

    public:
        Row(JSContext * ctx, lv_obj_t * parent=nullptr) ;
        Row(JSContext * ctx, JSValue jsobj, lv_obj_t * lvobj) ;

        static JSValue constructor(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue jsMethod(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;

    } ;

}