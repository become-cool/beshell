#include "Img.hpp"

namespace be::lv {
    DEFINE_NCLASS_META(Img, Obj)
    std::vector<JSCFunctionListEntry> Img::methods = {
        JS_CGETSET_DEF("src",Img::getSrc,Img::setSrc) ,
// AUTO GENERATE CODE START [GETSET LIST] --------
        JS_CGETSET_DEF("offsetX",Img::getOffsetX,Img::setOffsetX) ,
        JS_CGETSET_DEF("offsetY",Img::getOffsetY,Img::setOffsetY) ,
        JS_CGETSET_DEF("rotation",Img::getRotation,Img::setRotation) ,
        JS_CGETSET_DEF("scale",Img::getScale,Img::setScale) ,
        JS_CGETSET_DEF("scaleX",Img::getScaleX,Img::setScaleX) ,
        JS_CGETSET_DEF("scaleY",Img::getScaleY,Img::setScaleY) ,
        JS_CGETSET_DEF("blendMode",Img::getBlendMode,Img::setBlendMode) ,
        JS_CGETSET_DEF("antialias",Img::getAntialias,Img::setAntialias) ,
        JS_CGETSET_DEF("align",Img::getAlign,Img::setAlign) ,
// AUTO GENERATE CODE END [GETSET LIST] --------
// AUTO GENERATE CODE START [METHOD LIST] --------
        // Unsupported arg type:
        // lv_image_src_t lv_image_src_get_type(const void * src)
        // void lv_image_buf_set_palette(lv_image_dsc_t * dsc, uint8_t id, lv_color32_t c)
        // void lv_image_buf_free(lv_image_dsc_t * dsc)
        // lv_result_t lv_image_decoder_get_info(const void * src, lv_image_header_t * header)
        // lv_result_t lv_image_decoder_open(lv_image_decoder_dsc_t * dsc, const void * src, const lv_image_decoder_args_t * args)
        // lv_result_t lv_image_decoder_get_area(lv_image_decoder_dsc_t * dsc, const lv_area_t * full_area, lv_area_t * decoded_area)
        // void lv_image_decoder_close(lv_image_decoder_dsc_t * dsc)
        // void lv_image_decoder_delete(lv_image_decoder_t * decoder)
        // lv_image_decoder_t * lv_image_decoder_get_next(lv_image_decoder_t * decoder)
        // void lv_image_decoder_set_info_cb(lv_image_decoder_t * decoder, lv_image_decoder_info_f_t info_cb)
        // void lv_image_decoder_set_open_cb(lv_image_decoder_t * decoder, lv_image_decoder_open_f_t open_cb)
        // void lv_image_decoder_set_get_area_cb(lv_image_decoder_t * decoder, lv_image_decoder_get_area_cb_t read_line_cb)
        // void lv_image_decoder_set_close_cb(lv_image_decoder_t * decoder, lv_image_decoder_close_f_t close_cb)
        // void lv_image_decoder_set_cache_free_cb(lv_image_decoder_t * decoder, lv_cache_free_cb_t cache_free_cb)
        // lv_cache_entry_t * lv_image_decoder_add_to_cache(lv_image_decoder_t * decoder, lv_image_cache_data_t * search_key, const lv_draw_buf_t * decoded, void * user_data)
        // lv_draw_buf_t * lv_image_decoder_post_process(lv_image_decoder_dsc_t * dsc, lv_draw_buf_t * decoded)
        // void lv_image_cache_drop(const void * src)
// AUTO GENERATE CODE END [METHOD LIST] --------
    } ;

    Img::Img(JSContext * ctx, JSValue jsobj, lv_obj_t * lvobj)
        : Obj(ctx, Img::build(ctx,jsobj), lvobj)
    {}

    Img::Img(JSContext * ctx, lv_obj_t * parent)
        : Img(ctx, JS_NULL, lv_image_create(parent))
    {}
        
    JSValue Img::constructor(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        lv_obj_t * lvparent = nullptr ;
        if(argc>0) {
            JSVALUE_TO_LVOBJ_VAR(argv[0], lvparent)
        }
        Img * widget = new Img(ctx,lvparent) ;
        return widget->jsobj ;
    }


    JSValue Img::getSrc(JSContext *ctx, JSValueConst this_val){
        THIS_NCLASS(Img,thisobj)
        const char * value = (char *)lv_image_get_src(thisobj->lvobj()) ;
        return value? JS_NewString(ctx, value): JS_NULL ;
    }
    JSValue Img::setSrc(JSContext *ctx, JSValueConst this_val, JSValueConst value){
        THIS_NCLASS(Img,thisobj)
        const char * src = (char *)JS_ToCString(ctx, value) ;
        lv_image_set_src(thisobj->lvobj(), src) ;
        JS_FreeCString(ctx, src) ;
        return JS_UNDEFINED ;
    }



// AUTO GENERATE CODE START [GETSETS] --------
    // unspported type: const void *
    // JSValue Img::getSrc(JSContext *ctx, JSValueConst this_val){}
    // const void * lv_image_get_src(lv_obj_t * obj)
    // unspported type: const void *
    // JSValue Img::setSrc(JSContext *ctx, JSValueConst this_val, JSValueConst value){}
    // void lv_image_set_src(lv_obj_t * obj, const void * src)
    JSValue Img::getOffsetX(JSContext *ctx, JSValueConst this_val){
        THIS_NCLASS(Img,thisobj)
        int32_t value = lv_image_get_offset_x(thisobj->lvobj()) ;
        JSValue retval = JS_NewInt32(ctx, value) ;
        return retval ;
    }
    JSValue Img::setOffsetX(JSContext *ctx, JSValueConst this_val, JSValueConst val){
        THIS_NCLASS(Img,thisobj)
        int32_t offsetX ;
        if(JS_ToInt32(ctx, (int32_t *) &offsetX, val)!=0){
            JSTHROW("arg %s of method %s.%s() must be a %s","offsetX","Img","setOffsetX","number")
        }
        lv_image_set_offset_x(thisobj->lvobj(), offsetX) ;
        return JS_UNDEFINED ;
    }
    JSValue Img::getOffsetY(JSContext *ctx, JSValueConst this_val){
        THIS_NCLASS(Img,thisobj)
        int32_t value = lv_image_get_offset_y(thisobj->lvobj()) ;
        JSValue retval = JS_NewInt32(ctx, value) ;
        return retval ;
    }
    JSValue Img::setOffsetY(JSContext *ctx, JSValueConst this_val, JSValueConst val){
        THIS_NCLASS(Img,thisobj)
        int32_t offsetY ;
        if(JS_ToInt32(ctx, (int32_t *) &offsetY, val)!=0){
            JSTHROW("arg %s of method %s.%s() must be a %s","offsetY","Img","setOffsetY","number")
        }
        lv_image_set_offset_y(thisobj->lvobj(), offsetY) ;
        return JS_UNDEFINED ;
    }
    JSValue Img::getRotation(JSContext *ctx, JSValueConst this_val){
        THIS_NCLASS(Img,thisobj)
        int32_t value = lv_image_get_rotation(thisobj->lvobj()) ;
        JSValue retval = JS_NewInt32(ctx, value) ;
        return retval ;
    }
    JSValue Img::setRotation(JSContext *ctx, JSValueConst this_val, JSValueConst val){
        THIS_NCLASS(Img,thisobj)
        int32_t rotation ;
        if(JS_ToInt32(ctx, (int32_t *) &rotation, val)!=0){
            JSTHROW("arg %s of method %s.%s() must be a %s","rotation","Img","setRotation","number")
        }
        lv_image_set_rotation(thisobj->lvobj(), rotation) ;
        return JS_UNDEFINED ;
    }
    JSValue Img::getScale(JSContext *ctx, JSValueConst this_val){
        THIS_NCLASS(Img,thisobj)
        int32_t value = lv_image_get_scale(thisobj->lvobj()) ;
        JSValue retval = JS_NewInt32(ctx, value) ;
        return retval ;
    }
    JSValue Img::setScale(JSContext *ctx, JSValueConst this_val, JSValueConst val){
        THIS_NCLASS(Img,thisobj)
        uint32_t scale ;
        if(JS_ToUint32(ctx, (uint32_t *) &scale, val)!=0){
            JSTHROW("arg %s of method %s.%s() must be a %s","scale","Img","setScale","number")
        }
        lv_image_set_scale(thisobj->lvobj(), scale) ;
        return JS_UNDEFINED ;
    }
    JSValue Img::getScaleX(JSContext *ctx, JSValueConst this_val){
        THIS_NCLASS(Img,thisobj)
        int32_t value = lv_image_get_scale_x(thisobj->lvobj()) ;
        JSValue retval = JS_NewInt32(ctx, value) ;
        return retval ;
    }
    JSValue Img::setScaleX(JSContext *ctx, JSValueConst this_val, JSValueConst val){
        THIS_NCLASS(Img,thisobj)
        uint32_t scaleX ;
        if(JS_ToUint32(ctx, (uint32_t *) &scaleX, val)!=0){
            JSTHROW("arg %s of method %s.%s() must be a %s","scaleX","Img","setScaleX","number")
        }
        lv_image_set_scale_x(thisobj->lvobj(), scaleX) ;
        return JS_UNDEFINED ;
    }
    JSValue Img::getScaleY(JSContext *ctx, JSValueConst this_val){
        THIS_NCLASS(Img,thisobj)
        int32_t value = lv_image_get_scale_y(thisobj->lvobj()) ;
        JSValue retval = JS_NewInt32(ctx, value) ;
        return retval ;
    }
    JSValue Img::setScaleY(JSContext *ctx, JSValueConst this_val, JSValueConst val){
        THIS_NCLASS(Img,thisobj)
        uint32_t scaleY ;
        if(JS_ToUint32(ctx, (uint32_t *) &scaleY, val)!=0){
            JSTHROW("arg %s of method %s.%s() must be a %s","scaleY","Img","setScaleY","number")
        }
        lv_image_set_scale_y(thisobj->lvobj(), scaleY) ;
        return JS_UNDEFINED ;
    }
    JSValue Img::getBlendMode(JSContext *ctx, JSValueConst this_val){
        THIS_NCLASS(Img,thisobj)
        lv_blend_mode_t value = lv_image_get_blend_mode(thisobj->lvobj()) ;
        JSValue retval = lv_blend_mode_const_to_jsstr(ctx, value) ;
        return retval ;
    }
    JSValue Img::setBlendMode(JSContext *ctx, JSValueConst this_val, JSValueConst val){
        THIS_NCLASS(Img,thisobj)
        // argv blendMode
        const char * cstr_val = JS_ToCString(ctx, val) ;
        lv_blend_mode_t blendMode;
        if(lv_blend_mode_str_to_const(cstr_val,&blendMode)) {
            JS_ThrowReferenceError(ctx,"unknow %s value: %s","lv_blend_mode_t",cstr_val) ;
            JS_FreeCString(ctx, cstr_val) ;
            return JS_EXCEPTION ;
        }
        JS_FreeCString(ctx, cstr_val) ;
        lv_image_set_blend_mode(thisobj->lvobj(), blendMode) ;
        return JS_UNDEFINED ;
    }
    JSValue Img::getAntialias(JSContext *ctx, JSValueConst this_val){
        THIS_NCLASS(Img,thisobj)
        bool value = lv_image_get_antialias(thisobj->lvobj()) ;
        JSValue retval = JS_NewBool(ctx, value) ;
        return retval ;
    }
    JSValue Img::setAntialias(JSContext *ctx, JSValueConst this_val, JSValueConst val){
        THIS_NCLASS(Img,thisobj)
        bool antialias = JS_ToBool(ctx, val) ;
        lv_image_set_antialias(thisobj->lvobj(), antialias) ;
        return JS_UNDEFINED ;
    }
    JSValue Img::getAlign(JSContext *ctx, JSValueConst this_val){
        THIS_NCLASS(Img,thisobj)
        lv_image_align_t value = lv_image_get_align(thisobj->lvobj()) ;
        JSValue retval = lv_image_align_const_to_jsstr(ctx, value) ;
        return retval ;
    }
    JSValue Img::setAlign(JSContext *ctx, JSValueConst this_val, JSValueConst val){
        THIS_NCLASS(Img,thisobj)
        // argv align
        const char * cstr_val = JS_ToCString(ctx, val) ;
        lv_image_align_t align;
        if(lv_image_align_str_to_const(cstr_val,&align)) {
            JS_ThrowReferenceError(ctx,"unknow %s value: %s","lv_image_align_t",cstr_val) ;
            JS_FreeCString(ctx, cstr_val) ;
            return JS_EXCEPTION ;
        }
        JS_FreeCString(ctx, cstr_val) ;
        lv_image_set_align(thisobj->lvobj(), align) ;
        return JS_UNDEFINED ;
    }
// AUTO GENERATE CODE END [GETSETS] --------

// AUTO GENERATE CODE START [METHODS] --------
        // Unsupported arg type: const void *
        // lv_image_src_t lv_image_src_get_type(const void * src)

        // Unsupported arg type: lv_image_dsc_t *
        // void lv_image_buf_set_palette(lv_image_dsc_t * dsc, uint8_t id, lv_color32_t c)

        // Unsupported arg type: lv_image_dsc_t *
        // void lv_image_buf_free(lv_image_dsc_t * dsc)

        // Unsupported arg type: const void *
        // lv_result_t lv_image_decoder_get_info(const void * src, lv_image_header_t * header)

        // Unsupported arg type: lv_image_decoder_dsc_t *
        // lv_result_t lv_image_decoder_open(lv_image_decoder_dsc_t * dsc, const void * src, const lv_image_decoder_args_t * args)

        // Unsupported arg type: lv_image_decoder_dsc_t *
        // lv_result_t lv_image_decoder_get_area(lv_image_decoder_dsc_t * dsc, const lv_area_t * full_area, lv_area_t * decoded_area)

        // Unsupported arg type: lv_image_decoder_dsc_t *
        // void lv_image_decoder_close(lv_image_decoder_dsc_t * dsc)

        // Unsupported arg type: lv_image_decoder_t *
        // void lv_image_decoder_delete(lv_image_decoder_t * decoder)

        // Unsupported arg type: lv_image_decoder_t *
        // lv_image_decoder_t * lv_image_decoder_get_next(lv_image_decoder_t * decoder)

        // Unsupported arg type: lv_image_decoder_t *
        // void lv_image_decoder_set_info_cb(lv_image_decoder_t * decoder, lv_image_decoder_info_f_t info_cb)

        // Unsupported arg type: lv_image_decoder_t *
        // void lv_image_decoder_set_open_cb(lv_image_decoder_t * decoder, lv_image_decoder_open_f_t open_cb)

        // Unsupported arg type: lv_image_decoder_t *
        // void lv_image_decoder_set_get_area_cb(lv_image_decoder_t * decoder, lv_image_decoder_get_area_cb_t read_line_cb)

        // Unsupported arg type: lv_image_decoder_t *
        // void lv_image_decoder_set_close_cb(lv_image_decoder_t * decoder, lv_image_decoder_close_f_t close_cb)

        // Unsupported arg type: lv_image_decoder_t *
        // void lv_image_decoder_set_cache_free_cb(lv_image_decoder_t * decoder, lv_cache_free_cb_t cache_free_cb)

        // Unsupported arg type: lv_image_decoder_t *
        // lv_cache_entry_t * lv_image_decoder_add_to_cache(lv_image_decoder_t * decoder, lv_image_cache_data_t * search_key, const lv_draw_buf_t * decoded, void * user_data)

        // Unsupported arg type: lv_image_decoder_dsc_t *
        // lv_draw_buf_t * lv_image_decoder_post_process(lv_image_decoder_dsc_t * dsc, lv_draw_buf_t * decoded)

        // Unsupported arg type: const void *
        // void lv_image_cache_drop(const void * src)
// AUTO GENERATE CODE END [METHODS] --------

}
