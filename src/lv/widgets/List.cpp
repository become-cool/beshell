#include "List.hpp"

namespace be::lv {
    DEFINE_NCLASS_META(List, Obj)
    std::vector<JSCFunctionListEntry> List::methods = {
// AUTO GENERATE CODE START [GETSET LIST] --------


// AUTO GENERATE CODE END [GETSET LIST] --------
// AUTO GENERATE CODE START [METHOD LIST] --------
        JS_CFUNC_DEF("addText", 1, List::jsAddText),
        // Unsupported arg type:
        // lv_obj_t * lv_list_add_button(lv_obj_t * list, const void * icon, const char * txt)

// AUTO GENERATE CODE END [METHOD LIST] --------
    } ;

    List::List(JSContext * ctx, JSValue jsobj, lv_obj_t * lvobj)
        : Obj(ctx, List::build(ctx,jsobj), lvobj)
    {}

    List::List(JSContext * ctx, lv_obj_t * parent)
        : List(ctx, JS_NULL, lv_list_create(parent))
    {}
        
    JSValue List::constructor(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        lv_obj_t * lvparent = nullptr ;
        if(argc>0) {
            JSVALUE_TO_LVOBJ_VAR(argv[0], lvparent)
        }
        List * widget = new List(ctx,lvparent) ;
        return widget->jsobj ;
    }

// AUTO GENERATE CODE START [GETSETS] --------


// AUTO GENERATE CODE END [GETSETS] --------

// AUTO GENERATE CODE START [METHODS] --------
        JSValue List::jsAddText(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
            THIS_NCLASS(Obj,thisobj)
            CHECK_ARGC(1)
            char * txt = (char *)JS_ToCString(ctx, argv[0]) ;
            lv_obj_t * retval = lv_list_add_text( thisobj->lvobj(), txt ) ;
            JSValue jsretval = retval? be::lv::Obj::wrap(ctx, (lv_obj_t*)retval)->jsobj: JS_NULL ;
            return jsretval ;
        }

        // Unsupported arg type: const void *
        // lv_obj_t * lv_list_add_button(lv_obj_t * list, const void * icon, const char * txt)

// AUTO GENERATE CODE END [METHODS] --------

}
