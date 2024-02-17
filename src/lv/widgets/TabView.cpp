#include "TabView.hpp"

namespace be::lv {
    DEFINE_NCLASS_META(TabView, Obj)
    std::vector<JSCFunctionListEntry> TabView::methods = {
// AUTO GENERATE CODE START [GETSET LIST] --------
        JS_CGETSET_DEF("tabBarPosition",be::lv::Obj::invalidGetter,TabView::setTabBarPosition) ,
        JS_CGETSET_DEF("tabBarSize",be::lv::Obj::invalidGetter,TabView::setTabBarSize) ,
        JS_CGETSET_DEF("tabCount",TabView::getTabCount,be::lv::Obj::invalidSetter) ,
        JS_CGETSET_DEF("tabActive",TabView::getTabActive,be::lv::Obj::invalidSetter) ,
        JS_CGETSET_DEF("content",TabView::getContent,be::lv::Obj::invalidSetter) ,
        JS_CGETSET_DEF("tabBar",TabView::getTabBar,be::lv::Obj::invalidSetter) ,

// AUTO GENERATE CODE END [GETSET LIST] --------
// AUTO GENERATE CODE START [METHOD LIST] --------
        JS_CFUNC_DEF("addTab", 1, TabView::jsAddTab),
        JS_CFUNC_DEF("renameTab", 2, TabView::jsRenameTab),

// AUTO GENERATE CODE END [METHOD LIST] --------
    } ;

    TabView::TabView(JSContext * ctx, JSValue jsobj, lv_obj_t * lvobj)
        : Obj(ctx, TabView::build(ctx,jsobj), lvobj)
    {}

    TabView::TabView(JSContext * ctx, lv_obj_t * parent)
        : TabView(ctx, JS_NULL, lv_tabview_create(parent))
    {}
        
    JSValue TabView::constructor(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        lv_obj_t * lvparent = nullptr ;
        if(argc>0) {
            JSVALUE_TO_LVOBJ_VAR(argv[0], lvparent)
        }
        TabView * widget = new TabView(ctx,lvparent) ;
        return widget->jsobj ;
    }

// AUTO GENERATE CODE START [GETSETS] --------
    JSValue TabView::setTabBarPosition(JSContext *ctx, JSValueConst this_val, JSValueConst val){
        THIS_NCLASS(TabView,thisobj)
        // argv tabBarPosition
        const char * cstr_val = JS_ToCString(ctx, val) ;
        lv_dir_t tabBarPosition;
        if(lv_dir_str_to_const(cstr_val,&tabBarPosition)) {
            JS_ThrowReferenceError(ctx,"unknow %s value: %s","lv_dir_t",cstr_val) ;
            JS_FreeCString(ctx, cstr_val) ;
            return JS_EXCEPTION ;
        }
        JS_FreeCString(ctx, cstr_val) ;
        lv_tabview_set_tab_bar_position(thisobj->lvobj(), tabBarPosition) ;
        return JS_UNDEFINED ;
    }
    JSValue TabView::setTabBarSize(JSContext *ctx, JSValueConst this_val, JSValueConst val){
        THIS_NCLASS(TabView,thisobj)
        int32_t tabBarSize ;
        if(JS_ToInt32(ctx, (int32_t *) &tabBarSize, val)!=0){
            JSTHROW("arg %s of method %s.%s() must be a %s","tabBarSize","TabView","setTabBarSize","number")
        }
        lv_tabview_set_tab_bar_size(thisobj->lvobj(), tabBarSize) ;
        return JS_UNDEFINED ;
    }
    JSValue TabView::getTabCount(JSContext *ctx, JSValueConst this_val){
        THIS_NCLASS(TabView,thisobj)
        uint32_t value = lv_tabview_get_tab_count(thisobj->lvobj()) ;
        JSValue retval = JS_NewUint32(ctx, value) ;
        return retval ;
    }
    JSValue TabView::getTabActive(JSContext *ctx, JSValueConst this_val){
        THIS_NCLASS(TabView,thisobj)
        uint32_t value = lv_tabview_get_tab_active(thisobj->lvobj()) ;
        JSValue retval = JS_NewUint32(ctx, value) ;
        return retval ;
    }
    JSValue TabView::getContent(JSContext *ctx, JSValueConst this_val){
        THIS_NCLASS(TabView,thisobj)
        lv_obj_t * value = lv_tabview_get_content(thisobj->lvobj()) ;
        JSValue retval = value? be::lv::Obj::wrap(ctx, (lv_obj_t*)value)->jsobj: JS_NULL ;
        return retval ;
    }
    JSValue TabView::getTabBar(JSContext *ctx, JSValueConst this_val){
        THIS_NCLASS(TabView,thisobj)
        lv_obj_t * value = lv_tabview_get_tab_bar(thisobj->lvobj()) ;
        JSValue retval = value? be::lv::Obj::wrap(ctx, (lv_obj_t*)value)->jsobj: JS_NULL ;
        return retval ;
    }

// AUTO GENERATE CODE END [GETSETS] --------

// AUTO GENERATE CODE START [METHODS] --------
        JSValue TabView::jsAddTab(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
            THIS_NCLASS(Obj,thisobj)
            CHECK_ARGC(1)
            char * name = (char *)JS_ToCString(ctx, argv[0]) ;
            lv_obj_t * retval = lv_tabview_add_tab( thisobj->lvobj(), name ) ;
            JSValue jsretval = retval? be::lv::Obj::wrap(ctx, (lv_obj_t*)retval)->jsobj: JS_NULL ;
            return jsretval ;
        }

        JSValue TabView::jsRenameTab(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
            THIS_NCLASS(Obj,thisobj)
            CHECK_ARGC(2)
            uint32_t idx ;
            if(JS_ToUint32(ctx, (uint32_t *) &idx, argv[0])!=0){
                JSTHROW("arg %s of method %s.%s() must be a %s","idx","TabView","renameTab","number")
            }
            char * new_name = (char *)JS_ToCString(ctx, argv[1]) ;
            lv_tabview_rename_tab( thisobj->lvobj(), idx, new_name ) ;
            return JS_UNDEFINED ;
        }

// AUTO GENERATE CODE END [METHODS] --------

}
