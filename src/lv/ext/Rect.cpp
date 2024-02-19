#include "Rect.hpp"

using namespace std ;

namespace be::lv {

    DEFINE_NCLASS_META(Rect, Obj)

    std::vector<JSCFunctionListEntry> Rect::methods = {
        JS_CFUNC_DEF("method", 0, Rect::jsMethod),
    } ;

    Rect::Rect(JSContext * ctx, JSValue jsobj, lv_obj_t * lvobj)
        : Obj(ctx, Rect::build(ctx,jsobj), lvobj)
    {
        
        // this.removeStyleAll()
        // this.setStyle("pad-top", 0)
        // this.setStyle("pad-bottom", 0)
        // this.setStyle("pad-left", 0)
        // this.setStyle("pad-right", 0)
        // this.setStyle("border-width", 0)
        // this.setStyle("radius", 0)
        // this.setStyle("bg-opa", 0)
        // this.clearFlag("scrollable")

    }

    Rect::Rect(JSContext * ctx, lv_obj_t * parent)
        : Rect(ctx, JS_NULL, lv_obj_create(parent))
    {}

    JSValue Rect::constructor(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        lv_obj_t * lvparent = nullptr ;
        if(argc>0) {
            JSVALUE_TO_LVOBJ_VAR(argv[0], lvparent)
        }
        Rect * widget = new Rect(ctx,lvparent) ;
        return widget->jsobj ;
    }

    JSValue Rect::jsMethod(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        return JS_UNDEFINED ;
    }

}