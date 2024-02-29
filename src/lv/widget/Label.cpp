#include "Label.hpp"

namespace be::lv {
    DEFINE_NCLASS_META(Label, Obj)
    std::vector<JSCFunctionListEntry> Label::methods = {
        JS_CGETSET_DEF("font",Label::getFont,Label::setFont) ,
// AUTO GENERATE CODE START [GETSET LIST] --------
        JS_CGETSET_DEF("text",Label::getText,Label::setText) ,
        JS_CGETSET_DEF("textStatic",be::lv::Obj::invalidGetter,Label::setTextStatic) ,
        JS_CGETSET_DEF("longMode",Label::getLongMode,Label::setLongMode) ,
        JS_CGETSET_DEF("textSelectionStart",Label::getTextSelectionStart,Label::setTextSelectionStart) ,
        JS_CGETSET_DEF("textSelectionEnd",Label::getTextSelectionEnd,Label::setTextSelectionEnd) ,
// AUTO GENERATE CODE END [GETSET LIST] --------
// AUTO GENERATE CODE START [METHOD LIST] --------
        JS_CFUNC_DEF("insText", 2, Label::insText),
        JS_CFUNC_DEF("cutText", 2, Label::cutText),
        // Unsupported arg type:
        // lv_observer_t * lv_label_bind_text(lv_obj_t * obj, lv_subject_t * subject, const char * fmt)
        // void lv_label_set_text_fmt(lv_obj_t * obj, const char * fmt, ...)
        // void lv_label_get_letter_pos(const lv_obj_t * obj, uint32_t char_id, lv_point_t * pos)
        // uint32_t lv_label_get_letter_on(const lv_obj_t * obj, lv_point_t * pos_in, bool bidi)
        // bool lv_label_is_char_under_pos(const lv_obj_t * obj, lv_point_t * pos)
// AUTO GENERATE CODE END [METHOD LIST] --------
    } ;

    Label::Label(JSContext * ctx, JSValue jsobj, lv_obj_t * lvobj)
        : Obj(ctx, Label::build(ctx,jsobj), lvobj)
    {}

    Label::Label(JSContext * ctx, lv_obj_t * parent)
        : Label(ctx, JS_NULL, lv_label_create(parent))
    {}
        
    JSValue Label::constructor(JSContext *ctx, JSValueConst ctor, int argc, JSValueConst *argv) {
        lv_obj_t * lvparent = nullptr ;
        if(argc>0) {
            JSVALUE_TO_LVOBJ_VAR(argv[0], lvparent)
        }
        JSValue obj = newObject(ctx, ctor) ;
        if( JS_IsException(obj) ) {
            return obj ;
        }
        char * text = nullptr ;
        if(argc>1) {
            text = (char *)JS_ToCString(ctx, argv[1]) ;
        }
        Label * widget = new Label(ctx, obj, lv_label_create(lvparent)) ;
        if(text) {
            lv_label_set_text(widget->lvobj(),text) ;
            JS_FreeCString(ctx,text) ;
        }
        return widget->jsobj ;
    }

// AUTO GENERATE CODE START [GETSETS] --------
    #ifndef GETTER_Label_Text
    JSValue Label::getText(JSContext *ctx, JSValueConst this_val){
        THIS_NCLASS(Label,thisobj)
        char * value = lv_label_get_text(thisobj->lvobj()) ;
        JSValue retval = JS_NewString(ctx, value) ;
        return retval ;
    }
    #endif
    #ifndef SETTER_Label_Text
    JSValue Label::setText(JSContext *ctx, JSValueConst this_val, JSValueConst val){
        THIS_NCLASS(Label,thisobj)
        char * text = (char *)JS_ToCString(ctx, val) ;
        lv_label_set_text(thisobj->lvobj(), text) ;
        JS_FreeCString(ctx, text) ;
        return JS_UNDEFINED ;
    }
    #endif
    #ifndef SETTER_Label_TextStatic
    JSValue Label::setTextStatic(JSContext *ctx, JSValueConst this_val, JSValueConst val){
        THIS_NCLASS(Label,thisobj)
        char * textStatic = (char *)JS_ToCString(ctx, val) ;
        lv_label_set_text_static(thisobj->lvobj(), textStatic) ;
        JS_FreeCString(ctx, textStatic) ;
        return JS_UNDEFINED ;
    }
    #endif
    #ifndef GETTER_Label_LongMode
    JSValue Label::getLongMode(JSContext *ctx, JSValueConst this_val){
        THIS_NCLASS(Label,thisobj)
        lv_label_long_mode_t value = lv_label_get_long_mode(thisobj->lvobj()) ;
        JSValue retval = JS_NewUint32(ctx, value) ;
        return retval ;
    }
    #endif
    #ifndef SETTER_Label_LongMode
    JSValue Label::setLongMode(JSContext *ctx, JSValueConst this_val, JSValueConst val){
        THIS_NCLASS(Label,thisobj)
        // argv longMode
        const char * cstr_val = JS_ToCString(ctx, val) ;
        lv_label_long_mode_t longMode;
        if(!lv_label_long_mode_str_to_const(cstr_val,&longMode)) {
            JS_ThrowReferenceError(ctx,"unknow %s value: %s","lv_label_long_mode_t",cstr_val) ;
            JS_FreeCString(ctx, cstr_val) ;
            return JS_EXCEPTION ;
        }
        JS_FreeCString(ctx, cstr_val) ;
        lv_label_set_long_mode(thisobj->lvobj(), longMode) ;
        return JS_UNDEFINED ;
    }
    #endif
    #ifndef GETTER_Label_TextSelectionStart
    JSValue Label::getTextSelectionStart(JSContext *ctx, JSValueConst this_val){
        THIS_NCLASS(Label,thisobj)
        uint32_t value = lv_label_get_text_selection_start(thisobj->lvobj()) ;
        JSValue retval = JS_NewUint32(ctx, value) ;
        return retval ;
    }
    #endif
    #ifndef SETTER_Label_TextSelectionStart
    JSValue Label::setTextSelectionStart(JSContext *ctx, JSValueConst this_val, JSValueConst val){
        THIS_NCLASS(Label,thisobj)
        uint32_t textSelectionStart ;
        if(JS_ToUint32(ctx, (uint32_t *) &textSelectionStart, val)!=0){
            JSTHROW("arg %s of method %s.%s() must be a %s","textSelectionStart","Label","setTextSelectionStart","number")
        }
        lv_label_set_text_selection_start(thisobj->lvobj(), textSelectionStart) ;
        return JS_UNDEFINED ;
    }
    #endif
    #ifndef GETTER_Label_TextSelectionEnd
    JSValue Label::getTextSelectionEnd(JSContext *ctx, JSValueConst this_val){
        THIS_NCLASS(Label,thisobj)
        uint32_t value = lv_label_get_text_selection_end(thisobj->lvobj()) ;
        JSValue retval = JS_NewUint32(ctx, value) ;
        return retval ;
    }
    #endif
    #ifndef SETTER_Label_TextSelectionEnd
    JSValue Label::setTextSelectionEnd(JSContext *ctx, JSValueConst this_val, JSValueConst val){
        THIS_NCLASS(Label,thisobj)
        uint32_t textSelectionEnd ;
        if(JS_ToUint32(ctx, (uint32_t *) &textSelectionEnd, val)!=0){
            JSTHROW("arg %s of method %s.%s() must be a %s","textSelectionEnd","Label","setTextSelectionEnd","number")
        }
        lv_label_set_text_selection_end(thisobj->lvobj(), textSelectionEnd) ;
        return JS_UNDEFINED ;
    }
    #endif
// AUTO GENERATE CODE END [GETSETS] --------

// AUTO GENERATE CODE START [METHODS] --------
    // Unsupported arg type: lv_subject_t *
    // lv_observer_t * lv_label_bind_text(lv_obj_t * obj, lv_subject_t * subject, const char * fmt)

    // Unsupported arg type: 
    // void lv_label_set_text_fmt(lv_obj_t * obj, const char * fmt, ...)

    // Unsupported arg type: lv_point_t *
    // void lv_label_get_letter_pos(const lv_obj_t * obj, uint32_t char_id, lv_point_t * pos)

    // Unsupported arg type: lv_point_t *
    // uint32_t lv_label_get_letter_on(const lv_obj_t * obj, lv_point_t * pos_in, bool bidi)

    // Unsupported arg type: lv_point_t *
    // bool lv_label_is_char_under_pos(const lv_obj_t * obj, lv_point_t * pos)

    #ifndef METHOD_Label_insText
    JSValue Label::insText(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        THIS_NCLASS(Obj,thisobj)
        CHECK_ARGC(2)
        uint32_t pos ;
        if(JS_ToUint32(ctx, (uint32_t *) &pos, argv[0])!=0){
            JSTHROW("arg %s of method %s.%s() must be a %s","pos","Label","insText","number")
        }
        char * txt = (char *)JS_ToCString(ctx, argv[1]) ;
        lv_label_ins_text( thisobj->lvobj(), pos, txt ) ;
        return JS_UNDEFINED ;
    }
    #endif

    #ifndef METHOD_Label_cutText
    JSValue Label::cutText(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        THIS_NCLASS(Obj,thisobj)
        CHECK_ARGC(2)
        uint32_t pos ;
        if(JS_ToUint32(ctx, (uint32_t *) &pos, argv[0])!=0){
            JSTHROW("arg %s of method %s.%s() must be a %s","pos","Label","cutText","number")
        }
        uint32_t cnt ;
        if(JS_ToUint32(ctx, (uint32_t *) &cnt, argv[1])!=0){
            JSTHROW("arg %s of method %s.%s() must be a %s","cnt","Label","cutText","number")
        }
        lv_label_cut_text( thisobj->lvobj(), pos, cnt ) ;
        return JS_UNDEFINED ;
    }
    #endif
// AUTO GENERATE CODE END [METHODS] --------

}
