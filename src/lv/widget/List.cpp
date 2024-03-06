#include "List.hpp"

namespace be::lv {
    DEFINE_NCLASS_META(List, Obj)
    std::vector<JSCFunctionListEntry> List::methods = {
// AUTO GENERATE CODE START [GETSET LIST] --------

// AUTO GENERATE CODE END [GETSET LIST] --------
// AUTO GENERATE CODE START [METHOD LIST] --------
        JS_CFUNC_DEF("addText", 1, List::addText),
        JS_CFUNC_DEF("setButtonText", 2, List::setButtonText),
        // Unsupported arg type:
        // lv_obj_t * lv_list_add_button(lv_obj_t * list, const void * icon, const char * txt)
        // const char * lv_list_get_button_text(lv_obj_t * list, lv_obj_t * btn)
// AUTO GENERATE CODE END [METHOD LIST] --------
    } ;

    List::List(JSContext * ctx, JSValue jsobj, lv_obj_t * lvobj)
        : Obj(ctx, List::build(ctx,jsobj), lvobj)
    {}

    List::List(JSContext * ctx, lv_obj_t * parent)
        : List(ctx, JS_NULL, lv_list_create(parent))
    {}

    JSValue List::constructor(JSContext *ctx, JSValueConst ctor, int argc, JSValueConst *argv) {
        lv_obj_t * lvparent = nullptr ;
        if(argc>0) {
            JSVALUE_TO_LVOBJ_VAR(argv[0], lvparent)
        }
        JSValue obj = newObject(ctx, ctor) ;
        if( JS_IsException(obj) ) {
            return obj ;
        }
        List * widget = new List(ctx, obj, lv_list_create(lvparent)) ;
        return widget->jsobj ;
    }

// AUTO GENERATE CODE START [GETSETS] --------

// AUTO GENERATE CODE END [GETSETS] --------

// AUTO GENERATE CODE START [METHODS] --------
    #ifndef METHOD_List_addText
    JSValue List::addText(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        THIS_NCLASS(Obj,thisobj)
        CHECK_ARGC(1)
        char * txt = (char *)JS_ToCString(ctx, argv[0]) ;
        lv_obj_t * retval = lv_list_add_text( thisobj->lvobj(), txt ) ;
            JS_FreeCString(ctx, txt) ;
        JSValue jsretval = retval? be::lv::Obj::wrap(ctx, (lv_obj_t*)retval)->jsobj: JS_NULL ;
        return jsretval ;
    }
    #endif

    // Unsupported arg type: const void *
    // lv_obj_t * lv_list_add_button(lv_obj_t * list, const void * icon, const char * txt)

        // Unsupported return type: const char *
        // const char * lv_list_get_button_text(lv_obj_t * list, lv_obj_t * btn)

    #ifndef METHOD_List_setButtonText
    JSValue List::setButtonText(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        THIS_NCLASS(Obj,thisobj)
        CHECK_ARGC(2)
        JSVALUE_TO_LVOBJ(argv[0],btn)
        char * txt = (char *)JS_ToCString(ctx, argv[1]) ;
        lv_list_set_button_text( thisobj->lvobj(), btn, txt ) ;
        return JS_UNDEFINED ;
    }
    #endif
// AUTO GENERATE CODE END [METHODS] --------

}
