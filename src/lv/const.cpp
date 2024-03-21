#include "const.hpp"


namespace be::lv {

// AUTO GENERATE CODE START [CONST MAPPING] --------

// lv_flex_flow_t
bool lv_flex_flow_str_to_const(const char * name, lv_flex_flow_t * out) {
    if(strcmp(name,"row")==0) {
        (*out) = LV_FLEX_FLOW_ROW ;
    }
    else if(strcmp(name,"column")==0) {
        (*out) = LV_FLEX_FLOW_COLUMN ;
    }
    else if(strcmp(name,"row-wrap")==0) {
        (*out) = LV_FLEX_FLOW_ROW_WRAP ;
    }
    else if(strcmp(name,"row-reverse")==0) {
        (*out) = LV_FLEX_FLOW_ROW_REVERSE ;
    }
    else if(strcmp(name,"row-wrap-reverse")==0) {
        (*out) = LV_FLEX_FLOW_ROW_WRAP_REVERSE ;
    }
    else if(strcmp(name,"column-wrap")==0) {
        (*out) = LV_FLEX_FLOW_COLUMN_WRAP ;
    }
    else if(strcmp(name,"column-reverse")==0) {
        (*out) = LV_FLEX_FLOW_COLUMN_REVERSE ;
    }
    else if(strcmp(name,"column-wrap-reverse")==0) {
        (*out) = LV_FLEX_FLOW_COLUMN_WRAP_REVERSE ;
    }

    else {
        return false ;
    }
    return true ;
}
bool lv_flex_flow_jsstr_to_const(JSContext *ctx, JSValue jsstr, lv_flex_flow_t* out) {
    char * cstr = (char *)JS_ToCString(ctx, jsstr) ;
    bool suc = lv_flex_flow_str_to_const(cstr, out) ;
    JS_FreeCString(ctx, cstr) ;
    return suc ;
}
const char * lv_flex_flow_const_to_str(lv_flex_flow_t code) {

    switch(code) {
        case LV_FLEX_FLOW_ROW: return "row";
        case LV_FLEX_FLOW_COLUMN: return "column";
        case LV_FLEX_FLOW_ROW_WRAP: return "row-wrap";
        case LV_FLEX_FLOW_ROW_REVERSE: return "row-reverse";
        case LV_FLEX_FLOW_ROW_WRAP_REVERSE: return "row-wrap-reverse";
        case LV_FLEX_FLOW_COLUMN_WRAP: return "column-wrap";
        case LV_FLEX_FLOW_COLUMN_REVERSE: return "column-reverse";
        case LV_FLEX_FLOW_COLUMN_WRAP_REVERSE: return "column-wrap-reverse";

        default:
            return "unknow";
    }
}
JSValue lv_flex_flow_const_to_jsstr(JSContext *ctx, lv_flex_flow_t code) {
    return JS_NewString(ctx, lv_flex_flow_const_to_str(code));
}

// lv_flex_align_t
bool lv_flex_align_str_to_const(const char * name, lv_flex_align_t * out) {
    if(strcmp(name,"start")==0) {
        (*out) = LV_FLEX_ALIGN_START ;
    }
    else if(strcmp(name,"end")==0) {
        (*out) = LV_FLEX_ALIGN_END ;
    }
    else if(strcmp(name,"center")==0) {
        (*out) = LV_FLEX_ALIGN_CENTER ;
    }
    else if(strcmp(name,"space-evenly")==0) {
        (*out) = LV_FLEX_ALIGN_SPACE_EVENLY ;
    }
    else if(strcmp(name,"space-around")==0) {
        (*out) = LV_FLEX_ALIGN_SPACE_AROUND ;
    }
    else if(strcmp(name,"space-between")==0) {
        (*out) = LV_FLEX_ALIGN_SPACE_BETWEEN ;
    }

    else {
        return false ;
    }
    return true ;
}
bool lv_flex_align_jsstr_to_const(JSContext *ctx, JSValue jsstr, lv_flex_align_t* out) {
    char * cstr = (char *)JS_ToCString(ctx, jsstr) ;
    bool suc = lv_flex_align_str_to_const(cstr, out) ;
    JS_FreeCString(ctx, cstr) ;
    return suc ;
}
const char * lv_flex_align_const_to_str(lv_flex_align_t code) {

    switch(code) {
        case LV_FLEX_ALIGN_START: return "start";
        case LV_FLEX_ALIGN_END: return "end";
        case LV_FLEX_ALIGN_CENTER: return "center";
        case LV_FLEX_ALIGN_SPACE_EVENLY: return "space-evenly";
        case LV_FLEX_ALIGN_SPACE_AROUND: return "space-around";
        case LV_FLEX_ALIGN_SPACE_BETWEEN: return "space-between";

        default:
            return "unknow";
    }
}
JSValue lv_flex_align_const_to_jsstr(JSContext *ctx, lv_flex_align_t code) {
    return JS_NewString(ctx, lv_flex_align_const_to_str(code));
}

// lv_align_t
bool lv_align_str_to_const(const char * name, lv_align_t * out) {
    if(strcmp(name,"default")==0) {
        (*out) = LV_ALIGN_DEFAULT ;
    }
    else if(strcmp(name,"top-left")==0) {
        (*out) = LV_ALIGN_TOP_LEFT ;
    }
    else if(strcmp(name,"top-mid")==0) {
        (*out) = LV_ALIGN_TOP_MID ;
    }
    else if(strcmp(name,"top-right")==0) {
        (*out) = LV_ALIGN_TOP_RIGHT ;
    }
    else if(strcmp(name,"bottom-left")==0) {
        (*out) = LV_ALIGN_BOTTOM_LEFT ;
    }
    else if(strcmp(name,"bottom-mid")==0) {
        (*out) = LV_ALIGN_BOTTOM_MID ;
    }
    else if(strcmp(name,"bottom-right")==0) {
        (*out) = LV_ALIGN_BOTTOM_RIGHT ;
    }
    else if(strcmp(name,"left-mid")==0) {
        (*out) = LV_ALIGN_LEFT_MID ;
    }
    else if(strcmp(name,"right-mid")==0) {
        (*out) = LV_ALIGN_RIGHT_MID ;
    }
    else if(strcmp(name,"center")==0) {
        (*out) = LV_ALIGN_CENTER ;
    }
    else if(strcmp(name,"out-top-left")==0) {
        (*out) = LV_ALIGN_OUT_TOP_LEFT ;
    }
    else if(strcmp(name,"out-top-mid")==0) {
        (*out) = LV_ALIGN_OUT_TOP_MID ;
    }
    else if(strcmp(name,"out-top-right")==0) {
        (*out) = LV_ALIGN_OUT_TOP_RIGHT ;
    }
    else if(strcmp(name,"out-bottom-left")==0) {
        (*out) = LV_ALIGN_OUT_BOTTOM_LEFT ;
    }
    else if(strcmp(name,"out-bottom-mid")==0) {
        (*out) = LV_ALIGN_OUT_BOTTOM_MID ;
    }
    else if(strcmp(name,"out-bottom-right")==0) {
        (*out) = LV_ALIGN_OUT_BOTTOM_RIGHT ;
    }
    else if(strcmp(name,"out-left-top")==0) {
        (*out) = LV_ALIGN_OUT_LEFT_TOP ;
    }
    else if(strcmp(name,"out-left-mid")==0) {
        (*out) = LV_ALIGN_OUT_LEFT_MID ;
    }
    else if(strcmp(name,"out-left-bottom")==0) {
        (*out) = LV_ALIGN_OUT_LEFT_BOTTOM ;
    }
    else if(strcmp(name,"out-right-top")==0) {
        (*out) = LV_ALIGN_OUT_RIGHT_TOP ;
    }
    else if(strcmp(name,"out-right-mid")==0) {
        (*out) = LV_ALIGN_OUT_RIGHT_MID ;
    }
    else if(strcmp(name,"out-right-bottom")==0) {
        (*out) = LV_ALIGN_OUT_RIGHT_BOTTOM ;
    }

    else {
        return false ;
    }
    return true ;
}
bool lv_align_jsstr_to_const(JSContext *ctx, JSValue jsstr, lv_align_t* out) {
    char * cstr = (char *)JS_ToCString(ctx, jsstr) ;
    bool suc = lv_align_str_to_const(cstr, out) ;
    JS_FreeCString(ctx, cstr) ;
    return suc ;
}
const char * lv_align_const_to_str(lv_align_t code) {

    switch(code) {
        case LV_ALIGN_DEFAULT: return "default";
        case LV_ALIGN_TOP_LEFT: return "top-left";
        case LV_ALIGN_TOP_MID: return "top-mid";
        case LV_ALIGN_TOP_RIGHT: return "top-right";
        case LV_ALIGN_BOTTOM_LEFT: return "bottom-left";
        case LV_ALIGN_BOTTOM_MID: return "bottom-mid";
        case LV_ALIGN_BOTTOM_RIGHT: return "bottom-right";
        case LV_ALIGN_LEFT_MID: return "left-mid";
        case LV_ALIGN_RIGHT_MID: return "right-mid";
        case LV_ALIGN_CENTER: return "center";
        case LV_ALIGN_OUT_TOP_LEFT: return "out-top-left";
        case LV_ALIGN_OUT_TOP_MID: return "out-top-mid";
        case LV_ALIGN_OUT_TOP_RIGHT: return "out-top-right";
        case LV_ALIGN_OUT_BOTTOM_LEFT: return "out-bottom-left";
        case LV_ALIGN_OUT_BOTTOM_MID: return "out-bottom-mid";
        case LV_ALIGN_OUT_BOTTOM_RIGHT: return "out-bottom-right";
        case LV_ALIGN_OUT_LEFT_TOP: return "out-left-top";
        case LV_ALIGN_OUT_LEFT_MID: return "out-left-mid";
        case LV_ALIGN_OUT_LEFT_BOTTOM: return "out-left-bottom";
        case LV_ALIGN_OUT_RIGHT_TOP: return "out-right-top";
        case LV_ALIGN_OUT_RIGHT_MID: return "out-right-mid";
        case LV_ALIGN_OUT_RIGHT_BOTTOM: return "out-right-bottom";

        default:
            return "unknow";
    }
}
JSValue lv_align_const_to_jsstr(JSContext *ctx, lv_align_t code) {
    return JS_NewString(ctx, lv_align_const_to_str(code));
}

// lv_border_side_t
bool lv_border_side_str_to_const(const char * name, lv_border_side_t * out) {
    if(strcmp(name,"none")==0) {
        (*out) = LV_BORDER_SIDE_NONE ;
    }
    else if(strcmp(name,"bottom")==0) {
        (*out) = LV_BORDER_SIDE_BOTTOM ;
    }
    else if(strcmp(name,"top")==0) {
        (*out) = LV_BORDER_SIDE_TOP ;
    }
    else if(strcmp(name,"left")==0) {
        (*out) = LV_BORDER_SIDE_LEFT ;
    }
    else if(strcmp(name,"right")==0) {
        (*out) = LV_BORDER_SIDE_RIGHT ;
    }
    else if(strcmp(name,"full")==0) {
        (*out) = LV_BORDER_SIDE_FULL ;
    }
    else if(strcmp(name,"internal")==0) {
        (*out) = LV_BORDER_SIDE_INTERNAL ;
    }

    else {
        return false ;
    }
    return true ;
}
bool lv_border_side_jsstr_to_const(JSContext *ctx, JSValue jsstr, lv_border_side_t* out) {
    char * cstr = (char *)JS_ToCString(ctx, jsstr) ;
    bool suc = lv_border_side_str_to_const(cstr, out) ;
    JS_FreeCString(ctx, cstr) ;
    return suc ;
}
const char * lv_border_side_const_to_str(lv_border_side_t code) {

    switch(code) {
        case LV_BORDER_SIDE_NONE: return "none";
        case LV_BORDER_SIDE_BOTTOM: return "bottom";
        case LV_BORDER_SIDE_TOP: return "top";
        case LV_BORDER_SIDE_LEFT: return "left";
        case LV_BORDER_SIDE_RIGHT: return "right";
        case LV_BORDER_SIDE_FULL: return "full";
        case LV_BORDER_SIDE_INTERNAL: return "internal";

        default:
            return "unknow";
    }
}
JSValue lv_border_side_const_to_jsstr(JSContext *ctx, lv_border_side_t code) {
    return JS_NewString(ctx, lv_border_side_const_to_str(code));
}

// lv_text_align_t
bool lv_text_align_str_to_const(const char * name, lv_text_align_t * out) {
    if(strcmp(name,"auto")==0) {
        (*out) = LV_TEXT_ALIGN_AUTO ;
    }
    else if(strcmp(name,"left")==0) {
        (*out) = LV_TEXT_ALIGN_LEFT ;
    }
    else if(strcmp(name,"center")==0) {
        (*out) = LV_TEXT_ALIGN_CENTER ;
    }
    else if(strcmp(name,"right")==0) {
        (*out) = LV_TEXT_ALIGN_RIGHT ;
    }

    else {
        return false ;
    }
    return true ;
}
bool lv_text_align_jsstr_to_const(JSContext *ctx, JSValue jsstr, lv_text_align_t* out) {
    char * cstr = (char *)JS_ToCString(ctx, jsstr) ;
    bool suc = lv_text_align_str_to_const(cstr, out) ;
    JS_FreeCString(ctx, cstr) ;
    return suc ;
}
const char * lv_text_align_const_to_str(lv_text_align_t code) {

    switch(code) {
        case LV_TEXT_ALIGN_AUTO: return "auto";
        case LV_TEXT_ALIGN_LEFT: return "left";
        case LV_TEXT_ALIGN_CENTER: return "center";
        case LV_TEXT_ALIGN_RIGHT: return "right";

        default:
            return "unknow";
    }
}
JSValue lv_text_align_const_to_jsstr(JSContext *ctx, lv_text_align_t code) {
    return JS_NewString(ctx, lv_text_align_const_to_str(code));
}

// lv_base_dir_t
bool lv_base_dir_str_to_const(const char * name, lv_base_dir_t * out) {
    if(strcmp(name,"ltr")==0) {
        (*out) = LV_BASE_DIR_LTR ;
    }
    else if(strcmp(name,"rtl")==0) {
        (*out) = LV_BASE_DIR_RTL ;
    }
    else if(strcmp(name,"auto")==0) {
        (*out) = LV_BASE_DIR_AUTO ;
    }
    else if(strcmp(name,"neutral")==0) {
        (*out) = LV_BASE_DIR_NEUTRAL ;
    }
    else if(strcmp(name,"weak")==0) {
        (*out) = LV_BASE_DIR_WEAK ;
    }

    else {
        return false ;
    }
    return true ;
}
bool lv_base_dir_jsstr_to_const(JSContext *ctx, JSValue jsstr, lv_base_dir_t* out) {
    char * cstr = (char *)JS_ToCString(ctx, jsstr) ;
    bool suc = lv_base_dir_str_to_const(cstr, out) ;
    JS_FreeCString(ctx, cstr) ;
    return suc ;
}
const char * lv_base_dir_const_to_str(lv_base_dir_t code) {

    switch(code) {
        case LV_BASE_DIR_LTR: return "ltr";
        case LV_BASE_DIR_RTL: return "rtl";
        case LV_BASE_DIR_AUTO: return "auto";
        case LV_BASE_DIR_NEUTRAL: return "neutral";
        case LV_BASE_DIR_WEAK: return "weak";

        default:
            return "unknow";
    }
}
JSValue lv_base_dir_const_to_jsstr(JSContext *ctx, lv_base_dir_t code) {
    return JS_NewString(ctx, lv_base_dir_const_to_str(code));
}

// _lv_part_t
bool lv__part_str_to_const(const char * name, _lv_part_t * out) {
    if(strcmp(name,"main")==0) {
        (*out) = LV_PART_MAIN ;
    }
    else if(strcmp(name,"scrollbar")==0) {
        (*out) = LV_PART_SCROLLBAR ;
    }
    else if(strcmp(name,"indicator")==0) {
        (*out) = LV_PART_INDICATOR ;
    }
    else if(strcmp(name,"knob")==0) {
        (*out) = LV_PART_KNOB ;
    }
    else if(strcmp(name,"selected")==0) {
        (*out) = LV_PART_SELECTED ;
    }
    else if(strcmp(name,"items")==0) {
        (*out) = LV_PART_ITEMS ;
    }
    else if(strcmp(name,"cursor")==0) {
        (*out) = LV_PART_CURSOR ;
    }
    else if(strcmp(name,"custom-first")==0) {
        (*out) = LV_PART_CUSTOM_FIRST ;
    }
    else if(strcmp(name,"any")==0) {
        (*out) = LV_PART_ANY ;
    }

    else {
        return false ;
    }
    return true ;
}
bool lv__part_jsstr_to_const(JSContext *ctx, JSValue jsstr, _lv_part_t* out) {
    char * cstr = (char *)JS_ToCString(ctx, jsstr) ;
    bool suc = lv__part_str_to_const(cstr, out) ;
    JS_FreeCString(ctx, cstr) ;
    return suc ;
}
const char * lv__part_const_to_str(_lv_part_t code) {

    switch(code) {
        case LV_PART_MAIN: return "main";
        case LV_PART_SCROLLBAR: return "scrollbar";
        case LV_PART_INDICATOR: return "indicator";
        case LV_PART_KNOB: return "knob";
        case LV_PART_SELECTED: return "selected";
        case LV_PART_ITEMS: return "items";
        case LV_PART_CURSOR: return "cursor";
        case LV_PART_CUSTOM_FIRST: return "custom-first";
        case LV_PART_ANY: return "any";

        default:
            return "unknow";
    }
}
JSValue lv__part_const_to_jsstr(JSContext *ctx, _lv_part_t code) {
    return JS_NewString(ctx, lv__part_const_to_str(code));
}

// lv_scrollbar_mode_t
bool lv_scrollbar_mode_str_to_const(const char * name, lv_scrollbar_mode_t * out) {
    if(strcmp(name,"mode-off")==0) {
        (*out) = LV_SCROLLBAR_MODE_OFF ;
    }
    else if(strcmp(name,"mode-on")==0) {
        (*out) = LV_SCROLLBAR_MODE_ON ;
    }
    else if(strcmp(name,"mode-active")==0) {
        (*out) = LV_SCROLLBAR_MODE_ACTIVE ;
    }
    else if(strcmp(name,"mode-auto")==0) {
        (*out) = LV_SCROLLBAR_MODE_AUTO ;
    }

    else {
        return false ;
    }
    return true ;
}
bool lv_scrollbar_mode_jsstr_to_const(JSContext *ctx, JSValue jsstr, lv_scrollbar_mode_t* out) {
    char * cstr = (char *)JS_ToCString(ctx, jsstr) ;
    bool suc = lv_scrollbar_mode_str_to_const(cstr, out) ;
    JS_FreeCString(ctx, cstr) ;
    return suc ;
}
const char * lv_scrollbar_mode_const_to_str(lv_scrollbar_mode_t code) {

    switch(code) {
        case LV_SCROLLBAR_MODE_OFF: return "mode-off";
        case LV_SCROLLBAR_MODE_ON: return "mode-on";
        case LV_SCROLLBAR_MODE_ACTIVE: return "mode-active";
        case LV_SCROLLBAR_MODE_AUTO: return "mode-auto";

        default:
            return "unknow";
    }
}
JSValue lv_scrollbar_mode_const_to_jsstr(JSContext *ctx, lv_scrollbar_mode_t code) {
    return JS_NewString(ctx, lv_scrollbar_mode_const_to_str(code));
}

// lv_dir_t
bool lv_dir_str_to_const(const char * name, lv_dir_t * out) {
    if(strcmp(name,"none")==0) {
        (*out) = LV_DIR_NONE ;
    }
    else if(strcmp(name,"left")==0) {
        (*out) = LV_DIR_LEFT ;
    }
    else if(strcmp(name,"right")==0) {
        (*out) = LV_DIR_RIGHT ;
    }
    else if(strcmp(name,"top")==0) {
        (*out) = LV_DIR_TOP ;
    }
    else if(strcmp(name,"bottom")==0) {
        (*out) = LV_DIR_BOTTOM ;
    }
    else if(strcmp(name,"hor")==0) {
        (*out) = LV_DIR_HOR ;
    }
    else if(strcmp(name,"ver")==0) {
        (*out) = LV_DIR_VER ;
    }
    else if(strcmp(name,"all")==0) {
        (*out) = LV_DIR_ALL ;
    }

    else {
        return false ;
    }
    return true ;
}
bool lv_dir_jsstr_to_const(JSContext *ctx, JSValue jsstr, lv_dir_t* out) {
    char * cstr = (char *)JS_ToCString(ctx, jsstr) ;
    bool suc = lv_dir_str_to_const(cstr, out) ;
    JS_FreeCString(ctx, cstr) ;
    return suc ;
}
const char * lv_dir_const_to_str(lv_dir_t code) {

    switch(code) {
        case LV_DIR_NONE: return "none";
        case LV_DIR_LEFT: return "left";
        case LV_DIR_RIGHT: return "right";
        case LV_DIR_TOP: return "top";
        case LV_DIR_BOTTOM: return "bottom";
        case LV_DIR_HOR: return "hor";
        case LV_DIR_VER: return "ver";
        case LV_DIR_ALL: return "all";

        default:
            return "unknow";
    }
}
JSValue lv_dir_const_to_jsstr(JSContext *ctx, lv_dir_t code) {
    return JS_NewString(ctx, lv_dir_const_to_str(code));
}

// lv_scroll_snap_t
bool lv_scroll_snap_str_to_const(const char * name, lv_scroll_snap_t * out) {
    if(strcmp(name,"none")==0) {
        (*out) = LV_SCROLL_SNAP_NONE ;
    }
    else if(strcmp(name,"start")==0) {
        (*out) = LV_SCROLL_SNAP_START ;
    }
    else if(strcmp(name,"end")==0) {
        (*out) = LV_SCROLL_SNAP_END ;
    }
    else if(strcmp(name,"center")==0) {
        (*out) = LV_SCROLL_SNAP_CENTER ;
    }

    else {
        return false ;
    }
    return true ;
}
bool lv_scroll_snap_jsstr_to_const(JSContext *ctx, JSValue jsstr, lv_scroll_snap_t* out) {
    char * cstr = (char *)JS_ToCString(ctx, jsstr) ;
    bool suc = lv_scroll_snap_str_to_const(cstr, out) ;
    JS_FreeCString(ctx, cstr) ;
    return suc ;
}
const char * lv_scroll_snap_const_to_str(lv_scroll_snap_t code) {

    switch(code) {
        case LV_SCROLL_SNAP_NONE: return "none";
        case LV_SCROLL_SNAP_START: return "start";
        case LV_SCROLL_SNAP_END: return "end";
        case LV_SCROLL_SNAP_CENTER: return "center";

        default:
            return "unknow";
    }
}
JSValue lv_scroll_snap_const_to_jsstr(JSContext *ctx, lv_scroll_snap_t code) {
    return JS_NewString(ctx, lv_scroll_snap_const_to_str(code));
}

// lv_obj_flag_t
bool lv__obj_flag_str_to_const(const char * name, lv_obj_flag_t * out) {
    if(strcmp(name,"hidden")==0) {
        (*out) = LV_OBJ_FLAG_HIDDEN ;
    }
    else if(strcmp(name,"clickable")==0) {
        (*out) = LV_OBJ_FLAG_CLICKABLE ;
    }
    else if(strcmp(name,"click-focusable")==0) {
        (*out) = LV_OBJ_FLAG_CLICK_FOCUSABLE ;
    }
    else if(strcmp(name,"checkable")==0) {
        (*out) = LV_OBJ_FLAG_CHECKABLE ;
    }
    else if(strcmp(name,"scrollable")==0) {
        (*out) = LV_OBJ_FLAG_SCROLLABLE ;
    }
    else if(strcmp(name,"scroll-elastic")==0) {
        (*out) = LV_OBJ_FLAG_SCROLL_ELASTIC ;
    }
    else if(strcmp(name,"scroll-momentum")==0) {
        (*out) = LV_OBJ_FLAG_SCROLL_MOMENTUM ;
    }
    else if(strcmp(name,"scroll-one")==0) {
        (*out) = LV_OBJ_FLAG_SCROLL_ONE ;
    }
    else if(strcmp(name,"scroll-chain-hor")==0) {
        (*out) = LV_OBJ_FLAG_SCROLL_CHAIN_HOR ;
    }
    else if(strcmp(name,"scroll-chain-ver")==0) {
        (*out) = LV_OBJ_FLAG_SCROLL_CHAIN_VER ;
    }
    else if(strcmp(name,"scroll-chain")==0) {
        (*out) = LV_OBJ_FLAG_SCROLL_CHAIN ;
    }
    else if(strcmp(name,"scroll-on-focus")==0) {
        (*out) = LV_OBJ_FLAG_SCROLL_ON_FOCUS ;
    }
    else if(strcmp(name,"scroll-with-arrow")==0) {
        (*out) = LV_OBJ_FLAG_SCROLL_WITH_ARROW ;
    }
    else if(strcmp(name,"snappable")==0) {
        (*out) = LV_OBJ_FLAG_SNAPPABLE ;
    }
    else if(strcmp(name,"press-lock")==0) {
        (*out) = LV_OBJ_FLAG_PRESS_LOCK ;
    }
    else if(strcmp(name,"event-bubble")==0) {
        (*out) = LV_OBJ_FLAG_EVENT_BUBBLE ;
    }
    else if(strcmp(name,"gesture-bubble")==0) {
        (*out) = LV_OBJ_FLAG_GESTURE_BUBBLE ;
    }
    else if(strcmp(name,"adv-hittest")==0) {
        (*out) = LV_OBJ_FLAG_ADV_HITTEST ;
    }
    else if(strcmp(name,"ignore-layout")==0) {
        (*out) = LV_OBJ_FLAG_IGNORE_LAYOUT ;
    }
    else if(strcmp(name,"floating")==0) {
        (*out) = LV_OBJ_FLAG_FLOATING ;
    }
    else if(strcmp(name,"send-draw-task-events")==0) {
        (*out) = LV_OBJ_FLAG_SEND_DRAW_TASK_EVENTS ;
    }
    else if(strcmp(name,"overflow-visible")==0) {
        (*out) = LV_OBJ_FLAG_OVERFLOW_VISIBLE ;
    }
    else if(strcmp(name,"flex-in-new-track")==0) {
        (*out) = LV_OBJ_FLAG_FLEX_IN_NEW_TRACK ;
    }
    else if(strcmp(name,"layout-1")==0) {
        (*out) = LV_OBJ_FLAG_LAYOUT_1 ;
    }
    else if(strcmp(name,"layout-2")==0) {
        (*out) = LV_OBJ_FLAG_LAYOUT_2 ;
    }
    else if(strcmp(name,"widget-1")==0) {
        (*out) = LV_OBJ_FLAG_WIDGET_1 ;
    }
    else if(strcmp(name,"widget-2")==0) {
        (*out) = LV_OBJ_FLAG_WIDGET_2 ;
    }
    else if(strcmp(name,"user-1")==0) {
        (*out) = LV_OBJ_FLAG_USER_1 ;
    }
    else if(strcmp(name,"user-2")==0) {
        (*out) = LV_OBJ_FLAG_USER_2 ;
    }
    else if(strcmp(name,"user-3")==0) {
        (*out) = LV_OBJ_FLAG_USER_3 ;
    }
    else if(strcmp(name,"user-4")==0) {
        (*out) = LV_OBJ_FLAG_USER_4 ;
    }

    else {
        return false ;
    }
    return true ;
}
bool lv__obj_flag_jsstr_to_const(JSContext *ctx, JSValue jsstr, lv_obj_flag_t* out) {
    char * cstr = (char *)JS_ToCString(ctx, jsstr) ;
    bool suc = lv__obj_flag_str_to_const(cstr, out) ;
    JS_FreeCString(ctx, cstr) ;
    return suc ;
}
const char * lv__obj_flag_const_to_str(lv_obj_flag_t code) {

    switch(code) {
        case LV_OBJ_FLAG_HIDDEN: return "hidden";
        case LV_OBJ_FLAG_CLICKABLE: return "clickable";
        case LV_OBJ_FLAG_CLICK_FOCUSABLE: return "click-focusable";
        case LV_OBJ_FLAG_CHECKABLE: return "checkable";
        case LV_OBJ_FLAG_SCROLLABLE: return "scrollable";
        case LV_OBJ_FLAG_SCROLL_ELASTIC: return "scroll-elastic";
        case LV_OBJ_FLAG_SCROLL_MOMENTUM: return "scroll-momentum";
        case LV_OBJ_FLAG_SCROLL_ONE: return "scroll-one";
        case LV_OBJ_FLAG_SCROLL_CHAIN_HOR: return "scroll-chain-hor";
        case LV_OBJ_FLAG_SCROLL_CHAIN_VER: return "scroll-chain-ver";
        case LV_OBJ_FLAG_SCROLL_CHAIN: return "scroll-chain";
        case LV_OBJ_FLAG_SCROLL_ON_FOCUS: return "scroll-on-focus";
        case LV_OBJ_FLAG_SCROLL_WITH_ARROW: return "scroll-with-arrow";
        case LV_OBJ_FLAG_SNAPPABLE: return "snappable";
        case LV_OBJ_FLAG_PRESS_LOCK: return "press-lock";
        case LV_OBJ_FLAG_EVENT_BUBBLE: return "event-bubble";
        case LV_OBJ_FLAG_GESTURE_BUBBLE: return "gesture-bubble";
        case LV_OBJ_FLAG_ADV_HITTEST: return "adv-hittest";
        case LV_OBJ_FLAG_IGNORE_LAYOUT: return "ignore-layout";
        case LV_OBJ_FLAG_FLOATING: return "floating";
        case LV_OBJ_FLAG_SEND_DRAW_TASK_EVENTS: return "send-draw-task-events";
        case LV_OBJ_FLAG_OVERFLOW_VISIBLE: return "overflow-visible";
        case LV_OBJ_FLAG_FLEX_IN_NEW_TRACK: return "flex-in-new-track";
        case LV_OBJ_FLAG_LAYOUT_1: return "layout-1";
        case LV_OBJ_FLAG_LAYOUT_2: return "layout-2";
        case LV_OBJ_FLAG_WIDGET_1: return "widget-1";
        case LV_OBJ_FLAG_WIDGET_2: return "widget-2";
        case LV_OBJ_FLAG_USER_1: return "user-1";
        case LV_OBJ_FLAG_USER_2: return "user-2";
        case LV_OBJ_FLAG_USER_3: return "user-3";
        case LV_OBJ_FLAG_USER_4: return "user-4";

        default:
            return "unknow";
    }
}
JSValue lv__obj_flag_const_to_jsstr(JSContext *ctx, lv_obj_flag_t code) {
    return JS_NewString(ctx, lv__obj_flag_const_to_str(code));
}

// lv_state_t
bool lv_state_str_to_const(const char * name, lv_state_t * out) {
    if(strcmp(name,"default")==0) {
        (*out) = LV_STATE_DEFAULT ;
    }
    else if(strcmp(name,"checked")==0) {
        (*out) = LV_STATE_CHECKED ;
    }
    else if(strcmp(name,"focused")==0) {
        (*out) = LV_STATE_FOCUSED ;
    }
    else if(strcmp(name,"focus-key")==0) {
        (*out) = LV_STATE_FOCUS_KEY ;
    }
    else if(strcmp(name,"edited")==0) {
        (*out) = LV_STATE_EDITED ;
    }
    else if(strcmp(name,"hovered")==0) {
        (*out) = LV_STATE_HOVERED ;
    }
    else if(strcmp(name,"pressed")==0) {
        (*out) = LV_STATE_PRESSED ;
    }
    else if(strcmp(name,"scrolled")==0) {
        (*out) = LV_STATE_SCROLLED ;
    }
    else if(strcmp(name,"disabled")==0) {
        (*out) = LV_STATE_DISABLED ;
    }
    else if(strcmp(name,"user-1")==0) {
        (*out) = LV_STATE_USER_1 ;
    }
    else if(strcmp(name,"user-2")==0) {
        (*out) = LV_STATE_USER_2 ;
    }
    else if(strcmp(name,"user-3")==0) {
        (*out) = LV_STATE_USER_3 ;
    }
    else if(strcmp(name,"user-4")==0) {
        (*out) = LV_STATE_USER_4 ;
    }
    else if(strcmp(name,"any")==0) {
        (*out) = LV_STATE_ANY ;
    }

    else {
        return false ;
    }
    return true ;
}
bool lv_state_jsstr_to_const(JSContext *ctx, JSValue jsstr, lv_state_t* out) {
    char * cstr = (char *)JS_ToCString(ctx, jsstr) ;
    bool suc = lv_state_str_to_const(cstr, out) ;
    JS_FreeCString(ctx, cstr) ;
    return suc ;
}
const char * lv_state_const_to_str(lv_state_t code) {

    switch(code) {
        case LV_STATE_DEFAULT: return "default";
        case LV_STATE_CHECKED: return "checked";
        case LV_STATE_FOCUSED: return "focused";
        case LV_STATE_FOCUS_KEY: return "focus-key";
        case LV_STATE_EDITED: return "edited";
        case LV_STATE_HOVERED: return "hovered";
        case LV_STATE_PRESSED: return "pressed";
        case LV_STATE_SCROLLED: return "scrolled";
        case LV_STATE_DISABLED: return "disabled";
        case LV_STATE_USER_1: return "user-1";
        case LV_STATE_USER_2: return "user-2";
        case LV_STATE_USER_3: return "user-3";
        case LV_STATE_USER_4: return "user-4";
        case LV_STATE_ANY: return "any";

        default:
            return "unknow";
    }
}
JSValue lv_state_const_to_jsstr(JSContext *ctx, lv_state_t code) {
    return JS_NewString(ctx, lv_state_const_to_str(code));
}

// lv_event_code_t
bool lv_event_code_str_to_const(const char * name, lv_event_code_t * out) {
    if(strcmp(name,"all")==0) {
        (*out) = LV_EVENT_ALL ;
    }
    else if(strcmp(name,"pressed")==0) {
        (*out) = LV_EVENT_PRESSED ;
    }
    else if(strcmp(name,"pressing")==0) {
        (*out) = LV_EVENT_PRESSING ;
    }
    else if(strcmp(name,"press-lost")==0) {
        (*out) = LV_EVENT_PRESS_LOST ;
    }
    else if(strcmp(name,"short-clicked")==0) {
        (*out) = LV_EVENT_SHORT_CLICKED ;
    }
    else if(strcmp(name,"long-pressed")==0) {
        (*out) = LV_EVENT_LONG_PRESSED ;
    }
    else if(strcmp(name,"long-pressed-repeat")==0) {
        (*out) = LV_EVENT_LONG_PRESSED_REPEAT ;
    }
    else if(strcmp(name,"clicked")==0) {
        (*out) = LV_EVENT_CLICKED ;
    }
    else if(strcmp(name,"released")==0) {
        (*out) = LV_EVENT_RELEASED ;
    }
    else if(strcmp(name,"scroll-begin")==0) {
        (*out) = LV_EVENT_SCROLL_BEGIN ;
    }
    else if(strcmp(name,"scroll-throw-begin")==0) {
        (*out) = LV_EVENT_SCROLL_THROW_BEGIN ;
    }
    else if(strcmp(name,"scroll-end")==0) {
        (*out) = LV_EVENT_SCROLL_END ;
    }
    else if(strcmp(name,"scroll")==0) {
        (*out) = LV_EVENT_SCROLL ;
    }
    else if(strcmp(name,"gesture")==0) {
        (*out) = LV_EVENT_GESTURE ;
    }
    else if(strcmp(name,"key")==0) {
        (*out) = LV_EVENT_KEY ;
    }
    else if(strcmp(name,"focused")==0) {
        (*out) = LV_EVENT_FOCUSED ;
    }
    else if(strcmp(name,"defocused")==0) {
        (*out) = LV_EVENT_DEFOCUSED ;
    }
    else if(strcmp(name,"leave")==0) {
        (*out) = LV_EVENT_LEAVE ;
    }
    else if(strcmp(name,"hit-test")==0) {
        (*out) = LV_EVENT_HIT_TEST ;
    }
    else if(strcmp(name,"indev-reset")==0) {
        (*out) = LV_EVENT_INDEV_RESET ;
    }
    else if(strcmp(name,"cover-check")==0) {
        (*out) = LV_EVENT_COVER_CHECK ;
    }
    else if(strcmp(name,"refr-ext-draw-size")==0) {
        (*out) = LV_EVENT_REFR_EXT_DRAW_SIZE ;
    }
    else if(strcmp(name,"draw-main-begin")==0) {
        (*out) = LV_EVENT_DRAW_MAIN_BEGIN ;
    }
    else if(strcmp(name,"draw-main")==0) {
        (*out) = LV_EVENT_DRAW_MAIN ;
    }
    else if(strcmp(name,"draw-main-end")==0) {
        (*out) = LV_EVENT_DRAW_MAIN_END ;
    }
    else if(strcmp(name,"draw-post-begin")==0) {
        (*out) = LV_EVENT_DRAW_POST_BEGIN ;
    }
    else if(strcmp(name,"draw-post")==0) {
        (*out) = LV_EVENT_DRAW_POST ;
    }
    else if(strcmp(name,"draw-post-end")==0) {
        (*out) = LV_EVENT_DRAW_POST_END ;
    }
    else if(strcmp(name,"draw-task-added")==0) {
        (*out) = LV_EVENT_DRAW_TASK_ADDED ;
    }
    else if(strcmp(name,"value-changed")==0) {
        (*out) = LV_EVENT_VALUE_CHANGED ;
    }
    else if(strcmp(name,"insert")==0) {
        (*out) = LV_EVENT_INSERT ;
    }
    else if(strcmp(name,"refresh")==0) {
        (*out) = LV_EVENT_REFRESH ;
    }
    else if(strcmp(name,"ready")==0) {
        (*out) = LV_EVENT_READY ;
    }
    else if(strcmp(name,"cancel")==0) {
        (*out) = LV_EVENT_CANCEL ;
    }
    else if(strcmp(name,"create")==0) {
        (*out) = LV_EVENT_CREATE ;
    }
    else if(strcmp(name,"delete")==0) {
        (*out) = LV_EVENT_DELETE ;
    }
    else if(strcmp(name,"child-changed")==0) {
        (*out) = LV_EVENT_CHILD_CHANGED ;
    }
    else if(strcmp(name,"child-created")==0) {
        (*out) = LV_EVENT_CHILD_CREATED ;
    }
    else if(strcmp(name,"child-deleted")==0) {
        (*out) = LV_EVENT_CHILD_DELETED ;
    }
    else if(strcmp(name,"screen-unload-start")==0) {
        (*out) = LV_EVENT_SCREEN_UNLOAD_START ;
    }
    else if(strcmp(name,"screen-load-start")==0) {
        (*out) = LV_EVENT_SCREEN_LOAD_START ;
    }
    else if(strcmp(name,"screen-loaded")==0) {
        (*out) = LV_EVENT_SCREEN_LOADED ;
    }
    else if(strcmp(name,"screen-unloaded")==0) {
        (*out) = LV_EVENT_SCREEN_UNLOADED ;
    }
    else if(strcmp(name,"size-changed")==0) {
        (*out) = LV_EVENT_SIZE_CHANGED ;
    }
    else if(strcmp(name,"style-changed")==0) {
        (*out) = LV_EVENT_STYLE_CHANGED ;
    }
    else if(strcmp(name,"layout-changed")==0) {
        (*out) = LV_EVENT_LAYOUT_CHANGED ;
    }
    else if(strcmp(name,"get-self-size")==0) {
        (*out) = LV_EVENT_GET_SELF_SIZE ;
    }
    else if(strcmp(name,"invalidate-area")==0) {
        (*out) = LV_EVENT_INVALIDATE_AREA ;
    }
    else if(strcmp(name,"resolution-changed")==0) {
        (*out) = LV_EVENT_RESOLUTION_CHANGED ;
    }
    else if(strcmp(name,"color-format-changed")==0) {
        (*out) = LV_EVENT_COLOR_FORMAT_CHANGED ;
    }
    else if(strcmp(name,"refr-request")==0) {
        (*out) = LV_EVENT_REFR_REQUEST ;
    }
    else if(strcmp(name,"refr-start")==0) {
        (*out) = LV_EVENT_REFR_START ;
    }
    else if(strcmp(name,"refr-ready")==0) {
        (*out) = LV_EVENT_REFR_READY ;
    }
    else if(strcmp(name,"render-start")==0) {
        (*out) = LV_EVENT_RENDER_START ;
    }
    else if(strcmp(name,"render-ready")==0) {
        (*out) = LV_EVENT_RENDER_READY ;
    }
    else if(strcmp(name,"flush-start")==0) {
        (*out) = LV_EVENT_FLUSH_START ;
    }
    else if(strcmp(name,"flush-finish")==0) {
        (*out) = LV_EVENT_FLUSH_FINISH ;
    }
    else if(strcmp(name,"vsync")==0) {
        (*out) = LV_EVENT_VSYNC ;
    }
    else if(strcmp(name,"last")==0) {
        (*out) = _LV_EVENT_LAST ;
    }
    else if(strcmp(name,"preprocess")==0) {
        (*out) = LV_EVENT_PREPROCESS ;
    }

    else {
        return false ;
    }
    return true ;
}
bool lv_event_code_jsstr_to_const(JSContext *ctx, JSValue jsstr, lv_event_code_t* out) {
    char * cstr = (char *)JS_ToCString(ctx, jsstr) ;
    bool suc = lv_event_code_str_to_const(cstr, out) ;
    JS_FreeCString(ctx, cstr) ;
    return suc ;
}
const char * lv_event_code_const_to_str(lv_event_code_t code) {

    switch(code) {
        case LV_EVENT_ALL: return "all";
        case LV_EVENT_PRESSED: return "pressed";
        case LV_EVENT_PRESSING: return "pressing";
        case LV_EVENT_PRESS_LOST: return "press-lost";
        case LV_EVENT_SHORT_CLICKED: return "short-clicked";
        case LV_EVENT_LONG_PRESSED: return "long-pressed";
        case LV_EVENT_LONG_PRESSED_REPEAT: return "long-pressed-repeat";
        case LV_EVENT_CLICKED: return "clicked";
        case LV_EVENT_RELEASED: return "released";
        case LV_EVENT_SCROLL_BEGIN: return "scroll-begin";
        case LV_EVENT_SCROLL_THROW_BEGIN: return "scroll-throw-begin";
        case LV_EVENT_SCROLL_END: return "scroll-end";
        case LV_EVENT_SCROLL: return "scroll";
        case LV_EVENT_GESTURE: return "gesture";
        case LV_EVENT_KEY: return "key";
        case LV_EVENT_FOCUSED: return "focused";
        case LV_EVENT_DEFOCUSED: return "defocused";
        case LV_EVENT_LEAVE: return "leave";
        case LV_EVENT_HIT_TEST: return "hit-test";
        case LV_EVENT_INDEV_RESET: return "indev-reset";
        case LV_EVENT_COVER_CHECK: return "cover-check";
        case LV_EVENT_REFR_EXT_DRAW_SIZE: return "refr-ext-draw-size";
        case LV_EVENT_DRAW_MAIN_BEGIN: return "draw-main-begin";
        case LV_EVENT_DRAW_MAIN: return "draw-main";
        case LV_EVENT_DRAW_MAIN_END: return "draw-main-end";
        case LV_EVENT_DRAW_POST_BEGIN: return "draw-post-begin";
        case LV_EVENT_DRAW_POST: return "draw-post";
        case LV_EVENT_DRAW_POST_END: return "draw-post-end";
        case LV_EVENT_DRAW_TASK_ADDED: return "draw-task-added";
        case LV_EVENT_VALUE_CHANGED: return "value-changed";
        case LV_EVENT_INSERT: return "insert";
        case LV_EVENT_REFRESH: return "refresh";
        case LV_EVENT_READY: return "ready";
        case LV_EVENT_CANCEL: return "cancel";
        case LV_EVENT_CREATE: return "create";
        case LV_EVENT_DELETE: return "delete";
        case LV_EVENT_CHILD_CHANGED: return "child-changed";
        case LV_EVENT_CHILD_CREATED: return "child-created";
        case LV_EVENT_CHILD_DELETED: return "child-deleted";
        case LV_EVENT_SCREEN_UNLOAD_START: return "screen-unload-start";
        case LV_EVENT_SCREEN_LOAD_START: return "screen-load-start";
        case LV_EVENT_SCREEN_LOADED: return "screen-loaded";
        case LV_EVENT_SCREEN_UNLOADED: return "screen-unloaded";
        case LV_EVENT_SIZE_CHANGED: return "size-changed";
        case LV_EVENT_STYLE_CHANGED: return "style-changed";
        case LV_EVENT_LAYOUT_CHANGED: return "layout-changed";
        case LV_EVENT_GET_SELF_SIZE: return "get-self-size";
        case LV_EVENT_INVALIDATE_AREA: return "invalidate-area";
        case LV_EVENT_RESOLUTION_CHANGED: return "resolution-changed";
        case LV_EVENT_COLOR_FORMAT_CHANGED: return "color-format-changed";
        case LV_EVENT_REFR_REQUEST: return "refr-request";
        case LV_EVENT_REFR_START: return "refr-start";
        case LV_EVENT_REFR_READY: return "refr-ready";
        case LV_EVENT_RENDER_START: return "render-start";
        case LV_EVENT_RENDER_READY: return "render-ready";
        case LV_EVENT_FLUSH_START: return "flush-start";
        case LV_EVENT_FLUSH_FINISH: return "flush-finish";
        case LV_EVENT_VSYNC: return "vsync";
        case _LV_EVENT_LAST: return "last";
        case LV_EVENT_PREPROCESS: return "preprocess";

        default:
            return "unknow";
    }
}
JSValue lv_event_code_const_to_jsstr(JSContext *ctx, lv_event_code_t code) {
    return JS_NewString(ctx, lv_event_code_const_to_str(code));
}

// lv_anim_enable_t
bool lv_anim_enable_str_to_const(const char * name, lv_anim_enable_t * out) {
    if(strcmp(name,"off")==0) {
        (*out) = LV_ANIM_OFF ;
    }
    else if(strcmp(name,"on")==0) {
        (*out) = LV_ANIM_ON ;
    }

    else {
        return false ;
    }
    return true ;
}
bool lv_anim_enable_jsstr_to_const(JSContext *ctx, JSValue jsstr, lv_anim_enable_t* out) {
    char * cstr = (char *)JS_ToCString(ctx, jsstr) ;
    bool suc = lv_anim_enable_str_to_const(cstr, out) ;
    JS_FreeCString(ctx, cstr) ;
    return suc ;
}
const char * lv_anim_enable_const_to_str(lv_anim_enable_t code) {

    switch(code) {
        case LV_ANIM_OFF: return "off";
        case LV_ANIM_ON: return "on";

        default:
            return "unknow";
    }
}
JSValue lv_anim_enable_const_to_jsstr(JSContext *ctx, lv_anim_enable_t code) {
    return JS_NewString(ctx, lv_anim_enable_const_to_str(code));
}

// lv_style_prop_t
bool lv_style_prop_str_to_const(const char * name, lv_style_prop_t * out) {
    if(strcmp(name,"prop-inv")==0) {
        (*out) = LV_STYLE_PROP_INV ;
    }
    else if(strcmp(name,"width")==0) {
        (*out) = LV_STYLE_WIDTH ;
    }
    else if(strcmp(name,"height")==0) {
        (*out) = LV_STYLE_HEIGHT ;
    }
    else if(strcmp(name,"length")==0) {
        (*out) = LV_STYLE_LENGTH ;
    }
    else if(strcmp(name,"min-width")==0) {
        (*out) = LV_STYLE_MIN_WIDTH ;
    }
    else if(strcmp(name,"max-width")==0) {
        (*out) = LV_STYLE_MAX_WIDTH ;
    }
    else if(strcmp(name,"min-height")==0) {
        (*out) = LV_STYLE_MIN_HEIGHT ;
    }
    else if(strcmp(name,"max-height")==0) {
        (*out) = LV_STYLE_MAX_HEIGHT ;
    }
    else if(strcmp(name,"x")==0) {
        (*out) = LV_STYLE_X ;
    }
    else if(strcmp(name,"y")==0) {
        (*out) = LV_STYLE_Y ;
    }
    else if(strcmp(name,"align")==0) {
        (*out) = LV_STYLE_ALIGN ;
    }
    else if(strcmp(name,"radius")==0) {
        (*out) = LV_STYLE_RADIUS ;
    }
    else if(strcmp(name,"pad-top")==0) {
        (*out) = LV_STYLE_PAD_TOP ;
    }
    else if(strcmp(name,"pad-bottom")==0) {
        (*out) = LV_STYLE_PAD_BOTTOM ;
    }
    else if(strcmp(name,"pad-left")==0) {
        (*out) = LV_STYLE_PAD_LEFT ;
    }
    else if(strcmp(name,"pad-right")==0) {
        (*out) = LV_STYLE_PAD_RIGHT ;
    }
    else if(strcmp(name,"pad-row")==0) {
        (*out) = LV_STYLE_PAD_ROW ;
    }
    else if(strcmp(name,"pad-column")==0) {
        (*out) = LV_STYLE_PAD_COLUMN ;
    }
    else if(strcmp(name,"layout")==0) {
        (*out) = LV_STYLE_LAYOUT ;
    }
    else if(strcmp(name,"margin-top")==0) {
        (*out) = LV_STYLE_MARGIN_TOP ;
    }
    else if(strcmp(name,"margin-bottom")==0) {
        (*out) = LV_STYLE_MARGIN_BOTTOM ;
    }
    else if(strcmp(name,"margin-left")==0) {
        (*out) = LV_STYLE_MARGIN_LEFT ;
    }
    else if(strcmp(name,"margin-right")==0) {
        (*out) = LV_STYLE_MARGIN_RIGHT ;
    }
    else if(strcmp(name,"bg-color")==0) {
        (*out) = LV_STYLE_BG_COLOR ;
    }
    else if(strcmp(name,"bg-opa")==0) {
        (*out) = LV_STYLE_BG_OPA ;
    }
    else if(strcmp(name,"bg-grad-dir")==0) {
        (*out) = LV_STYLE_BG_GRAD_DIR ;
    }
    else if(strcmp(name,"bg-main-stop")==0) {
        (*out) = LV_STYLE_BG_MAIN_STOP ;
    }
    else if(strcmp(name,"bg-grad-stop")==0) {
        (*out) = LV_STYLE_BG_GRAD_STOP ;
    }
    else if(strcmp(name,"bg-grad-color")==0) {
        (*out) = LV_STYLE_BG_GRAD_COLOR ;
    }
    else if(strcmp(name,"bg-main-opa")==0) {
        (*out) = LV_STYLE_BG_MAIN_OPA ;
    }
    else if(strcmp(name,"bg-grad-opa")==0) {
        (*out) = LV_STYLE_BG_GRAD_OPA ;
    }
    else if(strcmp(name,"bg-grad")==0) {
        (*out) = LV_STYLE_BG_GRAD ;
    }
    else if(strcmp(name,"base-dir")==0) {
        (*out) = LV_STYLE_BASE_DIR ;
    }
    else if(strcmp(name,"bg-image-src")==0) {
        (*out) = LV_STYLE_BG_IMAGE_SRC ;
    }
    else if(strcmp(name,"bg-image-opa")==0) {
        (*out) = LV_STYLE_BG_IMAGE_OPA ;
    }
    else if(strcmp(name,"bg-image-recolor")==0) {
        (*out) = LV_STYLE_BG_IMAGE_RECOLOR ;
    }
    else if(strcmp(name,"bg-image-recolor-opa")==0) {
        (*out) = LV_STYLE_BG_IMAGE_RECOLOR_OPA ;
    }
    else if(strcmp(name,"bg-image-tiled")==0) {
        (*out) = LV_STYLE_BG_IMAGE_TILED ;
    }
    else if(strcmp(name,"clip-corner")==0) {
        (*out) = LV_STYLE_CLIP_CORNER ;
    }
    else if(strcmp(name,"border-width")==0) {
        (*out) = LV_STYLE_BORDER_WIDTH ;
    }
    else if(strcmp(name,"border-color")==0) {
        (*out) = LV_STYLE_BORDER_COLOR ;
    }
    else if(strcmp(name,"border-opa")==0) {
        (*out) = LV_STYLE_BORDER_OPA ;
    }
    else if(strcmp(name,"border-side")==0) {
        (*out) = LV_STYLE_BORDER_SIDE ;
    }
    else if(strcmp(name,"border-post")==0) {
        (*out) = LV_STYLE_BORDER_POST ;
    }
    else if(strcmp(name,"outline-width")==0) {
        (*out) = LV_STYLE_OUTLINE_WIDTH ;
    }
    else if(strcmp(name,"outline-color")==0) {
        (*out) = LV_STYLE_OUTLINE_COLOR ;
    }
    else if(strcmp(name,"outline-opa")==0) {
        (*out) = LV_STYLE_OUTLINE_OPA ;
    }
    else if(strcmp(name,"outline-pad")==0) {
        (*out) = LV_STYLE_OUTLINE_PAD ;
    }
    else if(strcmp(name,"shadow-width")==0) {
        (*out) = LV_STYLE_SHADOW_WIDTH ;
    }
    else if(strcmp(name,"shadow-color")==0) {
        (*out) = LV_STYLE_SHADOW_COLOR ;
    }
    else if(strcmp(name,"shadow-opa")==0) {
        (*out) = LV_STYLE_SHADOW_OPA ;
    }
    else if(strcmp(name,"shadow-offset-x")==0) {
        (*out) = LV_STYLE_SHADOW_OFFSET_X ;
    }
    else if(strcmp(name,"shadow-offset-y")==0) {
        (*out) = LV_STYLE_SHADOW_OFFSET_Y ;
    }
    else if(strcmp(name,"shadow-spread")==0) {
        (*out) = LV_STYLE_SHADOW_SPREAD ;
    }
    else if(strcmp(name,"image-opa")==0) {
        (*out) = LV_STYLE_IMAGE_OPA ;
    }
    else if(strcmp(name,"image-recolor")==0) {
        (*out) = LV_STYLE_IMAGE_RECOLOR ;
    }
    else if(strcmp(name,"image-recolor-opa")==0) {
        (*out) = LV_STYLE_IMAGE_RECOLOR_OPA ;
    }
    else if(strcmp(name,"line-width")==0) {
        (*out) = LV_STYLE_LINE_WIDTH ;
    }
    else if(strcmp(name,"line-dash-width")==0) {
        (*out) = LV_STYLE_LINE_DASH_WIDTH ;
    }
    else if(strcmp(name,"line-dash-gap")==0) {
        (*out) = LV_STYLE_LINE_DASH_GAP ;
    }
    else if(strcmp(name,"line-rounded")==0) {
        (*out) = LV_STYLE_LINE_ROUNDED ;
    }
    else if(strcmp(name,"line-color")==0) {
        (*out) = LV_STYLE_LINE_COLOR ;
    }
    else if(strcmp(name,"line-opa")==0) {
        (*out) = LV_STYLE_LINE_OPA ;
    }
    else if(strcmp(name,"arc-width")==0) {
        (*out) = LV_STYLE_ARC_WIDTH ;
    }
    else if(strcmp(name,"arc-rounded")==0) {
        (*out) = LV_STYLE_ARC_ROUNDED ;
    }
    else if(strcmp(name,"arc-color")==0) {
        (*out) = LV_STYLE_ARC_COLOR ;
    }
    else if(strcmp(name,"arc-opa")==0) {
        (*out) = LV_STYLE_ARC_OPA ;
    }
    else if(strcmp(name,"arc-image-src")==0) {
        (*out) = LV_STYLE_ARC_IMAGE_SRC ;
    }
    else if(strcmp(name,"text-color")==0) {
        (*out) = LV_STYLE_TEXT_COLOR ;
    }
    else if(strcmp(name,"text-opa")==0) {
        (*out) = LV_STYLE_TEXT_OPA ;
    }
    else if(strcmp(name,"text-font")==0) {
        (*out) = LV_STYLE_TEXT_FONT ;
    }
    else if(strcmp(name,"text-letter-space")==0) {
        (*out) = LV_STYLE_TEXT_LETTER_SPACE ;
    }
    else if(strcmp(name,"text-line-space")==0) {
        (*out) = LV_STYLE_TEXT_LINE_SPACE ;
    }
    else if(strcmp(name,"text-decor")==0) {
        (*out) = LV_STYLE_TEXT_DECOR ;
    }
    else if(strcmp(name,"text-align")==0) {
        (*out) = LV_STYLE_TEXT_ALIGN ;
    }
    else if(strcmp(name,"opa")==0) {
        (*out) = LV_STYLE_OPA ;
    }
    else if(strcmp(name,"opa-layered")==0) {
        (*out) = LV_STYLE_OPA_LAYERED ;
    }
    else if(strcmp(name,"color-filter-dsc")==0) {
        (*out) = LV_STYLE_COLOR_FILTER_DSC ;
    }
    else if(strcmp(name,"color-filter-opa")==0) {
        (*out) = LV_STYLE_COLOR_FILTER_OPA ;
    }
    else if(strcmp(name,"anim")==0) {
        (*out) = LV_STYLE_ANIM ;
    }
    else if(strcmp(name,"anim-duration")==0) {
        (*out) = LV_STYLE_ANIM_DURATION ;
    }
    else if(strcmp(name,"transition")==0) {
        (*out) = LV_STYLE_TRANSITION ;
    }
    else if(strcmp(name,"blend-mode")==0) {
        (*out) = LV_STYLE_BLEND_MODE ;
    }
    else if(strcmp(name,"transform-width")==0) {
        (*out) = LV_STYLE_TRANSFORM_WIDTH ;
    }
    else if(strcmp(name,"transform-height")==0) {
        (*out) = LV_STYLE_TRANSFORM_HEIGHT ;
    }
    else if(strcmp(name,"translate-x")==0) {
        (*out) = LV_STYLE_TRANSLATE_X ;
    }
    else if(strcmp(name,"translate-y")==0) {
        (*out) = LV_STYLE_TRANSLATE_Y ;
    }
    else if(strcmp(name,"transform-scale-x")==0) {
        (*out) = LV_STYLE_TRANSFORM_SCALE_X ;
    }
    else if(strcmp(name,"transform-scale-y")==0) {
        (*out) = LV_STYLE_TRANSFORM_SCALE_Y ;
    }
    else if(strcmp(name,"transform-rotation")==0) {
        (*out) = LV_STYLE_TRANSFORM_ROTATION ;
    }
    else if(strcmp(name,"transform-pivot-x")==0) {
        (*out) = LV_STYLE_TRANSFORM_PIVOT_X ;
    }
    else if(strcmp(name,"transform-pivot-y")==0) {
        (*out) = LV_STYLE_TRANSFORM_PIVOT_Y ;
    }
    else if(strcmp(name,"transform-skew-x")==0) {
        (*out) = LV_STYLE_TRANSFORM_SKEW_X ;
    }
    else if(strcmp(name,"transform-skew-y")==0) {
        (*out) = LV_STYLE_TRANSFORM_SKEW_Y ;
    }
    else if(strcmp(name,"flex-flow")==0) {
        (*out) = LV_STYLE_FLEX_FLOW ;
    }
    else if(strcmp(name,"flex-main-place")==0) {
        (*out) = LV_STYLE_FLEX_MAIN_PLACE ;
    }
    else if(strcmp(name,"flex-cross-place")==0) {
        (*out) = LV_STYLE_FLEX_CROSS_PLACE ;
    }
    else if(strcmp(name,"flex-track-place")==0) {
        (*out) = LV_STYLE_FLEX_TRACK_PLACE ;
    }
    else if(strcmp(name,"flex-grow")==0) {
        (*out) = LV_STYLE_FLEX_GROW ;
    }
    else if(strcmp(name,"grid-column-align")==0) {
        (*out) = LV_STYLE_GRID_COLUMN_ALIGN ;
    }
    else if(strcmp(name,"grid-row-align")==0) {
        (*out) = LV_STYLE_GRID_ROW_ALIGN ;
    }
    else if(strcmp(name,"grid-row-dsc-array")==0) {
        (*out) = LV_STYLE_GRID_ROW_DSC_ARRAY ;
    }
    else if(strcmp(name,"grid-column-dsc-array")==0) {
        (*out) = LV_STYLE_GRID_COLUMN_DSC_ARRAY ;
    }
    else if(strcmp(name,"grid-cell-column-pos")==0) {
        (*out) = LV_STYLE_GRID_CELL_COLUMN_POS ;
    }
    else if(strcmp(name,"grid-cell-column-span")==0) {
        (*out) = LV_STYLE_GRID_CELL_COLUMN_SPAN ;
    }
    else if(strcmp(name,"grid-cell-x-align")==0) {
        (*out) = LV_STYLE_GRID_CELL_X_ALIGN ;
    }
    else if(strcmp(name,"grid-cell-row-pos")==0) {
        (*out) = LV_STYLE_GRID_CELL_ROW_POS ;
    }
    else if(strcmp(name,"grid-cell-row-span")==0) {
        (*out) = LV_STYLE_GRID_CELL_ROW_SPAN ;
    }
    else if(strcmp(name,"grid-cell-y-align")==0) {
        (*out) = LV_STYLE_GRID_CELL_Y_ALIGN ;
    }
    else if(strcmp(name,"last-built-in-prop")==0) {
        (*out) = _LV_STYLE_LAST_BUILT_IN_PROP ;
    }
    else if(strcmp(name,"num-built-in-props")==0) {
        (*out) = _LV_STYLE_NUM_BUILT_IN_PROPS ;
    }
    else if(strcmp(name,"prop-any")==0) {
        (*out) = LV_STYLE_PROP_ANY ;
    }
    else if(strcmp(name,"prop-const")==0) {
        (*out) = _LV_STYLE_PROP_CONST ;
    }

    else {
        return false ;
    }
    return true ;
}
bool lv_style_prop_jsstr_to_const(JSContext *ctx, JSValue jsstr, lv_style_prop_t* out) {
    char * cstr = (char *)JS_ToCString(ctx, jsstr) ;
    bool suc = lv_style_prop_str_to_const(cstr, out) ;
    JS_FreeCString(ctx, cstr) ;
    return suc ;
}
const char * lv_style_prop_const_to_str(lv_style_prop_t code) {

    switch(code) {
        case LV_STYLE_PROP_INV: return "prop-inv";
        case LV_STYLE_WIDTH: return "width";
        case LV_STYLE_HEIGHT: return "height";
        case LV_STYLE_LENGTH: return "length";
        case LV_STYLE_MIN_WIDTH: return "min-width";
        case LV_STYLE_MAX_WIDTH: return "max-width";
        case LV_STYLE_MIN_HEIGHT: return "min-height";
        case LV_STYLE_MAX_HEIGHT: return "max-height";
        case LV_STYLE_X: return "x";
        case LV_STYLE_Y: return "y";
        case LV_STYLE_ALIGN: return "align";
        case LV_STYLE_RADIUS: return "radius";
        case LV_STYLE_PAD_TOP: return "pad-top";
        case LV_STYLE_PAD_BOTTOM: return "pad-bottom";
        case LV_STYLE_PAD_LEFT: return "pad-left";
        case LV_STYLE_PAD_RIGHT: return "pad-right";
        case LV_STYLE_PAD_ROW: return "pad-row";
        case LV_STYLE_PAD_COLUMN: return "pad-column";
        case LV_STYLE_LAYOUT: return "layout";
        case LV_STYLE_MARGIN_TOP: return "margin-top";
        case LV_STYLE_MARGIN_BOTTOM: return "margin-bottom";
        case LV_STYLE_MARGIN_LEFT: return "margin-left";
        case LV_STYLE_MARGIN_RIGHT: return "margin-right";
        case LV_STYLE_BG_COLOR: return "bg-color";
        case LV_STYLE_BG_OPA: return "bg-opa";
        case LV_STYLE_BG_GRAD_DIR: return "bg-grad-dir";
        case LV_STYLE_BG_MAIN_STOP: return "bg-main-stop";
        case LV_STYLE_BG_GRAD_STOP: return "bg-grad-stop";
        case LV_STYLE_BG_GRAD_COLOR: return "bg-grad-color";
        case LV_STYLE_BG_MAIN_OPA: return "bg-main-opa";
        case LV_STYLE_BG_GRAD_OPA: return "bg-grad-opa";
        case LV_STYLE_BG_GRAD: return "bg-grad";
        case LV_STYLE_BASE_DIR: return "base-dir";
        case LV_STYLE_BG_IMAGE_SRC: return "bg-image-src";
        case LV_STYLE_BG_IMAGE_OPA: return "bg-image-opa";
        case LV_STYLE_BG_IMAGE_RECOLOR: return "bg-image-recolor";
        case LV_STYLE_BG_IMAGE_RECOLOR_OPA: return "bg-image-recolor-opa";
        case LV_STYLE_BG_IMAGE_TILED: return "bg-image-tiled";
        case LV_STYLE_CLIP_CORNER: return "clip-corner";
        case LV_STYLE_BORDER_WIDTH: return "border-width";
        case LV_STYLE_BORDER_COLOR: return "border-color";
        case LV_STYLE_BORDER_OPA: return "border-opa";
        case LV_STYLE_BORDER_SIDE: return "border-side";
        case LV_STYLE_BORDER_POST: return "border-post";
        case LV_STYLE_OUTLINE_WIDTH: return "outline-width";
        case LV_STYLE_OUTLINE_COLOR: return "outline-color";
        case LV_STYLE_OUTLINE_OPA: return "outline-opa";
        case LV_STYLE_OUTLINE_PAD: return "outline-pad";
        case LV_STYLE_SHADOW_WIDTH: return "shadow-width";
        case LV_STYLE_SHADOW_COLOR: return "shadow-color";
        case LV_STYLE_SHADOW_OPA: return "shadow-opa";
        case LV_STYLE_SHADOW_OFFSET_X: return "shadow-offset-x";
        case LV_STYLE_SHADOW_OFFSET_Y: return "shadow-offset-y";
        case LV_STYLE_SHADOW_SPREAD: return "shadow-spread";
        case LV_STYLE_IMAGE_OPA: return "image-opa";
        case LV_STYLE_IMAGE_RECOLOR: return "image-recolor";
        case LV_STYLE_IMAGE_RECOLOR_OPA: return "image-recolor-opa";
        case LV_STYLE_LINE_WIDTH: return "line-width";
        case LV_STYLE_LINE_DASH_WIDTH: return "line-dash-width";
        case LV_STYLE_LINE_DASH_GAP: return "line-dash-gap";
        case LV_STYLE_LINE_ROUNDED: return "line-rounded";
        case LV_STYLE_LINE_COLOR: return "line-color";
        case LV_STYLE_LINE_OPA: return "line-opa";
        case LV_STYLE_ARC_WIDTH: return "arc-width";
        case LV_STYLE_ARC_ROUNDED: return "arc-rounded";
        case LV_STYLE_ARC_COLOR: return "arc-color";
        case LV_STYLE_ARC_OPA: return "arc-opa";
        case LV_STYLE_ARC_IMAGE_SRC: return "arc-image-src";
        case LV_STYLE_TEXT_COLOR: return "text-color";
        case LV_STYLE_TEXT_OPA: return "text-opa";
        case LV_STYLE_TEXT_FONT: return "text-font";
        case LV_STYLE_TEXT_LETTER_SPACE: return "text-letter-space";
        case LV_STYLE_TEXT_LINE_SPACE: return "text-line-space";
        case LV_STYLE_TEXT_DECOR: return "text-decor";
        case LV_STYLE_TEXT_ALIGN: return "text-align";
        case LV_STYLE_OPA: return "opa";
        case LV_STYLE_OPA_LAYERED: return "opa-layered";
        case LV_STYLE_COLOR_FILTER_DSC: return "color-filter-dsc";
        case LV_STYLE_COLOR_FILTER_OPA: return "color-filter-opa";
        case LV_STYLE_ANIM: return "anim";
        case LV_STYLE_ANIM_DURATION: return "anim-duration";
        case LV_STYLE_TRANSITION: return "transition";
        case LV_STYLE_BLEND_MODE: return "blend-mode";
        case LV_STYLE_TRANSFORM_WIDTH: return "transform-width";
        case LV_STYLE_TRANSFORM_HEIGHT: return "transform-height";
        case LV_STYLE_TRANSLATE_X: return "translate-x";
        case LV_STYLE_TRANSLATE_Y: return "translate-y";
        case LV_STYLE_TRANSFORM_SCALE_X: return "transform-scale-x";
        case LV_STYLE_TRANSFORM_SCALE_Y: return "transform-scale-y";
        case LV_STYLE_TRANSFORM_ROTATION: return "transform-rotation";
        case LV_STYLE_TRANSFORM_PIVOT_X: return "transform-pivot-x";
        case LV_STYLE_TRANSFORM_PIVOT_Y: return "transform-pivot-y";
        case LV_STYLE_TRANSFORM_SKEW_X: return "transform-skew-x";
        case LV_STYLE_TRANSFORM_SKEW_Y: return "transform-skew-y";
        case LV_STYLE_FLEX_FLOW: return "flex-flow";
        case LV_STYLE_FLEX_MAIN_PLACE: return "flex-main-place";
        case LV_STYLE_FLEX_CROSS_PLACE: return "flex-cross-place";
        case LV_STYLE_FLEX_TRACK_PLACE: return "flex-track-place";
        case LV_STYLE_FLEX_GROW: return "flex-grow";
        case LV_STYLE_GRID_COLUMN_ALIGN: return "grid-column-align";
        case LV_STYLE_GRID_ROW_ALIGN: return "grid-row-align";
        case LV_STYLE_GRID_ROW_DSC_ARRAY: return "grid-row-dsc-array";
        case LV_STYLE_GRID_COLUMN_DSC_ARRAY: return "grid-column-dsc-array";
        case LV_STYLE_GRID_CELL_COLUMN_POS: return "grid-cell-column-pos";
        case LV_STYLE_GRID_CELL_COLUMN_SPAN: return "grid-cell-column-span";
        case LV_STYLE_GRID_CELL_X_ALIGN: return "grid-cell-x-align";
        case LV_STYLE_GRID_CELL_ROW_POS: return "grid-cell-row-pos";
        case LV_STYLE_GRID_CELL_ROW_SPAN: return "grid-cell-row-span";
        case LV_STYLE_GRID_CELL_Y_ALIGN: return "grid-cell-y-align";
        case _LV_STYLE_LAST_BUILT_IN_PROP: return "last-built-in-prop";
        case _LV_STYLE_NUM_BUILT_IN_PROPS: return "num-built-in-props";
        case LV_STYLE_PROP_ANY: return "prop-any";

        default:
            return "unknow";
    }
}
JSValue lv_style_prop_const_to_jsstr(JSContext *ctx, lv_style_prop_t code) {
    return JS_NewString(ctx, lv_style_prop_const_to_str(code));
}

// lv_opa_t
bool lv_opa_str_to_const(const char * name, lv_opa_t * out) {
    if(strcmp(name,"0")==0) {
        (*out) = LV_OPA_0 ;
    }
    else if(strcmp(name,"10")==0) {
        (*out) = LV_OPA_10 ;
    }
    else if(strcmp(name,"20")==0) {
        (*out) = LV_OPA_20 ;
    }
    else if(strcmp(name,"30")==0) {
        (*out) = LV_OPA_30 ;
    }
    else if(strcmp(name,"40")==0) {
        (*out) = LV_OPA_40 ;
    }
    else if(strcmp(name,"50")==0) {
        (*out) = LV_OPA_50 ;
    }
    else if(strcmp(name,"60")==0) {
        (*out) = LV_OPA_60 ;
    }
    else if(strcmp(name,"70")==0) {
        (*out) = LV_OPA_70 ;
    }
    else if(strcmp(name,"80")==0) {
        (*out) = LV_OPA_80 ;
    }
    else if(strcmp(name,"90")==0) {
        (*out) = LV_OPA_90 ;
    }
    else if(strcmp(name,"100")==0) {
        (*out) = LV_OPA_100 ;
    }

    else {
        return false ;
    }
    return true ;
}
bool lv_opa_jsstr_to_const(JSContext *ctx, JSValue jsstr, lv_opa_t* out) {
    char * cstr = (char *)JS_ToCString(ctx, jsstr) ;
    bool suc = lv_opa_str_to_const(cstr, out) ;
    JS_FreeCString(ctx, cstr) ;
    return suc ;
}
const char * lv_opa_const_to_str(lv_opa_t code) {

    switch(code) {
        case LV_OPA_0: return "0";
        case LV_OPA_10: return "10";
        case LV_OPA_20: return "20";
        case LV_OPA_30: return "30";
        case LV_OPA_40: return "40";
        case LV_OPA_50: return "50";
        case LV_OPA_60: return "60";
        case LV_OPA_70: return "70";
        case LV_OPA_80: return "80";
        case LV_OPA_90: return "90";
        case LV_OPA_100: return "100";

        default:
            return "unknow";
    }
}
JSValue lv_opa_const_to_jsstr(JSContext *ctx, lv_opa_t code) {
    return JS_NewString(ctx, lv_opa_const_to_str(code));
}

// lv_grad_dir_t
bool lv_grad_dir_str_to_const(const char * name, lv_grad_dir_t * out) {
    if(strcmp(name,"none")==0) {
        (*out) = LV_GRAD_DIR_NONE ;
    }
    else if(strcmp(name,"ver")==0) {
        (*out) = LV_GRAD_DIR_VER ;
    }
    else if(strcmp(name,"hor")==0) {
        (*out) = LV_GRAD_DIR_HOR ;
    }

    else {
        return false ;
    }
    return true ;
}
bool lv_grad_dir_jsstr_to_const(JSContext *ctx, JSValue jsstr, lv_grad_dir_t* out) {
    char * cstr = (char *)JS_ToCString(ctx, jsstr) ;
    bool suc = lv_grad_dir_str_to_const(cstr, out) ;
    JS_FreeCString(ctx, cstr) ;
    return suc ;
}
const char * lv_grad_dir_const_to_str(lv_grad_dir_t code) {

    switch(code) {
        case LV_GRAD_DIR_NONE: return "none";
        case LV_GRAD_DIR_VER: return "ver";
        case LV_GRAD_DIR_HOR: return "hor";

        default:
            return "unknow";
    }
}
JSValue lv_grad_dir_const_to_jsstr(JSContext *ctx, lv_grad_dir_t code) {
    return JS_NewString(ctx, lv_grad_dir_const_to_str(code));
}

// lv_text_decor_t
bool lv_text_decor_str_to_const(const char * name, lv_text_decor_t * out) {
    if(strcmp(name,"none")==0) {
        (*out) = LV_TEXT_DECOR_NONE ;
    }
    else if(strcmp(name,"underline")==0) {
        (*out) = LV_TEXT_DECOR_UNDERLINE ;
    }
    else if(strcmp(name,"strikethrough")==0) {
        (*out) = LV_TEXT_DECOR_STRIKETHROUGH ;
    }

    else {
        return false ;
    }
    return true ;
}
bool lv_text_decor_jsstr_to_const(JSContext *ctx, JSValue jsstr, lv_text_decor_t* out) {
    char * cstr = (char *)JS_ToCString(ctx, jsstr) ;
    bool suc = lv_text_decor_str_to_const(cstr, out) ;
    JS_FreeCString(ctx, cstr) ;
    return suc ;
}
const char * lv_text_decor_const_to_str(lv_text_decor_t code) {

    switch(code) {
        case LV_TEXT_DECOR_NONE: return "none";
        case LV_TEXT_DECOR_UNDERLINE: return "underline";
        case LV_TEXT_DECOR_STRIKETHROUGH: return "strikethrough";

        default:
            return "unknow";
    }
}
JSValue lv_text_decor_const_to_jsstr(JSContext *ctx, lv_text_decor_t code) {
    return JS_NewString(ctx, lv_text_decor_const_to_str(code));
}

// lv_blend_mode_t
bool lv_blend_mode_str_to_const(const char * name, lv_blend_mode_t * out) {
    if(strcmp(name,"mode-normal")==0) {
        (*out) = LV_BLEND_MODE_NORMAL ;
    }
    else if(strcmp(name,"mode-additive")==0) {
        (*out) = LV_BLEND_MODE_ADDITIVE ;
    }
    else if(strcmp(name,"mode-subtractive")==0) {
        (*out) = LV_BLEND_MODE_SUBTRACTIVE ;
    }
    else if(strcmp(name,"mode-multiply")==0) {
        (*out) = LV_BLEND_MODE_MULTIPLY ;
    }

    else {
        return false ;
    }
    return true ;
}
bool lv_blend_mode_jsstr_to_const(JSContext *ctx, JSValue jsstr, lv_blend_mode_t* out) {
    char * cstr = (char *)JS_ToCString(ctx, jsstr) ;
    bool suc = lv_blend_mode_str_to_const(cstr, out) ;
    JS_FreeCString(ctx, cstr) ;
    return suc ;
}
const char * lv_blend_mode_const_to_str(lv_blend_mode_t code) {

    switch(code) {
        case LV_BLEND_MODE_NORMAL: return "mode-normal";
        case LV_BLEND_MODE_ADDITIVE: return "mode-additive";
        case LV_BLEND_MODE_SUBTRACTIVE: return "mode-subtractive";
        case LV_BLEND_MODE_MULTIPLY: return "mode-multiply";

        default:
            return "unknow";
    }
}
JSValue lv_blend_mode_const_to_jsstr(JSContext *ctx, lv_blend_mode_t code) {
    return JS_NewString(ctx, lv_blend_mode_const_to_str(code));
}

// lv_grid_align_t
bool lv_grid_align_str_to_const(const char * name, lv_grid_align_t * out) {
    if(strcmp(name,"start")==0) {
        (*out) = LV_GRID_ALIGN_START ;
    }
    else if(strcmp(name,"center")==0) {
        (*out) = LV_GRID_ALIGN_CENTER ;
    }
    else if(strcmp(name,"end")==0) {
        (*out) = LV_GRID_ALIGN_END ;
    }
    else if(strcmp(name,"stretch")==0) {
        (*out) = LV_GRID_ALIGN_STRETCH ;
    }
    else if(strcmp(name,"space-evenly")==0) {
        (*out) = LV_GRID_ALIGN_SPACE_EVENLY ;
    }
    else if(strcmp(name,"space-around")==0) {
        (*out) = LV_GRID_ALIGN_SPACE_AROUND ;
    }
    else if(strcmp(name,"space-between")==0) {
        (*out) = LV_GRID_ALIGN_SPACE_BETWEEN ;
    }

    else {
        return false ;
    }
    return true ;
}
bool lv_grid_align_jsstr_to_const(JSContext *ctx, JSValue jsstr, lv_grid_align_t* out) {
    char * cstr = (char *)JS_ToCString(ctx, jsstr) ;
    bool suc = lv_grid_align_str_to_const(cstr, out) ;
    JS_FreeCString(ctx, cstr) ;
    return suc ;
}
const char * lv_grid_align_const_to_str(lv_grid_align_t code) {

    switch(code) {
        case LV_GRID_ALIGN_START: return "start";
        case LV_GRID_ALIGN_CENTER: return "center";
        case LV_GRID_ALIGN_END: return "end";
        case LV_GRID_ALIGN_STRETCH: return "stretch";
        case LV_GRID_ALIGN_SPACE_EVENLY: return "space-evenly";
        case LV_GRID_ALIGN_SPACE_AROUND: return "space-around";
        case LV_GRID_ALIGN_SPACE_BETWEEN: return "space-between";

        default:
            return "unknow";
    }
}
JSValue lv_grid_align_const_to_jsstr(JSContext *ctx, lv_grid_align_t code) {
    return JS_NewString(ctx, lv_grid_align_const_to_str(code));
}

// lv_keyboard_mode_t
bool lv_keyboard_mode_str_to_const(const char * name, lv_keyboard_mode_t * out) {
    if(strcmp(name,"mode-text-lower")==0) {
        (*out) = LV_KEYBOARD_MODE_TEXT_LOWER ;
    }
    else if(strcmp(name,"mode-text-upper")==0) {
        (*out) = LV_KEYBOARD_MODE_TEXT_UPPER ;
    }
    else if(strcmp(name,"mode-special")==0) {
        (*out) = LV_KEYBOARD_MODE_SPECIAL ;
    }
    else if(strcmp(name,"mode-number")==0) {
        (*out) = LV_KEYBOARD_MODE_NUMBER ;
    }
    else if(strcmp(name,"mode-user-1")==0) {
        (*out) = LV_KEYBOARD_MODE_USER_1 ;
    }
    else if(strcmp(name,"mode-user-2")==0) {
        (*out) = LV_KEYBOARD_MODE_USER_2 ;
    }
    else if(strcmp(name,"mode-user-3")==0) {
        (*out) = LV_KEYBOARD_MODE_USER_3 ;
    }
    else if(strcmp(name,"mode-user-4")==0) {
        (*out) = LV_KEYBOARD_MODE_USER_4 ;
    }

    else {
        return false ;
    }
    return true ;
}
bool lv_keyboard_mode_jsstr_to_const(JSContext *ctx, JSValue jsstr, lv_keyboard_mode_t* out) {
    char * cstr = (char *)JS_ToCString(ctx, jsstr) ;
    bool suc = lv_keyboard_mode_str_to_const(cstr, out) ;
    JS_FreeCString(ctx, cstr) ;
    return suc ;
}
const char * lv_keyboard_mode_const_to_str(lv_keyboard_mode_t code) {

    switch(code) {
        case LV_KEYBOARD_MODE_TEXT_LOWER: return "mode-text-lower";
        case LV_KEYBOARD_MODE_TEXT_UPPER: return "mode-text-upper";
        case LV_KEYBOARD_MODE_SPECIAL: return "mode-special";
        case LV_KEYBOARD_MODE_NUMBER: return "mode-number";
        case LV_KEYBOARD_MODE_USER_1: return "mode-user-1";
        case LV_KEYBOARD_MODE_USER_2: return "mode-user-2";
        case LV_KEYBOARD_MODE_USER_3: return "mode-user-3";
        case LV_KEYBOARD_MODE_USER_4: return "mode-user-4";

        default:
            return "unknow";
    }
}
JSValue lv_keyboard_mode_const_to_jsstr(JSContext *ctx, lv_keyboard_mode_t code) {
    return JS_NewString(ctx, lv_keyboard_mode_const_to_str(code));
}

// lv_scale_mode_t
bool lv_scale_mode_str_to_const(const char * name, lv_scale_mode_t * out) {
    if(strcmp(name,"mode-horizontal-top")==0) {
        (*out) = LV_SCALE_MODE_HORIZONTAL_TOP ;
    }
    else if(strcmp(name,"mode-horizontal-bottom")==0) {
        (*out) = LV_SCALE_MODE_HORIZONTAL_BOTTOM ;
    }
    else if(strcmp(name,"mode-vertical-left")==0) {
        (*out) = LV_SCALE_MODE_VERTICAL_LEFT ;
    }
    else if(strcmp(name,"mode-vertical-right")==0) {
        (*out) = LV_SCALE_MODE_VERTICAL_RIGHT ;
    }
    else if(strcmp(name,"mode-round-inner")==0) {
        (*out) = LV_SCALE_MODE_ROUND_INNER ;
    }
    else if(strcmp(name,"mode-round-outer")==0) {
        (*out) = LV_SCALE_MODE_ROUND_OUTER ;
    }
    else if(strcmp(name,"mode-last")==0) {
        (*out) = _LV_SCALE_MODE_LAST ;
    }

    else {
        return false ;
    }
    return true ;
}
bool lv_scale_mode_jsstr_to_const(JSContext *ctx, JSValue jsstr, lv_scale_mode_t* out) {
    char * cstr = (char *)JS_ToCString(ctx, jsstr) ;
    bool suc = lv_scale_mode_str_to_const(cstr, out) ;
    JS_FreeCString(ctx, cstr) ;
    return suc ;
}
const char * lv_scale_mode_const_to_str(lv_scale_mode_t code) {

    switch(code) {
        case LV_SCALE_MODE_HORIZONTAL_TOP: return "mode-horizontal-top";
        case LV_SCALE_MODE_HORIZONTAL_BOTTOM: return "mode-horizontal-bottom";
        case LV_SCALE_MODE_VERTICAL_LEFT: return "mode-vertical-left";
        case LV_SCALE_MODE_VERTICAL_RIGHT: return "mode-vertical-right";
        case LV_SCALE_MODE_ROUND_INNER: return "mode-round-inner";
        case LV_SCALE_MODE_ROUND_OUTER: return "mode-round-outer";
        case _LV_SCALE_MODE_LAST: return "mode-last";

        default:
            return "unknow";
    }
}
JSValue lv_scale_mode_const_to_jsstr(JSContext *ctx, lv_scale_mode_t code) {
    return JS_NewString(ctx, lv_scale_mode_const_to_str(code));
}

// lv_table_cell_ctrl_t
bool lv_table_cell_ctrl_str_to_const(const char * name, lv_table_cell_ctrl_t * out) {
    if(strcmp(name,"merge-right")==0) {
        (*out) = LV_TABLE_CELL_CTRL_MERGE_RIGHT ;
    }
    else if(strcmp(name,"text-crop")==0) {
        (*out) = LV_TABLE_CELL_CTRL_TEXT_CROP ;
    }
    else if(strcmp(name,"custom-1")==0) {
        (*out) = LV_TABLE_CELL_CTRL_CUSTOM_1 ;
    }
    else if(strcmp(name,"custom-2")==0) {
        (*out) = LV_TABLE_CELL_CTRL_CUSTOM_2 ;
    }
    else if(strcmp(name,"custom-3")==0) {
        (*out) = LV_TABLE_CELL_CTRL_CUSTOM_3 ;
    }
    else if(strcmp(name,"custom-4")==0) {
        (*out) = LV_TABLE_CELL_CTRL_CUSTOM_4 ;
    }

    else {
        return false ;
    }
    return true ;
}
bool lv_table_cell_ctrl_jsstr_to_const(JSContext *ctx, JSValue jsstr, lv_table_cell_ctrl_t* out) {
    char * cstr = (char *)JS_ToCString(ctx, jsstr) ;
    bool suc = lv_table_cell_ctrl_str_to_const(cstr, out) ;
    JS_FreeCString(ctx, cstr) ;
    return suc ;
}
const char * lv_table_cell_ctrl_const_to_str(lv_table_cell_ctrl_t code) {

    switch(code) {
        case LV_TABLE_CELL_CTRL_MERGE_RIGHT: return "merge-right";
        case LV_TABLE_CELL_CTRL_TEXT_CROP: return "text-crop";
        case LV_TABLE_CELL_CTRL_CUSTOM_1: return "custom-1";
        case LV_TABLE_CELL_CTRL_CUSTOM_2: return "custom-2";
        case LV_TABLE_CELL_CTRL_CUSTOM_3: return "custom-3";
        case LV_TABLE_CELL_CTRL_CUSTOM_4: return "custom-4";

        default:
            return "unknow";
    }
}
JSValue lv_table_cell_ctrl_const_to_jsstr(JSContext *ctx, lv_table_cell_ctrl_t code) {
    return JS_NewString(ctx, lv_table_cell_ctrl_const_to_str(code));
}

// lv_arc_mode_t
bool lv_arc_mode_str_to_const(const char * name, lv_arc_mode_t * out) {
    if(strcmp(name,"mode-normal")==0) {
        (*out) = LV_ARC_MODE_NORMAL ;
    }
    else if(strcmp(name,"mode-symmetrical")==0) {
        (*out) = LV_ARC_MODE_SYMMETRICAL ;
    }
    else if(strcmp(name,"mode-reverse")==0) {
        (*out) = LV_ARC_MODE_REVERSE ;
    }

    else {
        return false ;
    }
    return true ;
}
bool lv_arc_mode_jsstr_to_const(JSContext *ctx, JSValue jsstr, lv_arc_mode_t* out) {
    char * cstr = (char *)JS_ToCString(ctx, jsstr) ;
    bool suc = lv_arc_mode_str_to_const(cstr, out) ;
    JS_FreeCString(ctx, cstr) ;
    return suc ;
}
const char * lv_arc_mode_const_to_str(lv_arc_mode_t code) {

    switch(code) {
        case LV_ARC_MODE_NORMAL: return "mode-normal";
        case LV_ARC_MODE_SYMMETRICAL: return "mode-symmetrical";
        case LV_ARC_MODE_REVERSE: return "mode-reverse";

        default:
            return "unknow";
    }
}
JSValue lv_arc_mode_const_to_jsstr(JSContext *ctx, lv_arc_mode_t code) {
    return JS_NewString(ctx, lv_arc_mode_const_to_str(code));
}

// lv_buttonmatrix_ctrl_t
bool lv_buttonmatrix_ctrl_str_to_const(const char * name, lv_buttonmatrix_ctrl_t * out) {
    if(strcmp(name,"h")==0) {
        (*out) = _LV_BUTTONMATRIX_WIDTH ;
    }
    else if(strcmp(name,"hidden")==0) {
        (*out) = LV_BUTTONMATRIX_CTRL_HIDDEN ;
    }
    else if(strcmp(name,"no-repeat")==0) {
        (*out) = LV_BUTTONMATRIX_CTRL_NO_REPEAT ;
    }
    else if(strcmp(name,"disabled")==0) {
        (*out) = LV_BUTTONMATRIX_CTRL_DISABLED ;
    }
    else if(strcmp(name,"checkable")==0) {
        (*out) = LV_BUTTONMATRIX_CTRL_CHECKABLE ;
    }
    else if(strcmp(name,"checked")==0) {
        (*out) = LV_BUTTONMATRIX_CTRL_CHECKED ;
    }
    else if(strcmp(name,"click-trig")==0) {
        (*out) = LV_BUTTONMATRIX_CTRL_CLICK_TRIG ;
    }
    else if(strcmp(name,"popover")==0) {
        (*out) = LV_BUTTONMATRIX_CTRL_POPOVER ;
    }
    else if(strcmp(name,"reserved-1")==0) {
        (*out) = _LV_BUTTONMATRIX_CTRL_RESERVED_1 ;
    }
    else if(strcmp(name,"reserved-2")==0) {
        (*out) = _LV_BUTTONMATRIX_CTRL_RESERVED_2 ;
    }
    else if(strcmp(name,"reserved-3")==0) {
        (*out) = _LV_BUTTONMATRIX_CTRL_RESERVED_3 ;
    }
    else if(strcmp(name,"custom-1")==0) {
        (*out) = LV_BUTTONMATRIX_CTRL_CUSTOM_1 ;
    }
    else if(strcmp(name,"custom-2")==0) {
        (*out) = LV_BUTTONMATRIX_CTRL_CUSTOM_2 ;
    }

    else {
        return false ;
    }
    return true ;
}
bool lv_buttonmatrix_ctrl_jsstr_to_const(JSContext *ctx, JSValue jsstr, lv_buttonmatrix_ctrl_t* out) {
    char * cstr = (char *)JS_ToCString(ctx, jsstr) ;
    bool suc = lv_buttonmatrix_ctrl_str_to_const(cstr, out) ;
    JS_FreeCString(ctx, cstr) ;
    return suc ;
}
const char * lv_buttonmatrix_ctrl_const_to_str(lv_buttonmatrix_ctrl_t code) {

    switch(code) {
        case _LV_BUTTONMATRIX_WIDTH: return "h";
        case LV_BUTTONMATRIX_CTRL_HIDDEN: return "hidden";
        case LV_BUTTONMATRIX_CTRL_NO_REPEAT: return "no-repeat";
        case LV_BUTTONMATRIX_CTRL_DISABLED: return "disabled";
        case LV_BUTTONMATRIX_CTRL_CHECKABLE: return "checkable";
        case LV_BUTTONMATRIX_CTRL_CHECKED: return "checked";
        case LV_BUTTONMATRIX_CTRL_CLICK_TRIG: return "click-trig";
        case LV_BUTTONMATRIX_CTRL_POPOVER: return "popover";
        case _LV_BUTTONMATRIX_CTRL_RESERVED_1: return "reserved-1";
        case _LV_BUTTONMATRIX_CTRL_RESERVED_2: return "reserved-2";
        case _LV_BUTTONMATRIX_CTRL_RESERVED_3: return "reserved-3";
        case LV_BUTTONMATRIX_CTRL_CUSTOM_1: return "custom-1";
        case LV_BUTTONMATRIX_CTRL_CUSTOM_2: return "custom-2";

        default:
            return "unknow";
    }
}
JSValue lv_buttonmatrix_ctrl_const_to_jsstr(JSContext *ctx, lv_buttonmatrix_ctrl_t code) {
    return JS_NewString(ctx, lv_buttonmatrix_ctrl_const_to_str(code));
}

// lv_chart_type_t
bool lv_chart_type_str_to_const(const char * name, lv_chart_type_t * out) {
    if(strcmp(name,"none")==0) {
        (*out) = LV_CHART_TYPE_NONE ;
    }
    else if(strcmp(name,"line")==0) {
        (*out) = LV_CHART_TYPE_LINE ;
    }
    else if(strcmp(name,"bar")==0) {
        (*out) = LV_CHART_TYPE_BAR ;
    }
    else if(strcmp(name,"scatter")==0) {
        (*out) = LV_CHART_TYPE_SCATTER ;
    }

    else {
        return false ;
    }
    return true ;
}
bool lv_chart_type_jsstr_to_const(JSContext *ctx, JSValue jsstr, lv_chart_type_t* out) {
    char * cstr = (char *)JS_ToCString(ctx, jsstr) ;
    bool suc = lv_chart_type_str_to_const(cstr, out) ;
    JS_FreeCString(ctx, cstr) ;
    return suc ;
}
const char * lv_chart_type_const_to_str(lv_chart_type_t code) {

    switch(code) {
        case LV_CHART_TYPE_NONE: return "none";
        case LV_CHART_TYPE_LINE: return "line";
        case LV_CHART_TYPE_BAR: return "bar";
        case LV_CHART_TYPE_SCATTER: return "scatter";

        default:
            return "unknow";
    }
}
JSValue lv_chart_type_const_to_jsstr(JSContext *ctx, lv_chart_type_t code) {
    return JS_NewString(ctx, lv_chart_type_const_to_str(code));
}

// lv_chart_update_mode_t
bool lv_chart_update_mode_str_to_const(const char * name, lv_chart_update_mode_t * out) {
    if(strcmp(name,"mode-shift")==0) {
        (*out) = LV_CHART_UPDATE_MODE_SHIFT ;
    }
    else if(strcmp(name,"mode-circular")==0) {
        (*out) = LV_CHART_UPDATE_MODE_CIRCULAR ;
    }

    else {
        return false ;
    }
    return true ;
}
bool lv_chart_update_mode_jsstr_to_const(JSContext *ctx, JSValue jsstr, lv_chart_update_mode_t* out) {
    char * cstr = (char *)JS_ToCString(ctx, jsstr) ;
    bool suc = lv_chart_update_mode_str_to_const(cstr, out) ;
    JS_FreeCString(ctx, cstr) ;
    return suc ;
}
const char * lv_chart_update_mode_const_to_str(lv_chart_update_mode_t code) {

    switch(code) {
        case LV_CHART_UPDATE_MODE_SHIFT: return "mode-shift";
        case LV_CHART_UPDATE_MODE_CIRCULAR: return "mode-circular";

        default:
            return "unknow";
    }
}
JSValue lv_chart_update_mode_const_to_jsstr(JSContext *ctx, lv_chart_update_mode_t code) {
    return JS_NewString(ctx, lv_chart_update_mode_const_to_str(code));
}

// lv_chart_axis_t
bool lv_chart_axis_str_to_const(const char * name, lv_chart_axis_t * out) {
    if(strcmp(name,"primary-y")==0) {
        (*out) = LV_CHART_AXIS_PRIMARY_Y ;
    }
    else if(strcmp(name,"secondary-y")==0) {
        (*out) = LV_CHART_AXIS_SECONDARY_Y ;
    }
    else if(strcmp(name,"primary-x")==0) {
        (*out) = LV_CHART_AXIS_PRIMARY_X ;
    }
    else if(strcmp(name,"secondary-x")==0) {
        (*out) = LV_CHART_AXIS_SECONDARY_X ;
    }
    else if(strcmp(name,"last")==0) {
        (*out) = _LV_CHART_AXIS_LAST ;
    }

    else {
        return false ;
    }
    return true ;
}
bool lv_chart_axis_jsstr_to_const(JSContext *ctx, JSValue jsstr, lv_chart_axis_t* out) {
    char * cstr = (char *)JS_ToCString(ctx, jsstr) ;
    bool suc = lv_chart_axis_str_to_const(cstr, out) ;
    JS_FreeCString(ctx, cstr) ;
    return suc ;
}
const char * lv_chart_axis_const_to_str(lv_chart_axis_t code) {

    switch(code) {
        case LV_CHART_AXIS_PRIMARY_Y: return "primary-y";
        case LV_CHART_AXIS_SECONDARY_Y: return "secondary-y";
        case LV_CHART_AXIS_PRIMARY_X: return "primary-x";
        case LV_CHART_AXIS_SECONDARY_X: return "secondary-x";
        case _LV_CHART_AXIS_LAST: return "last";

        default:
            return "unknow";
    }
}
JSValue lv_chart_axis_const_to_jsstr(JSContext *ctx, lv_chart_axis_t code) {
    return JS_NewString(ctx, lv_chart_axis_const_to_str(code));
}

// lv_image_align_t
bool lv_image_align_str_to_const(const char * name, lv_image_align_t * out) {
    if(strcmp(name,"default")==0) {
        (*out) = LV_IMAGE_ALIGN_DEFAULT ;
    }
    else if(strcmp(name,"top-left")==0) {
        (*out) = LV_IMAGE_ALIGN_TOP_LEFT ;
    }
    else if(strcmp(name,"top-mid")==0) {
        (*out) = LV_IMAGE_ALIGN_TOP_MID ;
    }
    else if(strcmp(name,"top-right")==0) {
        (*out) = LV_IMAGE_ALIGN_TOP_RIGHT ;
    }
    else if(strcmp(name,"bottom-left")==0) {
        (*out) = LV_IMAGE_ALIGN_BOTTOM_LEFT ;
    }
    else if(strcmp(name,"bottom-mid")==0) {
        (*out) = LV_IMAGE_ALIGN_BOTTOM_MID ;
    }
    else if(strcmp(name,"bottom-right")==0) {
        (*out) = LV_IMAGE_ALIGN_BOTTOM_RIGHT ;
    }
    else if(strcmp(name,"left-mid")==0) {
        (*out) = LV_IMAGE_ALIGN_LEFT_MID ;
    }
    else if(strcmp(name,"right-mid")==0) {
        (*out) = LV_IMAGE_ALIGN_RIGHT_MID ;
    }
    else if(strcmp(name,"center")==0) {
        (*out) = LV_IMAGE_ALIGN_CENTER ;
    }
    else if(strcmp(name,"auto-transform")==0) {
        (*out) = _LV_IMAGE_ALIGN_AUTO_TRANSFORM ;
    }
    else if(strcmp(name,"stretch")==0) {
        (*out) = LV_IMAGE_ALIGN_STRETCH ;
    }
    else if(strcmp(name,"tile")==0) {
        (*out) = LV_IMAGE_ALIGN_TILE ;
    }

    else {
        return false ;
    }
    return true ;
}
bool lv_image_align_jsstr_to_const(JSContext *ctx, JSValue jsstr, lv_image_align_t* out) {
    char * cstr = (char *)JS_ToCString(ctx, jsstr) ;
    bool suc = lv_image_align_str_to_const(cstr, out) ;
    JS_FreeCString(ctx, cstr) ;
    return suc ;
}
const char * lv_image_align_const_to_str(lv_image_align_t code) {

    switch(code) {
        case LV_IMAGE_ALIGN_DEFAULT: return "default";
        case LV_IMAGE_ALIGN_TOP_LEFT: return "top-left";
        case LV_IMAGE_ALIGN_TOP_MID: return "top-mid";
        case LV_IMAGE_ALIGN_TOP_RIGHT: return "top-right";
        case LV_IMAGE_ALIGN_BOTTOM_LEFT: return "bottom-left";
        case LV_IMAGE_ALIGN_BOTTOM_MID: return "bottom-mid";
        case LV_IMAGE_ALIGN_BOTTOM_RIGHT: return "bottom-right";
        case LV_IMAGE_ALIGN_LEFT_MID: return "left-mid";
        case LV_IMAGE_ALIGN_RIGHT_MID: return "right-mid";
        case LV_IMAGE_ALIGN_CENTER: return "center";
        case _LV_IMAGE_ALIGN_AUTO_TRANSFORM: return "auto-transform";
        case LV_IMAGE_ALIGN_STRETCH: return "stretch";
        case LV_IMAGE_ALIGN_TILE: return "tile";

        default:
            return "unknow";
    }
}
JSValue lv_image_align_const_to_jsstr(JSContext *ctx, lv_image_align_t code) {
    return JS_NewString(ctx, lv_image_align_const_to_str(code));
}

// lv_label_long_mode_t
bool lv_label_long_mode_str_to_const(const char * name, lv_label_long_mode_t * out) {
    if(strcmp(name,"wrap")==0) {
        (*out) = LV_LABEL_LONG_WRAP ;
    }
    else if(strcmp(name,"dot")==0) {
        (*out) = LV_LABEL_LONG_DOT ;
    }
    else if(strcmp(name,"scroll")==0) {
        (*out) = LV_LABEL_LONG_SCROLL ;
    }
    else if(strcmp(name,"scroll-circular")==0) {
        (*out) = LV_LABEL_LONG_SCROLL_CIRCULAR ;
    }
    else if(strcmp(name,"clip")==0) {
        (*out) = LV_LABEL_LONG_CLIP ;
    }

    else {
        return false ;
    }
    return true ;
}
bool lv_label_long_mode_jsstr_to_const(JSContext *ctx, JSValue jsstr, lv_label_long_mode_t* out) {
    char * cstr = (char *)JS_ToCString(ctx, jsstr) ;
    bool suc = lv_label_long_mode_str_to_const(cstr, out) ;
    JS_FreeCString(ctx, cstr) ;
    return suc ;
}
const char * lv_label_long_mode_const_to_str(lv_label_long_mode_t code) {

    switch(code) {
        case LV_LABEL_LONG_WRAP: return "wrap";
        case LV_LABEL_LONG_DOT: return "dot";
        case LV_LABEL_LONG_SCROLL: return "scroll";
        case LV_LABEL_LONG_SCROLL_CIRCULAR: return "scroll-circular";
        case LV_LABEL_LONG_CLIP: return "clip";

        default:
            return "unknow";
    }
}
JSValue lv_label_long_mode_const_to_jsstr(JSContext *ctx, lv_label_long_mode_t code) {
    return JS_NewString(ctx, lv_label_long_mode_const_to_str(code));
}

// lv_bar_mode_t
bool lv_bar_mode_str_to_const(const char * name, lv_bar_mode_t * out) {
    if(strcmp(name,"mode-normal")==0) {
        (*out) = LV_BAR_MODE_NORMAL ;
    }
    else if(strcmp(name,"mode-symmetrical")==0) {
        (*out) = LV_BAR_MODE_SYMMETRICAL ;
    }
    else if(strcmp(name,"mode-range")==0) {
        (*out) = LV_BAR_MODE_RANGE ;
    }

    else {
        return false ;
    }
    return true ;
}
bool lv_bar_mode_jsstr_to_const(JSContext *ctx, JSValue jsstr, lv_bar_mode_t* out) {
    char * cstr = (char *)JS_ToCString(ctx, jsstr) ;
    bool suc = lv_bar_mode_str_to_const(cstr, out) ;
    JS_FreeCString(ctx, cstr) ;
    return suc ;
}
const char * lv_bar_mode_const_to_str(lv_bar_mode_t code) {

    switch(code) {
        case LV_BAR_MODE_NORMAL: return "mode-normal";
        case LV_BAR_MODE_SYMMETRICAL: return "mode-symmetrical";
        case LV_BAR_MODE_RANGE: return "mode-range";

        default:
            return "unknow";
    }
}
JSValue lv_bar_mode_const_to_jsstr(JSContext *ctx, lv_bar_mode_t code) {
    return JS_NewString(ctx, lv_bar_mode_const_to_str(code));
}

// lv_imagebutton_state_t
bool lv_imagebutton_state_str_to_const(const char * name, lv_imagebutton_state_t * out) {
    if(strcmp(name,"released")==0) {
        (*out) = LV_IMAGEBUTTON_STATE_RELEASED ;
    }
    else if(strcmp(name,"pressed")==0) {
        (*out) = LV_IMAGEBUTTON_STATE_PRESSED ;
    }
    else if(strcmp(name,"disabled")==0) {
        (*out) = LV_IMAGEBUTTON_STATE_DISABLED ;
    }
    else if(strcmp(name,"checked-released")==0) {
        (*out) = LV_IMAGEBUTTON_STATE_CHECKED_RELEASED ;
    }
    else if(strcmp(name,"checked-pressed")==0) {
        (*out) = LV_IMAGEBUTTON_STATE_CHECKED_PRESSED ;
    }
    else if(strcmp(name,"checked-disabled")==0) {
        (*out) = LV_IMAGEBUTTON_STATE_CHECKED_DISABLED ;
    }
    else if(strcmp(name,"num")==0) {
        (*out) = _LV_IMAGEBUTTON_STATE_NUM ;
    }

    else {
        return false ;
    }
    return true ;
}
bool lv_imagebutton_state_jsstr_to_const(JSContext *ctx, JSValue jsstr, lv_imagebutton_state_t* out) {
    char * cstr = (char *)JS_ToCString(ctx, jsstr) ;
    bool suc = lv_imagebutton_state_str_to_const(cstr, out) ;
    JS_FreeCString(ctx, cstr) ;
    return suc ;
}
const char * lv_imagebutton_state_const_to_str(lv_imagebutton_state_t code) {

    switch(code) {
        case LV_IMAGEBUTTON_STATE_RELEASED: return "released";
        case LV_IMAGEBUTTON_STATE_PRESSED: return "pressed";
        case LV_IMAGEBUTTON_STATE_DISABLED: return "disabled";
        case LV_IMAGEBUTTON_STATE_CHECKED_RELEASED: return "checked-released";
        case LV_IMAGEBUTTON_STATE_CHECKED_PRESSED: return "checked-pressed";
        case LV_IMAGEBUTTON_STATE_CHECKED_DISABLED: return "checked-disabled";
        case _LV_IMAGEBUTTON_STATE_NUM: return "num";

        default:
            return "unknow";
    }
}
JSValue lv_imagebutton_state_const_to_jsstr(JSContext *ctx, lv_imagebutton_state_t code) {
    return JS_NewString(ctx, lv_imagebutton_state_const_to_str(code));
}

// lv_menu_mode_header_t
bool lv_menu_mode_header_str_to_const(const char * name, lv_menu_mode_header_t * out) {
    if(strcmp(name,"top-fixed")==0) {
        (*out) = LV_MENU_HEADER_TOP_FIXED ;
    }
    else if(strcmp(name,"top-unfixed")==0) {
        (*out) = LV_MENU_HEADER_TOP_UNFIXED ;
    }
    else if(strcmp(name,"bottom-fixed")==0) {
        (*out) = LV_MENU_HEADER_BOTTOM_FIXED ;
    }

    else {
        return false ;
    }
    return true ;
}
bool lv_menu_mode_header_jsstr_to_const(JSContext *ctx, JSValue jsstr, lv_menu_mode_header_t* out) {
    char * cstr = (char *)JS_ToCString(ctx, jsstr) ;
    bool suc = lv_menu_mode_header_str_to_const(cstr, out) ;
    JS_FreeCString(ctx, cstr) ;
    return suc ;
}
const char * lv_menu_mode_header_const_to_str(lv_menu_mode_header_t code) {

    switch(code) {
        case LV_MENU_HEADER_TOP_FIXED: return "top-fixed";
        case LV_MENU_HEADER_TOP_UNFIXED: return "top-unfixed";
        case LV_MENU_HEADER_BOTTOM_FIXED: return "bottom-fixed";

        default:
            return "unknow";
    }
}
JSValue lv_menu_mode_header_const_to_jsstr(JSContext *ctx, lv_menu_mode_header_t code) {
    return JS_NewString(ctx, lv_menu_mode_header_const_to_str(code));
}

// lv_menu_mode_root_back_button_t
bool lv_menu_mode_root_back_button_str_to_const(const char * name, lv_menu_mode_root_back_button_t * out) {
    if(strcmp(name,"disabled")==0) {
        (*out) = LV_MENU_ROOT_BACK_BUTTON_DISABLED ;
    }
    else if(strcmp(name,"enabled")==0) {
        (*out) = LV_MENU_ROOT_BACK_BUTTON_ENABLED ;
    }

    else {
        return false ;
    }
    return true ;
}
bool lv_menu_mode_root_back_button_jsstr_to_const(JSContext *ctx, JSValue jsstr, lv_menu_mode_root_back_button_t* out) {
    char * cstr = (char *)JS_ToCString(ctx, jsstr) ;
    bool suc = lv_menu_mode_root_back_button_str_to_const(cstr, out) ;
    JS_FreeCString(ctx, cstr) ;
    return suc ;
}
const char * lv_menu_mode_root_back_button_const_to_str(lv_menu_mode_root_back_button_t code) {

    switch(code) {
        case LV_MENU_ROOT_BACK_BUTTON_DISABLED: return "disabled";
        case LV_MENU_ROOT_BACK_BUTTON_ENABLED: return "enabled";

        default:
            return "unknow";
    }
}
JSValue lv_menu_mode_root_back_button_const_to_jsstr(JSContext *ctx, lv_menu_mode_root_back_button_t code) {
    return JS_NewString(ctx, lv_menu_mode_root_back_button_const_to_str(code));
}

// lv_roller_mode_t
bool lv_roller_mode_str_to_const(const char * name, lv_roller_mode_t * out) {
    if(strcmp(name,"mode-normal")==0) {
        (*out) = LV_ROLLER_MODE_NORMAL ;
    }
    else if(strcmp(name,"mode-infinite")==0) {
        (*out) = LV_ROLLER_MODE_INFINITE ;
    }

    else {
        return false ;
    }
    return true ;
}
bool lv_roller_mode_jsstr_to_const(JSContext *ctx, JSValue jsstr, lv_roller_mode_t* out) {
    char * cstr = (char *)JS_ToCString(ctx, jsstr) ;
    bool suc = lv_roller_mode_str_to_const(cstr, out) ;
    JS_FreeCString(ctx, cstr) ;
    return suc ;
}
const char * lv_roller_mode_const_to_str(lv_roller_mode_t code) {

    switch(code) {
        case LV_ROLLER_MODE_NORMAL: return "mode-normal";
        case LV_ROLLER_MODE_INFINITE: return "mode-infinite";

        default:
            return "unknow";
    }
}
JSValue lv_roller_mode_const_to_jsstr(JSContext *ctx, lv_roller_mode_t code) {
    return JS_NewString(ctx, lv_roller_mode_const_to_str(code));
}

// lv_span_overflow_t
bool lv_span_overflow_str_to_const(const char * name, lv_span_overflow_t * out) {
    if(strcmp(name,"clip")==0) {
        (*out) = LV_SPAN_OVERFLOW_CLIP ;
    }
    else if(strcmp(name,"ellipsis")==0) {
        (*out) = LV_SPAN_OVERFLOW_ELLIPSIS ;
    }
    else if(strcmp(name,"last")==0) {
        (*out) = _LV_SPAN_OVERFLOW_LAST ;
    }

    else {
        return false ;
    }
    return true ;
}
bool lv_span_overflow_jsstr_to_const(JSContext *ctx, JSValue jsstr, lv_span_overflow_t* out) {
    char * cstr = (char *)JS_ToCString(ctx, jsstr) ;
    bool suc = lv_span_overflow_str_to_const(cstr, out) ;
    JS_FreeCString(ctx, cstr) ;
    return suc ;
}
const char * lv_span_overflow_const_to_str(lv_span_overflow_t code) {

    switch(code) {
        case LV_SPAN_OVERFLOW_CLIP: return "clip";
        case LV_SPAN_OVERFLOW_ELLIPSIS: return "ellipsis";
        case _LV_SPAN_OVERFLOW_LAST: return "last";

        default:
            return "unknow";
    }
}
JSValue lv_span_overflow_const_to_jsstr(JSContext *ctx, lv_span_overflow_t code) {
    return JS_NewString(ctx, lv_span_overflow_const_to_str(code));
}

// lv_span_mode_t
bool lv_span_mode_str_to_const(const char * name, lv_span_mode_t * out) {
    if(strcmp(name,"mode-fixed")==0) {
        (*out) = LV_SPAN_MODE_FIXED ;
    }
    else if(strcmp(name,"mode-expand")==0) {
        (*out) = LV_SPAN_MODE_EXPAND ;
    }
    else if(strcmp(name,"mode-break")==0) {
        (*out) = LV_SPAN_MODE_BREAK ;
    }
    else if(strcmp(name,"mode-last")==0) {
        (*out) = _LV_SPAN_MODE_LAST ;
    }

    else {
        return false ;
    }
    return true ;
}
bool lv_span_mode_jsstr_to_const(JSContext *ctx, JSValue jsstr, lv_span_mode_t* out) {
    char * cstr = (char *)JS_ToCString(ctx, jsstr) ;
    bool suc = lv_span_mode_str_to_const(cstr, out) ;
    JS_FreeCString(ctx, cstr) ;
    return suc ;
}
const char * lv_span_mode_const_to_str(lv_span_mode_t code) {

    switch(code) {
        case LV_SPAN_MODE_FIXED: return "mode-fixed";
        case LV_SPAN_MODE_EXPAND: return "mode-expand";
        case LV_SPAN_MODE_BREAK: return "mode-break";
        case _LV_SPAN_MODE_LAST: return "mode-last";

        default:
            return "unknow";
    }
}
JSValue lv_span_mode_const_to_jsstr(JSContext *ctx, lv_span_mode_t code) {
    return JS_NewString(ctx, lv_span_mode_const_to_str(code));
}
// AUTO GENERATE CODE END [CONST MAPPING] --------

}
