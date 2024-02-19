#include "Column.hpp"

using namespace std ;

namespace be::lv {

    DEFINE_NCLASS_META(Column, Obj)

    std::vector<JSCFunctionListEntry> Column::methods = {
        JS_CFUNC_DEF("method", 0, Column::jsMethod),
    } ;

    Column::Column(JSContext * ctx, JSValue jsobj, lv_obj_t * lvobj)
        : Obj(ctx, Column::build(ctx,jsobj), lvobj)
    {
        lv_obj_set_flex_flow(lvobj, LV_FLEX_FLOW_COLUMN) ;
    }

    Column::Column(JSContext * ctx, lv_obj_t * parent)
        : Column(ctx, JS_NULL, lv_obj_create(parent))
    {}

    JSValue Column::constructor(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        lv_obj_t * lvparent = nullptr ;
        if(argc>0) {
            JSVALUE_TO_LVOBJ_VAR(argv[0], lvparent)
        }
        Column * widget = new Column(ctx,lvparent) ;
        return widget->jsobj ;
    }

    JSValue Column::jsMethod(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        return JS_UNDEFINED ;
    }

}