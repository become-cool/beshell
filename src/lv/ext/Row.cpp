#include "Row.hpp"

using namespace std ;

namespace be::lv {

    DEFINE_NCLASS_META(Row, Obj)

    std::vector<JSCFunctionListEntry> Row::methods = {
        JS_CFUNC_DEF("method", 0, Row::jsMethod),
    } ;

    Row::Row(JSContext * ctx, JSValue jsobj, lv_obj_t * lvobj)
        : Obj(ctx, Row::build(ctx,jsobj), lvobj)
    {
        lv_obj_set_flex_flow(lvobj, LV_FLEX_FLOW_ROW) ;
    }

    Row::Row(JSContext * ctx, lv_obj_t * parent)
        : Row(ctx, JS_NULL, lv_obj_create(parent))
    {}

    JSValue Row::constructor(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {        lv_obj_t * lvparent = nullptr ;
        if(argc>0) {
            JSVALUE_TO_LVOBJ_VAR(argv[0], lvparent)
        }
        auto obj = new Row(ctx,lvparent) ;
        return obj->jsobj ;
    }

    JSValue Row::jsMethod(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        return JS_UNDEFINED ;
    }

}