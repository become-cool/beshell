#include "Win.hpp"

namespace be::lv {
    DEFINE_NCLASS_META(Win, Obj)
    std::vector<JSCFunctionListEntry> Win::methods = {
// AUTO GENERATE CODE START [GETSET LIST] --------
        JS_CGETSET_DEF("header",Win::getHeader,be::lv::Obj::invalidSetter) ,
        JS_CGETSET_DEF("content",Win::getContent,be::lv::Obj::invalidSetter) ,
// AUTO GENERATE CODE END [GETSET LIST] --------
// AUTO GENERATE CODE START [METHOD LIST] --------
        JS_CFUNC_DEF("addTitle", 1, Win::jsAddTitle),
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
        
    JSValue Win::constructor(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        lv_obj_t * lvparent = nullptr ;
        if(argc>0) {
            JSVALUE_TO_LVOBJ_VAR(argv[0], lvparent)
        }
        Win * widget = new Win(ctx,lvparent) ;
        return widget->jsobj ;
    }

// AUTO GENERATE CODE START [GETSETS] --------
    JSValue Win::getHeader(JSContext *ctx, JSValueConst this_val){
        THIS_NCLASS(Win,thisobj)
        lv_obj_t * value = lv_win_get_header(thisobj->lvobj()) ;
        JSValue retval = value? be::lv::Obj::wrap(ctx, (lv_obj_t*)value)->jsobj: JS_NULL ;
        return retval ;
    }
    JSValue Win::getContent(JSContext *ctx, JSValueConst this_val){
        THIS_NCLASS(Win,thisobj)
        lv_obj_t * value = lv_win_get_content(thisobj->lvobj()) ;
        JSValue retval = value? be::lv::Obj::wrap(ctx, (lv_obj_t*)value)->jsobj: JS_NULL ;
        return retval ;
    }
// AUTO GENERATE CODE END [GETSETS] --------

// AUTO GENERATE CODE START [METHODS] --------
        JSValue Win::jsAddTitle(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
            THIS_NCLASS(Obj,thisobj)
            CHECK_ARGC(1)
            char * txt = (char *)JS_ToCString(ctx, argv[0]) ;
            lv_obj_t * retval = lv_win_add_title( thisobj->lvobj(), txt ) ;
            JSValue jsretval = retval? be::lv::Obj::wrap(ctx, (lv_obj_t*)retval)->jsobj: JS_NULL ;
            return jsretval ;
        }

        // Unsupported arg type: const void *
        // lv_obj_t * lv_win_add_button(lv_obj_t * win, const void * icon, int32_t btn_w)
// AUTO GENERATE CODE END [METHODS] --------

}
