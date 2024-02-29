#include "ImgBtn.hpp"

namespace be::lv {
    DEFINE_NCLASS_META(ImgBtn, Obj)
    std::vector<JSCFunctionListEntry> ImgBtn::methods = {
// AUTO GENERATE CODE START [GETSET LIST] --------
        JS_CGETSET_DEF("state",be::lv::Obj::invalidGetter,ImgBtn::setState) ,

// AUTO GENERATE CODE END [GETSET LIST] --------
// AUTO GENERATE CODE START [METHOD LIST] --------
        // Unsupported arg type:
        // void lv_imagebutton_set_src(lv_obj_t * imagebutton, lv_imagebutton_state_t state, const void * src_left, const void * src_mid, const void * src_right)
        // const void * lv_imagebutton_get_src_left(lv_obj_t * imagebutton, lv_imagebutton_state_t state)
        // const void * lv_imagebutton_get_src_middle(lv_obj_t * imagebutton, lv_imagebutton_state_t state)
        // const void * lv_imagebutton_get_src_right(lv_obj_t * imagebutton, lv_imagebutton_state_t state)

// AUTO GENERATE CODE END [METHOD LIST] --------
    } ;

    ImgBtn::ImgBtn(JSContext * ctx, JSValue jsobj, lv_obj_t * lvobj)
        : Obj(ctx, ImgBtn::build(ctx,jsobj), lvobj)
    {}

    ImgBtn::ImgBtn(JSContext * ctx, lv_obj_t * parent)
        : ImgBtn(ctx, JS_NULL, lv_imagebutton_create(parent))
    {}

    JSValue ImgBtn::constructor(JSContext *ctx, JSValueConst ctor, int argc, JSValueConst *argv) {
        lv_obj_t * lvparent = nullptr ;
        if(argc>0) {
            JSVALUE_TO_LVOBJ_VAR(argv[0], lvparent)
        }
        JSValue obj = newObject(ctx, ctor) ;
        if( JS_IsException(obj) ) {
            return obj ;
        }
        ImgBtn * widget = new ImgBtn(ctx, obj, lv_imagebutton_create(lvparent)) ;
        return widget->jsobj ;
    }

// AUTO GENERATE CODE START [GETSETS] --------
    #ifndef SETTER_ImgBtn_State
    JSValue ImgBtn::setState(JSContext *ctx, JSValueConst this_val, JSValueConst val){
        THIS_NCLASS(ImgBtn,thisobj)
        // argv state
        const char * cstr_val = JS_ToCString(ctx, val) ;
        lv_imagebutton_state_t state;
        if(!lv_imagebutton_state_str_to_const(cstr_val,&state)) {
            JS_ThrowReferenceError(ctx,"unknow %s value: %s","lv_imagebutton_state_t",cstr_val) ;
            JS_FreeCString(ctx, cstr_val) ;
            return JS_EXCEPTION ;
        }
        JS_FreeCString(ctx, cstr_val) ;
        lv_imagebutton_set_state(thisobj->lvobj(), state) ;
        return JS_UNDEFINED ;
    }
    #endif

// AUTO GENERATE CODE END [GETSETS] --------

// AUTO GENERATE CODE START [METHODS] --------
    // Unsupported arg type: const void *
    // void lv_imagebutton_set_src(lv_obj_t * imagebutton, lv_imagebutton_state_t state, const void * src_left, const void * src_mid, const void * src_right)

        // Unsupported return type: const void *
        // const void * lv_imagebutton_get_src_left(lv_obj_t * imagebutton, lv_imagebutton_state_t state)

        // Unsupported return type: const void *
        // const void * lv_imagebutton_get_src_middle(lv_obj_t * imagebutton, lv_imagebutton_state_t state)

        // Unsupported return type: const void *
        // const void * lv_imagebutton_get_src_right(lv_obj_t * imagebutton, lv_imagebutton_state_t state)

// AUTO GENERATE CODE END [METHODS] --------

}
