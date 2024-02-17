#include "ImgBtn.hpp"

namespace be::lv {
    DEFINE_NCLASS_META(ImgBtn, Obj)
    std::vector<JSCFunctionListEntry> ImgBtn::methods = {
// AUTO GENERATE CODE START [GETSET LIST] --------
        JS_CGETSET_DEF("state",be::lv::Obj::invalidGetter,ImgBtn::setState) ,

// AUTO GENERATE CODE END [GETSET LIST] --------
// AUTO GENERATE CODE START [METHOD LIST] --------


// AUTO GENERATE CODE END [METHOD LIST] --------
    } ;

    ImgBtn::ImgBtn(JSContext * ctx, JSValue jsobj, lv_obj_t * lvobj)
        : Obj(ctx, ImgBtn::build(ctx,jsobj), lvobj)
    {}

    ImgBtn::ImgBtn(JSContext * ctx, lv_obj_t * parent)
        : ImgBtn(ctx, JS_NULL, lv_imagebutton_create(parent))
    {}
        
    JSValue ImgBtn::constructor(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        lv_obj_t * lvparent = nullptr ;
        if(argc>0) {
            JSVALUE_TO_LVOBJ_VAR(argv[0], lvparent)
        }
        ImgBtn * widget = new ImgBtn(ctx,lvparent) ;
        return widget->jsobj ;
    }

// AUTO GENERATE CODE START [GETSETS] --------
    JSValue ImgBtn::setState(JSContext *ctx, JSValueConst this_val, JSValueConst val){
        THIS_NCLASS(ImgBtn,thisobj)
        // argv state
        const char * cstr_val = JS_ToCString(ctx, val) ;
        lv_imagebutton_state_t state;
        if(lv_imagebutton_state_str_to_const(cstr_val,&state)) {
            JS_ThrowReferenceError(ctx,"unknow %s value: %s","lv_imagebutton_state_t",cstr_val) ;
            JS_FreeCString(ctx, cstr_val) ;
            return JS_EXCEPTION ;
        }
        JS_FreeCString(ctx, cstr_val) ;
        lv_imagebutton_set_state(thisobj->lvobj(), state) ;
        return JS_UNDEFINED ;
    }

// AUTO GENERATE CODE END [GETSETS] --------

// AUTO GENERATE CODE START [METHODS] --------


// AUTO GENERATE CODE END [METHODS] --------

}
