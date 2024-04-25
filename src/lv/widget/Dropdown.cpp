#include "Dropdown.hpp"

namespace be::lv {
    DEFINE_NCLASS_META(Dropdown, Obj)
    std::vector<JSCFunctionListEntry> Dropdown::methods = {
// AUTO GENERATE CODE START [GETSET LIST] --------
        JS_CGETSET_DEF("text",be::lv::Obj::invalidGetter,Dropdown::setText) ,
        JS_CGETSET_DEF("options",be::lv::Obj::invalidGetter,Dropdown::setOptions) ,
        JS_CGETSET_DEF("optionsStatic",be::lv::Obj::invalidGetter,Dropdown::setOptionsStatic) ,
        JS_CGETSET_DEF("selected",Dropdown::getSelected,Dropdown::setSelected) ,
        JS_CGETSET_DEF("dir",Dropdown::getDir,Dropdown::setDir) ,
        JS_CGETSET_DEF("selectedHighlight",Dropdown::getSelectedHighlight,Dropdown::setSelectedHighlight) ,
        JS_CGETSET_DEF("list",Dropdown::getList,be::lv::Obj::invalidSetter) ,
        JS_CGETSET_DEF("optionCount",Dropdown::getOptionCount,be::lv::Obj::invalidSetter) ,
// AUTO GENERATE CODE END [GETSET LIST] --------
// AUTO GENERATE CODE START [METHOD LIST] --------
        JS_CFUNC_DEF("addOption", 2, Dropdown::addOption),
        JS_CFUNC_DEF("clearOptions", 0, Dropdown::clearOptions),
        JS_CFUNC_DEF("getSelectedStr", 2, Dropdown::getSelectedStr),
        JS_CFUNC_DEF("getOptionIndex", 1, Dropdown::getOptionIndex),
        JS_CFUNC_DEF("open", 0, Dropdown::open),
        JS_CFUNC_DEF("close", 0, Dropdown::close),
        JS_CFUNC_DEF("isOpen", 0, Dropdown::isOpen),
        // Unsupported arg type:
        // lv_observer_t * lv_dropdown_bind_value(lv_obj_t * obj, lv_subject_t * subject)
// AUTO GENERATE CODE END [METHOD LIST] --------
    } ;

    Dropdown::Dropdown(JSContext * ctx, JSValue jsobj, lv_obj_t * lvobj)
        : Obj(ctx, Dropdown::build(ctx,jsobj), lvobj)
    {}

    Dropdown::Dropdown(JSContext * ctx, lv_obj_t * parent)
        : Dropdown(ctx, JS_NULL, lv_dropdown_create(parent))
    {}

    JSValue Dropdown::constructor(JSContext *ctx, JSValueConst ctor, int argc, JSValueConst *argv) {
        lv_obj_t * lvparent = nullptr ;
        if(argc>0) {
            JSVALUE_TO_LVOBJ_VAR(argv[0], lvparent)
        }
        JSValue obj = newObject(ctx, ctor) ;
        if( JS_IsException(obj) ) {
            return obj ;
        }
        Dropdown * widget = new Dropdown(ctx, obj, lv_dropdown_create(lvparent)) ;
        return widget->jsobj ;
    }

// AUTO GENERATE CODE START [GETSETS] --------
    // unspported type: const char *
    // JSValue Dropdown::getText(JSContext *ctx, JSValueConst this_val){}
    // const char * lv_dropdown_get_text(lv_obj_t * obj)
    #ifndef SETTER_Dropdown_Text
    JSValue Dropdown::setText(JSContext *ctx, JSValueConst this_val, JSValueConst val){
        THIS_NCLASS(Dropdown,thisobj)
        char * text = (char *)JS_ToCString(ctx, val) ;
        lv_dropdown_set_text(thisobj->lvobj(), text) ;
        JS_FreeCString(ctx, text) ;
        return JS_UNDEFINED ;
    }
    #endif
    // unspported type: const char *
    // JSValue Dropdown::getOptions(JSContext *ctx, JSValueConst this_val){}
    // const char * lv_dropdown_get_options(const lv_obj_t * obj)
    #ifndef SETTER_Dropdown_Options
    JSValue Dropdown::setOptions(JSContext *ctx, JSValueConst this_val, JSValueConst val){
        THIS_NCLASS(Dropdown,thisobj)
        char * options = (char *)JS_ToCString(ctx, val) ;
        lv_dropdown_set_options(thisobj->lvobj(), options) ;
        JS_FreeCString(ctx, options) ;
        return JS_UNDEFINED ;
    }
    #endif
    #ifndef SETTER_Dropdown_OptionsStatic
    JSValue Dropdown::setOptionsStatic(JSContext *ctx, JSValueConst this_val, JSValueConst val){
        THIS_NCLASS(Dropdown,thisobj)
        char * optionsStatic = (char *)JS_ToCString(ctx, val) ;
        lv_dropdown_set_options_static(thisobj->lvobj(), optionsStatic) ;
        JS_FreeCString(ctx, optionsStatic) ;
        return JS_UNDEFINED ;
    }
    #endif
    #ifndef GETTER_Dropdown_Selected
    JSValue Dropdown::getSelected(JSContext *ctx, JSValueConst this_val){
        THIS_NCLASS(Dropdown,thisobj)
        uint32_t value = lv_dropdown_get_selected(thisobj->lvobj()) ;
        JSValue retval = JS_NewUint32(ctx, value) ;
        return retval ;
    }
    #endif
    #ifndef SETTER_Dropdown_Selected
    JSValue Dropdown::setSelected(JSContext *ctx, JSValueConst this_val, JSValueConst val){
        THIS_NCLASS(Dropdown,thisobj)
        uint32_t selected ;
        if(JS_ToUint32(ctx, (uint32_t *) &selected, val)!=0){
            JSTHROW("arg %s of method %s.%s() must be a %s","selected","Dropdown","setSelected","number")
        }
        lv_dropdown_set_selected(thisobj->lvobj(), selected) ;
        return JS_UNDEFINED ;
    }
    #endif
    #ifndef GETTER_Dropdown_Dir
    JSValue Dropdown::getDir(JSContext *ctx, JSValueConst this_val){
        THIS_NCLASS(Dropdown,thisobj)
        lv_dir_t value = lv_dropdown_get_dir(thisobj->lvobj()) ;
        JSValue retval = lv_dir_const_to_jsstr(ctx, value) ;
        return retval ;
    }
    #endif
    #ifndef SETTER_Dropdown_Dir
    JSValue Dropdown::setDir(JSContext *ctx, JSValueConst this_val, JSValueConst val){
        THIS_NCLASS(Dropdown,thisobj)
        // argv dir
        const char * cstr_val = JS_ToCString(ctx, val) ;
        lv_dir_t dir;
        if(!lv_dir_str_to_const(cstr_val,&dir)) {
            JS_ThrowReferenceError(ctx,"unknow %s value: %s","lv_dir_t",cstr_val) ;
            JS_FreeCString(ctx, cstr_val) ;
            return JS_EXCEPTION ;
        }
        JS_FreeCString(ctx, cstr_val) ;
        lv_dropdown_set_dir(thisobj->lvobj(), dir) ;
        return JS_UNDEFINED ;
    }
    #endif
    // unspported type: const char *
    // JSValue Dropdown::getSymbol(JSContext *ctx, JSValueConst this_val){}
    // const char * lv_dropdown_get_symbol(lv_obj_t * obj)
    // unspported type: const void *
    // JSValue Dropdown::setSymbol(JSContext *ctx, JSValueConst this_val, JSValueConst value){}
    // void lv_dropdown_set_symbol(lv_obj_t * obj, const void * symbol)
    #ifndef GETTER_Dropdown_SelectedHighlight
    JSValue Dropdown::getSelectedHighlight(JSContext *ctx, JSValueConst this_val){
        THIS_NCLASS(Dropdown,thisobj)
        bool value = lv_dropdown_get_selected_highlight(thisobj->lvobj()) ;
        JSValue retval = JS_NewBool(ctx, value) ;
        return retval ;
    }
    #endif
    #ifndef SETTER_Dropdown_SelectedHighlight
    JSValue Dropdown::setSelectedHighlight(JSContext *ctx, JSValueConst this_val, JSValueConst val){
        THIS_NCLASS(Dropdown,thisobj)
        bool selectedHighlight = JS_ToBool(ctx, val) ;
        lv_dropdown_set_selected_highlight(thisobj->lvobj(), selectedHighlight) ;
        return JS_UNDEFINED ;
    }
    #endif
    #ifndef GETTER_Dropdown_List
    JSValue Dropdown::getList(JSContext *ctx, JSValueConst this_val){
        THIS_NCLASS(Dropdown,thisobj)
        lv_obj_t * value = lv_dropdown_get_list(thisobj->lvobj()) ;
        JSValue retval = value? be::lv::Obj::wrap(ctx, (lv_obj_t*)value)->jsobj: JS_NULL ;
        return retval ;
    }
    #endif
    #ifndef GETTER_Dropdown_OptionCount
    JSValue Dropdown::getOptionCount(JSContext *ctx, JSValueConst this_val){
        THIS_NCLASS(Dropdown,thisobj)
        uint32_t value = lv_dropdown_get_option_count(thisobj->lvobj()) ;
        JSValue retval = JS_NewUint32(ctx, value) ;
        return retval ;
    }
    #endif
// AUTO GENERATE CODE END [GETSETS] --------

// AUTO GENERATE CODE START [METHODS] --------
    // Unsupported arg type: lv_subject_t *
    // lv_observer_t * lv_dropdown_bind_value(lv_obj_t * obj, lv_subject_t * subject)

    #ifndef METHOD_Dropdown_addOption
    JSValue Dropdown::addOption(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        THIS_NCLASS(Obj,thisobj)
        ASSERT_ARGC(2)
        char * option = (char *)JS_ToCString(ctx, argv[0]) ;
        uint32_t pos ;
        if(JS_ToUint32(ctx, (uint32_t *) &pos, argv[1])!=0){
            JSTHROW("arg %s of method %s.%s() must be a %s","pos","Dropdown","addOption","number")
        }
        lv_dropdown_add_option( thisobj->lvobj(), option, pos ) ;
        return JS_UNDEFINED ;
    }
    #endif

    #ifndef METHOD_Dropdown_clearOptions
    JSValue Dropdown::clearOptions(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        THIS_NCLASS(Obj,thisobj)
        lv_dropdown_clear_options( thisobj->lvobj() ) ;
        return JS_UNDEFINED ;
    }
    #endif

    #ifndef METHOD_Dropdown_getSelectedStr
    JSValue Dropdown::getSelectedStr(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        THIS_NCLASS(Obj,thisobj)
        ASSERT_ARGC(2)
        char * buf = (char *)JS_ToCString(ctx, argv[0]) ;
        uint32_t buf_size ;
        if(JS_ToUint32(ctx, (uint32_t *) &buf_size, argv[1])!=0){
            JSTHROW("arg %s of method %s.%s() must be a %s","buf_size","Dropdown","getSelectedStr","number")
        }
        lv_dropdown_get_selected_str( thisobj->lvobj(), buf, buf_size ) ;
        return JS_UNDEFINED ;
    }
    #endif

    #ifndef METHOD_Dropdown_getOptionIndex
    JSValue Dropdown::getOptionIndex(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        THIS_NCLASS(Obj,thisobj)
        ASSERT_ARGC(1)
        char * option = (char *)JS_ToCString(ctx, argv[0]) ;
        int32_t retval = lv_dropdown_get_option_index( thisobj->lvobj(), option ) ;
            JS_FreeCString(ctx, option) ;
        JSValue jsretval = JS_NewInt32(ctx, retval) ;
        return jsretval ;
    }
    #endif

    #ifndef METHOD_Dropdown_open
    JSValue Dropdown::open(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        THIS_NCLASS(Obj,thisobj)
        lv_dropdown_open( thisobj->lvobj() ) ;
        return JS_UNDEFINED ;
    }
    #endif

    #ifndef METHOD_Dropdown_close
    JSValue Dropdown::close(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        THIS_NCLASS(Obj,thisobj)
        lv_dropdown_close( thisobj->lvobj() ) ;
        return JS_UNDEFINED ;
    }
    #endif

    #ifndef METHOD_Dropdown_isOpen
    JSValue Dropdown::isOpen(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        THIS_NCLASS(Obj,thisobj)
        bool retval = lv_dropdown_is_open( thisobj->lvobj() ) ;
        JSValue jsretval = JS_NewBool(ctx, retval) ;
        return jsretval ;
    }
    #endif
// AUTO GENERATE CODE END [METHODS] --------

}
