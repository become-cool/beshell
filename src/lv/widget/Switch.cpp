#include "Switch.hpp"

namespace be::lv {
    DEFINE_NCLASS_META(Switch, Obj)
    std::vector<JSCFunctionListEntry> Switch::methods = {
// AUTO GENERATE CODE START [GETSET LIST] --------

// AUTO GENERATE CODE END [GETSET LIST] --------
// AUTO GENERATE CODE START [METHOD LIST] --------

// AUTO GENERATE CODE END [METHOD LIST] --------
    } ;

    Switch::Switch(JSContext * ctx, JSValue jsobj, lv_obj_t * lvobj)
        : Obj(ctx, Switch::build(ctx,jsobj), lvobj)
    {}

    Switch::Switch(JSContext * ctx, lv_obj_t * parent)
        : Switch(ctx, JS_NULL, lv_switch_create(parent))
    {}
        
    JSValue Switch::constructor(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        lv_obj_t * lvparent = nullptr ;
        if(argc>0) {
            JSVALUE_TO_LVOBJ_VAR(argv[0], lvparent)
        }
        Switch * widget = new Switch(ctx,lvparent) ;
        return widget->jsobj ;
    }

// AUTO GENERATE CODE START [GETSETS] --------

// AUTO GENERATE CODE END [GETSETS] --------

// AUTO GENERATE CODE START [METHODS] --------

// AUTO GENERATE CODE END [METHODS] --------

}
