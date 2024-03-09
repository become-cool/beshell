#include "TextArea.hpp"

namespace be::lv {
    DEFINE_NCLASS_META(TextArea, Obj)
    std::vector<JSCFunctionListEntry> TextArea::methods = {
// AUTO GENERATE CODE START [GETSET LIST] --------
        JS_CGETSET_DEF("text",be::lv::Obj::invalidGetter,TextArea::setText) ,
        JS_CGETSET_DEF("placeholderText",be::lv::Obj::invalidGetter,TextArea::setPlaceholderText) ,
        JS_CGETSET_DEF("cursorPos",TextArea::getCursorPos,TextArea::setCursorPos) ,
        JS_CGETSET_DEF("cursorClickPos",TextArea::getCursorClickPos,TextArea::setCursorClickPos) ,
        JS_CGETSET_DEF("passwordMode",TextArea::getPasswordMode,TextArea::setPasswordMode) ,
        JS_CGETSET_DEF("passwordBullet",be::lv::Obj::invalidGetter,TextArea::setPasswordBullet) ,
        JS_CGETSET_DEF("oneLine",TextArea::getOneLine,TextArea::setOneLine) ,
        JS_CGETSET_DEF("acceptedChars",be::lv::Obj::invalidGetter,TextArea::setAcceptedChars) ,
        JS_CGETSET_DEF("maxLength",TextArea::getMaxLength,TextArea::setMaxLength) ,
        JS_CGETSET_DEF("insertReplace",be::lv::Obj::invalidGetter,TextArea::setInsertReplace) ,
        JS_CGETSET_DEF("textSelection",TextArea::getTextSelection,TextArea::setTextSelection) ,
        JS_CGETSET_DEF("passwordShowTime",TextArea::getPasswordShowTime,TextArea::setPasswordShowTime) ,
        JS_CGETSET_DEF("align",be::lv::Obj::invalidGetter,TextArea::setAlign) ,
        JS_CGETSET_DEF("label",TextArea::getLabel,be::lv::Obj::invalidSetter) ,
        JS_CGETSET_DEF("currentChar",TextArea::getCurrentChar,be::lv::Obj::invalidSetter) ,
// AUTO GENERATE CODE END [GETSET LIST] --------
// AUTO GENERATE CODE START [METHOD LIST] --------
        JS_CFUNC_DEF("addChar", 1, TextArea::addChar),
        JS_CFUNC_DEF("addText", 1, TextArea::addText),
        JS_CFUNC_DEF("deleteChar", 0, TextArea::deleteChar),
        JS_CFUNC_DEF("deleteCharForward", 0, TextArea::deleteCharForward),
        JS_CFUNC_DEF("textIsSelected", 0, TextArea::textIsSelected),
        JS_CFUNC_DEF("clearSelection", 0, TextArea::clearSelection),
        JS_CFUNC_DEF("cursorRight", 0, TextArea::cursorRight),
        JS_CFUNC_DEF("cursorLeft", 0, TextArea::cursorLeft),
        JS_CFUNC_DEF("cursorDown", 0, TextArea::cursorDown),
        JS_CFUNC_DEF("cursorUp", 0, TextArea::cursorUp),
// AUTO GENERATE CODE END [METHOD LIST] --------
    } ;

    TextArea::TextArea(JSContext * ctx, JSValue jsobj, lv_obj_t * lvobj)
        : Obj(ctx, TextArea::build(ctx,jsobj), lvobj)
    {}

    TextArea::TextArea(JSContext * ctx, lv_obj_t * parent)
        : TextArea(ctx, JS_NULL, lv_textarea_create(parent))
    {}

    JSValue TextArea::constructor(JSContext *ctx, JSValueConst ctor, int argc, JSValueConst *argv) {
        lv_obj_t * lvparent = nullptr ;
        if(argc>0) {
            JSVALUE_TO_LVOBJ_VAR(argv[0], lvparent)
        }
        JSValue obj = newObject(ctx, ctor) ;
        if( JS_IsException(obj) ) {
            return obj ;
        }
        TextArea * widget = new TextArea(ctx, obj, lv_textarea_create(lvparent)) ;
        return widget->jsobj ;
    }

// AUTO GENERATE CODE START [GETSETS] --------
    // unspported type: const char *
    // JSValue TextArea::getText(JSContext *ctx, JSValueConst this_val){}
    // const char * lv_textarea_get_text(const lv_obj_t * obj)
    #ifndef SETTER_TextArea_Text
    JSValue TextArea::setText(JSContext *ctx, JSValueConst this_val, JSValueConst val){
        THIS_NCLASS(TextArea,thisobj)
        char * text = (char *)JS_ToCString(ctx, val) ;
        lv_textarea_set_text(thisobj->lvobj(), text) ;
        JS_FreeCString(ctx, text) ;
        return JS_UNDEFINED ;
    }
    #endif
    // unspported type: const char *
    // JSValue TextArea::getPlaceholderText(JSContext *ctx, JSValueConst this_val){}
    // const char * lv_textarea_get_placeholder_text(lv_obj_t * obj)
    #ifndef SETTER_TextArea_PlaceholderText
    JSValue TextArea::setPlaceholderText(JSContext *ctx, JSValueConst this_val, JSValueConst val){
        THIS_NCLASS(TextArea,thisobj)
        char * placeholderText = (char *)JS_ToCString(ctx, val) ;
        lv_textarea_set_placeholder_text(thisobj->lvobj(), placeholderText) ;
        JS_FreeCString(ctx, placeholderText) ;
        return JS_UNDEFINED ;
    }
    #endif
    #ifndef GETTER_TextArea_CursorPos
    JSValue TextArea::getCursorPos(JSContext *ctx, JSValueConst this_val){
        THIS_NCLASS(TextArea,thisobj)
        uint32_t value = lv_textarea_get_cursor_pos(thisobj->lvobj()) ;
        JSValue retval = JS_NewUint32(ctx, value) ;
        return retval ;
    }
    #endif
    #ifndef SETTER_TextArea_CursorPos
    JSValue TextArea::setCursorPos(JSContext *ctx, JSValueConst this_val, JSValueConst val){
        THIS_NCLASS(TextArea,thisobj)
        int32_t cursorPos ;
        if(JS_ToInt32(ctx, (int32_t *) &cursorPos, val)!=0){
            JSTHROW("arg %s of method %s.%s() must be a %s","cursorPos","TextArea","setCursorPos","number")
        }
        lv_textarea_set_cursor_pos(thisobj->lvobj(), cursorPos) ;
        return JS_UNDEFINED ;
    }
    #endif
    #ifndef GETTER_TextArea_CursorClickPos
    JSValue TextArea::getCursorClickPos(JSContext *ctx, JSValueConst this_val){
        THIS_NCLASS(TextArea,thisobj)
        bool value = lv_textarea_get_cursor_click_pos(thisobj->lvobj()) ;
        JSValue retval = JS_NewBool(ctx, value) ;
        return retval ;
    }
    #endif
    #ifndef SETTER_TextArea_CursorClickPos
    JSValue TextArea::setCursorClickPos(JSContext *ctx, JSValueConst this_val, JSValueConst val){
        THIS_NCLASS(TextArea,thisobj)
        bool cursorClickPos = JS_ToBool(ctx, val) ;
        lv_textarea_set_cursor_click_pos(thisobj->lvobj(), cursorClickPos) ;
        return JS_UNDEFINED ;
    }
    #endif
    #ifndef GETTER_TextArea_PasswordMode
    JSValue TextArea::getPasswordMode(JSContext *ctx, JSValueConst this_val){
        THIS_NCLASS(TextArea,thisobj)
        bool value = lv_textarea_get_password_mode(thisobj->lvobj()) ;
        JSValue retval = JS_NewBool(ctx, value) ;
        return retval ;
    }
    #endif
    #ifndef SETTER_TextArea_PasswordMode
    JSValue TextArea::setPasswordMode(JSContext *ctx, JSValueConst this_val, JSValueConst val){
        THIS_NCLASS(TextArea,thisobj)
        bool passwordMode = JS_ToBool(ctx, val) ;
        lv_textarea_set_password_mode(thisobj->lvobj(), passwordMode) ;
        return JS_UNDEFINED ;
    }
    #endif
    // unspported type: const char *
    // JSValue TextArea::getPasswordBullet(JSContext *ctx, JSValueConst this_val){}
    // const char * lv_textarea_get_password_bullet(lv_obj_t * obj)
    #ifndef SETTER_TextArea_PasswordBullet
    JSValue TextArea::setPasswordBullet(JSContext *ctx, JSValueConst this_val, JSValueConst val){
        THIS_NCLASS(TextArea,thisobj)
        char * passwordBullet = (char *)JS_ToCString(ctx, val) ;
        lv_textarea_set_password_bullet(thisobj->lvobj(), passwordBullet) ;
        JS_FreeCString(ctx, passwordBullet) ;
        return JS_UNDEFINED ;
    }
    #endif
    #ifndef GETTER_TextArea_OneLine
    JSValue TextArea::getOneLine(JSContext *ctx, JSValueConst this_val){
        THIS_NCLASS(TextArea,thisobj)
        bool value = lv_textarea_get_one_line(thisobj->lvobj()) ;
        JSValue retval = JS_NewBool(ctx, value) ;
        return retval ;
    }
    #endif
    #ifndef SETTER_TextArea_OneLine
    JSValue TextArea::setOneLine(JSContext *ctx, JSValueConst this_val, JSValueConst val){
        THIS_NCLASS(TextArea,thisobj)
        bool oneLine = JS_ToBool(ctx, val) ;
        lv_textarea_set_one_line(thisobj->lvobj(), oneLine) ;
        return JS_UNDEFINED ;
    }
    #endif
    // unspported type: const char *
    // JSValue TextArea::getAcceptedChars(JSContext *ctx, JSValueConst this_val){}
    // const char * lv_textarea_get_accepted_chars(lv_obj_t * obj)
    #ifndef SETTER_TextArea_AcceptedChars
    JSValue TextArea::setAcceptedChars(JSContext *ctx, JSValueConst this_val, JSValueConst val){
        THIS_NCLASS(TextArea,thisobj)
        char * acceptedChars = (char *)JS_ToCString(ctx, val) ;
        lv_textarea_set_accepted_chars(thisobj->lvobj(), acceptedChars) ;
        JS_FreeCString(ctx, acceptedChars) ;
        return JS_UNDEFINED ;
    }
    #endif
    #ifndef GETTER_TextArea_MaxLength
    JSValue TextArea::getMaxLength(JSContext *ctx, JSValueConst this_val){
        THIS_NCLASS(TextArea,thisobj)
        uint32_t value = lv_textarea_get_max_length(thisobj->lvobj()) ;
        JSValue retval = JS_NewUint32(ctx, value) ;
        return retval ;
    }
    #endif
    #ifndef SETTER_TextArea_MaxLength
    JSValue TextArea::setMaxLength(JSContext *ctx, JSValueConst this_val, JSValueConst val){
        THIS_NCLASS(TextArea,thisobj)
        uint32_t maxLength ;
        if(JS_ToUint32(ctx, (uint32_t *) &maxLength, val)!=0){
            JSTHROW("arg %s of method %s.%s() must be a %s","maxLength","TextArea","setMaxLength","number")
        }
        lv_textarea_set_max_length(thisobj->lvobj(), maxLength) ;
        return JS_UNDEFINED ;
    }
    #endif
    #ifndef SETTER_TextArea_InsertReplace
    JSValue TextArea::setInsertReplace(JSContext *ctx, JSValueConst this_val, JSValueConst val){
        THIS_NCLASS(TextArea,thisobj)
        char * insertReplace = (char *)JS_ToCString(ctx, val) ;
        lv_textarea_set_insert_replace(thisobj->lvobj(), insertReplace) ;
        JS_FreeCString(ctx, insertReplace) ;
        return JS_UNDEFINED ;
    }
    #endif
    #ifndef GETTER_TextArea_TextSelection
    JSValue TextArea::getTextSelection(JSContext *ctx, JSValueConst this_val){
        THIS_NCLASS(TextArea,thisobj)
        bool value = lv_textarea_get_text_selection(thisobj->lvobj()) ;
        JSValue retval = JS_NewBool(ctx, value) ;
        return retval ;
    }
    #endif
    #ifndef SETTER_TextArea_TextSelection
    JSValue TextArea::setTextSelection(JSContext *ctx, JSValueConst this_val, JSValueConst val){
        THIS_NCLASS(TextArea,thisobj)
        bool textSelection = JS_ToBool(ctx, val) ;
        lv_textarea_set_text_selection(thisobj->lvobj(), textSelection) ;
        return JS_UNDEFINED ;
    }
    #endif
    #ifndef GETTER_TextArea_PasswordShowTime
    JSValue TextArea::getPasswordShowTime(JSContext *ctx, JSValueConst this_val){
        THIS_NCLASS(TextArea,thisobj)
        uint32_t value = lv_textarea_get_password_show_time(thisobj->lvobj()) ;
        JSValue retval = JS_NewUint32(ctx, value) ;
        return retval ;
    }
    #endif
    #ifndef SETTER_TextArea_PasswordShowTime
    JSValue TextArea::setPasswordShowTime(JSContext *ctx, JSValueConst this_val, JSValueConst val){
        THIS_NCLASS(TextArea,thisobj)
        uint32_t passwordShowTime ;
        if(JS_ToUint32(ctx, (uint32_t *) &passwordShowTime, val)!=0){
            JSTHROW("arg %s of method %s.%s() must be a %s","passwordShowTime","TextArea","setPasswordShowTime","number")
        }
        lv_textarea_set_password_show_time(thisobj->lvobj(), passwordShowTime) ;
        return JS_UNDEFINED ;
    }
    #endif
    #ifndef SETTER_TextArea_Align
    JSValue TextArea::setAlign(JSContext *ctx, JSValueConst this_val, JSValueConst val){
        THIS_NCLASS(TextArea,thisobj)
        // argv align
        const char * cstr_val = JS_ToCString(ctx, val) ;
        lv_text_align_t align;
        if(!lv_text_align_str_to_const(cstr_val,&align)) {
            JS_ThrowReferenceError(ctx,"unknow %s value: %s","lv_text_align_t",cstr_val) ;
            JS_FreeCString(ctx, cstr_val) ;
            return JS_EXCEPTION ;
        }
        JS_FreeCString(ctx, cstr_val) ;
        lv_textarea_set_align(thisobj->lvobj(), align) ;
        return JS_UNDEFINED ;
    }
    #endif
    #ifndef GETTER_TextArea_Label
    JSValue TextArea::getLabel(JSContext *ctx, JSValueConst this_val){
        THIS_NCLASS(TextArea,thisobj)
        lv_obj_t * value = lv_textarea_get_label(thisobj->lvobj()) ;
        JSValue retval = value? be::lv::Obj::wrap(ctx, (lv_obj_t*)value)->jsobj: JS_NULL ;
        return retval ;
    }
    #endif
    #ifndef GETTER_TextArea_CurrentChar
    JSValue TextArea::getCurrentChar(JSContext *ctx, JSValueConst this_val){
        THIS_NCLASS(TextArea,thisobj)
        uint32_t value = lv_textarea_get_current_char(thisobj->lvobj()) ;
        JSValue retval = JS_NewUint32(ctx, value) ;
        return retval ;
    }
    #endif
// AUTO GENERATE CODE END [GETSETS] --------

// AUTO GENERATE CODE START [METHODS] --------
    #ifndef METHOD_TextArea_addChar
    JSValue TextArea::addChar(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        THIS_NCLASS(Obj,thisobj)
        CHECK_ARGC(1)
        uint32_t c ;
        if(JS_ToUint32(ctx, (uint32_t *) &c, argv[0])!=0){
            JSTHROW("arg %s of method %s.%s() must be a %s","c","TextArea","addChar","number")
        }
        lv_textarea_add_char( thisobj->lvobj(), c ) ;
        return JS_UNDEFINED ;
    }
    #endif

    #ifndef METHOD_TextArea_addText
    JSValue TextArea::addText(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        THIS_NCLASS(Obj,thisobj)
        CHECK_ARGC(1)
        char * txt = (char *)JS_ToCString(ctx, argv[0]) ;
        lv_textarea_add_text( thisobj->lvobj(), txt ) ;
        return JS_UNDEFINED ;
    }
    #endif

    #ifndef METHOD_TextArea_deleteChar
    JSValue TextArea::deleteChar(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        THIS_NCLASS(Obj,thisobj)
        lv_textarea_delete_char( thisobj->lvobj() ) ;
        return JS_UNDEFINED ;
    }
    #endif

    #ifndef METHOD_TextArea_deleteCharForward
    JSValue TextArea::deleteCharForward(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        THIS_NCLASS(Obj,thisobj)
        lv_textarea_delete_char_forward( thisobj->lvobj() ) ;
        return JS_UNDEFINED ;
    }
    #endif

    #ifndef METHOD_TextArea_textIsSelected
    JSValue TextArea::textIsSelected(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        THIS_NCLASS(Obj,thisobj)
        bool retval = lv_textarea_text_is_selected( thisobj->lvobj() ) ;
        JSValue jsretval = JS_NewBool(ctx, retval) ;
        return jsretval ;
    }
    #endif

    #ifndef METHOD_TextArea_clearSelection
    JSValue TextArea::clearSelection(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        THIS_NCLASS(Obj,thisobj)
        lv_textarea_clear_selection( thisobj->lvobj() ) ;
        return JS_UNDEFINED ;
    }
    #endif

    #ifndef METHOD_TextArea_cursorRight
    JSValue TextArea::cursorRight(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        THIS_NCLASS(Obj,thisobj)
        lv_textarea_cursor_right( thisobj->lvobj() ) ;
        return JS_UNDEFINED ;
    }
    #endif

    #ifndef METHOD_TextArea_cursorLeft
    JSValue TextArea::cursorLeft(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        THIS_NCLASS(Obj,thisobj)
        lv_textarea_cursor_left( thisobj->lvobj() ) ;
        return JS_UNDEFINED ;
    }
    #endif

    #ifndef METHOD_TextArea_cursorDown
    JSValue TextArea::cursorDown(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        THIS_NCLASS(Obj,thisobj)
        lv_textarea_cursor_down( thisobj->lvobj() ) ;
        return JS_UNDEFINED ;
    }
    #endif

    #ifndef METHOD_TextArea_cursorUp
    JSValue TextArea::cursorUp(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        THIS_NCLASS(Obj,thisobj)
        lv_textarea_cursor_up( thisobj->lvobj() ) ;
        return JS_UNDEFINED ;
    }
    #endif
// AUTO GENERATE CODE END [METHODS] --------

}
