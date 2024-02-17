#include "Btn.hpp"

namespace be::lv {
    DEFINE_NCLASS_META(Btn, Obj)
    std::vector<JSCFunctionListEntry> Btn::methods = {
// AUTO GENERATE CODE START [GETSET LIST] --------

// AUTO GENERATE CODE END [GETSET LIST] --------
// AUTO GENERATE CODE START [METHOD LIST] --------
        // Unsupported arg type:
        // lv_observer_t * lv_button_bind_checked(lv_obj_t * obj, lv_subject_t * subject)
// AUTO GENERATE CODE END [METHOD LIST] --------
    } ;

    Btn::Btn(JSContext * ctx, JSValue jsobj, lv_obj_t * lvobj)
        : Obj(ctx, Btn::build(ctx,jsobj), lvobj)
    {}

    Btn::Btn(JSContext * ctx, lv_obj_t * parent)
        : Btn(ctx, JS_NULL, lv_button_create(parent))
    {}
        
    JSValue Btn::constructor(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        lv_obj_t * lvparent = nullptr ;
        if(argc>0) {
            JSVALUE_TO_LVOBJ_VAR(argv[0], lvparent)
        }
        Btn * widget = new Btn(ctx,lvparent) ;
        return widget->jsobj ;
    }

// AUTO GENERATE CODE START [GETSETS] --------

// AUTO GENERATE CODE END [GETSETS] --------

// AUTO GENERATE CODE START [METHODS] --------
        // Unsupported arg type: lv_subject_t *
        // lv_observer_t * lv_button_bind_checked(lv_obj_t * obj, lv_subject_t * subject)
// AUTO GENERATE CODE END [METHODS] --------

}
