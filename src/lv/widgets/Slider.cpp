#include "Slider.hpp"

namespace be::lv {
    DEFINE_NCLASS_META(Slider, Obj)
    std::vector<JSCFunctionListEntry> Slider::methods = {
// AUTO GENERATE CODE START [GETSET LIST] --------

// AUTO GENERATE CODE END [GETSET LIST] --------
// AUTO GENERATE CODE START [METHOD LIST] --------
        // Unsupported arg type:
        // lv_observer_t * lv_slider_bind_value(lv_obj_t * obj, lv_subject_t * subject)
        // bool lv_slider_is_dragged(const lv_obj_t * obj)
// AUTO GENERATE CODE END [METHOD LIST] --------
    } ;

    Slider::Slider(JSContext * ctx, JSValue jsobj, lv_obj_t * lvobj)
        : Obj(ctx, Slider::build(ctx,jsobj), lvobj)
    {}

    Slider::Slider(JSContext * ctx, lv_obj_t * parent)
        : Slider(ctx, JS_NULL, lv_slider_create(parent))
    {}
        
    JSValue Slider::constructor(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        lv_obj_t * lvparent = nullptr ;
        if(argc>0) {
            JSVALUE_TO_LVOBJ_VAR(argv[0], lvparent)
        }
        Slider * widget = new Slider(ctx,lvparent) ;
        return widget->jsobj ;
    }

// AUTO GENERATE CODE START [GETSETS] --------

// AUTO GENERATE CODE END [GETSETS] --------

// AUTO GENERATE CODE START [METHODS] --------
        // Unsupported arg type: lv_subject_t *
        // lv_observer_t * lv_slider_bind_value(lv_obj_t * obj, lv_subject_t * subject)

        // Unsupported arg type: const lv_obj_t *
        // bool lv_slider_is_dragged(const lv_obj_t * obj)
// AUTO GENERATE CODE END [METHODS] --------

}
