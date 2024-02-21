#include "Keyboard.hpp"

namespace be::lv {
    DEFINE_NCLASS_META(Keyboard, Obj)
    std::vector<JSCFunctionListEntry> Keyboard::methods = {
// AUTO GENERATE CODE START [GETSET LIST] --------
        JS_CGETSET_DEF("textarea",Keyboard::getTextarea,Keyboard::setTextarea) ,
        JS_CGETSET_DEF("mode",Keyboard::getMode,Keyboard::setMode) ,
        JS_CGETSET_DEF("popovers",be::lv::Obj::invalidGetter,Keyboard::setPopovers) ,
// AUTO GENERATE CODE END [GETSET LIST] --------
// AUTO GENERATE CODE START [METHOD LIST] --------
        // Unsupported arg type:
        // void lv_keyboard_set_map(lv_obj_t * kb, lv_keyboard_mode_t mode, const char * map[], const lv_buttonmatrix_ctrl_t ctrl_map[])
        // void lv_keyboard_def_event_cb(lv_event_t * e)
// AUTO GENERATE CODE END [METHOD LIST] --------
    } ;

    Keyboard::Keyboard(JSContext * ctx, JSValue jsobj, lv_obj_t * lvobj)
        : Obj(ctx, Keyboard::build(ctx,jsobj), lvobj)
    {}

    Keyboard::Keyboard(JSContext * ctx, lv_obj_t * parent)
        : Keyboard(ctx, JS_NULL, lv_keyboard_create(parent))
    {}
        
    JSValue Keyboard::constructor(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        lv_obj_t * lvparent = nullptr ;
        if(argc>0) {
            JSVALUE_TO_LVOBJ_VAR(argv[0], lvparent)
        }
        Keyboard * widget = new Keyboard(ctx,lvparent) ;
        return widget->jsobj ;
    }

// AUTO GENERATE CODE START [GETSETS] --------
    #ifndef GETTER_Keyboard_Textarea
    JSValue Keyboard::getTextarea(JSContext *ctx, JSValueConst this_val){
        THIS_NCLASS(Keyboard,thisobj)
        lv_obj_t * value = lv_keyboard_get_textarea(thisobj->lvobj()) ;
        JSValue retval = value? be::lv::Obj::wrap(ctx, (lv_obj_t*)value)->jsobj: JS_NULL ;
        return retval ;
    }
    #endif
    #ifndef SETTER_Keyboard_Textarea
    JSValue Keyboard::setTextarea(JSContext *ctx, JSValueConst this_val, JSValueConst val){
        THIS_NCLASS(Keyboard,thisobj)
        JSVALUE_TO_LVOBJ(val,textarea)
        lv_keyboard_set_textarea(thisobj->lvobj(), textarea) ;
        return JS_UNDEFINED ;
    }
    #endif
    #ifndef GETTER_Keyboard_Mode
    JSValue Keyboard::getMode(JSContext *ctx, JSValueConst this_val){
        THIS_NCLASS(Keyboard,thisobj)
        lv_keyboard_mode_t value = lv_keyboard_get_mode(thisobj->lvobj()) ;
        JSValue retval = JS_NewUint32(ctx, value) ;
        return retval ;
    }
    #endif
    #ifndef SETTER_Keyboard_Mode
    JSValue Keyboard::setMode(JSContext *ctx, JSValueConst this_val, JSValueConst val){
        THIS_NCLASS(Keyboard,thisobj)
        // argv mode
        const char * cstr_val = JS_ToCString(ctx, val) ;
        lv_keyboard_mode_t mode;
        if(!lv_keyboard_mode_str_to_const(cstr_val,&mode)) {
            JS_ThrowReferenceError(ctx,"unknow %s value: %s","lv_keyboard_mode_t",cstr_val) ;
            JS_FreeCString(ctx, cstr_val) ;
            return JS_EXCEPTION ;
        }
        JS_FreeCString(ctx, cstr_val) ;
        lv_keyboard_set_mode(thisobj->lvobj(), mode) ;
        return JS_UNDEFINED ;
    }
    #endif
    #ifndef SETTER_Keyboard_Popovers
    JSValue Keyboard::setPopovers(JSContext *ctx, JSValueConst this_val, JSValueConst val){
        THIS_NCLASS(Keyboard,thisobj)
        bool popovers = JS_ToBool(ctx, val) ;
        lv_keyboard_set_popovers(thisobj->lvobj(), popovers) ;
        return JS_UNDEFINED ;
    }
    #endif
// AUTO GENERATE CODE END [GETSETS] --------

// AUTO GENERATE CODE START [METHODS] --------
    // Unsupported arg type: const char **
    // void lv_keyboard_set_map(lv_obj_t * kb, lv_keyboard_mode_t mode, const char * map[], const lv_buttonmatrix_ctrl_t ctrl_map[])

    // Unsupported arg type: lv_event_t *
    // void lv_keyboard_def_event_cb(lv_event_t * e)
// AUTO GENERATE CODE END [METHODS] --------

}
