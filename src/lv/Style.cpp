#include "Style.hpp"
#include "driver/display/DisplayModule.hpp"
#include "const.hpp"

using namespace std ;

namespace be::lv {

    DEFINE_NCLASS_META(Style, NativeClass)

    std::vector<JSCFunctionListEntry> Style::methods = {
// AUTO GENERATE CODE START [PROPS] --------
// AUTO GENERATE CODE END [PROPS] --------
    } ;

    Style::Style(JSContext * ctx)
        : NativeClass(ctx,build(ctx,JS_NULL))
    {}

    JSValue Style::constructor(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        auto obj = new Style(ctx) ;
        return obj->jsobj ;
    }

    

// AUTO GENERATE CODE START [VALUE CONVERT FUNC] --------
    JSValue Style::propToJS(JSContext * ctx, lv_style_prop_t prop, lv_style_value_t value) {
        if( prop==LV_STYLE_FLEX_FLOW ){
            return lv_flex_flow_const_to_jsstr(ctx, (lv_flex_flow_t)value.num) ;
        }
        else if( prop==LV_STYLE_FLEX_MAIN_PLACE
            || prop==LV_STYLE_FLEX_CROSS_PLACE
            || prop==LV_STYLE_FLEX_TRACK_PLACE
        ) {
            return lv_flex_align_const_to_jsstr(ctx, (lv_flex_align_t)value.num) ;
        }
        else if( prop==LV_STYLE_FLEX_GROW ) {
            return JS_NewUint32(ctx, value.num) ;
        }
        switch(prop) {
        case LV_STYLE_ALIGN:
            return lv_align_const_to_jsstr(ctx, value.num) ;
        case LV_STYLE_BORDER_SIDE:
            return lv_border_side_const_to_jsstr(ctx, value.num) ;
        case LV_STYLE_TEXT_ALIGN:
            return lv_text_align_const_to_jsstr(ctx, value.num) ;
        case LV_STYLE_BASE_DIR:
            return lv_base_dir_const_to_jsstr(ctx, value.num) ;

        case LV_STYLE_WIDTH: 
        case LV_STYLE_MIN_WIDTH: 
        case LV_STYLE_MAX_WIDTH: 
        case LV_STYLE_HEIGHT: 
        case LV_STYLE_MIN_HEIGHT: 
        case LV_STYLE_MAX_HEIGHT: 
        case LV_STYLE_LENGTH: 
        case LV_STYLE_X: 
        case LV_STYLE_Y: 
        case LV_STYLE_TRANSFORM_WIDTH: 
        case LV_STYLE_TRANSFORM_HEIGHT: 
        case LV_STYLE_TRANSLATE_X: 
        case LV_STYLE_TRANSLATE_Y: 
        case LV_STYLE_TRANSFORM_SCALE_X: 
        case LV_STYLE_TRANSFORM_SCALE_Y: 
        case LV_STYLE_TRANSFORM_ROTATION: 
        case LV_STYLE_TRANSFORM_PIVOT_X: 
        case LV_STYLE_TRANSFORM_PIVOT_Y: 
        case LV_STYLE_TRANSFORM_SKEW_X: 
        case LV_STYLE_TRANSFORM_SKEW_Y: 
        case LV_STYLE_PAD_TOP: 
        case LV_STYLE_PAD_BOTTOM: 
        case LV_STYLE_PAD_LEFT: 
        case LV_STYLE_PAD_RIGHT: 
        case LV_STYLE_PAD_ROW: 
        case LV_STYLE_PAD_COLUMN: 
        case LV_STYLE_MARGIN_TOP: 
        case LV_STYLE_MARGIN_BOTTOM: 
        case LV_STYLE_MARGIN_LEFT: 
        case LV_STYLE_MARGIN_RIGHT: 
        case LV_STYLE_BG_OPA: 
        case LV_STYLE_BG_GRAD_DIR: 
        case LV_STYLE_BG_MAIN_STOP: 
        case LV_STYLE_BG_GRAD_STOP: 
        case LV_STYLE_BG_MAIN_OPA: 
        case LV_STYLE_BG_GRAD_OPA: 
        case LV_STYLE_BG_IMAGE_OPA: 
        case LV_STYLE_BG_IMAGE_RECOLOR_OPA: 
        case LV_STYLE_BG_IMAGE_TILED: 
        case LV_STYLE_BORDER_OPA: 
        case LV_STYLE_BORDER_WIDTH: 
        case LV_STYLE_BORDER_POST: 
        case LV_STYLE_OUTLINE_WIDTH: 
        case LV_STYLE_OUTLINE_OPA: 
        case LV_STYLE_OUTLINE_PAD: 
        case LV_STYLE_SHADOW_WIDTH: 
        case LV_STYLE_SHADOW_OFFSET_X: 
        case LV_STYLE_SHADOW_OFFSET_Y: 
        case LV_STYLE_SHADOW_SPREAD: 
        case LV_STYLE_SHADOW_OPA: 
        case LV_STYLE_IMAGE_OPA: 
        case LV_STYLE_IMAGE_RECOLOR_OPA: 
        case LV_STYLE_LINE_WIDTH: 
        case LV_STYLE_LINE_DASH_WIDTH: 
        case LV_STYLE_LINE_DASH_GAP: 
        case LV_STYLE_LINE_ROUNDED: 
        case LV_STYLE_LINE_OPA: 
        case LV_STYLE_ARC_WIDTH: 
        case LV_STYLE_ARC_ROUNDED: 
        case LV_STYLE_ARC_OPA: 
        case LV_STYLE_TEXT_OPA: 
        case LV_STYLE_TEXT_LETTER_SPACE: 
        case LV_STYLE_TEXT_LINE_SPACE: 
        case LV_STYLE_TEXT_DECOR: 
        case LV_STYLE_RADIUS: 
        case LV_STYLE_CLIP_CORNER: 
        case LV_STYLE_OPA: 
        case LV_STYLE_OPA_LAYERED: 
        case LV_STYLE_COLOR_FILTER_OPA: 
        case LV_STYLE_ANIM_DURATION: 
        case LV_STYLE_BLEND_MODE: 
        case LV_STYLE_LAYOUT: 
        case LV_STYLE_FLEX_FLOW: 
        case LV_STYLE_FLEX_MAIN_PLACE: 
        case LV_STYLE_FLEX_CROSS_PLACE: 
        case LV_STYLE_FLEX_TRACK_PLACE: 
        case LV_STYLE_FLEX_GROW: 
        case LV_STYLE_GRID_COLUMN_ALIGN: 
        case LV_STYLE_GRID_ROW_ALIGN: 
        case LV_STYLE_GRID_CELL_COLUMN_POS: 
        case LV_STYLE_GRID_CELL_X_ALIGN: 
        case LV_STYLE_GRID_CELL_COLUMN_SPAN: 
        case LV_STYLE_GRID_CELL_ROW_POS: 
        case LV_STYLE_GRID_CELL_Y_ALIGN: 
        case LV_STYLE_GRID_CELL_ROW_SPAN:
            return JS_NewInt32(ctx, (int32_t)value.num) ;

        case LV_STYLE_BG_COLOR: 
        case LV_STYLE_BG_GRAD_COLOR: 
        case LV_STYLE_BG_IMAGE_RECOLOR: 
        case LV_STYLE_BORDER_COLOR: 
        case LV_STYLE_OUTLINE_COLOR: 
        case LV_STYLE_SHADOW_COLOR: 
        case LV_STYLE_IMAGE_RECOLOR: 
        case LV_STYLE_LINE_COLOR: 
        case LV_STYLE_ARC_COLOR: 
        case LV_STYLE_TEXT_COLOR:
            return JS_NewUint32(ctx, be::driver::display::fromRGB(value.color.red,value.color.green,value.color.blue)) ;

        default:
            return JS_NewString(ctx, "unknow type") ;
        }
    }


    bool Style::propToValue(JSContext * ctx, lv_style_prop_t prop, JSValue jsval, lv_style_value_t * value) {
        if( prop==LV_STYLE_FLEX_FLOW ){
            return lv_flex_flow_jsstr_to_const(ctx, jsval, (lv_flex_flow_t*)&(value->num)) ;
        }
        else if( prop==LV_STYLE_FLEX_MAIN_PLACE
            || prop==LV_STYLE_FLEX_CROSS_PLACE
            || prop==LV_STYLE_FLEX_TRACK_PLACE
        ) {
            return lv_flex_align_jsstr_to_const(ctx, jsval, (lv_flex_align_t*)&(value->num)) ;
        }
        else if( prop==LV_STYLE_FLEX_GROW ) {
            return JS_ToUint32(ctx, (uint32_t*)&(value->num), jsval)==0 ;
        }
        switch(prop) {
        case LV_STYLE_ALIGN:
            return lv_align_jsstr_to_const(ctx, jsval, (lv_align_t *)&(value->num)) ;
        case LV_STYLE_BORDER_SIDE:
            return lv_border_side_jsstr_to_const(ctx, jsval, (lv_border_side_t *)&(value->num)) ;
        case LV_STYLE_TEXT_ALIGN:
            return lv_text_align_jsstr_to_const(ctx, jsval, (lv_text_align_t *)&(value->num)) ;
        case LV_STYLE_BASE_DIR:
            return lv_base_dir_jsstr_to_const(ctx, jsval, (lv_base_dir_t *)&(value->num)) ;

        case LV_STYLE_WIDTH: 
        case LV_STYLE_MIN_WIDTH: 
        case LV_STYLE_MAX_WIDTH: 
        case LV_STYLE_HEIGHT: 
        case LV_STYLE_MIN_HEIGHT: 
        case LV_STYLE_MAX_HEIGHT: 
        case LV_STYLE_LENGTH: 
        case LV_STYLE_X: 
        case LV_STYLE_Y: 
        case LV_STYLE_TRANSFORM_WIDTH: 
        case LV_STYLE_TRANSFORM_HEIGHT: 
        case LV_STYLE_TRANSLATE_X: 
        case LV_STYLE_TRANSLATE_Y: 
        case LV_STYLE_TRANSFORM_SCALE_X: 
        case LV_STYLE_TRANSFORM_SCALE_Y: 
        case LV_STYLE_TRANSFORM_ROTATION: 
        case LV_STYLE_TRANSFORM_PIVOT_X: 
        case LV_STYLE_TRANSFORM_PIVOT_Y: 
        case LV_STYLE_TRANSFORM_SKEW_X: 
        case LV_STYLE_TRANSFORM_SKEW_Y: 
        case LV_STYLE_PAD_TOP: 
        case LV_STYLE_PAD_BOTTOM: 
        case LV_STYLE_PAD_LEFT: 
        case LV_STYLE_PAD_RIGHT: 
        case LV_STYLE_PAD_ROW: 
        case LV_STYLE_PAD_COLUMN: 
        case LV_STYLE_MARGIN_TOP: 
        case LV_STYLE_MARGIN_BOTTOM: 
        case LV_STYLE_MARGIN_LEFT: 
        case LV_STYLE_MARGIN_RIGHT: 
        case LV_STYLE_BG_OPA: 
        case LV_STYLE_BG_GRAD_DIR: 
        case LV_STYLE_BG_MAIN_STOP: 
        case LV_STYLE_BG_GRAD_STOP: 
        case LV_STYLE_BG_MAIN_OPA: 
        case LV_STYLE_BG_GRAD_OPA: 
        case LV_STYLE_BG_IMAGE_OPA: 
        case LV_STYLE_BG_IMAGE_RECOLOR_OPA: 
        case LV_STYLE_BG_IMAGE_TILED: 
        case LV_STYLE_BORDER_OPA: 
        case LV_STYLE_BORDER_WIDTH: 
        case LV_STYLE_BORDER_POST: 
        case LV_STYLE_OUTLINE_WIDTH: 
        case LV_STYLE_OUTLINE_OPA: 
        case LV_STYLE_OUTLINE_PAD: 
        case LV_STYLE_SHADOW_WIDTH: 
        case LV_STYLE_SHADOW_OFFSET_X: 
        case LV_STYLE_SHADOW_OFFSET_Y: 
        case LV_STYLE_SHADOW_SPREAD: 
        case LV_STYLE_SHADOW_OPA: 
        case LV_STYLE_IMAGE_OPA: 
        case LV_STYLE_IMAGE_RECOLOR_OPA: 
        case LV_STYLE_LINE_WIDTH: 
        case LV_STYLE_LINE_DASH_WIDTH: 
        case LV_STYLE_LINE_DASH_GAP: 
        case LV_STYLE_LINE_ROUNDED: 
        case LV_STYLE_LINE_OPA: 
        case LV_STYLE_ARC_WIDTH: 
        case LV_STYLE_ARC_ROUNDED: 
        case LV_STYLE_ARC_OPA: 
        case LV_STYLE_TEXT_OPA: 
        case LV_STYLE_TEXT_LETTER_SPACE: 
        case LV_STYLE_TEXT_LINE_SPACE: 
        case LV_STYLE_TEXT_DECOR: 
        case LV_STYLE_RADIUS: 
        case LV_STYLE_CLIP_CORNER: 
        case LV_STYLE_OPA: 
        case LV_STYLE_OPA_LAYERED: 
        case LV_STYLE_COLOR_FILTER_OPA: 
        case LV_STYLE_ANIM_DURATION: 
        case LV_STYLE_BLEND_MODE: 
        case LV_STYLE_LAYOUT: 
        case LV_STYLE_FLEX_FLOW: 
        case LV_STYLE_FLEX_MAIN_PLACE: 
        case LV_STYLE_FLEX_CROSS_PLACE: 
        case LV_STYLE_FLEX_TRACK_PLACE: 
        case LV_STYLE_FLEX_GROW: 
        case LV_STYLE_GRID_COLUMN_ALIGN: 
        case LV_STYLE_GRID_ROW_ALIGN: 
        case LV_STYLE_GRID_CELL_COLUMN_POS: 
        case LV_STYLE_GRID_CELL_X_ALIGN: 
        case LV_STYLE_GRID_CELL_COLUMN_SPAN: 
        case LV_STYLE_GRID_CELL_ROW_POS: 
        case LV_STYLE_GRID_CELL_Y_ALIGN: 
        case LV_STYLE_GRID_CELL_ROW_SPAN:
            return JS_ToInt32(ctx, (int32_t*)&(value->num), jsval)==0 ;

        case LV_STYLE_BG_COLOR: 
        case LV_STYLE_BG_GRAD_COLOR: 
        case LV_STYLE_BG_IMAGE_RECOLOR: 
        case LV_STYLE_BORDER_COLOR: 
        case LV_STYLE_OUTLINE_COLOR: 
        case LV_STYLE_SHADOW_COLOR: 
        case LV_STYLE_IMAGE_RECOLOR: 
        case LV_STYLE_LINE_COLOR: 
        case LV_STYLE_ARC_COLOR: 
        case LV_STYLE_TEXT_COLOR: {
            uint32_t cval ;
            if(JS_ToUint32(ctx, &cval, jsval)!=0) {
                return false ;
            }
            be::driver::display::toRGB((uint16_t)cval, &(value->color.red), &(value->color.green), &(value->color.blue)) ;
            return true ;
        }

        default:
            return false ;
        }
    }
// AUTO GENERATE CODE END [VALUE CONVERT FUNC] --------

}