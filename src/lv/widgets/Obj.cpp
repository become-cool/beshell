#include "Obj.hpp"
#include "../all-widgets.hpp"

using namespace std ;

namespace be::lv {

    DEFINE_NCLASS_META(Obj, NativeClass)

    
    JSValue dbginfo(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        THIS_NCLASS(Obj,thisobj)
        dp(thisobj)
        dp(thisobj->lvobj())
        return JS_UNDEFINED ;
    }
    
    std::vector<JSCFunctionListEntry> Obj::methods = {
        JS_CFUNC_DEF("dbginfo", 0, dbginfo),

// AUTO GENERATE CODE START [GETSET LIST] --------
        JS_CGETSET_DEF("state",Obj::getState,be::lv::Obj::invalidSetter) ,
        JS_CGETSET_DEF("eventCount",Obj::getEventCount,be::lv::Obj::invalidSetter) ,
        JS_CGETSET_DEF("x",Obj::getX,Obj::setX) ,
        JS_CGETSET_DEF("y",Obj::getY,Obj::setY) ,
        JS_CGETSET_DEF("width",Obj::getWidth,Obj::setWidth) ,
        JS_CGETSET_DEF("height",Obj::getHeight,Obj::setHeight) ,
        JS_CGETSET_DEF("contentWidth",Obj::getContentWidth,Obj::setContentWidth) ,
        JS_CGETSET_DEF("contentHeight",Obj::getContentHeight,Obj::setContentHeight) ,
        JS_CGETSET_DEF("layout",be::lv::Obj::invalidGetter,Obj::setLayout) ,
        JS_CGETSET_DEF("align",be::lv::Obj::invalidGetter,Obj::setAlign) ,
        JS_CGETSET_DEF("xAligned",Obj::getXAligned,be::lv::Obj::invalidSetter) ,
        JS_CGETSET_DEF("yAligned",Obj::getYAligned,be::lv::Obj::invalidSetter) ,
        JS_CGETSET_DEF("selfWidth",Obj::getSelfWidth,be::lv::Obj::invalidSetter) ,
        JS_CGETSET_DEF("selfHeight",Obj::getSelfHeight,be::lv::Obj::invalidSetter) ,
        JS_CGETSET_DEF("extClickArea",be::lv::Obj::invalidGetter,Obj::setExtClickArea) ,
        JS_CGETSET_DEF("scrollbarMode",Obj::getScrollbarMode,Obj::setScrollbarMode) ,
        JS_CGETSET_DEF("scrollDir",Obj::getScrollDir,Obj::setScrollDir) ,
        JS_CGETSET_DEF("scrollSnapX",Obj::getScrollSnapX,Obj::setScrollSnapX) ,
        JS_CGETSET_DEF("scrollSnapY",Obj::getScrollSnapY,Obj::setScrollSnapY) ,
        JS_CGETSET_DEF("scrollX",Obj::getScrollX,be::lv::Obj::invalidSetter) ,
        JS_CGETSET_DEF("scrollY",Obj::getScrollY,be::lv::Obj::invalidSetter) ,
        JS_CGETSET_DEF("scrollTop",Obj::getScrollTop,be::lv::Obj::invalidSetter) ,
        JS_CGETSET_DEF("scrollBottom",Obj::getScrollBottom,be::lv::Obj::invalidSetter) ,
        JS_CGETSET_DEF("scrollLeft",Obj::getScrollLeft,be::lv::Obj::invalidSetter) ,
        JS_CGETSET_DEF("scrollRight",Obj::getScrollRight,be::lv::Obj::invalidSetter) ,
        JS_CGETSET_DEF("parent",Obj::getParent,Obj::setParent) ,
        JS_CGETSET_DEF("screen",Obj::getScreen,be::lv::Obj::invalidSetter) ,
        JS_CGETSET_DEF("childCount",Obj::getChildCount,be::lv::Obj::invalidSetter) ,
        JS_CGETSET_DEF("index",Obj::getIndex,be::lv::Obj::invalidSetter) ,
        JS_CGETSET_DEF("flexFlow",be::lv::Obj::invalidGetter,Obj::setFlexFlow) ,
        JS_CGETSET_DEF("flexGrow",be::lv::Obj::invalidGetter,Obj::setFlexGrow) ,
// AUTO GENERATE CODE END [GETSET LIST] --------
// AUTO GENERATE CODE START [METHOD LIST] --------
        JS_CFUNC_DEF("addFlag", 1, Obj::jsAddFlag),
        JS_CFUNC_DEF("removeFlag", 1, Obj::jsRemoveFlag),
        JS_CFUNC_DEF("updateFlag", 2, Obj::jsUpdateFlag),
        JS_CFUNC_DEF("addState", 1, Obj::jsAddState),
        JS_CFUNC_DEF("removeState", 1, Obj::jsRemoveState),
        JS_CFUNC_DEF("allocateSpecAttr", 0, Obj::jsAllocateSpecAttr),
        JS_CFUNC_DEF("classInitObj", 0, Obj::jsClassInitObj),
        JS_CFUNC_DEF("isEditable", 0, Obj::jsIsEditable),
        JS_CFUNC_DEF("isGroupDef", 0, Obj::jsIsGroupDef),
        JS_CFUNC_DEF("calculateExtDrawSize", 1, Obj::jsCalculateExtDrawSize),
        JS_CFUNC_DEF("refreshExtDrawSize", 0, Obj::jsRefreshExtDrawSize),
        JS_CFUNC_DEF("removeEvent", 1, Obj::jsRemoveEvent),
        JS_CFUNC_DEF("refrSize", 0, Obj::jsRefrSize),
        JS_CFUNC_DEF("markLayoutAsDirty", 0, Obj::jsMarkLayoutAsDirty),
        JS_CFUNC_DEF("alignTo", 4, Obj::jsAlignTo),
        JS_CFUNC_DEF("refreshSelfSize", 0, Obj::jsRefreshSelfSize),
        JS_CFUNC_DEF("refrPos", 0, Obj::jsRefrPos),
        JS_CFUNC_DEF("moveTo", 2, Obj::jsMoveTo),
        JS_CFUNC_DEF("moveChildrenBy", 3, Obj::jsMoveChildrenBy),
        JS_CFUNC_DEF("scrollBy", 3, Obj::jsScrollBy),
        JS_CFUNC_DEF("scrollByBounded", 3, Obj::jsScrollByBounded),
        JS_CFUNC_DEF("scrollTo", 3, Obj::jsScrollTo),
        JS_CFUNC_DEF("scrollToX", 2, Obj::jsScrollToX),
        JS_CFUNC_DEF("scrollToY", 2, Obj::jsScrollToY),
        JS_CFUNC_DEF("scrollToView", 1, Obj::jsScrollToView),
        JS_CFUNC_DEF("scrollToViewRecursive", 1, Obj::jsScrollToViewRecursive),
        JS_CFUNC_DEF("updateSnap", 1, Obj::jsUpdateSnap),
        JS_CFUNC_DEF("scrollbarInvalidate", 0, Obj::jsScrollbarInvalidate),
        JS_CFUNC_DEF("readjustScroll", 1, Obj::jsReadjustScroll),
        JS_CFUNC_DEF("removeStyleAll", 0, Obj::jsRemoveStyleAll),
        JS_CFUNC_DEF("fadeIn", 2, Obj::jsFadeIn),
        JS_CFUNC_DEF("fadeOut", 2, Obj::jsFadeOut),
        JS_CFUNC_DEF("delete", 0, Obj::jsDelete),
        JS_CFUNC_DEF("clean", 0, Obj::jsClean),
        JS_CFUNC_DEF("deleteDelayed", 1, Obj::jsDeleteDelayed),
        JS_CFUNC_DEF("deleteAsync", 0, Obj::jsDeleteAsync),
        JS_CFUNC_DEF("swap", 1, Obj::jsSwap),
        JS_CFUNC_DEF("moveToIndex", 1, Obj::jsMoveToIndex),
        JS_CFUNC_DEF("dumpTree", 0, Obj::jsDumpTree),
        // Unsupported arg type:
        // bool lv_obj_has_flag(const lv_obj_t * obj, lv_obj_flag_t f)
        // bool lv_obj_has_flag_any(const lv_obj_t * obj, lv_obj_flag_t f)
        // bool lv_obj_has_state(const lv_obj_t * obj, lv_state_t state)
        // bool lv_obj_check_type(const lv_obj_t * obj, const lv_obj_class_t * class_p)
        // bool lv_obj_has_class(const lv_obj_t * obj, const lv_obj_class_t * class_p)
        // bool lv_obj_is_valid(const lv_obj_t * obj)
        // lv_obj_t * lv_obj_class_create_obj(const lv_obj_class_t * class_p, lv_obj_t * parent)
        // void lv_obj_init_draw_rect_dsc(lv_obj_t * obj, uint32_t part, lv_draw_rect_dsc_t * draw_dsc)
        // void lv_obj_init_draw_label_dsc(lv_obj_t * obj, uint32_t part, lv_draw_label_dsc_t * draw_dsc)
        // void lv_obj_init_draw_image_dsc(lv_obj_t * obj, uint32_t part, lv_draw_image_dsc_t * draw_dsc)
        // void lv_obj_init_draw_line_dsc(lv_obj_t * obj, uint32_t part, lv_draw_line_dsc_t * draw_dsc)
        // void lv_obj_init_draw_arc_dsc(lv_obj_t * obj, uint32_t part, lv_draw_arc_dsc_t * draw_dsc)
        // lv_result_t lv_obj_send_event(lv_obj_t * obj, lv_event_code_t event_code, void * param)
        // lv_result_t lv_obj_event_base(const lv_obj_class_t * class_p, lv_event_t * e)
        // void lv_obj_add_event_cb(lv_obj_t * obj, lv_event_cb_t event_cb, lv_event_code_t filter, void * user_data)
        // bool lv_obj_remove_event_cb(lv_obj_t * obj, lv_event_cb_t event_cb)
        // uint32_t lv_obj_remove_event_cb_with_user_data(lv_obj_t * obj, lv_event_cb_t event_cb, void * user_data)
        // bool lv_obj_is_layout_positioned(const lv_obj_t * obj)
        // void lv_obj_update_layout(const lv_obj_t * obj)
        // void lv_obj_transform_point(const lv_obj_t * obj, lv_point_t * p, bool recursive, bool inv)
        // void lv_obj_invalidate_area(const lv_obj_t * obj, const lv_area_t * area)
        // void lv_obj_invalidate(const lv_obj_t * obj)
        // bool lv_obj_area_is_visible(const lv_obj_t * obj, lv_area_t * area)
        // bool lv_obj_is_visible(const lv_obj_t * obj)
        // bool lv_obj_hit_test(lv_obj_t * obj, const lv_point_t * point)
        // bool lv_obj_is_scrolling(const lv_obj_t * obj)
        // void lv_obj_add_style(lv_obj_t * obj, const lv_style_t * style, lv_style_selector_t selector)
        // bool lv_obj_replace_style(lv_obj_t * obj, const lv_style_t * old_style, const lv_style_t * new_style, lv_style_selector_t selector)
        // void lv_obj_remove_style(lv_obj_t * obj, const lv_style_t * style, lv_style_selector_t selector)
        // void lv_obj_report_style_change(lv_style_t * style)
        // void lv_obj_refresh_style(lv_obj_t * obj, lv_part_t part, lv_style_prop_t prop)
        // void lv_obj_enable_style_refresh(bool en)
        // bool lv_obj_has_style_prop(const lv_obj_t * obj, lv_style_selector_t selector, lv_style_prop_t prop)
        // bool lv_obj_remove_local_style_prop(lv_obj_t * obj, lv_style_prop_t prop, lv_style_selector_t selector)
        // lv_text_align_t lv_obj_calculate_style_text_align(const lv_obj_t * obj, lv_part_t part, const char * txt)
        // void lv_obj_delete_anim_completed_cb(lv_anim_t * a)
        // void lv_obj_tree_walk(lv_obj_t * start_obj, lv_obj_tree_walk_cb_t cb, void * user_data)
        // void lv_obj_redraw(lv_layer_t * layer, lv_obj_t * obj)
        // lv_observer_t * lv_obj_bind_flag_if_eq(lv_obj_t * obj, lv_subject_t * subject, lv_obj_flag_t flag, int32_t ref_value)
        // lv_observer_t * lv_obj_bind_flag_if_not_eq(lv_obj_t * obj, lv_subject_t * subject, lv_obj_flag_t flag, int32_t ref_value)
        // lv_observer_t * lv_obj_bind_state_if_eq(lv_obj_t * obj, lv_subject_t * subject, lv_state_t state, int32_t ref_value)
        // lv_observer_t * lv_obj_bind_state_if_not_eq(lv_obj_t * obj, lv_subject_t * subject, lv_state_t state, int32_t ref_value)
// AUTO GENERATE CODE END [METHOD LIST] --------
    } ;

    
    Obj::Obj(JSContext * ctx, JSValue jsobj, lv_obj_t * lvobj)
        : NativeClass(ctx, build(ctx, jsobj))
        , _lvobj(lvobj)
    {
        lv_obj_set_user_data(_lvobj, (void *)this) ;
    }

    Obj::Obj(JSContext * ctx, lv_obj_t * parent)
        : Obj(ctx, JS_NULL, lv_obj_create(parent))
    {}

    JSValue Obj::constructor(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        lv_obj_t * lvparent = nullptr ;
        if(argc>0) {
            JSVALUE_TO_LVOBJ_VAR(argv[0], lvparent)
        }
        Obj * widget = new Obj(ctx,lvparent) ;
        return widget->jsobj ;
    }

    lv_obj_t * Obj::lvobj() const {
        return _lvobj ;
    }
    
    #define CREATE_WIDGET(typename, classname)                                  \
        if(lv_obj_check_type(lvobj, &lv_##typename##_class)) {                  \
            return new classname(ctx, classname::build(ctx, JS_NULL), lvobj) ;  \
        }
    Obj * Obj::wrap(JSContext * ctx, lv_obj_t * lvobj) {
        Obj * widget = (Obj *)lv_obj_get_user_data(lvobj) ;
        if(widget) {
            return widget ;
        }

// AUTO GENERATE CODE START [WRAP WIDGETS] --------
        CREATE_WIDGET(obj, Obj)
        else CREATE_WIDGET(animimg, AnimImg)
        else CREATE_WIDGET(button, Btn)
        else CREATE_WIDGET(canvas, Canvas)
        else CREATE_WIDGET(dropdown, Dropdown)
        else CREATE_WIDGET(keyboard, Keyboard)
        else CREATE_WIDGET(line, Line)
        else CREATE_WIDGET(msgbox, MsgBox)
        else CREATE_WIDGET(scale, Scale)
        else CREATE_WIDGET(spinbox, SpinBox)
        else CREATE_WIDGET(table, Table)
        else CREATE_WIDGET(tileview, TileView)
        else CREATE_WIDGET(arc, Arc)
        else CREATE_WIDGET(buttonmatrix, BtnMatrix)
        else CREATE_WIDGET(chart, Chart)
        else CREATE_WIDGET(image, Img)
        else CREATE_WIDGET(label, Label)
        else CREATE_WIDGET(list, List)
        else CREATE_WIDGET(slider, Slider)
        else CREATE_WIDGET(spinner, Spinner)
        else CREATE_WIDGET(tabview, TabView)
        else CREATE_WIDGET(win, Win)
        else CREATE_WIDGET(bar, Bar)
        else CREATE_WIDGET(calendar, Calendar)
        else CREATE_WIDGET(checkbox, Checkbox)
        else CREATE_WIDGET(imagebutton, ImgBtn)
        else CREATE_WIDGET(led, Led)
        else CREATE_WIDGET(menu, Menu)
        else CREATE_WIDGET(roller, Roller)
        else CREATE_WIDGET(spangroup, Span)
        else CREATE_WIDGET(switch, Switch)
        else CREATE_WIDGET(textarea, TextArea)
// AUTO GENERATE CODE END [WRAP WIDGETS] --------
        else {
            return new Obj(ctx, JS_NULL, lvobj) ;
        }
    }

    JSValue Obj::invalidGetter(JSContext *ctx, JSValueConst this_val) {
        return JS_UNDEFINED ;
    }
    JSValue Obj::invalidSetter(JSContext *ctx, JSValueConst this_val, JSValueConst value) {
        return JS_UNDEFINED ;
    }
    
// AUTO GENERATE CODE START [GETSETS] --------
    JSValue Obj::getState(JSContext *ctx, JSValueConst this_val){
        THIS_NCLASS(Obj,thisobj)
        lv_state_t value = lv_obj_get_state(thisobj->lvobj()) ;
        JSValue retval = JS_NewUint32(ctx, value) ;
        return retval ;
    }
    // unspported type: lv_group_t *
    // JSValue Obj::getGroup(JSContext *ctx, JSValueConst this_val){}
    // lv_group_t * lv_obj_get_group(const lv_obj_t * obj)
    // unspported type: const lv_obj_class_t *
    // JSValue Obj::getClass(JSContext *ctx, JSValueConst this_val){}
    // const lv_obj_class_t * lv_obj_get_class(const lv_obj_t * obj)
    JSValue Obj::getEventCount(JSContext *ctx, JSValueConst this_val){
        THIS_NCLASS(Obj,thisobj)
        uint32_t value = lv_obj_get_event_count(thisobj->lvobj()) ;
        JSValue retval = JS_NewUint32(ctx, value) ;
        return retval ;
    }
    JSValue Obj::getX(JSContext *ctx, JSValueConst this_val){
        THIS_NCLASS(Obj,thisobj)
        int32_t value = lv_obj_get_x(thisobj->lvobj()) ;
        JSValue retval = JS_NewInt32(ctx, value) ;
        return retval ;
    }
    JSValue Obj::setX(JSContext *ctx, JSValueConst this_val, JSValueConst val){
        THIS_NCLASS(Obj,thisobj)
        int32_t x ;
        if(JS_ToInt32(ctx, (int32_t *) &x, val)!=0){
            JSTHROW("arg %s of method %s.%s() must be a %s","x","Obj","setX","number")
        }
        lv_obj_set_x(thisobj->lvobj(), x) ;
        return JS_UNDEFINED ;
    }
    JSValue Obj::getY(JSContext *ctx, JSValueConst this_val){
        THIS_NCLASS(Obj,thisobj)
        int32_t value = lv_obj_get_y(thisobj->lvobj()) ;
        JSValue retval = JS_NewInt32(ctx, value) ;
        return retval ;
    }
    JSValue Obj::setY(JSContext *ctx, JSValueConst this_val, JSValueConst val){
        THIS_NCLASS(Obj,thisobj)
        int32_t y ;
        if(JS_ToInt32(ctx, (int32_t *) &y, val)!=0){
            JSTHROW("arg %s of method %s.%s() must be a %s","y","Obj","setY","number")
        }
        lv_obj_set_y(thisobj->lvobj(), y) ;
        return JS_UNDEFINED ;
    }
    JSValue Obj::getWidth(JSContext *ctx, JSValueConst this_val){
        THIS_NCLASS(Obj,thisobj)
        int32_t value = lv_obj_get_width(thisobj->lvobj()) ;
        JSValue retval = JS_NewInt32(ctx, value) ;
        return retval ;
    }
    JSValue Obj::setWidth(JSContext *ctx, JSValueConst this_val, JSValueConst val){
        THIS_NCLASS(Obj,thisobj)
        int32_t width ;
        if(JS_ToInt32(ctx, (int32_t *) &width, val)!=0){
            JSTHROW("arg %s of method %s.%s() must be a %s","width","Obj","setWidth","number")
        }
        lv_obj_set_width(thisobj->lvobj(), width) ;
        return JS_UNDEFINED ;
    }
    JSValue Obj::getHeight(JSContext *ctx, JSValueConst this_val){
        THIS_NCLASS(Obj,thisobj)
        int32_t value = lv_obj_get_height(thisobj->lvobj()) ;
        JSValue retval = JS_NewInt32(ctx, value) ;
        return retval ;
    }
    JSValue Obj::setHeight(JSContext *ctx, JSValueConst this_val, JSValueConst val){
        THIS_NCLASS(Obj,thisobj)
        int32_t height ;
        if(JS_ToInt32(ctx, (int32_t *) &height, val)!=0){
            JSTHROW("arg %s of method %s.%s() must be a %s","height","Obj","setHeight","number")
        }
        lv_obj_set_height(thisobj->lvobj(), height) ;
        return JS_UNDEFINED ;
    }
    JSValue Obj::getContentWidth(JSContext *ctx, JSValueConst this_val){
        THIS_NCLASS(Obj,thisobj)
        int32_t value = lv_obj_get_content_width(thisobj->lvobj()) ;
        JSValue retval = JS_NewInt32(ctx, value) ;
        return retval ;
    }
    JSValue Obj::setContentWidth(JSContext *ctx, JSValueConst this_val, JSValueConst val){
        THIS_NCLASS(Obj,thisobj)
        int32_t contentWidth ;
        if(JS_ToInt32(ctx, (int32_t *) &contentWidth, val)!=0){
            JSTHROW("arg %s of method %s.%s() must be a %s","contentWidth","Obj","setContentWidth","number")
        }
        lv_obj_set_content_width(thisobj->lvobj(), contentWidth) ;
        return JS_UNDEFINED ;
    }
    JSValue Obj::getContentHeight(JSContext *ctx, JSValueConst this_val){
        THIS_NCLASS(Obj,thisobj)
        int32_t value = lv_obj_get_content_height(thisobj->lvobj()) ;
        JSValue retval = JS_NewInt32(ctx, value) ;
        return retval ;
    }
    JSValue Obj::setContentHeight(JSContext *ctx, JSValueConst this_val, JSValueConst val){
        THIS_NCLASS(Obj,thisobj)
        int32_t contentHeight ;
        if(JS_ToInt32(ctx, (int32_t *) &contentHeight, val)!=0){
            JSTHROW("arg %s of method %s.%s() must be a %s","contentHeight","Obj","setContentHeight","number")
        }
        lv_obj_set_content_height(thisobj->lvobj(), contentHeight) ;
        return JS_UNDEFINED ;
    }
    JSValue Obj::setLayout(JSContext *ctx, JSValueConst this_val, JSValueConst val){
        THIS_NCLASS(Obj,thisobj)
        uint32_t layout ;
        if(JS_ToUint32(ctx, (uint32_t *) &layout, val)!=0){
            JSTHROW("arg %s of method %s.%s() must be a %s","layout","Obj","setLayout","number")
        }
        lv_obj_set_layout(thisobj->lvobj(), layout) ;
        return JS_UNDEFINED ;
    }
    JSValue Obj::setAlign(JSContext *ctx, JSValueConst this_val, JSValueConst val){
        THIS_NCLASS(Obj,thisobj)
        // argv align
        const char * cstr_val = JS_ToCString(ctx, val) ;
        lv_align_t align;
        if(lv_align_str_to_const(cstr_val,&align)) {
            JS_ThrowReferenceError(ctx,"unknow %s value: %s","lv_align_t",cstr_val) ;
            JS_FreeCString(ctx, cstr_val) ;
            return JS_EXCEPTION ;
        }
        JS_FreeCString(ctx, cstr_val) ;
        lv_obj_set_align(thisobj->lvobj(), align) ;
        return JS_UNDEFINED ;
    }
    JSValue Obj::getXAligned(JSContext *ctx, JSValueConst this_val){
        THIS_NCLASS(Obj,thisobj)
        int32_t value = lv_obj_get_x_aligned(thisobj->lvobj()) ;
        JSValue retval = JS_NewInt32(ctx, value) ;
        return retval ;
    }
    JSValue Obj::getYAligned(JSContext *ctx, JSValueConst this_val){
        THIS_NCLASS(Obj,thisobj)
        int32_t value = lv_obj_get_y_aligned(thisobj->lvobj()) ;
        JSValue retval = JS_NewInt32(ctx, value) ;
        return retval ;
    }
    JSValue Obj::getSelfWidth(JSContext *ctx, JSValueConst this_val){
        THIS_NCLASS(Obj,thisobj)
        int32_t value = lv_obj_get_self_width(thisobj->lvobj()) ;
        JSValue retval = JS_NewInt32(ctx, value) ;
        return retval ;
    }
    JSValue Obj::getSelfHeight(JSContext *ctx, JSValueConst this_val){
        THIS_NCLASS(Obj,thisobj)
        int32_t value = lv_obj_get_self_height(thisobj->lvobj()) ;
        JSValue retval = JS_NewInt32(ctx, value) ;
        return retval ;
    }
    JSValue Obj::setExtClickArea(JSContext *ctx, JSValueConst this_val, JSValueConst val){
        THIS_NCLASS(Obj,thisobj)
        int32_t extClickArea ;
        if(JS_ToInt32(ctx, (int32_t *) &extClickArea, val)!=0){
            JSTHROW("arg %s of method %s.%s() must be a %s","extClickArea","Obj","setExtClickArea","number")
        }
        lv_obj_set_ext_click_area(thisobj->lvobj(), extClickArea) ;
        return JS_UNDEFINED ;
    }
    JSValue Obj::getScrollbarMode(JSContext *ctx, JSValueConst this_val){
        THIS_NCLASS(Obj,thisobj)
        lv_scrollbar_mode_t value = lv_obj_get_scrollbar_mode(thisobj->lvobj()) ;
        JSValue retval = JS_NewUint32(ctx, value) ;
        return retval ;
    }
    JSValue Obj::setScrollbarMode(JSContext *ctx, JSValueConst this_val, JSValueConst val){
        THIS_NCLASS(Obj,thisobj)
        // argv scrollbarMode
        const char * cstr_val = JS_ToCString(ctx, val) ;
        lv_scrollbar_mode_t scrollbarMode;
        if(lv_scrollbar_mode_str_to_const(cstr_val,&scrollbarMode)) {
            JS_ThrowReferenceError(ctx,"unknow %s value: %s","lv_scrollbar_mode_t",cstr_val) ;
            JS_FreeCString(ctx, cstr_val) ;
            return JS_EXCEPTION ;
        }
        JS_FreeCString(ctx, cstr_val) ;
        lv_obj_set_scrollbar_mode(thisobj->lvobj(), scrollbarMode) ;
        return JS_UNDEFINED ;
    }
    JSValue Obj::getScrollDir(JSContext *ctx, JSValueConst this_val){
        THIS_NCLASS(Obj,thisobj)
        lv_dir_t value = lv_obj_get_scroll_dir(thisobj->lvobj()) ;
        JSValue retval = lv_dir_const_to_jsstr(ctx, value) ;
        return retval ;
    }
    JSValue Obj::setScrollDir(JSContext *ctx, JSValueConst this_val, JSValueConst val){
        THIS_NCLASS(Obj,thisobj)
        // argv scrollDir
        const char * cstr_val = JS_ToCString(ctx, val) ;
        lv_dir_t scrollDir;
        if(lv_dir_str_to_const(cstr_val,&scrollDir)) {
            JS_ThrowReferenceError(ctx,"unknow %s value: %s","lv_dir_t",cstr_val) ;
            JS_FreeCString(ctx, cstr_val) ;
            return JS_EXCEPTION ;
        }
        JS_FreeCString(ctx, cstr_val) ;
        lv_obj_set_scroll_dir(thisobj->lvobj(), scrollDir) ;
        return JS_UNDEFINED ;
    }
    JSValue Obj::getScrollSnapX(JSContext *ctx, JSValueConst this_val){
        THIS_NCLASS(Obj,thisobj)
        lv_scroll_snap_t value = lv_obj_get_scroll_snap_x(thisobj->lvobj()) ;
        JSValue retval = JS_NewUint32(ctx, value) ;
        return retval ;
    }
    JSValue Obj::setScrollSnapX(JSContext *ctx, JSValueConst this_val, JSValueConst val){
        THIS_NCLASS(Obj,thisobj)
        // argv scrollSnapX
        const char * cstr_val = JS_ToCString(ctx, val) ;
        lv_scroll_snap_t scrollSnapX;
        if(lv_scroll_snap_str_to_const(cstr_val,&scrollSnapX)) {
            JS_ThrowReferenceError(ctx,"unknow %s value: %s","lv_scroll_snap_t",cstr_val) ;
            JS_FreeCString(ctx, cstr_val) ;
            return JS_EXCEPTION ;
        }
        JS_FreeCString(ctx, cstr_val) ;
        lv_obj_set_scroll_snap_x(thisobj->lvobj(), scrollSnapX) ;
        return JS_UNDEFINED ;
    }
    JSValue Obj::getScrollSnapY(JSContext *ctx, JSValueConst this_val){
        THIS_NCLASS(Obj,thisobj)
        lv_scroll_snap_t value = lv_obj_get_scroll_snap_y(thisobj->lvobj()) ;
        JSValue retval = JS_NewUint32(ctx, value) ;
        return retval ;
    }
    JSValue Obj::setScrollSnapY(JSContext *ctx, JSValueConst this_val, JSValueConst val){
        THIS_NCLASS(Obj,thisobj)
        // argv scrollSnapY
        const char * cstr_val = JS_ToCString(ctx, val) ;
        lv_scroll_snap_t scrollSnapY;
        if(lv_scroll_snap_str_to_const(cstr_val,&scrollSnapY)) {
            JS_ThrowReferenceError(ctx,"unknow %s value: %s","lv_scroll_snap_t",cstr_val) ;
            JS_FreeCString(ctx, cstr_val) ;
            return JS_EXCEPTION ;
        }
        JS_FreeCString(ctx, cstr_val) ;
        lv_obj_set_scroll_snap_y(thisobj->lvobj(), scrollSnapY) ;
        return JS_UNDEFINED ;
    }
    JSValue Obj::getScrollX(JSContext *ctx, JSValueConst this_val){
        THIS_NCLASS(Obj,thisobj)
        int32_t value = lv_obj_get_scroll_x(thisobj->lvobj()) ;
        JSValue retval = JS_NewInt32(ctx, value) ;
        return retval ;
    }
    JSValue Obj::getScrollY(JSContext *ctx, JSValueConst this_val){
        THIS_NCLASS(Obj,thisobj)
        int32_t value = lv_obj_get_scroll_y(thisobj->lvobj()) ;
        JSValue retval = JS_NewInt32(ctx, value) ;
        return retval ;
    }
    JSValue Obj::getScrollTop(JSContext *ctx, JSValueConst this_val){
        THIS_NCLASS(Obj,thisobj)
        int32_t value = lv_obj_get_scroll_top(thisobj->lvobj()) ;
        JSValue retval = JS_NewInt32(ctx, value) ;
        return retval ;
    }
    JSValue Obj::getScrollBottom(JSContext *ctx, JSValueConst this_val){
        THIS_NCLASS(Obj,thisobj)
        int32_t value = lv_obj_get_scroll_bottom(thisobj->lvobj()) ;
        JSValue retval = JS_NewInt32(ctx, value) ;
        return retval ;
    }
    JSValue Obj::getScrollLeft(JSContext *ctx, JSValueConst this_val){
        THIS_NCLASS(Obj,thisobj)
        int32_t value = lv_obj_get_scroll_left(thisobj->lvobj()) ;
        JSValue retval = JS_NewInt32(ctx, value) ;
        return retval ;
    }
    JSValue Obj::getScrollRight(JSContext *ctx, JSValueConst this_val){
        THIS_NCLASS(Obj,thisobj)
        int32_t value = lv_obj_get_scroll_right(thisobj->lvobj()) ;
        JSValue retval = JS_NewInt32(ctx, value) ;
        return retval ;
    }
    JSValue Obj::getParent(JSContext *ctx, JSValueConst this_val){
        THIS_NCLASS(Obj,thisobj)
        lv_obj_t * value = lv_obj_get_parent(thisobj->lvobj()) ;
        JSValue retval = value? be::lv::Obj::wrap(ctx, (lv_obj_t*)value)->jsobj: JS_NULL ;
        return retval ;
    }
    JSValue Obj::setParent(JSContext *ctx, JSValueConst this_val, JSValueConst val){
        THIS_NCLASS(Obj,thisobj)
        JSVALUE_TO_LVOBJ(val,parent)
        lv_obj_set_parent(thisobj->lvobj(), parent) ;
        return JS_UNDEFINED ;
    }
    JSValue Obj::getScreen(JSContext *ctx, JSValueConst this_val){
        THIS_NCLASS(Obj,thisobj)
        lv_obj_t * value = lv_obj_get_screen(thisobj->lvobj()) ;
        JSValue retval = value? be::lv::Obj::wrap(ctx, (lv_obj_t*)value)->jsobj: JS_NULL ;
        return retval ;
    }
    // unspported type: lv_display_t *
    // JSValue Obj::getDisplay(JSContext *ctx, JSValueConst this_val){}
    // lv_display_t * lv_obj_get_display(const lv_obj_t * obj)
    JSValue Obj::getChildCount(JSContext *ctx, JSValueConst this_val){
        THIS_NCLASS(Obj,thisobj)
        uint32_t value = lv_obj_get_child_count(thisobj->lvobj()) ;
        JSValue retval = JS_NewUint32(ctx, value) ;
        return retval ;
    }
    JSValue Obj::getIndex(JSContext *ctx, JSValueConst this_val){
        THIS_NCLASS(Obj,thisobj)
        int32_t value = lv_obj_get_index(thisobj->lvobj()) ;
        JSValue retval = JS_NewInt32(ctx, value) ;
        return retval ;
    }
    JSValue Obj::setFlexFlow(JSContext *ctx, JSValueConst this_val, JSValueConst val){
        THIS_NCLASS(Obj,thisobj)
        // argv flexFlow
        const char * cstr_val = JS_ToCString(ctx, val) ;
        lv_flex_flow_t flexFlow;
        if(lv_flex_flow_str_to_const(cstr_val,&flexFlow)) {
            JS_ThrowReferenceError(ctx,"unknow %s value: %s","lv_flex_flow_t",cstr_val) ;
            JS_FreeCString(ctx, cstr_val) ;
            return JS_EXCEPTION ;
        }
        JS_FreeCString(ctx, cstr_val) ;
        lv_obj_set_flex_flow(thisobj->lvobj(), flexFlow) ;
        return JS_UNDEFINED ;
    }
    JSValue Obj::setFlexGrow(JSContext *ctx, JSValueConst this_val, JSValueConst val){
        THIS_NCLASS(Obj,thisobj)
        uint8_t flexGrow ;
        if(JS_ToUint32(ctx, (uint32_t *) &flexGrow, val)!=0){
            JSTHROW("arg %s of method %s.%s() must be a %s","flexGrow","Obj","setFlexGrow","number")
        }
        lv_obj_set_flex_grow(thisobj->lvobj(), flexGrow) ;
        return JS_UNDEFINED ;
    }
// AUTO GENERATE CODE END [GETSETS] --------


// AUTO GENERATE CODE START [METHODS] --------
        JSValue Obj::jsAddFlag(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
            THIS_NCLASS(Obj,thisobj)
            CHECK_ARGC(1)
            // argv f
            const char * cstr_argv_0_ = JS_ToCString(ctx, argv[0]) ;
            lv_obj_flag_t f;
            if(lv__obj_flag_str_to_const(cstr_argv_0_,&f)) {
                JS_ThrowReferenceError(ctx,"unknow %s value: %s","lv_obj_flag_t",cstr_argv_0_) ;
                JS_FreeCString(ctx, cstr_argv_0_) ;
                return JS_EXCEPTION ;
            }
            JS_FreeCString(ctx, cstr_argv_0_) ;
            lv_obj_add_flag( thisobj->lvobj(), f ) ;
            return JS_UNDEFINED ;
        }

        JSValue Obj::jsRemoveFlag(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
            THIS_NCLASS(Obj,thisobj)
            CHECK_ARGC(1)
            // argv f
            const char * cstr_argv_0_ = JS_ToCString(ctx, argv[0]) ;
            lv_obj_flag_t f;
            if(lv__obj_flag_str_to_const(cstr_argv_0_,&f)) {
                JS_ThrowReferenceError(ctx,"unknow %s value: %s","lv_obj_flag_t",cstr_argv_0_) ;
                JS_FreeCString(ctx, cstr_argv_0_) ;
                return JS_EXCEPTION ;
            }
            JS_FreeCString(ctx, cstr_argv_0_) ;
            lv_obj_remove_flag( thisobj->lvobj(), f ) ;
            return JS_UNDEFINED ;
        }

        JSValue Obj::jsUpdateFlag(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
            THIS_NCLASS(Obj,thisobj)
            CHECK_ARGC(2)
            // argv f
            const char * cstr_argv_0_ = JS_ToCString(ctx, argv[0]) ;
            lv_obj_flag_t f;
            if(lv__obj_flag_str_to_const(cstr_argv_0_,&f)) {
                JS_ThrowReferenceError(ctx,"unknow %s value: %s","lv_obj_flag_t",cstr_argv_0_) ;
                JS_FreeCString(ctx, cstr_argv_0_) ;
                return JS_EXCEPTION ;
            }
            JS_FreeCString(ctx, cstr_argv_0_) ;
            bool v = JS_ToBool(ctx, argv[1]) ;
            lv_obj_update_flag( thisobj->lvobj(), f, v ) ;
            return JS_UNDEFINED ;
        }

        JSValue Obj::jsAddState(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
            THIS_NCLASS(Obj,thisobj)
            CHECK_ARGC(1)
            // argv state
            const char * cstr_argv_0_ = JS_ToCString(ctx, argv[0]) ;
            lv_state_t state;
            if(lv_state_str_to_const(cstr_argv_0_,&state)) {
                JS_ThrowReferenceError(ctx,"unknow %s value: %s","lv_state_t",cstr_argv_0_) ;
                JS_FreeCString(ctx, cstr_argv_0_) ;
                return JS_EXCEPTION ;
            }
            JS_FreeCString(ctx, cstr_argv_0_) ;
            lv_obj_add_state( thisobj->lvobj(), state ) ;
            return JS_UNDEFINED ;
        }

        JSValue Obj::jsRemoveState(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
            THIS_NCLASS(Obj,thisobj)
            CHECK_ARGC(1)
            // argv state
            const char * cstr_argv_0_ = JS_ToCString(ctx, argv[0]) ;
            lv_state_t state;
            if(lv_state_str_to_const(cstr_argv_0_,&state)) {
                JS_ThrowReferenceError(ctx,"unknow %s value: %s","lv_state_t",cstr_argv_0_) ;
                JS_FreeCString(ctx, cstr_argv_0_) ;
                return JS_EXCEPTION ;
            }
            JS_FreeCString(ctx, cstr_argv_0_) ;
            lv_obj_remove_state( thisobj->lvobj(), state ) ;
            return JS_UNDEFINED ;
        }

        // Unsupported arg type: const lv_obj_t *
        // bool lv_obj_has_flag(const lv_obj_t * obj, lv_obj_flag_t f)

        // Unsupported arg type: const lv_obj_t *
        // bool lv_obj_has_flag_any(const lv_obj_t * obj, lv_obj_flag_t f)

        // Unsupported arg type: const lv_obj_t *
        // bool lv_obj_has_state(const lv_obj_t * obj, lv_state_t state)

        JSValue Obj::jsAllocateSpecAttr(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
            THIS_NCLASS(Obj,thisobj)
            lv_obj_allocate_spec_attr( thisobj->lvobj() ) ;
            return JS_UNDEFINED ;
        }

        // Unsupported arg type: const lv_obj_t *
        // bool lv_obj_check_type(const lv_obj_t * obj, const lv_obj_class_t * class_p)

        // Unsupported arg type: const lv_obj_t *
        // bool lv_obj_has_class(const lv_obj_t * obj, const lv_obj_class_t * class_p)

        // Unsupported arg type: const lv_obj_t *
        // bool lv_obj_is_valid(const lv_obj_t * obj)

        // Unsupported arg type: const lv_obj_class_t *
        // lv_obj_t * lv_obj_class_create_obj(const lv_obj_class_t * class_p, lv_obj_t * parent)

        JSValue Obj::jsClassInitObj(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
            THIS_NCLASS(Obj,thisobj)
            lv_obj_class_init_obj( thisobj->lvobj() ) ;
            return JS_UNDEFINED ;
        }

        JSValue Obj::jsIsEditable(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
            THIS_NCLASS(Obj,thisobj)
            bool retval = lv_obj_is_editable( thisobj->lvobj() ) ;
            JSValue jsretval = JS_NewBool(ctx, retval) ;
            return jsretval ;
        }

        JSValue Obj::jsIsGroupDef(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
            THIS_NCLASS(Obj,thisobj)
            bool retval = lv_obj_is_group_def( thisobj->lvobj() ) ;
            JSValue jsretval = JS_NewBool(ctx, retval) ;
            return jsretval ;
        }

        // Unsupported arg type: lv_draw_rect_dsc_t *
        // void lv_obj_init_draw_rect_dsc(lv_obj_t * obj, uint32_t part, lv_draw_rect_dsc_t * draw_dsc)

        // Unsupported arg type: lv_draw_label_dsc_t *
        // void lv_obj_init_draw_label_dsc(lv_obj_t * obj, uint32_t part, lv_draw_label_dsc_t * draw_dsc)

        // Unsupported arg type: lv_draw_image_dsc_t *
        // void lv_obj_init_draw_image_dsc(lv_obj_t * obj, uint32_t part, lv_draw_image_dsc_t * draw_dsc)

        // Unsupported arg type: lv_draw_line_dsc_t *
        // void lv_obj_init_draw_line_dsc(lv_obj_t * obj, uint32_t part, lv_draw_line_dsc_t * draw_dsc)

        // Unsupported arg type: lv_draw_arc_dsc_t *
        // void lv_obj_init_draw_arc_dsc(lv_obj_t * obj, uint32_t part, lv_draw_arc_dsc_t * draw_dsc)

        JSValue Obj::jsCalculateExtDrawSize(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
            THIS_NCLASS(Obj,thisobj)
            CHECK_ARGC(1)
            uint32_t part ;
            if(JS_ToUint32(ctx, (uint32_t *) &part, argv[0])!=0){
                JSTHROW("arg %s of method %s.%s() must be a %s","part","Obj","calculateExtDrawSize","number")
            }
            int32_t retval = lv_obj_calculate_ext_draw_size( thisobj->lvobj(), part ) ;
            JSValue jsretval = JS_NewInt32(ctx, retval) ;
            return jsretval ;
        }

        JSValue Obj::jsRefreshExtDrawSize(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
            THIS_NCLASS(Obj,thisobj)
            lv_obj_refresh_ext_draw_size( thisobj->lvobj() ) ;
            return JS_UNDEFINED ;
        }

        // Unsupported arg type: void *
        // lv_result_t lv_obj_send_event(lv_obj_t * obj, lv_event_code_t event_code, void * param)

        // Unsupported arg type: const lv_obj_class_t *
        // lv_result_t lv_obj_event_base(const lv_obj_class_t * class_p, lv_event_t * e)

        // Unsupported arg type: lv_event_cb_t
        // void lv_obj_add_event_cb(lv_obj_t * obj, lv_event_cb_t event_cb, lv_event_code_t filter, void * user_data)

        JSValue Obj::jsRemoveEvent(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
            THIS_NCLASS(Obj,thisobj)
            CHECK_ARGC(1)
            uint32_t index ;
            if(JS_ToUint32(ctx, (uint32_t *) &index, argv[0])!=0){
                JSTHROW("arg %s of method %s.%s() must be a %s","index","Obj","removeEvent","number")
            }
            bool retval = lv_obj_remove_event( thisobj->lvobj(), index ) ;
            JSValue jsretval = JS_NewBool(ctx, retval) ;
            return jsretval ;
        }

        // Unsupported arg type: lv_event_cb_t
        // bool lv_obj_remove_event_cb(lv_obj_t * obj, lv_event_cb_t event_cb)

        // Unsupported arg type: lv_event_cb_t
        // uint32_t lv_obj_remove_event_cb_with_user_data(lv_obj_t * obj, lv_event_cb_t event_cb, void * user_data)

        JSValue Obj::jsRefrSize(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
            THIS_NCLASS(Obj,thisobj)
            bool retval = lv_obj_refr_size( thisobj->lvobj() ) ;
            JSValue jsretval = JS_NewBool(ctx, retval) ;
            return jsretval ;
        }

        // Unsupported arg type: const lv_obj_t *
        // bool lv_obj_is_layout_positioned(const lv_obj_t * obj)

        JSValue Obj::jsMarkLayoutAsDirty(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
            THIS_NCLASS(Obj,thisobj)
            lv_obj_mark_layout_as_dirty( thisobj->lvobj() ) ;
            return JS_UNDEFINED ;
        }

        // Unsupported arg type: const lv_obj_t *
        // void lv_obj_update_layout(const lv_obj_t * obj)

        JSValue Obj::jsAlignTo(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
            THIS_NCLASS(Obj,thisobj)
            CHECK_ARGC(4)
            JSVALUE_TO_LVOBJ(argv[0],base)
            // argv align
            const char * cstr_argv_1_ = JS_ToCString(ctx, argv[1]) ;
            lv_align_t align;
            if(lv_align_str_to_const(cstr_argv_1_,&align)) {
                JS_ThrowReferenceError(ctx,"unknow %s value: %s","lv_align_t",cstr_argv_1_) ;
                JS_FreeCString(ctx, cstr_argv_1_) ;
                return JS_EXCEPTION ;
            }
            JS_FreeCString(ctx, cstr_argv_1_) ;
            int32_t x_ofs ;
            if(JS_ToInt32(ctx, (int32_t *) &x_ofs, argv[2])!=0){
                JSTHROW("arg %s of method %s.%s() must be a %s","x_ofs","Obj","alignTo","number")
            }
            int32_t y_ofs ;
            if(JS_ToInt32(ctx, (int32_t *) &y_ofs, argv[3])!=0){
                JSTHROW("arg %s of method %s.%s() must be a %s","y_ofs","Obj","alignTo","number")
            }
            lv_obj_align_to( thisobj->lvobj(), base, align, x_ofs, y_ofs ) ;
            return JS_UNDEFINED ;
        }

        JSValue Obj::jsRefreshSelfSize(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
            THIS_NCLASS(Obj,thisobj)
            bool retval = lv_obj_refresh_self_size( thisobj->lvobj() ) ;
            JSValue jsretval = JS_NewBool(ctx, retval) ;
            return jsretval ;
        }

        JSValue Obj::jsRefrPos(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
            THIS_NCLASS(Obj,thisobj)
            lv_obj_refr_pos( thisobj->lvobj() ) ;
            return JS_UNDEFINED ;
        }

        JSValue Obj::jsMoveTo(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
            THIS_NCLASS(Obj,thisobj)
            CHECK_ARGC(2)
            int32_t x ;
            if(JS_ToInt32(ctx, (int32_t *) &x, argv[0])!=0){
                JSTHROW("arg %s of method %s.%s() must be a %s","x","Obj","moveTo","number")
            }
            int32_t y ;
            if(JS_ToInt32(ctx, (int32_t *) &y, argv[1])!=0){
                JSTHROW("arg %s of method %s.%s() must be a %s","y","Obj","moveTo","number")
            }
            lv_obj_move_to( thisobj->lvobj(), x, y ) ;
            return JS_UNDEFINED ;
        }

        JSValue Obj::jsMoveChildrenBy(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
            THIS_NCLASS(Obj,thisobj)
            CHECK_ARGC(3)
            int32_t x_diff ;
            if(JS_ToInt32(ctx, (int32_t *) &x_diff, argv[0])!=0){
                JSTHROW("arg %s of method %s.%s() must be a %s","x_diff","Obj","moveChildrenBy","number")
            }
            int32_t y_diff ;
            if(JS_ToInt32(ctx, (int32_t *) &y_diff, argv[1])!=0){
                JSTHROW("arg %s of method %s.%s() must be a %s","y_diff","Obj","moveChildrenBy","number")
            }
            bool ignore_floating = JS_ToBool(ctx, argv[2]) ;
            lv_obj_move_children_by( thisobj->lvobj(), x_diff, y_diff, ignore_floating ) ;
            return JS_UNDEFINED ;
        }

        // Unsupported arg type: const lv_obj_t *
        // void lv_obj_transform_point(const lv_obj_t * obj, lv_point_t * p, bool recursive, bool inv)

        // Unsupported arg type: const lv_obj_t *
        // void lv_obj_invalidate_area(const lv_obj_t * obj, const lv_area_t * area)

        // Unsupported arg type: const lv_obj_t *
        // void lv_obj_invalidate(const lv_obj_t * obj)

        // Unsupported arg type: const lv_obj_t *
        // bool lv_obj_area_is_visible(const lv_obj_t * obj, lv_area_t * area)

        // Unsupported arg type: const lv_obj_t *
        // bool lv_obj_is_visible(const lv_obj_t * obj)

        // Unsupported arg type: const lv_point_t *
        // bool lv_obj_hit_test(lv_obj_t * obj, const lv_point_t * point)

        JSValue Obj::jsScrollBy(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
            THIS_NCLASS(Obj,thisobj)
            CHECK_ARGC(3)
            int32_t x ;
            if(JS_ToInt32(ctx, (int32_t *) &x, argv[0])!=0){
                JSTHROW("arg %s of method %s.%s() must be a %s","x","Obj","scrollBy","number")
            }
            int32_t y ;
            if(JS_ToInt32(ctx, (int32_t *) &y, argv[1])!=0){
                JSTHROW("arg %s of method %s.%s() must be a %s","y","Obj","scrollBy","number")
            }
            // argv anim_en
            const char * cstr_argv_2_ = JS_ToCString(ctx, argv[2]) ;
            lv_anim_enable_t anim_en;
            if(lv_anim_enable_str_to_const(cstr_argv_2_,&anim_en)) {
                JS_ThrowReferenceError(ctx,"unknow %s value: %s","lv_anim_enable_t",cstr_argv_2_) ;
                JS_FreeCString(ctx, cstr_argv_2_) ;
                return JS_EXCEPTION ;
            }
            JS_FreeCString(ctx, cstr_argv_2_) ;
            lv_obj_scroll_by( thisobj->lvobj(), x, y, anim_en ) ;
            return JS_UNDEFINED ;
        }

        JSValue Obj::jsScrollByBounded(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
            THIS_NCLASS(Obj,thisobj)
            CHECK_ARGC(3)
            int32_t dx ;
            if(JS_ToInt32(ctx, (int32_t *) &dx, argv[0])!=0){
                JSTHROW("arg %s of method %s.%s() must be a %s","dx","Obj","scrollByBounded","number")
            }
            int32_t dy ;
            if(JS_ToInt32(ctx, (int32_t *) &dy, argv[1])!=0){
                JSTHROW("arg %s of method %s.%s() must be a %s","dy","Obj","scrollByBounded","number")
            }
            // argv anim_en
            const char * cstr_argv_2_ = JS_ToCString(ctx, argv[2]) ;
            lv_anim_enable_t anim_en;
            if(lv_anim_enable_str_to_const(cstr_argv_2_,&anim_en)) {
                JS_ThrowReferenceError(ctx,"unknow %s value: %s","lv_anim_enable_t",cstr_argv_2_) ;
                JS_FreeCString(ctx, cstr_argv_2_) ;
                return JS_EXCEPTION ;
            }
            JS_FreeCString(ctx, cstr_argv_2_) ;
            lv_obj_scroll_by_bounded( thisobj->lvobj(), dx, dy, anim_en ) ;
            return JS_UNDEFINED ;
        }

        JSValue Obj::jsScrollTo(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
            THIS_NCLASS(Obj,thisobj)
            CHECK_ARGC(3)
            int32_t x ;
            if(JS_ToInt32(ctx, (int32_t *) &x, argv[0])!=0){
                JSTHROW("arg %s of method %s.%s() must be a %s","x","Obj","scrollTo","number")
            }
            int32_t y ;
            if(JS_ToInt32(ctx, (int32_t *) &y, argv[1])!=0){
                JSTHROW("arg %s of method %s.%s() must be a %s","y","Obj","scrollTo","number")
            }
            // argv anim_en
            const char * cstr_argv_2_ = JS_ToCString(ctx, argv[2]) ;
            lv_anim_enable_t anim_en;
            if(lv_anim_enable_str_to_const(cstr_argv_2_,&anim_en)) {
                JS_ThrowReferenceError(ctx,"unknow %s value: %s","lv_anim_enable_t",cstr_argv_2_) ;
                JS_FreeCString(ctx, cstr_argv_2_) ;
                return JS_EXCEPTION ;
            }
            JS_FreeCString(ctx, cstr_argv_2_) ;
            lv_obj_scroll_to( thisobj->lvobj(), x, y, anim_en ) ;
            return JS_UNDEFINED ;
        }

        JSValue Obj::jsScrollToX(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
            THIS_NCLASS(Obj,thisobj)
            CHECK_ARGC(2)
            int32_t x ;
            if(JS_ToInt32(ctx, (int32_t *) &x, argv[0])!=0){
                JSTHROW("arg %s of method %s.%s() must be a %s","x","Obj","scrollToX","number")
            }
            // argv anim_en
            const char * cstr_argv_1_ = JS_ToCString(ctx, argv[1]) ;
            lv_anim_enable_t anim_en;
            if(lv_anim_enable_str_to_const(cstr_argv_1_,&anim_en)) {
                JS_ThrowReferenceError(ctx,"unknow %s value: %s","lv_anim_enable_t",cstr_argv_1_) ;
                JS_FreeCString(ctx, cstr_argv_1_) ;
                return JS_EXCEPTION ;
            }
            JS_FreeCString(ctx, cstr_argv_1_) ;
            lv_obj_scroll_to_x( thisobj->lvobj(), x, anim_en ) ;
            return JS_UNDEFINED ;
        }

        JSValue Obj::jsScrollToY(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
            THIS_NCLASS(Obj,thisobj)
            CHECK_ARGC(2)
            int32_t y ;
            if(JS_ToInt32(ctx, (int32_t *) &y, argv[0])!=0){
                JSTHROW("arg %s of method %s.%s() must be a %s","y","Obj","scrollToY","number")
            }
            // argv anim_en
            const char * cstr_argv_1_ = JS_ToCString(ctx, argv[1]) ;
            lv_anim_enable_t anim_en;
            if(lv_anim_enable_str_to_const(cstr_argv_1_,&anim_en)) {
                JS_ThrowReferenceError(ctx,"unknow %s value: %s","lv_anim_enable_t",cstr_argv_1_) ;
                JS_FreeCString(ctx, cstr_argv_1_) ;
                return JS_EXCEPTION ;
            }
            JS_FreeCString(ctx, cstr_argv_1_) ;
            lv_obj_scroll_to_y( thisobj->lvobj(), y, anim_en ) ;
            return JS_UNDEFINED ;
        }

        JSValue Obj::jsScrollToView(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
            THIS_NCLASS(Obj,thisobj)
            CHECK_ARGC(1)
            // argv anim_en
            const char * cstr_argv_0_ = JS_ToCString(ctx, argv[0]) ;
            lv_anim_enable_t anim_en;
            if(lv_anim_enable_str_to_const(cstr_argv_0_,&anim_en)) {
                JS_ThrowReferenceError(ctx,"unknow %s value: %s","lv_anim_enable_t",cstr_argv_0_) ;
                JS_FreeCString(ctx, cstr_argv_0_) ;
                return JS_EXCEPTION ;
            }
            JS_FreeCString(ctx, cstr_argv_0_) ;
            lv_obj_scroll_to_view( thisobj->lvobj(), anim_en ) ;
            return JS_UNDEFINED ;
        }

        JSValue Obj::jsScrollToViewRecursive(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
            THIS_NCLASS(Obj,thisobj)
            CHECK_ARGC(1)
            // argv anim_en
            const char * cstr_argv_0_ = JS_ToCString(ctx, argv[0]) ;
            lv_anim_enable_t anim_en;
            if(lv_anim_enable_str_to_const(cstr_argv_0_,&anim_en)) {
                JS_ThrowReferenceError(ctx,"unknow %s value: %s","lv_anim_enable_t",cstr_argv_0_) ;
                JS_FreeCString(ctx, cstr_argv_0_) ;
                return JS_EXCEPTION ;
            }
            JS_FreeCString(ctx, cstr_argv_0_) ;
            lv_obj_scroll_to_view_recursive( thisobj->lvobj(), anim_en ) ;
            return JS_UNDEFINED ;
        }

        // Unsupported arg type: const lv_obj_t *
        // bool lv_obj_is_scrolling(const lv_obj_t * obj)

        JSValue Obj::jsUpdateSnap(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
            THIS_NCLASS(Obj,thisobj)
            CHECK_ARGC(1)
            // argv anim_en
            const char * cstr_argv_0_ = JS_ToCString(ctx, argv[0]) ;
            lv_anim_enable_t anim_en;
            if(lv_anim_enable_str_to_const(cstr_argv_0_,&anim_en)) {
                JS_ThrowReferenceError(ctx,"unknow %s value: %s","lv_anim_enable_t",cstr_argv_0_) ;
                JS_FreeCString(ctx, cstr_argv_0_) ;
                return JS_EXCEPTION ;
            }
            JS_FreeCString(ctx, cstr_argv_0_) ;
            lv_obj_update_snap( thisobj->lvobj(), anim_en ) ;
            return JS_UNDEFINED ;
        }

        JSValue Obj::jsScrollbarInvalidate(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
            THIS_NCLASS(Obj,thisobj)
            lv_obj_scrollbar_invalidate( thisobj->lvobj() ) ;
            return JS_UNDEFINED ;
        }

        JSValue Obj::jsReadjustScroll(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
            THIS_NCLASS(Obj,thisobj)
            CHECK_ARGC(1)
            // argv anim_en
            const char * cstr_argv_0_ = JS_ToCString(ctx, argv[0]) ;
            lv_anim_enable_t anim_en;
            if(lv_anim_enable_str_to_const(cstr_argv_0_,&anim_en)) {
                JS_ThrowReferenceError(ctx,"unknow %s value: %s","lv_anim_enable_t",cstr_argv_0_) ;
                JS_FreeCString(ctx, cstr_argv_0_) ;
                return JS_EXCEPTION ;
            }
            JS_FreeCString(ctx, cstr_argv_0_) ;
            lv_obj_readjust_scroll( thisobj->lvobj(), anim_en ) ;
            return JS_UNDEFINED ;
        }

        // Unsupported arg type: const lv_style_t *
        // void lv_obj_add_style(lv_obj_t * obj, const lv_style_t * style, lv_style_selector_t selector)

        // Unsupported arg type: const lv_style_t *
        // bool lv_obj_replace_style(lv_obj_t * obj, const lv_style_t * old_style, const lv_style_t * new_style, lv_style_selector_t selector)

        // Unsupported arg type: const lv_style_t *
        // void lv_obj_remove_style(lv_obj_t * obj, const lv_style_t * style, lv_style_selector_t selector)

        JSValue Obj::jsRemoveStyleAll(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
            THIS_NCLASS(Obj,thisobj)
            lv_obj_remove_style_all( thisobj->lvobj() ) ;
            return JS_UNDEFINED ;
        }

        // Unsupported arg type: lv_style_t *
        // void lv_obj_report_style_change(lv_style_t * style)

        // Unsupported arg type: lv_part_t
        // void lv_obj_refresh_style(lv_obj_t * obj, lv_part_t part, lv_style_prop_t prop)

        // Unsupported arg type: bool
        // void lv_obj_enable_style_refresh(bool en)

        // Unsupported arg type: const lv_obj_t *
        // bool lv_obj_has_style_prop(const lv_obj_t * obj, lv_style_selector_t selector, lv_style_prop_t prop)

        // Unsupported arg type: lv_style_selector_t
        // bool lv_obj_remove_local_style_prop(lv_obj_t * obj, lv_style_prop_t prop, lv_style_selector_t selector)

        JSValue Obj::jsFadeIn(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
            THIS_NCLASS(Obj,thisobj)
            CHECK_ARGC(2)
            uint32_t time ;
            if(JS_ToUint32(ctx, (uint32_t *) &time, argv[0])!=0){
                JSTHROW("arg %s of method %s.%s() must be a %s","time","Obj","fadeIn","number")
            }
            uint32_t delay ;
            if(JS_ToUint32(ctx, (uint32_t *) &delay, argv[1])!=0){
                JSTHROW("arg %s of method %s.%s() must be a %s","delay","Obj","fadeIn","number")
            }
            lv_obj_fade_in( thisobj->lvobj(), time, delay ) ;
            return JS_UNDEFINED ;
        }

        JSValue Obj::jsFadeOut(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
            THIS_NCLASS(Obj,thisobj)
            CHECK_ARGC(2)
            uint32_t time ;
            if(JS_ToUint32(ctx, (uint32_t *) &time, argv[0])!=0){
                JSTHROW("arg %s of method %s.%s() must be a %s","time","Obj","fadeOut","number")
            }
            uint32_t delay ;
            if(JS_ToUint32(ctx, (uint32_t *) &delay, argv[1])!=0){
                JSTHROW("arg %s of method %s.%s() must be a %s","delay","Obj","fadeOut","number")
            }
            lv_obj_fade_out( thisobj->lvobj(), time, delay ) ;
            return JS_UNDEFINED ;
        }

        // Unsupported arg type: const lv_obj_t *
        // lv_text_align_t lv_obj_calculate_style_text_align(const lv_obj_t * obj, lv_part_t part, const char * txt)

        JSValue Obj::jsDelete(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
            THIS_NCLASS(Obj,thisobj)
            lv_obj_delete( thisobj->lvobj() ) ;
            return JS_UNDEFINED ;
        }

        JSValue Obj::jsClean(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
            THIS_NCLASS(Obj,thisobj)
            lv_obj_clean( thisobj->lvobj() ) ;
            return JS_UNDEFINED ;
        }

        JSValue Obj::jsDeleteDelayed(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
            THIS_NCLASS(Obj,thisobj)
            CHECK_ARGC(1)
            uint32_t delay_ms ;
            if(JS_ToUint32(ctx, (uint32_t *) &delay_ms, argv[0])!=0){
                JSTHROW("arg %s of method %s.%s() must be a %s","delay_ms","Obj","deleteDelayed","number")
            }
            lv_obj_delete_delayed( thisobj->lvobj(), delay_ms ) ;
            return JS_UNDEFINED ;
        }

        // Unsupported arg type: lv_anim_t *
        // void lv_obj_delete_anim_completed_cb(lv_anim_t * a)

        JSValue Obj::jsDeleteAsync(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
            THIS_NCLASS(Obj,thisobj)
            lv_obj_delete_async( thisobj->lvobj() ) ;
            return JS_UNDEFINED ;
        }

        JSValue Obj::jsSwap(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
            THIS_NCLASS(Obj,thisobj)
            CHECK_ARGC(1)
            JSVALUE_TO_LVOBJ(argv[0],obj2)
            lv_obj_swap( thisobj->lvobj(), obj2 ) ;
            return JS_UNDEFINED ;
        }

        JSValue Obj::jsMoveToIndex(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
            THIS_NCLASS(Obj,thisobj)
            CHECK_ARGC(1)
            int32_t index ;
            if(JS_ToInt32(ctx, (int32_t *) &index, argv[0])!=0){
                JSTHROW("arg %s of method %s.%s() must be a %s","index","Obj","moveToIndex","number")
            }
            lv_obj_move_to_index( thisobj->lvobj(), index ) ;
            return JS_UNDEFINED ;
        }

        // Unsupported arg type: lv_obj_tree_walk_cb_t
        // void lv_obj_tree_walk(lv_obj_t * start_obj, lv_obj_tree_walk_cb_t cb, void * user_data)

        JSValue Obj::jsDumpTree(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
            THIS_NCLASS(Obj,thisobj)
            lv_obj_dump_tree( thisobj->lvobj() ) ;
            return JS_UNDEFINED ;
        }

        // Unsupported arg type: lv_layer_t *
        // void lv_obj_redraw(lv_layer_t * layer, lv_obj_t * obj)

        // Unsupported arg type: lv_subject_t *
        // lv_observer_t * lv_obj_bind_flag_if_eq(lv_obj_t * obj, lv_subject_t * subject, lv_obj_flag_t flag, int32_t ref_value)

        // Unsupported arg type: lv_subject_t *
        // lv_observer_t * lv_obj_bind_flag_if_not_eq(lv_obj_t * obj, lv_subject_t * subject, lv_obj_flag_t flag, int32_t ref_value)

        // Unsupported arg type: lv_subject_t *
        // lv_observer_t * lv_obj_bind_state_if_eq(lv_obj_t * obj, lv_subject_t * subject, lv_state_t state, int32_t ref_value)

        // Unsupported arg type: lv_subject_t *
        // lv_observer_t * lv_obj_bind_state_if_not_eq(lv_obj_t * obj, lv_subject_t * subject, lv_state_t state, int32_t ref_value)
// AUTO GENERATE CODE END [METHODS] --------
}
