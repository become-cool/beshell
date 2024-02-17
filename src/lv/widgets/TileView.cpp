#include "TileView.hpp"

namespace be::lv {
    DEFINE_NCLASS_META(TileView, Obj)
    std::vector<JSCFunctionListEntry> TileView::methods = {
// AUTO GENERATE CODE START [GETSET LIST] --------
        JS_CGETSET_DEF("tileActive",TileView::getTileActive,be::lv::Obj::invalidSetter) ,
// AUTO GENERATE CODE END [GETSET LIST] --------
// AUTO GENERATE CODE START [METHOD LIST] --------
        JS_CFUNC_DEF("addTile", 3, TileView::jsAddTile),
// AUTO GENERATE CODE END [METHOD LIST] --------
    } ;

    TileView::TileView(JSContext * ctx, JSValue jsobj, lv_obj_t * lvobj)
        : Obj(ctx, TileView::build(ctx,jsobj), lvobj)
    {}

    TileView::TileView(JSContext * ctx, lv_obj_t * parent)
        : TileView(ctx, JS_NULL, lv_tileview_create(parent))
    {}
        
    JSValue TileView::constructor(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        lv_obj_t * lvparent = nullptr ;
        if(argc>0) {
            JSVALUE_TO_LVOBJ_VAR(argv[0], lvparent)
        }
        TileView * widget = new TileView(ctx,lvparent) ;
        return widget->jsobj ;
    }

// AUTO GENERATE CODE START [GETSETS] --------
    JSValue TileView::getTileActive(JSContext *ctx, JSValueConst this_val){
        THIS_NCLASS(TileView,thisobj)
        lv_obj_t * value = lv_tileview_get_tile_active(thisobj->lvobj()) ;
        JSValue retval = value? be::lv::Obj::wrap(ctx, (lv_obj_t*)value)->jsobj: JS_NULL ;
        return retval ;
    }
// AUTO GENERATE CODE END [GETSETS] --------

// AUTO GENERATE CODE START [METHODS] --------
        JSValue TileView::jsAddTile(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
            THIS_NCLASS(Obj,thisobj)
            CHECK_ARGC(3)
            uint8_t col_id ;
            if(JS_ToUint32(ctx, (uint32_t *) &col_id, argv[0])!=0){
                JSTHROW("arg %s of method %s.%s() must be a %s","col_id","TileView","addTile","number")
            }
            uint8_t row_id ;
            if(JS_ToUint32(ctx, (uint32_t *) &row_id, argv[1])!=0){
                JSTHROW("arg %s of method %s.%s() must be a %s","row_id","TileView","addTile","number")
            }
            // argv dir
            const char * cstr_argv_2_ = JS_ToCString(ctx, argv[2]) ;
            lv_dir_t dir;
            if(lv_dir_str_to_const(cstr_argv_2_,&dir)) {
                JS_ThrowReferenceError(ctx,"unknow %s value: %s","lv_dir_t",cstr_argv_2_) ;
                JS_FreeCString(ctx, cstr_argv_2_) ;
                return JS_EXCEPTION ;
            }
            JS_FreeCString(ctx, cstr_argv_2_) ;
            lv_obj_t * retval = lv_tileview_add_tile( thisobj->lvobj(), col_id, row_id, dir ) ;
            JSValue jsretval = retval? be::lv::Obj::wrap(ctx, (lv_obj_t*)retval)->jsobj: JS_NULL ;
            return jsretval ;
        }
// AUTO GENERATE CODE END [METHODS] --------

}
