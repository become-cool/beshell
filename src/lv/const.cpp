#include "const.hpp"


namespace be::lv {

// AUTO GENERATE CODE START [CONST MAPPING] --------

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

// lv_event_code_t
bool lv_event_code_str_to_const(const char * name, lv_event_code_t * out) {
    if(strcmp(name,"")==0) {
        (*out) = LV_EVENT_ALL ;
    }
    else if(strcmp(name,"ed")==0) {
        (*out) = LV_EVENT_PRESSED ;
    }
    else if(strcmp(name,"ing")==0) {
        (*out) = LV_EVENT_PRESSING ;
    }
    else if(strcmp(name,"lost")==0) {
        (*out) = LV_EVENT_PRESS_LOST ;
    }
    else if(strcmp(name,"clicked")==0) {
        (*out) = LV_EVENT_SHORT_CLICKED ;
    }
    else if(strcmp(name,"pressed")==0) {
        (*out) = LV_EVENT_LONG_PRESSED ;
    }
    else if(strcmp(name,"pressed-repeat")==0) {
        (*out) = LV_EVENT_LONG_PRESSED_REPEAT ;
    }
    else if(strcmp(name,"ed")==0) {
        (*out) = LV_EVENT_CLICKED ;
    }
    else if(strcmp(name,"sed")==0) {
        (*out) = LV_EVENT_RELEASED ;
    }
    else if(strcmp(name,"l-begin")==0) {
        (*out) = LV_EVENT_SCROLL_BEGIN ;
    }
    else if(strcmp(name,"l-throw-begin")==0) {
        (*out) = LV_EVENT_SCROLL_THROW_BEGIN ;
    }
    else if(strcmp(name,"l-end")==0) {
        (*out) = LV_EVENT_SCROLL_END ;
    }
    else if(strcmp(name,"l")==0) {
        (*out) = LV_EVENT_SCROLL ;
    }
    else if(strcmp(name,"re")==0) {
        (*out) = LV_EVENT_GESTURE ;
    }
    else if(strcmp(name,"")==0) {
        (*out) = LV_EVENT_KEY ;
    }
    else if(strcmp(name,"ed")==0) {
        (*out) = LV_EVENT_FOCUSED ;
    }
    else if(strcmp(name,"used")==0) {
        (*out) = LV_EVENT_DEFOCUSED ;
    }
    else if(strcmp(name,"")==0) {
        (*out) = LV_EVENT_LEAVE ;
    }
    else if(strcmp(name,"est")==0) {
        (*out) = LV_EVENT_HIT_TEST ;
    }
    else if(strcmp(name,"reset")==0) {
        (*out) = LV_EVENT_INDEV_RESET ;
    }
    else if(strcmp(name,"check")==0) {
        (*out) = LV_EVENT_COVER_CHECK ;
    }
    else if(strcmp(name,"ext-draw-size")==0) {
        (*out) = LV_EVENT_REFR_EXT_DRAW_SIZE ;
    }
    else if(strcmp(name,"main-begin")==0) {
        (*out) = LV_EVENT_DRAW_MAIN_BEGIN ;
    }
    else if(strcmp(name,"main")==0) {
        (*out) = LV_EVENT_DRAW_MAIN ;
    }
    else if(strcmp(name,"main-end")==0) {
        (*out) = LV_EVENT_DRAW_MAIN_END ;
    }
    else if(strcmp(name,"post-begin")==0) {
        (*out) = LV_EVENT_DRAW_POST_BEGIN ;
    }
    else if(strcmp(name,"post")==0) {
        (*out) = LV_EVENT_DRAW_POST ;
    }
    else if(strcmp(name,"post-end")==0) {
        (*out) = LV_EVENT_DRAW_POST_END ;
    }
    else if(strcmp(name,"task-added")==0) {
        (*out) = LV_EVENT_DRAW_TASK_ADDED ;
    }
    else if(strcmp(name,"changed")==0) {
        (*out) = LV_EVENT_VALUE_CHANGED ;
    }
    else if(strcmp(name,"t")==0) {
        (*out) = LV_EVENT_INSERT ;
    }
    else if(strcmp(name,"sh")==0) {
        (*out) = LV_EVENT_REFRESH ;
    }
    else if(strcmp(name,"")==0) {
        (*out) = LV_EVENT_READY ;
    }
    else if(strcmp(name,"l")==0) {
        (*out) = LV_EVENT_CANCEL ;
    }
    else if(strcmp(name,"e")==0) {
        (*out) = LV_EVENT_CREATE ;
    }
    else if(strcmp(name,"e")==0) {
        (*out) = LV_EVENT_DELETE ;
    }
    else if(strcmp(name,"changed")==0) {
        (*out) = LV_EVENT_CHILD_CHANGED ;
    }
    else if(strcmp(name,"created")==0) {
        (*out) = LV_EVENT_CHILD_CREATED ;
    }
    else if(strcmp(name,"deleted")==0) {
        (*out) = LV_EVENT_CHILD_DELETED ;
    }
    else if(strcmp(name,"n-unload-start")==0) {
        (*out) = LV_EVENT_SCREEN_UNLOAD_START ;
    }
    else if(strcmp(name,"n-load-start")==0) {
        (*out) = LV_EVENT_SCREEN_LOAD_START ;
    }
    else if(strcmp(name,"n-loaded")==0) {
        (*out) = LV_EVENT_SCREEN_LOADED ;
    }
    else if(strcmp(name,"n-unloaded")==0) {
        (*out) = LV_EVENT_SCREEN_UNLOADED ;
    }
    else if(strcmp(name,"changed")==0) {
        (*out) = LV_EVENT_SIZE_CHANGED ;
    }
    else if(strcmp(name,"changed")==0) {
        (*out) = LV_EVENT_STYLE_CHANGED ;
    }
    else if(strcmp(name,"t-changed")==0) {
        (*out) = LV_EVENT_LAYOUT_CHANGED ;
    }
    else if(strcmp(name,"elf-size")==0) {
        (*out) = LV_EVENT_GET_SELF_SIZE ;
    }
    else if(strcmp(name,"idate-area")==0) {
        (*out) = LV_EVENT_INVALIDATE_AREA ;
    }
    else if(strcmp(name,"ution-changed")==0) {
        (*out) = LV_EVENT_RESOLUTION_CHANGED ;
    }
    else if(strcmp(name,"format-changed")==0) {
        (*out) = LV_EVENT_COLOR_FORMAT_CHANGED ;
    }
    else if(strcmp(name,"request")==0) {
        (*out) = LV_EVENT_REFR_REQUEST ;
    }
    else if(strcmp(name,"start")==0) {
        (*out) = LV_EVENT_REFR_START ;
    }
    else if(strcmp(name,"ready")==0) {
        (*out) = LV_EVENT_REFR_READY ;
    }
    else if(strcmp(name,"r-start")==0) {
        (*out) = LV_EVENT_RENDER_START ;
    }
    else if(strcmp(name,"r-ready")==0) {
        (*out) = LV_EVENT_RENDER_READY ;
    }
    else if(strcmp(name,"start")==0) {
        (*out) = LV_EVENT_FLUSH_START ;
    }
    else if(strcmp(name,"finish")==0) {
        (*out) = LV_EVENT_FLUSH_FINISH ;
    }
    else if(strcmp(name,"")==0) {
        (*out) = LV_EVENT_VSYNC ;
    }
    else if(strcmp(name,"")==0) {
        (*out) = _LV_EVENT_LAST ;
    }
    else if(strcmp(name,"ocess")==0) {
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
        case LV_EVENT_ALL: return "";
        case LV_EVENT_PRESSED: return "ed";
        case LV_EVENT_PRESSING: return "ing";
        case LV_EVENT_PRESS_LOST: return "lost";
        case LV_EVENT_SHORT_CLICKED: return "clicked";
        case LV_EVENT_LONG_PRESSED: return "pressed";
        case LV_EVENT_LONG_PRESSED_REPEAT: return "pressed-repeat";
        case LV_EVENT_CLICKED: return "ed";
        case LV_EVENT_RELEASED: return "sed";
        case LV_EVENT_SCROLL_BEGIN: return "l-begin";
        case LV_EVENT_SCROLL_THROW_BEGIN: return "l-throw-begin";
        case LV_EVENT_SCROLL_END: return "l-end";
        case LV_EVENT_SCROLL: return "l";
        case LV_EVENT_GESTURE: return "re";
        case LV_EVENT_KEY: return "";
        case LV_EVENT_FOCUSED: return "ed";
        case LV_EVENT_DEFOCUSED: return "used";
        case LV_EVENT_LEAVE: return "";
        case LV_EVENT_HIT_TEST: return "est";
        case LV_EVENT_INDEV_RESET: return "reset";
        case LV_EVENT_COVER_CHECK: return "check";
        case LV_EVENT_REFR_EXT_DRAW_SIZE: return "ext-draw-size";
        case LV_EVENT_DRAW_MAIN_BEGIN: return "main-begin";
        case LV_EVENT_DRAW_MAIN: return "main";
        case LV_EVENT_DRAW_MAIN_END: return "main-end";
        case LV_EVENT_DRAW_POST_BEGIN: return "post-begin";
        case LV_EVENT_DRAW_POST: return "post";
        case LV_EVENT_DRAW_POST_END: return "post-end";
        case LV_EVENT_DRAW_TASK_ADDED: return "task-added";
        case LV_EVENT_VALUE_CHANGED: return "changed";
        case LV_EVENT_INSERT: return "t";
        case LV_EVENT_REFRESH: return "sh";
        case LV_EVENT_READY: return "";
        case LV_EVENT_CANCEL: return "l";
        case LV_EVENT_CREATE: return "e";
        case LV_EVENT_DELETE: return "e";
        case LV_EVENT_CHILD_CHANGED: return "changed";
        case LV_EVENT_CHILD_CREATED: return "created";
        case LV_EVENT_CHILD_DELETED: return "deleted";
        case LV_EVENT_SCREEN_UNLOAD_START: return "n-unload-start";
        case LV_EVENT_SCREEN_LOAD_START: return "n-load-start";
        case LV_EVENT_SCREEN_LOADED: return "n-loaded";
        case LV_EVENT_SCREEN_UNLOADED: return "n-unloaded";
        case LV_EVENT_SIZE_CHANGED: return "changed";
        case LV_EVENT_STYLE_CHANGED: return "changed";
        case LV_EVENT_LAYOUT_CHANGED: return "t-changed";
        case LV_EVENT_GET_SELF_SIZE: return "elf-size";
        case LV_EVENT_INVALIDATE_AREA: return "idate-area";
        case LV_EVENT_RESOLUTION_CHANGED: return "ution-changed";
        case LV_EVENT_COLOR_FORMAT_CHANGED: return "format-changed";
        case LV_EVENT_REFR_REQUEST: return "request";
        case LV_EVENT_REFR_START: return "start";
        case LV_EVENT_REFR_READY: return "ready";
        case LV_EVENT_RENDER_START: return "r-start";
        case LV_EVENT_RENDER_READY: return "r-ready";
        case LV_EVENT_FLUSH_START: return "start";
        case LV_EVENT_FLUSH_FINISH: return "finish";
        case LV_EVENT_VSYNC: return "";
        case _LV_EVENT_LAST: return "";
        case LV_EVENT_PREPROCESS: return "ocess";

        default:
            return "unknow";
    }
}
JSValue lv_event_code_const_to_jsstr(JSContext *ctx, lv_event_code_t code) {
    return JS_NewString(ctx, lv_event_code_const_to_str(code));
}

// lv_anim_enable_t
bool lv_anim_enable_str_to_const(const char * name, lv_anim_enable_t * out) {
    if(strcmp(name,"")==0) {
        (*out) = LV_ANIM_OFF ;
    }
    else if(strcmp(name,"")==0) {
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
        case LV_ANIM_OFF: return "";
        case LV_ANIM_ON: return "";

        default:
            return "unknow";
    }
}
JSValue lv_anim_enable_const_to_jsstr(JSContext *ctx, lv_anim_enable_t code) {
    return JS_NewString(ctx, lv_anim_enable_const_to_str(code));
}

// lv_style_prop_t
bool lv_style_prop_str_to_const(const char * name, lv_style_prop_t * out) {
    if(strcmp(name,"inv")==0) {
        (*out) = LV_STYLE_PROP_INV ;
    }
    else if(strcmp(name,"")==0) {
        (*out) = LV_STYLE_WIDTH ;
    }
    else if(strcmp(name,"t")==0) {
        (*out) = LV_STYLE_HEIGHT ;
    }
    else if(strcmp(name,"h")==0) {
        (*out) = LV_STYLE_LENGTH ;
    }
    else if(strcmp(name,"idth")==0) {
        (*out) = LV_STYLE_MIN_WIDTH ;
    }
    else if(strcmp(name,"idth")==0) {
        (*out) = LV_STYLE_MAX_WIDTH ;
    }
    else if(strcmp(name,"eight")==0) {
        (*out) = LV_STYLE_MIN_HEIGHT ;
    }
    else if(strcmp(name,"eight")==0) {
        (*out) = LV_STYLE_MAX_HEIGHT ;
    }
    else if(strcmp(name,"")==0) {
        (*out) = LV_STYLE_X ;
    }
    else if(strcmp(name,"")==0) {
        (*out) = LV_STYLE_Y ;
    }
    else if(strcmp(name,"")==0) {
        (*out) = LV_STYLE_ALIGN ;
    }
    else if(strcmp(name,"s")==0) {
        (*out) = LV_STYLE_RADIUS ;
    }
    else if(strcmp(name,"op")==0) {
        (*out) = LV_STYLE_PAD_TOP ;
    }
    else if(strcmp(name,"ottom")==0) {
        (*out) = LV_STYLE_PAD_BOTTOM ;
    }
    else if(strcmp(name,"eft")==0) {
        (*out) = LV_STYLE_PAD_LEFT ;
    }
    else if(strcmp(name,"ight")==0) {
        (*out) = LV_STYLE_PAD_RIGHT ;
    }
    else if(strcmp(name,"ow")==0) {
        (*out) = LV_STYLE_PAD_ROW ;
    }
    else if(strcmp(name,"olumn")==0) {
        (*out) = LV_STYLE_PAD_COLUMN ;
    }
    else if(strcmp(name,"t")==0) {
        (*out) = LV_STYLE_LAYOUT ;
    }
    else if(strcmp(name,"n-top")==0) {
        (*out) = LV_STYLE_MARGIN_TOP ;
    }
    else if(strcmp(name,"n-bottom")==0) {
        (*out) = LV_STYLE_MARGIN_BOTTOM ;
    }
    else if(strcmp(name,"n-left")==0) {
        (*out) = LV_STYLE_MARGIN_LEFT ;
    }
    else if(strcmp(name,"n-right")==0) {
        (*out) = LV_STYLE_MARGIN_RIGHT ;
    }
    else if(strcmp(name,"lor")==0) {
        (*out) = LV_STYLE_BG_COLOR ;
    }
    else if(strcmp(name,"a")==0) {
        (*out) = LV_STYLE_BG_OPA ;
    }
    else if(strcmp(name,"ad-dir")==0) {
        (*out) = LV_STYLE_BG_GRAD_DIR ;
    }
    else if(strcmp(name,"in-stop")==0) {
        (*out) = LV_STYLE_BG_MAIN_STOP ;
    }
    else if(strcmp(name,"ad-stop")==0) {
        (*out) = LV_STYLE_BG_GRAD_STOP ;
    }
    else if(strcmp(name,"ad-color")==0) {
        (*out) = LV_STYLE_BG_GRAD_COLOR ;
    }
    else if(strcmp(name,"in-opa")==0) {
        (*out) = LV_STYLE_BG_MAIN_OPA ;
    }
    else if(strcmp(name,"ad-opa")==0) {
        (*out) = LV_STYLE_BG_GRAD_OPA ;
    }
    else if(strcmp(name,"ad")==0) {
        (*out) = LV_STYLE_BG_GRAD ;
    }
    else if(strcmp(name,"dir")==0) {
        (*out) = LV_STYLE_BASE_DIR ;
    }
    else if(strcmp(name,"age-src")==0) {
        (*out) = LV_STYLE_BG_IMAGE_SRC ;
    }
    else if(strcmp(name,"age-opa")==0) {
        (*out) = LV_STYLE_BG_IMAGE_OPA ;
    }
    else if(strcmp(name,"age-recolor")==0) {
        (*out) = LV_STYLE_BG_IMAGE_RECOLOR ;
    }
    else if(strcmp(name,"age-recolor-opa")==0) {
        (*out) = LV_STYLE_BG_IMAGE_RECOLOR_OPA ;
    }
    else if(strcmp(name,"age-tiled")==0) {
        (*out) = LV_STYLE_BG_IMAGE_TILED ;
    }
    else if(strcmp(name,"corner")==0) {
        (*out) = LV_STYLE_CLIP_CORNER ;
    }
    else if(strcmp(name,"r-width")==0) {
        (*out) = LV_STYLE_BORDER_WIDTH ;
    }
    else if(strcmp(name,"r-color")==0) {
        (*out) = LV_STYLE_BORDER_COLOR ;
    }
    else if(strcmp(name,"r-opa")==0) {
        (*out) = LV_STYLE_BORDER_OPA ;
    }
    else if(strcmp(name,"r-side")==0) {
        (*out) = LV_STYLE_BORDER_SIDE ;
    }
    else if(strcmp(name,"r-post")==0) {
        (*out) = LV_STYLE_BORDER_POST ;
    }
    else if(strcmp(name,"ne-width")==0) {
        (*out) = LV_STYLE_OUTLINE_WIDTH ;
    }
    else if(strcmp(name,"ne-color")==0) {
        (*out) = LV_STYLE_OUTLINE_COLOR ;
    }
    else if(strcmp(name,"ne-opa")==0) {
        (*out) = LV_STYLE_OUTLINE_OPA ;
    }
    else if(strcmp(name,"ne-pad")==0) {
        (*out) = LV_STYLE_OUTLINE_PAD ;
    }
    else if(strcmp(name,"w-width")==0) {
        (*out) = LV_STYLE_SHADOW_WIDTH ;
    }
    else if(strcmp(name,"w-color")==0) {
        (*out) = LV_STYLE_SHADOW_COLOR ;
    }
    else if(strcmp(name,"w-opa")==0) {
        (*out) = LV_STYLE_SHADOW_OPA ;
    }
    else if(strcmp(name,"w-offset-x")==0) {
        (*out) = LV_STYLE_SHADOW_OFFSET_X ;
    }
    else if(strcmp(name,"w-offset-y")==0) {
        (*out) = LV_STYLE_SHADOW_OFFSET_Y ;
    }
    else if(strcmp(name,"w-spread")==0) {
        (*out) = LV_STYLE_SHADOW_SPREAD ;
    }
    else if(strcmp(name,"opa")==0) {
        (*out) = LV_STYLE_IMAGE_OPA ;
    }
    else if(strcmp(name,"recolor")==0) {
        (*out) = LV_STYLE_IMAGE_RECOLOR ;
    }
    else if(strcmp(name,"recolor-opa")==0) {
        (*out) = LV_STYLE_IMAGE_RECOLOR_OPA ;
    }
    else if(strcmp(name,"width")==0) {
        (*out) = LV_STYLE_LINE_WIDTH ;
    }
    else if(strcmp(name,"dash-width")==0) {
        (*out) = LV_STYLE_LINE_DASH_WIDTH ;
    }
    else if(strcmp(name,"dash-gap")==0) {
        (*out) = LV_STYLE_LINE_DASH_GAP ;
    }
    else if(strcmp(name,"rounded")==0) {
        (*out) = LV_STYLE_LINE_ROUNDED ;
    }
    else if(strcmp(name,"color")==0) {
        (*out) = LV_STYLE_LINE_COLOR ;
    }
    else if(strcmp(name,"opa")==0) {
        (*out) = LV_STYLE_LINE_OPA ;
    }
    else if(strcmp(name,"idth")==0) {
        (*out) = LV_STYLE_ARC_WIDTH ;
    }
    else if(strcmp(name,"ounded")==0) {
        (*out) = LV_STYLE_ARC_ROUNDED ;
    }
    else if(strcmp(name,"olor")==0) {
        (*out) = LV_STYLE_ARC_COLOR ;
    }
    else if(strcmp(name,"pa")==0) {
        (*out) = LV_STYLE_ARC_OPA ;
    }
    else if(strcmp(name,"mage-src")==0) {
        (*out) = LV_STYLE_ARC_IMAGE_SRC ;
    }
    else if(strcmp(name,"color")==0) {
        (*out) = LV_STYLE_TEXT_COLOR ;
    }
    else if(strcmp(name,"opa")==0) {
        (*out) = LV_STYLE_TEXT_OPA ;
    }
    else if(strcmp(name,"font")==0) {
        (*out) = LV_STYLE_TEXT_FONT ;
    }
    else if(strcmp(name,"letter-space")==0) {
        (*out) = LV_STYLE_TEXT_LETTER_SPACE ;
    }
    else if(strcmp(name,"line-space")==0) {
        (*out) = LV_STYLE_TEXT_LINE_SPACE ;
    }
    else if(strcmp(name,"decor")==0) {
        (*out) = LV_STYLE_TEXT_DECOR ;
    }
    else if(strcmp(name,"align")==0) {
        (*out) = LV_STYLE_TEXT_ALIGN ;
    }
    else if(strcmp(name,"")==0) {
        (*out) = LV_STYLE_OPA ;
    }
    else if(strcmp(name,"ayered")==0) {
        (*out) = LV_STYLE_OPA_LAYERED ;
    }
    else if(strcmp(name,"filter-dsc")==0) {
        (*out) = LV_STYLE_COLOR_FILTER_DSC ;
    }
    else if(strcmp(name,"filter-opa")==0) {
        (*out) = LV_STYLE_COLOR_FILTER_OPA ;
    }
    else if(strcmp(name,"")==0) {
        (*out) = LV_STYLE_ANIM ;
    }
    else if(strcmp(name,"duration")==0) {
        (*out) = LV_STYLE_ANIM_DURATION ;
    }
    else if(strcmp(name,"ition")==0) {
        (*out) = LV_STYLE_TRANSITION ;
    }
    else if(strcmp(name,"mode")==0) {
        (*out) = LV_STYLE_BLEND_MODE ;
    }
    else if(strcmp(name,"form-width")==0) {
        (*out) = LV_STYLE_TRANSFORM_WIDTH ;
    }
    else if(strcmp(name,"form-height")==0) {
        (*out) = LV_STYLE_TRANSFORM_HEIGHT ;
    }
    else if(strcmp(name,"late-x")==0) {
        (*out) = LV_STYLE_TRANSLATE_X ;
    }
    else if(strcmp(name,"late-y")==0) {
        (*out) = LV_STYLE_TRANSLATE_Y ;
    }
    else if(strcmp(name,"form-scale-x")==0) {
        (*out) = LV_STYLE_TRANSFORM_SCALE_X ;
    }
    else if(strcmp(name,"form-scale-y")==0) {
        (*out) = LV_STYLE_TRANSFORM_SCALE_Y ;
    }
    else if(strcmp(name,"form-rotation")==0) {
        (*out) = LV_STYLE_TRANSFORM_ROTATION ;
    }
    else if(strcmp(name,"form-pivot-x")==0) {
        (*out) = LV_STYLE_TRANSFORM_PIVOT_X ;
    }
    else if(strcmp(name,"form-pivot-y")==0) {
        (*out) = LV_STYLE_TRANSFORM_PIVOT_Y ;
    }
    else if(strcmp(name,"form-skew-x")==0) {
        (*out) = LV_STYLE_TRANSFORM_SKEW_X ;
    }
    else if(strcmp(name,"form-skew-y")==0) {
        (*out) = LV_STYLE_TRANSFORM_SKEW_Y ;
    }
    else if(strcmp(name,"flow")==0) {
        (*out) = LV_STYLE_FLEX_FLOW ;
    }
    else if(strcmp(name,"main-place")==0) {
        (*out) = LV_STYLE_FLEX_MAIN_PLACE ;
    }
    else if(strcmp(name,"cross-place")==0) {
        (*out) = LV_STYLE_FLEX_CROSS_PLACE ;
    }
    else if(strcmp(name,"track-place")==0) {
        (*out) = LV_STYLE_FLEX_TRACK_PLACE ;
    }
    else if(strcmp(name,"grow")==0) {
        (*out) = LV_STYLE_FLEX_GROW ;
    }
    else if(strcmp(name,"column-align")==0) {
        (*out) = LV_STYLE_GRID_COLUMN_ALIGN ;
    }
    else if(strcmp(name,"row-align")==0) {
        (*out) = LV_STYLE_GRID_ROW_ALIGN ;
    }
    else if(strcmp(name,"row-dsc-array")==0) {
        (*out) = LV_STYLE_GRID_ROW_DSC_ARRAY ;
    }
    else if(strcmp(name,"column-dsc-array")==0) {
        (*out) = LV_STYLE_GRID_COLUMN_DSC_ARRAY ;
    }
    else if(strcmp(name,"cell-column-pos")==0) {
        (*out) = LV_STYLE_GRID_CELL_COLUMN_POS ;
    }
    else if(strcmp(name,"cell-column-span")==0) {
        (*out) = LV_STYLE_GRID_CELL_COLUMN_SPAN ;
    }
    else if(strcmp(name,"cell-x-align")==0) {
        (*out) = LV_STYLE_GRID_CELL_X_ALIGN ;
    }
    else if(strcmp(name,"cell-row-pos")==0) {
        (*out) = LV_STYLE_GRID_CELL_ROW_POS ;
    }
    else if(strcmp(name,"cell-row-span")==0) {
        (*out) = LV_STYLE_GRID_CELL_ROW_SPAN ;
    }
    else if(strcmp(name,"cell-y-align")==0) {
        (*out) = LV_STYLE_GRID_CELL_Y_ALIGN ;
    }
    else if(strcmp(name,"built-in-prop")==0) {
        (*out) = _LV_STYLE_LAST_BUILT_IN_PROP ;
    }
    else if(strcmp(name,"built-in-props")==0) {
        (*out) = _LV_STYLE_NUM_BUILT_IN_PROPS ;
    }
    else if(strcmp(name,"any")==0) {
        (*out) = LV_STYLE_PROP_ANY ;
    }
    else if(strcmp(name,"const")==0) {
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
        case LV_STYLE_PROP_INV: return "inv";
        case LV_STYLE_WIDTH: return "";
        case LV_STYLE_HEIGHT: return "t";
        case LV_STYLE_LENGTH: return "h";
        case LV_STYLE_MIN_WIDTH: return "idth";
        case LV_STYLE_MAX_WIDTH: return "idth";
        case LV_STYLE_MIN_HEIGHT: return "eight";
        case LV_STYLE_MAX_HEIGHT: return "eight";
        case LV_STYLE_X: return "";
        case LV_STYLE_Y: return "";
        case LV_STYLE_ALIGN: return "";
        case LV_STYLE_RADIUS: return "s";
        case LV_STYLE_PAD_TOP: return "op";
        case LV_STYLE_PAD_BOTTOM: return "ottom";
        case LV_STYLE_PAD_LEFT: return "eft";
        case LV_STYLE_PAD_RIGHT: return "ight";
        case LV_STYLE_PAD_ROW: return "ow";
        case LV_STYLE_PAD_COLUMN: return "olumn";
        case LV_STYLE_LAYOUT: return "t";
        case LV_STYLE_MARGIN_TOP: return "n-top";
        case LV_STYLE_MARGIN_BOTTOM: return "n-bottom";
        case LV_STYLE_MARGIN_LEFT: return "n-left";
        case LV_STYLE_MARGIN_RIGHT: return "n-right";
        case LV_STYLE_BG_COLOR: return "lor";
        case LV_STYLE_BG_OPA: return "a";
        case LV_STYLE_BG_GRAD_DIR: return "ad-dir";
        case LV_STYLE_BG_MAIN_STOP: return "in-stop";
        case LV_STYLE_BG_GRAD_STOP: return "ad-stop";
        case LV_STYLE_BG_GRAD_COLOR: return "ad-color";
        case LV_STYLE_BG_MAIN_OPA: return "in-opa";
        case LV_STYLE_BG_GRAD_OPA: return "ad-opa";
        case LV_STYLE_BG_GRAD: return "ad";
        case LV_STYLE_BASE_DIR: return "dir";
        case LV_STYLE_BG_IMAGE_SRC: return "age-src";
        case LV_STYLE_BG_IMAGE_OPA: return "age-opa";
        case LV_STYLE_BG_IMAGE_RECOLOR: return "age-recolor";
        case LV_STYLE_BG_IMAGE_RECOLOR_OPA: return "age-recolor-opa";
        case LV_STYLE_BG_IMAGE_TILED: return "age-tiled";
        case LV_STYLE_CLIP_CORNER: return "corner";
        case LV_STYLE_BORDER_WIDTH: return "r-width";
        case LV_STYLE_BORDER_COLOR: return "r-color";
        case LV_STYLE_BORDER_OPA: return "r-opa";
        case LV_STYLE_BORDER_SIDE: return "r-side";
        case LV_STYLE_BORDER_POST: return "r-post";
        case LV_STYLE_OUTLINE_WIDTH: return "ne-width";
        case LV_STYLE_OUTLINE_COLOR: return "ne-color";
        case LV_STYLE_OUTLINE_OPA: return "ne-opa";
        case LV_STYLE_OUTLINE_PAD: return "ne-pad";
        case LV_STYLE_SHADOW_WIDTH: return "w-width";
        case LV_STYLE_SHADOW_COLOR: return "w-color";
        case LV_STYLE_SHADOW_OPA: return "w-opa";
        case LV_STYLE_SHADOW_OFFSET_X: return "w-offset-x";
        case LV_STYLE_SHADOW_OFFSET_Y: return "w-offset-y";
        case LV_STYLE_SHADOW_SPREAD: return "w-spread";
        case LV_STYLE_IMAGE_OPA: return "opa";
        case LV_STYLE_IMAGE_RECOLOR: return "recolor";
        case LV_STYLE_IMAGE_RECOLOR_OPA: return "recolor-opa";
        case LV_STYLE_LINE_WIDTH: return "width";
        case LV_STYLE_LINE_DASH_WIDTH: return "dash-width";
        case LV_STYLE_LINE_DASH_GAP: return "dash-gap";
        case LV_STYLE_LINE_ROUNDED: return "rounded";
        case LV_STYLE_LINE_COLOR: return "color";
        case LV_STYLE_LINE_OPA: return "opa";
        case LV_STYLE_ARC_WIDTH: return "idth";
        case LV_STYLE_ARC_ROUNDED: return "ounded";
        case LV_STYLE_ARC_COLOR: return "olor";
        case LV_STYLE_ARC_OPA: return "pa";
        case LV_STYLE_ARC_IMAGE_SRC: return "mage-src";
        case LV_STYLE_TEXT_COLOR: return "color";
        case LV_STYLE_TEXT_OPA: return "opa";
        case LV_STYLE_TEXT_FONT: return "font";
        case LV_STYLE_TEXT_LETTER_SPACE: return "letter-space";
        case LV_STYLE_TEXT_LINE_SPACE: return "line-space";
        case LV_STYLE_TEXT_DECOR: return "decor";
        case LV_STYLE_TEXT_ALIGN: return "align";
        case LV_STYLE_OPA: return "";
        case LV_STYLE_OPA_LAYERED: return "ayered";
        case LV_STYLE_COLOR_FILTER_DSC: return "filter-dsc";
        case LV_STYLE_COLOR_FILTER_OPA: return "filter-opa";
        case LV_STYLE_ANIM: return "";
        case LV_STYLE_ANIM_DURATION: return "duration";
        case LV_STYLE_TRANSITION: return "ition";
        case LV_STYLE_BLEND_MODE: return "mode";
        case LV_STYLE_TRANSFORM_WIDTH: return "form-width";
        case LV_STYLE_TRANSFORM_HEIGHT: return "form-height";
        case LV_STYLE_TRANSLATE_X: return "late-x";
        case LV_STYLE_TRANSLATE_Y: return "late-y";
        case LV_STYLE_TRANSFORM_SCALE_X: return "form-scale-x";
        case LV_STYLE_TRANSFORM_SCALE_Y: return "form-scale-y";
        case LV_STYLE_TRANSFORM_ROTATION: return "form-rotation";
        case LV_STYLE_TRANSFORM_PIVOT_X: return "form-pivot-x";
        case LV_STYLE_TRANSFORM_PIVOT_Y: return "form-pivot-y";
        case LV_STYLE_TRANSFORM_SKEW_X: return "form-skew-x";
        case LV_STYLE_TRANSFORM_SKEW_Y: return "form-skew-y";
        case LV_STYLE_FLEX_FLOW: return "flow";
        case LV_STYLE_FLEX_MAIN_PLACE: return "main-place";
        case LV_STYLE_FLEX_CROSS_PLACE: return "cross-place";
        case LV_STYLE_FLEX_TRACK_PLACE: return "track-place";
        case LV_STYLE_FLEX_GROW: return "grow";
        case LV_STYLE_GRID_COLUMN_ALIGN: return "column-align";
        case LV_STYLE_GRID_ROW_ALIGN: return "row-align";
        case LV_STYLE_GRID_ROW_DSC_ARRAY: return "row-dsc-array";
        case LV_STYLE_GRID_COLUMN_DSC_ARRAY: return "column-dsc-array";
        case LV_STYLE_GRID_CELL_COLUMN_POS: return "cell-column-pos";
        case LV_STYLE_GRID_CELL_COLUMN_SPAN: return "cell-column-span";
        case LV_STYLE_GRID_CELL_X_ALIGN: return "cell-x-align";
        case LV_STYLE_GRID_CELL_ROW_POS: return "cell-row-pos";
        case LV_STYLE_GRID_CELL_ROW_SPAN: return "cell-row-span";
        case LV_STYLE_GRID_CELL_Y_ALIGN: return "cell-y-align";
        case _LV_STYLE_LAST_BUILT_IN_PROP: return "built-in-prop";
        case _LV_STYLE_NUM_BUILT_IN_PROPS: return "built-in-props";
        case LV_STYLE_PROP_ANY: return "any";

        default:
            return "unknow";
    }
}
JSValue lv_style_prop_const_to_jsstr(JSContext *ctx, lv_style_prop_t code) {
    return JS_NewString(ctx, lv_style_prop_const_to_str(code));
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