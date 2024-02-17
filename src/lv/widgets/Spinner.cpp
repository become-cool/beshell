#include "Spinner.hpp"

namespace be::lv {
    DEFINE_NCLASS_META(Spinner, Obj)
    std::vector<JSCFunctionListEntry> Spinner::methods = {
// AUTO GENERATE CODE START [GETSET LIST] --------


// AUTO GENERATE CODE END [GETSET LIST] --------
// AUTO GENERATE CODE START [METHOD LIST] --------


// AUTO GENERATE CODE END [METHOD LIST] --------
    } ;

    Spinner::Spinner(JSContext * ctx, JSValue jsobj, lv_obj_t * lvobj)
        : Obj(ctx, Spinner::build(ctx,jsobj), lvobj)
    {}

    Spinner::Spinner(JSContext * ctx, lv_obj_t * parent)
        : Spinner(ctx, JS_NULL, lv_spinner_create(parent))
    {}
        
    JSValue Spinner::constructor(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        lv_obj_t * lvparent = nullptr ;
        if(argc>0) {
            JSVALUE_TO_LVOBJ_VAR(argv[0], lvparent)
        }
        Spinner * widget = new Spinner(ctx,lvparent) ;
        return widget->jsobj ;
    }

// AUTO GENERATE CODE START [GETSETS] --------


// AUTO GENERATE CODE END [GETSETS] --------

// AUTO GENERATE CODE START [METHODS] --------


// AUTO GENERATE CODE END [METHODS] --------

}
