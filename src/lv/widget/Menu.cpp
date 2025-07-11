#include "Menu.hpp"

namespace be::lv {
    DEFINE_NCLASS_META(Menu, Obj)
    std::vector<JSCFunctionListEntry> Menu::methods = {
// AUTO GENERATE CODE START [GETSET LIST] --------
        JS_CGETSET_DEF("page",be::lv::Obj::invalidGetter,Menu::setPage) ,
        JS_CGETSET_DEF("sidebarPage",be::lv::Obj::invalidGetter,Menu::setSidebarPage) ,
        JS_CGETSET_DEF("modeHeader",be::lv::Obj::invalidGetter,Menu::setModeHeader) ,
        JS_CGETSET_DEF("modeRootBackButton",be::lv::Obj::invalidGetter,Menu::setModeRootBackButton) ,
        JS_CGETSET_DEF("curMainPage",Menu::getCurMainPage,be::lv::Obj::invalidSetter) ,
        JS_CGETSET_DEF("curSidebarPage",Menu::getCurSidebarPage,be::lv::Obj::invalidSetter) ,
        JS_CGETSET_DEF("mainHeader",Menu::getMainHeader,be::lv::Obj::invalidSetter) ,
        JS_CGETSET_DEF("mainHeaderBackButton",Menu::getMainHeaderBackButton,be::lv::Obj::invalidSetter) ,
        JS_CGETSET_DEF("sidebarHeader",Menu::getSidebarHeader,be::lv::Obj::invalidSetter) ,
        JS_CGETSET_DEF("sidebarHeaderBackButton",Menu::getSidebarHeaderBackButton,be::lv::Obj::invalidSetter) ,
// AUTO GENERATE CODE END [GETSET LIST] --------
// AUTO GENERATE CODE START [METHOD LIST] --------
        JS_CFUNC_DEF("contCreate", 0, Menu::contCreate),
        JS_CFUNC_DEF("sectionCreate", 0, Menu::sectionCreate),
        JS_CFUNC_DEF("separatorCreate", 0, Menu::separatorCreate),
        JS_CFUNC_DEF("setLoadPageEvent", 2, Menu::setLoadPageEvent),
        JS_CFUNC_DEF("backButtonIsRoot", 1, Menu::backButtonIsRoot),
        JS_CFUNC_DEF("clearHistory", 0, Menu::clearHistory),
        // Unsupported arg type:
        // lv_obj_t * lv_menu_page_create(lv_obj_t * parent, char const * const title)
// AUTO GENERATE CODE END [METHOD LIST] --------
    } ;

    Menu::Menu(JSContext * ctx, JSValue jsobj, lv_obj_t * lvobj)
        : Obj(ctx, Menu::build(ctx,jsobj), lvobj)
    {}

    Menu::Menu(JSContext * ctx, lv_obj_t * parent)
        : Menu(ctx, JS_NULL, lv_menu_create(parent))
    {}

    JSValue Menu::constructor(JSContext *ctx, JSValueConst ctor, int argc, JSValueConst *argv) {
        lv_obj_t * lvparent = nullptr ;
        if(argc>0) {
            JSVALUE_TO_LVOBJ_VAR(argv[0], lvparent)
        }
        JSValue obj = newObject(ctx, ctor) ;
        if( JS_IsException(obj) ) {
            return obj ;
        }
        Menu * widget = new Menu(ctx, obj, lv_menu_create(lvparent)) ;
        return widget->jsobj ;
    }

// AUTO GENERATE CODE START [GETSETS] --------
    /**
     * 访问: 只写
     *
     * > 该属性是对 LVGL C API `lv_menu_set_page` 的封装，
     * > 具体用法请参考  [LVGL文档](https://docs.lvgl.io/9.0/API/index.html)
     *
     * @module lv
     * @class Menu
     * @property page:[obj](obj.html)
     *
     */
    #ifndef SETTER_Menu_Page
    JSValue Menu::setPage(JSContext *ctx, JSValueConst this_val, JSValueConst val){
        THIS_NCLASS(Menu,thisobj)
        JSVALUE_TO_LVOBJ(val,page)
        lv_menu_set_page(thisobj->lvobj(), page) ;
        return JS_UNDEFINED ;
    }
    #endif
    // unspported type: char const * const
    // JSValue Menu::setPageTitle(JSContext *ctx, JSValueConst this_val, JSValueConst value){}
    // void lv_menu_set_page_title(lv_obj_t * page, char const * const title)
    // unspported type: char const * const
    // JSValue Menu::setPageTitleStatic(JSContext *ctx, JSValueConst this_val, JSValueConst value){}
    // void lv_menu_set_page_title_static(lv_obj_t * page, char const * const title)
    /**
     * 访问: 只写
     *
     * > 该属性是对 LVGL C API `lv_menu_set_sidebar_page` 的封装，
     * > 具体用法请参考  [LVGL文档](https://docs.lvgl.io/9.0/API/index.html)
     *
     * @module lv
     * @class Menu
     * @property sidebarPage:[obj](obj.html)
     *
     */
    #ifndef SETTER_Menu_SidebarPage
    JSValue Menu::setSidebarPage(JSContext *ctx, JSValueConst this_val, JSValueConst val){
        THIS_NCLASS(Menu,thisobj)
        JSVALUE_TO_LVOBJ(val,sidebarPage)
        lv_menu_set_sidebar_page(thisobj->lvobj(), sidebarPage) ;
        return JS_UNDEFINED ;
    }
    #endif
    /**
     * 访问: 只写
     *
     * > 该属性是对 LVGL C API `lv_menu_set_mode_header` 的封装，
     * > 具体用法请参考  [LVGL文档](https://docs.lvgl.io/9.0/API/index.html)
     *
     * @module lv
     * @class Menu
     * @property modeHeader:LV-CONST
     *
     */
    #ifndef SETTER_Menu_ModeHeader
    JSValue Menu::setModeHeader(JSContext *ctx, JSValueConst this_val, JSValueConst val){
        THIS_NCLASS(Menu,thisobj)
        // argv modeHeader
        const char * cstr_val = JS_ToCString(ctx, val) ;
        lv_menu_mode_header_t modeHeader;
        if(!lv_menu_mode_header_str_to_const(cstr_val,&modeHeader)) {
            JS_ThrowReferenceError(ctx,"unknow %s value: %s","lv_menu_mode_header_t",cstr_val) ;
            JS_FreeCString(ctx, cstr_val) ;
            return JS_EXCEPTION ;
        }
        JS_FreeCString(ctx, cstr_val) ;
        lv_menu_set_mode_header(thisobj->lvobj(), modeHeader) ;
        return JS_UNDEFINED ;
    }
    #endif
    /**
     * 访问: 只写
     *
     * > 该属性是对 LVGL C API `lv_menu_set_mode_root_back_button` 的封装，
     * > 具体用法请参考  [LVGL文档](https://docs.lvgl.io/9.0/API/index.html)
     *
     * @module lv
     * @class Menu
     * @property modeRootBackButton:LV-CONST
     *
     */
    #ifndef SETTER_Menu_ModeRootBackButton
    JSValue Menu::setModeRootBackButton(JSContext *ctx, JSValueConst this_val, JSValueConst val){
        THIS_NCLASS(Menu,thisobj)
        // argv modeRootBackButton
        const char * cstr_val = JS_ToCString(ctx, val) ;
        lv_menu_mode_root_back_button_t modeRootBackButton;
        if(!lv_menu_mode_root_back_button_str_to_const(cstr_val,&modeRootBackButton)) {
            JS_ThrowReferenceError(ctx,"unknow %s value: %s","lv_menu_mode_root_back_button_t",cstr_val) ;
            JS_FreeCString(ctx, cstr_val) ;
            return JS_EXCEPTION ;
        }
        JS_FreeCString(ctx, cstr_val) ;
        lv_menu_set_mode_root_back_button(thisobj->lvobj(), modeRootBackButton) ;
        return JS_UNDEFINED ;
    }
    #endif
    /**
     * 访问: 只读
     *
     * > 该属性是对 LVGL C API `lv_menu_get_cur_main_page` 的封装，
     * > 具体用法请参考  [LVGL文档](https://docs.lvgl.io/9.0/API/index.html)
     *
     * @module lv
     * @class Menu
     * @property curMainPage:[obj](obj.html)
     *
     */
    #ifndef GETTER_Menu_CurMainPage
    JSValue Menu::getCurMainPage(JSContext *ctx, JSValueConst this_val){
        THIS_NCLASS(Menu,thisobj)
        lv_obj_t * value = lv_menu_get_cur_main_page(thisobj->lvobj()) ;
        JSValue retval = value? be::lv::Obj::wrap(ctx, (lv_obj_t*)value)->jsobj: JS_NULL ;
        return retval ;
    }
    #endif
    /**
     * 访问: 只读
     *
     * > 该属性是对 LVGL C API `lv_menu_get_cur_sidebar_page` 的封装，
     * > 具体用法请参考  [LVGL文档](https://docs.lvgl.io/9.0/API/index.html)
     *
     * @module lv
     * @class Menu
     * @property curSidebarPage:[obj](obj.html)
     *
     */
    #ifndef GETTER_Menu_CurSidebarPage
    JSValue Menu::getCurSidebarPage(JSContext *ctx, JSValueConst this_val){
        THIS_NCLASS(Menu,thisobj)
        lv_obj_t * value = lv_menu_get_cur_sidebar_page(thisobj->lvobj()) ;
        JSValue retval = value? be::lv::Obj::wrap(ctx, (lv_obj_t*)value)->jsobj: JS_NULL ;
        return retval ;
    }
    #endif
    /**
     * 访问: 只读
     *
     * > 该属性是对 LVGL C API `lv_menu_get_main_header` 的封装，
     * > 具体用法请参考  [LVGL文档](https://docs.lvgl.io/9.0/API/index.html)
     *
     * @module lv
     * @class Menu
     * @property mainHeader:[obj](obj.html)
     *
     */
    #ifndef GETTER_Menu_MainHeader
    JSValue Menu::getMainHeader(JSContext *ctx, JSValueConst this_val){
        THIS_NCLASS(Menu,thisobj)
        lv_obj_t * value = lv_menu_get_main_header(thisobj->lvobj()) ;
        JSValue retval = value? be::lv::Obj::wrap(ctx, (lv_obj_t*)value)->jsobj: JS_NULL ;
        return retval ;
    }
    #endif
    /**
     * 访问: 只读
     *
     * > 该属性是对 LVGL C API `lv_menu_get_main_header_back_button` 的封装，
     * > 具体用法请参考  [LVGL文档](https://docs.lvgl.io/9.0/API/index.html)
     *
     * @module lv
     * @class Menu
     * @property mainHeaderBackButton:[obj](obj.html)
     *
     */
    #ifndef GETTER_Menu_MainHeaderBackButton
    JSValue Menu::getMainHeaderBackButton(JSContext *ctx, JSValueConst this_val){
        THIS_NCLASS(Menu,thisobj)
        lv_obj_t * value = lv_menu_get_main_header_back_button(thisobj->lvobj()) ;
        JSValue retval = value? be::lv::Obj::wrap(ctx, (lv_obj_t*)value)->jsobj: JS_NULL ;
        return retval ;
    }
    #endif
    /**
     * 访问: 只读
     *
     * > 该属性是对 LVGL C API `lv_menu_get_sidebar_header` 的封装，
     * > 具体用法请参考  [LVGL文档](https://docs.lvgl.io/9.0/API/index.html)
     *
     * @module lv
     * @class Menu
     * @property sidebarHeader:[obj](obj.html)
     *
     */
    #ifndef GETTER_Menu_SidebarHeader
    JSValue Menu::getSidebarHeader(JSContext *ctx, JSValueConst this_val){
        THIS_NCLASS(Menu,thisobj)
        lv_obj_t * value = lv_menu_get_sidebar_header(thisobj->lvobj()) ;
        JSValue retval = value? be::lv::Obj::wrap(ctx, (lv_obj_t*)value)->jsobj: JS_NULL ;
        return retval ;
    }
    #endif
    /**
     * 访问: 只读
     *
     * > 该属性是对 LVGL C API `lv_menu_get_sidebar_header_back_button` 的封装，
     * > 具体用法请参考  [LVGL文档](https://docs.lvgl.io/9.0/API/index.html)
     *
     * @module lv
     * @class Menu
     * @property sidebarHeaderBackButton:[obj](obj.html)
     *
     */
    #ifndef GETTER_Menu_SidebarHeaderBackButton
    JSValue Menu::getSidebarHeaderBackButton(JSContext *ctx, JSValueConst this_val){
        THIS_NCLASS(Menu,thisobj)
        lv_obj_t * value = lv_menu_get_sidebar_header_back_button(thisobj->lvobj()) ;
        JSValue retval = value? be::lv::Obj::wrap(ctx, (lv_obj_t*)value)->jsobj: JS_NULL ;
        return retval ;
    }
    #endif
// AUTO GENERATE CODE END [GETSETS] --------

// AUTO GENERATE CODE START [METHODS] --------
    // Unsupported arg type: char const * const
    // lv_obj_t * lv_menu_page_create(lv_obj_t * parent, char const * const title)

    #ifndef METHOD_Menu_contCreate
    /**
     * > 该方法是对 LVGL C API `lv_menu_cont_create` 的封装，具体用法请参考  [LVGL文档](https://docs.lvgl.io/9.0/API/index.html)
     *
     * @module lv
     * @class Menu
     * @method contCreate
     *
     */
    JSValue Menu::contCreate(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        THIS_NCLASS(Obj,thisobj)
        lv_obj_t * retval = lv_menu_cont_create( thisobj->lvobj() ) ;
        JSValue jsretval = retval? be::lv::Obj::wrap(ctx, (lv_obj_t*)retval)->jsobj: JS_NULL ;
        return jsretval ;
    }
    #endif

    #ifndef METHOD_Menu_sectionCreate
    /**
     * > 该方法是对 LVGL C API `lv_menu_section_create` 的封装，具体用法请参考  [LVGL文档](https://docs.lvgl.io/9.0/API/index.html)
     *
     * @module lv
     * @class Menu
     * @method sectionCreate
     *
     */
    JSValue Menu::sectionCreate(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        THIS_NCLASS(Obj,thisobj)
        lv_obj_t * retval = lv_menu_section_create( thisobj->lvobj() ) ;
        JSValue jsretval = retval? be::lv::Obj::wrap(ctx, (lv_obj_t*)retval)->jsobj: JS_NULL ;
        return jsretval ;
    }
    #endif

    #ifndef METHOD_Menu_separatorCreate
    /**
     * > 该方法是对 LVGL C API `lv_menu_separator_create` 的封装，具体用法请参考  [LVGL文档](https://docs.lvgl.io/9.0/API/index.html)
     *
     * @module lv
     * @class Menu
     * @method separatorCreate
     *
     */
    JSValue Menu::separatorCreate(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        THIS_NCLASS(Obj,thisobj)
        lv_obj_t * retval = lv_menu_separator_create( thisobj->lvobj() ) ;
        JSValue jsretval = retval? be::lv::Obj::wrap(ctx, (lv_obj_t*)retval)->jsobj: JS_NULL ;
        return jsretval ;
    }
    #endif

    #ifndef METHOD_Menu_setLoadPageEvent
    /**
     * > 该方法是对 LVGL C API `lv_menu_set_load_page_event` 的封装，具体用法请参考  [LVGL文档](https://docs.lvgl.io/9.0/API/index.html)
     *
     * @module lv
     * @class Menu
     * @method setLoadPageEvent
     *
     */
    JSValue Menu::setLoadPageEvent(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        THIS_NCLASS(Obj,thisobj)
        ASSERT_ARGC(2)
        JSVALUE_TO_LVOBJ(argv[0],obj)
        JSVALUE_TO_LVOBJ(argv[1],page)
        lv_menu_set_load_page_event( thisobj->lvobj(), obj, page ) ;
        return JS_UNDEFINED ;
    }
    #endif

    #ifndef METHOD_Menu_backButtonIsRoot
    /**
     * > 该方法是对 LVGL C API `lv_menu_back_button_is_root` 的封装，具体用法请参考  [LVGL文档](https://docs.lvgl.io/9.0/API/index.html)
     *
     * @module lv
     * @class Menu
     * @method backButtonIsRoot
     *
     */
    JSValue Menu::backButtonIsRoot(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        THIS_NCLASS(Obj,thisobj)
        ASSERT_ARGC(1)
        JSVALUE_TO_LVOBJ(argv[0],obj)
        bool retval = lv_menu_back_button_is_root( thisobj->lvobj(), obj ) ;
        JSValue jsretval = JS_NewBool(ctx, retval) ;
        return jsretval ;
    }
    #endif

    #ifndef METHOD_Menu_clearHistory
    /**
     * > 该方法是对 LVGL C API `lv_menu_clear_history` 的封装，具体用法请参考  [LVGL文档](https://docs.lvgl.io/9.0/API/index.html)
     *
     * @module lv
     * @class Menu
     * @method clearHistory
     *
     */
    JSValue Menu::clearHistory(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        THIS_NCLASS(Obj,thisobj)
        lv_menu_clear_history( thisobj->lvobj() ) ;
        return JS_UNDEFINED ;
    }
    #endif
// AUTO GENERATE CODE END [METHODS] --------

}
