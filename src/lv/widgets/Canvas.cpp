#include "Canvas.hpp"

namespace be::lv {
    DEFINE_NCLASS_META(Canvas, Obj)
    std::vector<JSCFunctionListEntry> Canvas::methods = {
// AUTO GENERATE CODE START [GETSET LIST] --------


// AUTO GENERATE CODE END [GETSET LIST] --------
// AUTO GENERATE CODE START [METHOD LIST] --------
        JS_CFUNC_DEF("fillBg", 2, Canvas::jsFillBg),
        // Unsupported arg type:
        // void lv_canvas_copy_buf(lv_obj_t * obj, const lv_area_t * canvas_area, lv_draw_buf_t * dest_buf, const lv_area_t * dest_area)
        // void lv_canvas_init_layer(lv_obj_t * canvas, lv_layer_t * layer)
        // void lv_canvas_finish_layer(lv_obj_t * canvas, lv_layer_t * layer)

// AUTO GENERATE CODE END [METHOD LIST] --------
    } ;

    Canvas::Canvas(JSContext * ctx, JSValue jsobj, lv_obj_t * lvobj)
        : Obj(ctx, Canvas::build(ctx,jsobj), lvobj)
    {}

    Canvas::Canvas(JSContext * ctx, lv_obj_t * parent)
        : Canvas(ctx, JS_NULL, lv_canvas_create(parent))
    {}
        
    JSValue Canvas::constructor(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        lv_obj_t * lvparent = nullptr ;
        if(argc>0) {
            JSVALUE_TO_LVOBJ_VAR(argv[0], lvparent)
        }
        Canvas * widget = new Canvas(ctx,lvparent) ;
        return widget->jsobj ;
    }

// AUTO GENERATE CODE START [GETSETS] --------
    // unspported type: lv_draw_buf_t *
    // JSValue Canvas::getDrawBuf(JSContext *ctx, JSValueConst this_val){}
    // lv_draw_buf_t * lv_canvas_get_draw_buf(lv_obj_t * obj)
    // unspported type: lv_draw_buf_t *
    // JSValue Canvas::setDrawBuf(JSContext *ctx, JSValueConst this_val, JSValueConst value){}
    // void lv_canvas_set_draw_buf(lv_obj_t * obj, lv_draw_buf_t * draw_buf)
    // unspported type: lv_image_dsc_t *
    // JSValue Canvas::getImage(JSContext *ctx, JSValueConst this_val){}
    // lv_image_dsc_t * lv_canvas_get_image(lv_obj_t * canvas)
    // unspported type: const void *
    // JSValue Canvas::getBuf(JSContext *ctx, JSValueConst this_val){}
    // const void * lv_canvas_get_buf(lv_obj_t * canvas)

// AUTO GENERATE CODE END [GETSETS] --------

// AUTO GENERATE CODE START [METHODS] --------
        // Unsupported arg type: const lv_area_t *
        // void lv_canvas_copy_buf(lv_obj_t * obj, const lv_area_t * canvas_area, lv_draw_buf_t * dest_buf, const lv_area_t * dest_area)

        JSValue Canvas::jsFillBg(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
            THIS_NCLASS(Obj,thisobj)
            CHECK_ARGC(2)
            lv_color_t color ;
            if(JS_ToUint32(ctx, (uint32_t *) &color, argv[0])!=0){
                JSTHROW("arg %s of method %s.%s() must be a %s","color","Canvas","fillBg","number")
            }
            uint8_t opa ;
            if(JS_ToUint32(ctx, (uint32_t *) &opa, argv[1])!=0){
                JSTHROW("arg %s of method %s.%s() must be a %s","opa","Canvas","fillBg","number")
            }
            lv_canvas_fill_bg( thisobj->lvobj(), color, opa ) ;
            return JS_UNDEFINED ;
        }

        // Unsupported arg type: lv_layer_t *
        // void lv_canvas_init_layer(lv_obj_t * canvas, lv_layer_t * layer)

        // Unsupported arg type: lv_layer_t *
        // void lv_canvas_finish_layer(lv_obj_t * canvas, lv_layer_t * layer)

// AUTO GENERATE CODE END [METHODS] --------

}
