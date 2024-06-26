#include "MsgBox.hpp"

namespace be::lv {
    DEFINE_NCLASS_META(MsgBox, Obj)
    std::vector<JSCFunctionListEntry> MsgBox::methods = {
// AUTO GENERATE CODE START [GETSET LIST] --------
        JS_CGETSET_DEF("header",MsgBox::getHeader,be::lv::Obj::invalidSetter) ,
        JS_CGETSET_DEF("footer",MsgBox::getFooter,be::lv::Obj::invalidSetter) ,
        JS_CGETSET_DEF("content",MsgBox::getContent,be::lv::Obj::invalidSetter) ,
        JS_CGETSET_DEF("title",MsgBox::getTitle,be::lv::Obj::invalidSetter) ,
// AUTO GENERATE CODE END [GETSET LIST] --------
// AUTO GENERATE CODE START [METHOD LIST] --------
        JS_CFUNC_DEF("addTitle", 1, MsgBox::addTitle),
        JS_CFUNC_DEF("addText", 1, MsgBox::addText),
        JS_CFUNC_DEF("addFooterButton", 1, MsgBox::addFooterButton),
        JS_CFUNC_DEF("addCloseButton", 0, MsgBox::addCloseButton),
        JS_CFUNC_DEF("close", 0, MsgBox::close),
        JS_CFUNC_DEF("closeAsync", 0, MsgBox::closeAsync),
        // Unsupported arg type:
        // lv_obj_t * lv_msgbox_add_header_button(lv_obj_t * obj, const void * icon)
// AUTO GENERATE CODE END [METHOD LIST] --------
    } ;

    MsgBox::MsgBox(JSContext * ctx, JSValue jsobj, lv_obj_t * lvobj)
        : Obj(ctx, MsgBox::build(ctx,jsobj), lvobj)
    {}

    MsgBox::MsgBox(JSContext * ctx, lv_obj_t * parent)
        : MsgBox(ctx, JS_NULL, lv_msgbox_create(parent))
    {}

    JSValue MsgBox::constructor(JSContext *ctx, JSValueConst ctor, int argc, JSValueConst *argv) {
        lv_obj_t * lvparent = nullptr ;
        if(argc>0) {
            JSVALUE_TO_LVOBJ_VAR(argv[0], lvparent)
        }
        JSValue obj = newObject(ctx, ctor) ;
        if( JS_IsException(obj) ) {
            return obj ;
        }
        MsgBox * widget = new MsgBox(ctx, obj, lv_msgbox_create(lvparent)) ;
        return widget->jsobj ;
    }

// AUTO GENERATE CODE START [GETSETS] --------
    /**
     * 访问: 只读
     *
     * > 该属性是对 LVGL C API `lv_msgbox_get_header` 的封装，
     * > 具体用法请参考  [LVGL文档](https://docs.lvgl.io/9.0/API/index.html)
     *
     * @module lv
     * @class MsgBox
     * @property header:[obj](obj.html)
     *
     */
    #ifndef GETTER_MsgBox_Header
    JSValue MsgBox::getHeader(JSContext *ctx, JSValueConst this_val){
        THIS_NCLASS(MsgBox,thisobj)
        lv_obj_t * value = lv_msgbox_get_header(thisobj->lvobj()) ;
        JSValue retval = value? be::lv::Obj::wrap(ctx, (lv_obj_t*)value)->jsobj: JS_NULL ;
        return retval ;
    }
    #endif
    /**
     * 访问: 只读
     *
     * > 该属性是对 LVGL C API `lv_msgbox_get_footer` 的封装，
     * > 具体用法请参考  [LVGL文档](https://docs.lvgl.io/9.0/API/index.html)
     *
     * @module lv
     * @class MsgBox
     * @property footer:[obj](obj.html)
     *
     */
    #ifndef GETTER_MsgBox_Footer
    JSValue MsgBox::getFooter(JSContext *ctx, JSValueConst this_val){
        THIS_NCLASS(MsgBox,thisobj)
        lv_obj_t * value = lv_msgbox_get_footer(thisobj->lvobj()) ;
        JSValue retval = value? be::lv::Obj::wrap(ctx, (lv_obj_t*)value)->jsobj: JS_NULL ;
        return retval ;
    }
    #endif
    /**
     * 访问: 只读
     *
     * > 该属性是对 LVGL C API `lv_msgbox_get_content` 的封装，
     * > 具体用法请参考  [LVGL文档](https://docs.lvgl.io/9.0/API/index.html)
     *
     * @module lv
     * @class MsgBox
     * @property content:[obj](obj.html)
     *
     */
    #ifndef GETTER_MsgBox_Content
    JSValue MsgBox::getContent(JSContext *ctx, JSValueConst this_val){
        THIS_NCLASS(MsgBox,thisobj)
        lv_obj_t * value = lv_msgbox_get_content(thisobj->lvobj()) ;
        JSValue retval = value? be::lv::Obj::wrap(ctx, (lv_obj_t*)value)->jsobj: JS_NULL ;
        return retval ;
    }
    #endif
    /**
     * 访问: 只读
     *
     * > 该属性是对 LVGL C API `lv_msgbox_get_title` 的封装，
     * > 具体用法请参考  [LVGL文档](https://docs.lvgl.io/9.0/API/index.html)
     *
     * @module lv
     * @class MsgBox
     * @property title:[obj](obj.html)
     *
     */
    #ifndef GETTER_MsgBox_Title
    JSValue MsgBox::getTitle(JSContext *ctx, JSValueConst this_val){
        THIS_NCLASS(MsgBox,thisobj)
        lv_obj_t * value = lv_msgbox_get_title(thisobj->lvobj()) ;
        JSValue retval = value? be::lv::Obj::wrap(ctx, (lv_obj_t*)value)->jsobj: JS_NULL ;
        return retval ;
    }
    #endif
// AUTO GENERATE CODE END [GETSETS] --------

// AUTO GENERATE CODE START [METHODS] --------
    #ifndef METHOD_MsgBox_addTitle
    /**
     * > 该方法是对 LVGL C API `lv_msgbox_add_title` 的封装，具体用法请参考  [LVGL文档](https://docs.lvgl.io/9.0/API/index.html)
     *
     * @module lv
     * @class MsgBox
     * @method addTitle
     *
     */
    JSValue MsgBox::addTitle(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        THIS_NCLASS(Obj,thisobj)
        ASSERT_ARGC(1)
        char * title = (char *)JS_ToCString(ctx, argv[0]) ;
        lv_obj_t * retval = lv_msgbox_add_title( thisobj->lvobj(), title ) ;
            JS_FreeCString(ctx, title) ;
        JSValue jsretval = retval? be::lv::Obj::wrap(ctx, (lv_obj_t*)retval)->jsobj: JS_NULL ;
        return jsretval ;
    }
    #endif

    // Unsupported arg type: const void *
    // lv_obj_t * lv_msgbox_add_header_button(lv_obj_t * obj, const void * icon)

    #ifndef METHOD_MsgBox_addText
    /**
     * > 该方法是对 LVGL C API `lv_msgbox_add_text` 的封装，具体用法请参考  [LVGL文档](https://docs.lvgl.io/9.0/API/index.html)
     *
     * @module lv
     * @class MsgBox
     * @method addText
     *
     */
    JSValue MsgBox::addText(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        THIS_NCLASS(Obj,thisobj)
        ASSERT_ARGC(1)
        char * text = (char *)JS_ToCString(ctx, argv[0]) ;
        lv_obj_t * retval = lv_msgbox_add_text( thisobj->lvobj(), text ) ;
            JS_FreeCString(ctx, text) ;
        JSValue jsretval = retval? be::lv::Obj::wrap(ctx, (lv_obj_t*)retval)->jsobj: JS_NULL ;
        return jsretval ;
    }
    #endif

    #ifndef METHOD_MsgBox_addFooterButton
    /**
     * > 该方法是对 LVGL C API `lv_msgbox_add_footer_button` 的封装，具体用法请参考  [LVGL文档](https://docs.lvgl.io/9.0/API/index.html)
     *
     * @module lv
     * @class MsgBox
     * @method addFooterButton
     *
     */
    JSValue MsgBox::addFooterButton(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        THIS_NCLASS(Obj,thisobj)
        ASSERT_ARGC(1)
        char * text = (char *)JS_ToCString(ctx, argv[0]) ;
        lv_obj_t * retval = lv_msgbox_add_footer_button( thisobj->lvobj(), text ) ;
            JS_FreeCString(ctx, text) ;
        JSValue jsretval = retval? be::lv::Obj::wrap(ctx, (lv_obj_t*)retval)->jsobj: JS_NULL ;
        return jsretval ;
    }
    #endif

    #ifndef METHOD_MsgBox_addCloseButton
    /**
     * > 该方法是对 LVGL C API `lv_msgbox_add_close_button` 的封装，具体用法请参考  [LVGL文档](https://docs.lvgl.io/9.0/API/index.html)
     *
     * @module lv
     * @class MsgBox
     * @method addCloseButton
     *
     */
    JSValue MsgBox::addCloseButton(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        THIS_NCLASS(Obj,thisobj)
        lv_obj_t * retval = lv_msgbox_add_close_button( thisobj->lvobj() ) ;
        JSValue jsretval = retval? be::lv::Obj::wrap(ctx, (lv_obj_t*)retval)->jsobj: JS_NULL ;
        return jsretval ;
    }
    #endif

    #ifndef METHOD_MsgBox_close
    /**
     * > 该方法是对 LVGL C API `lv_msgbox_close` 的封装，具体用法请参考  [LVGL文档](https://docs.lvgl.io/9.0/API/index.html)
     *
     * @module lv
     * @class MsgBox
     * @method close
     *
     */
    JSValue MsgBox::close(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        THIS_NCLASS(Obj,thisobj)
        lv_msgbox_close( thisobj->lvobj() ) ;
        return JS_UNDEFINED ;
    }
    #endif

    #ifndef METHOD_MsgBox_closeAsync
    /**
     * > 该方法是对 LVGL C API `lv_msgbox_close_async` 的封装，具体用法请参考  [LVGL文档](https://docs.lvgl.io/9.0/API/index.html)
     *
     * @module lv
     * @class MsgBox
     * @method closeAsync
     *
     */
    JSValue MsgBox::closeAsync(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        THIS_NCLASS(Obj,thisobj)
        lv_msgbox_close_async( thisobj->lvobj() ) ;
        return JS_UNDEFINED ;
    }
    #endif
// AUTO GENERATE CODE END [METHODS] --------

}
