#include "Win.hpp"

namespace be::lv {
    DEFINE_NCLASS_META(Win, Obj)
    std::vector<JSCFunctionListEntry> Win::methods = {
// AUTO GENERATE CODE START [GETSET LIST] --------
        JS_CGETSET_DEF("header",Win::getHeader,be::lv::Obj::invalidSetter) ,
        JS_CGETSET_DEF("content",Win::getContent,be::lv::Obj::invalidSetter) ,
// AUTO GENERATE CODE END [GETSET LIST] --------
// AUTO GENERATE CODE START [METHOD LIST] --------
        JS_CFUNC_DEF("addTitle", 1, Win::addTitle),
        // Unsupported arg type:
        // lv_obj_t * lv_win_add_button(lv_obj_t * win, const void * icon, int32_t btn_w)
// AUTO GENERATE CODE END [METHOD LIST] --------
    } ;

    Win::Win(JSContext * ctx, JSValue jsobj, lv_obj_t * lvobj)
        : Obj(ctx, Win::build(ctx,jsobj), lvobj)
    {}

    Win::Win(JSContext * ctx, lv_obj_t * parent)
        : Win(ctx, JS_NULL, lv_win_create(parent))
    {}

    JSValue Win::constructor(JSContext *ctx, JSValueConst ctor, int argc, JSValueConst *argv) {
        lv_obj_t * lvparent = nullptr ;
        if(argc>0) {
            JSVALUE_TO_LVOBJ_VAR(argv[0], lvparent)
        }
        JSValue obj = newObject(ctx, ctor) ;
        if( JS_IsException(obj) ) {
            return obj ;
        }
        Win * widget = new Win(ctx, obj, lv_win_create(lvparent)) ;
        return widget->jsobj ;
    }

// AUTO GENERATE CODE START [GETSETS] --------
    #ifndef GETTER_Win_Header
    JSValue Win::getHeader(JSContext *ctx, JSValueConst this_val){
        THIS_NCLASS(Win,thisobj)
        lv_obj_t * value = lv_win_get_header(thisobj->lvobj()) ;
        JSValue retval = value? be::lv::Obj::wrap(ctx, (lv_obj_t*)value)->jsobj: JS_NULL ;
        return retval ;
    }
    #endif
    #ifndef GETTER_Win_Content
    JSValue Win::getContent(JSContext *ctx, JSValueConst this_val){
        THIS_NCLASS(Win,thisobj)
        lv_obj_t * value = lv_win_get_content(thisobj->lvobj()) ;
        JSValue retval = value? be::lv::Obj::wrap(ctx, (lv_obj_t*)value)->jsobj: JS_NULL ;
        return retval ;
    }
    #endif
// AUTO GENERATE CODE END [GETSETS] --------

// AUTO GENERATE CODE START [METHODS] --------
    #ifndef METHOD_Win_addTitle
    JSValue Win::addTitle(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        THIS_NCLASS(Obj,thisobj)
        ASSERT_ARGC(1)
        char * txt = (char *)JS_ToCString(ctx, argv[0]) ;
        lv_obj_t * retval = lv_win_add_title( thisobj->lvobj(), txt ) ;
            JS_FreeCString(ctx, txt) ;
        JSValue jsretval = retval? be::lv::Obj::wrap(ctx, (lv_obj_t*)retval)->jsobj: JS_NULL ;
        return jsretval ;
    }
    #endif

    // Unsupported arg type: const void *
    // lv_obj_t * lv_win_add_button(lv_obj_t * win, const void * icon, int32_t btn_w)
// AUTO GENERATE CODE END [METHODS] --------

}
