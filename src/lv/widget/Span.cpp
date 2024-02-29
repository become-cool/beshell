#include "Span.hpp"

namespace be::lv {
    DEFINE_NCLASS_META(Span, Obj)
    std::vector<JSCFunctionListEntry> Span::methods = {
// AUTO GENERATE CODE START [GETSET LIST] --------
        JS_CGETSET_DEF("align",Span::getAlign,Span::setAlign) ,
        JS_CGETSET_DEF("overflow",Span::getOverflow,Span::setOverflow) ,
        JS_CGETSET_DEF("indent",Span::getIndent,Span::setIndent) ,
        JS_CGETSET_DEF("mode",Span::getMode,Span::setMode) ,
        JS_CGETSET_DEF("maxLines",Span::getMaxLines,Span::setMaxLines) ,
        JS_CGETSET_DEF("spanCount",Span::getSpanCount,be::lv::Obj::invalidSetter) ,
        JS_CGETSET_DEF("maxLineHeight",Span::getMaxLineHeight,be::lv::Obj::invalidSetter) ,

// AUTO GENERATE CODE END [GETSET LIST] --------
// AUTO GENERATE CODE START [METHOD LIST] --------
        JS_CFUNC_DEF("newSpan", 0, Span::newSpan),
        JS_CFUNC_DEF("getChild", 1, Span::getChild),
        JS_CFUNC_DEF("getExpandWidth", 1, Span::getExpandWidth),
        JS_CFUNC_DEF("getExpandHeight", 1, Span::getExpandHeight),
        JS_CFUNC_DEF("refrMode", 0, Span::refrMode),
        // Unsupported arg type:
        // void lv_spangroup_delete_span(lv_obj_t * obj, lv_span_t * span)

// AUTO GENERATE CODE END [METHOD LIST] --------
    } ;

    Span::Span(JSContext * ctx, JSValue jsobj, lv_obj_t * lvobj)
        : Obj(ctx, Span::build(ctx,jsobj), lvobj)
    {}

    Span::Span(JSContext * ctx, lv_obj_t * parent)
        : Span(ctx, JS_NULL, lv_spangroup_create(parent))
    {}

    JSValue Span::constructor(JSContext *ctx, JSValueConst ctor, int argc, JSValueConst *argv) {
        lv_obj_t * lvparent = nullptr ;
        if(argc>0) {
            JSVALUE_TO_LVOBJ_VAR(argv[0], lvparent)
        }
        JSValue obj = newObject(ctx, ctor) ;
        if( JS_IsException(obj) ) {
            return obj ;
        }
        Span * widget = new Span(ctx, obj, lv_spangroup_create(lvparent)) ;
        return widget->jsobj ;
    }

// AUTO GENERATE CODE START [GETSETS] --------
    #ifndef GETTER_Span_Align
    JSValue Span::getAlign(JSContext *ctx, JSValueConst this_val){
        THIS_NCLASS(Span,thisobj)
        lv_text_align_t value = lv_spangroup_get_align(thisobj->lvobj()) ;
        JSValue retval = JS_NewUint32(ctx, value) ;
        return retval ;
    }
    #endif
    #ifndef SETTER_Span_Align
    JSValue Span::setAlign(JSContext *ctx, JSValueConst this_val, JSValueConst val){
        THIS_NCLASS(Span,thisobj)
        // argv align
        const char * cstr_val = JS_ToCString(ctx, val) ;
        lv_text_align_t align;
        if(!lv_text_align_str_to_const(cstr_val,&align)) {
            JS_ThrowReferenceError(ctx,"unknow %s value: %s","lv_text_align_t",cstr_val) ;
            JS_FreeCString(ctx, cstr_val) ;
            return JS_EXCEPTION ;
        }
        JS_FreeCString(ctx, cstr_val) ;
        lv_spangroup_set_align(thisobj->lvobj(), align) ;
        return JS_UNDEFINED ;
    }
    #endif
    #ifndef GETTER_Span_Overflow
    JSValue Span::getOverflow(JSContext *ctx, JSValueConst this_val){
        THIS_NCLASS(Span,thisobj)
        lv_span_overflow_t value = lv_spangroup_get_overflow(thisobj->lvobj()) ;
        JSValue retval = lv_span_overflow_const_to_jsstr(ctx, value) ;
        return retval ;
    }
    #endif
    #ifndef SETTER_Span_Overflow
    JSValue Span::setOverflow(JSContext *ctx, JSValueConst this_val, JSValueConst val){
        THIS_NCLASS(Span,thisobj)
        // argv overflow
        const char * cstr_val = JS_ToCString(ctx, val) ;
        lv_span_overflow_t overflow;
        if(!lv_span_overflow_str_to_const(cstr_val,&overflow)) {
            JS_ThrowReferenceError(ctx,"unknow %s value: %s","lv_span_overflow_t",cstr_val) ;
            JS_FreeCString(ctx, cstr_val) ;
            return JS_EXCEPTION ;
        }
        JS_FreeCString(ctx, cstr_val) ;
        lv_spangroup_set_overflow(thisobj->lvobj(), overflow) ;
        return JS_UNDEFINED ;
    }
    #endif
    #ifndef GETTER_Span_Indent
    JSValue Span::getIndent(JSContext *ctx, JSValueConst this_val){
        THIS_NCLASS(Span,thisobj)
        int32_t value = lv_spangroup_get_indent(thisobj->lvobj()) ;
        JSValue retval = JS_NewInt32(ctx, value) ;
        return retval ;
    }
    #endif
    #ifndef SETTER_Span_Indent
    JSValue Span::setIndent(JSContext *ctx, JSValueConst this_val, JSValueConst val){
        THIS_NCLASS(Span,thisobj)
        int32_t indent ;
        if(JS_ToInt32(ctx, (int32_t *) &indent, val)!=0){
            JSTHROW("arg %s of method %s.%s() must be a %s","indent","Span","setIndent","number")
        }
        lv_spangroup_set_indent(thisobj->lvobj(), indent) ;
        return JS_UNDEFINED ;
    }
    #endif
    #ifndef GETTER_Span_Mode
    JSValue Span::getMode(JSContext *ctx, JSValueConst this_val){
        THIS_NCLASS(Span,thisobj)
        lv_span_mode_t value = lv_spangroup_get_mode(thisobj->lvobj()) ;
        JSValue retval = lv_span_mode_const_to_jsstr(ctx, value) ;
        return retval ;
    }
    #endif
    #ifndef SETTER_Span_Mode
    JSValue Span::setMode(JSContext *ctx, JSValueConst this_val, JSValueConst val){
        THIS_NCLASS(Span,thisobj)
        // argv mode
        const char * cstr_val = JS_ToCString(ctx, val) ;
        lv_span_mode_t mode;
        if(!lv_span_mode_str_to_const(cstr_val,&mode)) {
            JS_ThrowReferenceError(ctx,"unknow %s value: %s","lv_span_mode_t",cstr_val) ;
            JS_FreeCString(ctx, cstr_val) ;
            return JS_EXCEPTION ;
        }
        JS_FreeCString(ctx, cstr_val) ;
        lv_spangroup_set_mode(thisobj->lvobj(), mode) ;
        return JS_UNDEFINED ;
    }
    #endif
    #ifndef GETTER_Span_MaxLines
    JSValue Span::getMaxLines(JSContext *ctx, JSValueConst this_val){
        THIS_NCLASS(Span,thisobj)
        int32_t value = lv_spangroup_get_max_lines(thisobj->lvobj()) ;
        JSValue retval = JS_NewInt32(ctx, value) ;
        return retval ;
    }
    #endif
    #ifndef SETTER_Span_MaxLines
    JSValue Span::setMaxLines(JSContext *ctx, JSValueConst this_val, JSValueConst val){
        THIS_NCLASS(Span,thisobj)
        int32_t maxLines ;
        if(JS_ToInt32(ctx, (int32_t *) &maxLines, val)!=0){
            JSTHROW("arg %s of method %s.%s() must be a %s","maxLines","Span","setMaxLines","number")
        }
        lv_spangroup_set_max_lines(thisobj->lvobj(), maxLines) ;
        return JS_UNDEFINED ;
    }
    #endif
    #ifndef GETTER_Span_SpanCount
    JSValue Span::getSpanCount(JSContext *ctx, JSValueConst this_val){
        THIS_NCLASS(Span,thisobj)
        uint32_t value = lv_spangroup_get_span_count(thisobj->lvobj()) ;
        JSValue retval = JS_NewUint32(ctx, value) ;
        return retval ;
    }
    #endif
    #ifndef GETTER_Span_MaxLineHeight
    JSValue Span::getMaxLineHeight(JSContext *ctx, JSValueConst this_val){
        THIS_NCLASS(Span,thisobj)
        int32_t value = lv_spangroup_get_max_line_height(thisobj->lvobj()) ;
        JSValue retval = JS_NewInt32(ctx, value) ;
        return retval ;
    }
    #endif

// AUTO GENERATE CODE END [GETSETS] --------

// AUTO GENERATE CODE START [METHODS] --------
    #ifndef METHOD_Span_newSpan
    JSValue Span::newSpan(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        THIS_NCLASS(Obj,thisobj)
        lv_span_t * retval = lv_spangroup_new_span( thisobj->lvobj() ) ;
        JSValue jsretval = retval? be::lv::Obj::wrap(ctx, (lv_obj_t*)retval)->jsobj: JS_NULL ;
        return jsretval ;
    }
    #endif

    // Unsupported arg type: lv_span_t *
    // void lv_spangroup_delete_span(lv_obj_t * obj, lv_span_t * span)

    #ifndef METHOD_Span_getChild
    JSValue Span::getChild(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        THIS_NCLASS(Obj,thisobj)
        CHECK_ARGC(1)
        int32_t id ;
        if(JS_ToInt32(ctx, (int32_t *) &id, argv[0])!=0){
            JSTHROW("arg %s of method %s.%s() must be a %s","id","Span","getChild","number")
        }
        lv_span_t * retval = lv_spangroup_get_child( thisobj->lvobj(), id ) ;
        JSValue jsretval = retval? be::lv::Obj::wrap(ctx, (lv_obj_t*)retval)->jsobj: JS_NULL ;
        return jsretval ;
    }
    #endif

    #ifndef METHOD_Span_getExpandWidth
    JSValue Span::getExpandWidth(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        THIS_NCLASS(Obj,thisobj)
        CHECK_ARGC(1)
        uint32_t max_width ;
        if(JS_ToUint32(ctx, (uint32_t *) &max_width, argv[0])!=0){
            JSTHROW("arg %s of method %s.%s() must be a %s","max_width","Span","getExpandWidth","number")
        }
        uint32_t retval = lv_spangroup_get_expand_width( thisobj->lvobj(), max_width ) ;
        JSValue jsretval = JS_NewUint32(ctx, retval) ;
        return jsretval ;
    }
    #endif

    #ifndef METHOD_Span_getExpandHeight
    JSValue Span::getExpandHeight(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        THIS_NCLASS(Obj,thisobj)
        CHECK_ARGC(1)
        int32_t width ;
        if(JS_ToInt32(ctx, (int32_t *) &width, argv[0])!=0){
            JSTHROW("arg %s of method %s.%s() must be a %s","width","Span","getExpandHeight","number")
        }
        int32_t retval = lv_spangroup_get_expand_height( thisobj->lvobj(), width ) ;
        JSValue jsretval = JS_NewInt32(ctx, retval) ;
        return jsretval ;
    }
    #endif

    #ifndef METHOD_Span_refrMode
    JSValue Span::refrMode(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        THIS_NCLASS(Obj,thisobj)
        lv_spangroup_refr_mode( thisobj->lvobj() ) ;
        return JS_UNDEFINED ;
    }
    #endif

// AUTO GENERATE CODE END [METHODS] --------

}
