#include <stdlib.h>
#include <emscripten.h>
#include "lv_conf.h"
#include "lvgl.h"
#include "../../main/debug.h"


// AUTO GENERATE CODE START [CONST MAPPING] --------
// "lv_res_t": /^(_?)LV_RES_/ ,
EMSCRIPTEN_KEEPALIVE char * lv_res_to_name(uint32_t v) {
    if(LV_RES_INV==v){ return "inv" ; }
    else if(LV_RES_OK==v){ return "ok" ; }
    else {
        return "<unknow>" ;
    }
}
EMSCRIPTEN_KEEPALIVE uint32_t lv_res_to_value(char * n) {
    if(strcmp("inv", n)==0){ return LV_RES_INV ; }
    else if(strcmp("ok", n)==0){ return LV_RES_OK ; }
    else {
        return (uint32_t)-1 ;
    }
}
// "lv_anim_enable_t": /^(_?)LV_ANIM_/ ,
EMSCRIPTEN_KEEPALIVE char * lv_anim_enable_to_name(uint32_t v) {
    if(LV_ANIM_OFF==v){ return "off" ; }
    else if(LV_ANIM_ON==v){ return "on" ; }
    else {
        return "<unknow>" ;
    }
}
EMSCRIPTEN_KEEPALIVE uint32_t lv_anim_enable_to_value(char * n) {
    if(strcmp("off", n)==0){ return LV_ANIM_OFF ; }
    else if(strcmp("on", n)==0){ return LV_ANIM_ON ; }
    else {
        return (uint32_t)-1 ;
    }
}
// "lv_palette_t": /^(_?)LV_PALETTE_/ ,
EMSCRIPTEN_KEEPALIVE char * lv_palette_to_name(uint32_t v) {
    if(LV_PALETTE_RED==v){ return "red" ; }
    else if(LV_PALETTE_PINK==v){ return "pink" ; }
    else if(LV_PALETTE_PURPLE==v){ return "purple" ; }
    else if(LV_PALETTE_DEEP_PURPLE==v){ return "deep-purple" ; }
    else if(LV_PALETTE_INDIGO==v){ return "indigo" ; }
    else if(LV_PALETTE_BLUE==v){ return "blue" ; }
    else if(LV_PALETTE_LIGHT_BLUE==v){ return "light-blue" ; }
    else if(LV_PALETTE_CYAN==v){ return "cyan" ; }
    else if(LV_PALETTE_TEAL==v){ return "teal" ; }
    else if(LV_PALETTE_GREEN==v){ return "green" ; }
    else if(LV_PALETTE_LIGHT_GREEN==v){ return "light-green" ; }
    else if(LV_PALETTE_LIME==v){ return "lime" ; }
    else if(LV_PALETTE_YELLOW==v){ return "yellow" ; }
    else if(LV_PALETTE_AMBER==v){ return "amber" ; }
    else if(LV_PALETTE_ORANGE==v){ return "orange" ; }
    else if(LV_PALETTE_DEEP_ORANGE==v){ return "deep-orange" ; }
    else if(LV_PALETTE_BROWN==v){ return "brown" ; }
    else if(LV_PALETTE_BLUE_GREY==v){ return "blue-grey" ; }
    else if(LV_PALETTE_GREY==v){ return "grey" ; }
    else if(_LV_PALETTE_LAST==v){ return "_last" ; }
    else if(LV_PALETTE_NONE==v){ return "none" ; }
    else {
        return "<unknow>" ;
    }
}
EMSCRIPTEN_KEEPALIVE uint32_t lv_palette_to_value(char * n) {
    if(strcmp("red", n)==0){ return LV_PALETTE_RED ; }
    else if(strcmp("pink", n)==0){ return LV_PALETTE_PINK ; }
    else if(strcmp("purple", n)==0){ return LV_PALETTE_PURPLE ; }
    else if(strcmp("deep-purple", n)==0){ return LV_PALETTE_DEEP_PURPLE ; }
    else if(strcmp("indigo", n)==0){ return LV_PALETTE_INDIGO ; }
    else if(strcmp("blue", n)==0){ return LV_PALETTE_BLUE ; }
    else if(strcmp("light-blue", n)==0){ return LV_PALETTE_LIGHT_BLUE ; }
    else if(strcmp("cyan", n)==0){ return LV_PALETTE_CYAN ; }
    else if(strcmp("teal", n)==0){ return LV_PALETTE_TEAL ; }
    else if(strcmp("green", n)==0){ return LV_PALETTE_GREEN ; }
    else if(strcmp("light-green", n)==0){ return LV_PALETTE_LIGHT_GREEN ; }
    else if(strcmp("lime", n)==0){ return LV_PALETTE_LIME ; }
    else if(strcmp("yellow", n)==0){ return LV_PALETTE_YELLOW ; }
    else if(strcmp("amber", n)==0){ return LV_PALETTE_AMBER ; }
    else if(strcmp("orange", n)==0){ return LV_PALETTE_ORANGE ; }
    else if(strcmp("deep-orange", n)==0){ return LV_PALETTE_DEEP_ORANGE ; }
    else if(strcmp("brown", n)==0){ return LV_PALETTE_BROWN ; }
    else if(strcmp("blue-grey", n)==0){ return LV_PALETTE_BLUE_GREY ; }
    else if(strcmp("grey", n)==0){ return LV_PALETTE_GREY ; }
    else if(strcmp("_last", n)==0){ return _LV_PALETTE_LAST ; }
    else if(strcmp("none", n)==0){ return LV_PALETTE_NONE ; }
    else {
        return (uint32_t)-1 ;
    }
}
// "lv_align_t": /^(_?)LV_ALIGN_/ ,
EMSCRIPTEN_KEEPALIVE char * lv_align_to_name(uint32_t v) {
    if(LV_ALIGN_DEFAULT==v){ return "default" ; }
    else if(LV_ALIGN_TOP_LEFT==v){ return "top-left" ; }
    else if(LV_ALIGN_TOP_MID==v){ return "top-mid" ; }
    else if(LV_ALIGN_TOP_RIGHT==v){ return "top-right" ; }
    else if(LV_ALIGN_BOTTOM_LEFT==v){ return "bottom-left" ; }
    else if(LV_ALIGN_BOTTOM_MID==v){ return "bottom-mid" ; }
    else if(LV_ALIGN_BOTTOM_RIGHT==v){ return "bottom-right" ; }
    else if(LV_ALIGN_LEFT_MID==v){ return "left-mid" ; }
    else if(LV_ALIGN_RIGHT_MID==v){ return "right-mid" ; }
    else if(LV_ALIGN_CENTER==v){ return "center" ; }
    else if(LV_ALIGN_OUT_TOP_LEFT==v){ return "out-top-left" ; }
    else if(LV_ALIGN_OUT_TOP_MID==v){ return "out-top-mid" ; }
    else if(LV_ALIGN_OUT_TOP_RIGHT==v){ return "out-top-right" ; }
    else if(LV_ALIGN_OUT_BOTTOM_LEFT==v){ return "out-bottom-left" ; }
    else if(LV_ALIGN_OUT_BOTTOM_MID==v){ return "out-bottom-mid" ; }
    else if(LV_ALIGN_OUT_BOTTOM_RIGHT==v){ return "out-bottom-right" ; }
    else if(LV_ALIGN_OUT_LEFT_TOP==v){ return "out-left-top" ; }
    else if(LV_ALIGN_OUT_LEFT_MID==v){ return "out-left-mid" ; }
    else if(LV_ALIGN_OUT_LEFT_BOTTOM==v){ return "out-left-bottom" ; }
    else if(LV_ALIGN_OUT_RIGHT_TOP==v){ return "out-right-top" ; }
    else if(LV_ALIGN_OUT_RIGHT_MID==v){ return "out-right-mid" ; }
    else if(LV_ALIGN_OUT_RIGHT_BOTTOM==v){ return "out-right-bottom" ; }
    else {
        return "<unknow>" ;
    }
}
EMSCRIPTEN_KEEPALIVE uint32_t lv_align_to_value(char * n) {
    if(strcmp("default", n)==0){ return LV_ALIGN_DEFAULT ; }
    else if(strcmp("top-left", n)==0){ return LV_ALIGN_TOP_LEFT ; }
    else if(strcmp("top-mid", n)==0){ return LV_ALIGN_TOP_MID ; }
    else if(strcmp("top-right", n)==0){ return LV_ALIGN_TOP_RIGHT ; }
    else if(strcmp("bottom-left", n)==0){ return LV_ALIGN_BOTTOM_LEFT ; }
    else if(strcmp("bottom-mid", n)==0){ return LV_ALIGN_BOTTOM_MID ; }
    else if(strcmp("bottom-right", n)==0){ return LV_ALIGN_BOTTOM_RIGHT ; }
    else if(strcmp("left-mid", n)==0){ return LV_ALIGN_LEFT_MID ; }
    else if(strcmp("right-mid", n)==0){ return LV_ALIGN_RIGHT_MID ; }
    else if(strcmp("center", n)==0){ return LV_ALIGN_CENTER ; }
    else if(strcmp("out-top-left", n)==0){ return LV_ALIGN_OUT_TOP_LEFT ; }
    else if(strcmp("out-top-mid", n)==0){ return LV_ALIGN_OUT_TOP_MID ; }
    else if(strcmp("out-top-right", n)==0){ return LV_ALIGN_OUT_TOP_RIGHT ; }
    else if(strcmp("out-bottom-left", n)==0){ return LV_ALIGN_OUT_BOTTOM_LEFT ; }
    else if(strcmp("out-bottom-mid", n)==0){ return LV_ALIGN_OUT_BOTTOM_MID ; }
    else if(strcmp("out-bottom-right", n)==0){ return LV_ALIGN_OUT_BOTTOM_RIGHT ; }
    else if(strcmp("out-left-top", n)==0){ return LV_ALIGN_OUT_LEFT_TOP ; }
    else if(strcmp("out-left-mid", n)==0){ return LV_ALIGN_OUT_LEFT_MID ; }
    else if(strcmp("out-left-bottom", n)==0){ return LV_ALIGN_OUT_LEFT_BOTTOM ; }
    else if(strcmp("out-right-top", n)==0){ return LV_ALIGN_OUT_RIGHT_TOP ; }
    else if(strcmp("out-right-mid", n)==0){ return LV_ALIGN_OUT_RIGHT_MID ; }
    else if(strcmp("out-right-bottom", n)==0){ return LV_ALIGN_OUT_RIGHT_BOTTOM ; }
    else {
        return (uint32_t)-1 ;
    }
}
// "lv_dir_t": /^(_?)LV_DIR_/ ,
EMSCRIPTEN_KEEPALIVE char * lv_dir_to_name(uint32_t v) {
    if(LV_DIR_NONE==v){ return "none" ; }
    else if(LV_DIR_LEFT==v){ return "left" ; }
    else if(LV_DIR_RIGHT==v){ return "right" ; }
    else if(LV_DIR_TOP==v){ return "top" ; }
    else if(LV_DIR_BOTTOM==v){ return "bottom" ; }
    else if(LV_DIR_HOR==v){ return "hor" ; }
    else if(LV_DIR_VER==v){ return "ver" ; }
    else if(LV_DIR_ALL==v){ return "all" ; }
    else {
        return "<unknow>" ;
    }
}
EMSCRIPTEN_KEEPALIVE uint32_t lv_dir_to_value(char * n) {
    if(strcmp("none", n)==0){ return LV_DIR_NONE ; }
    else if(strcmp("left", n)==0){ return LV_DIR_LEFT ; }
    else if(strcmp("right", n)==0){ return LV_DIR_RIGHT ; }
    else if(strcmp("top", n)==0){ return LV_DIR_TOP ; }
    else if(strcmp("bottom", n)==0){ return LV_DIR_BOTTOM ; }
    else if(strcmp("hor", n)==0){ return LV_DIR_HOR ; }
    else if(strcmp("ver", n)==0){ return LV_DIR_VER ; }
    else if(strcmp("all", n)==0){ return LV_DIR_ALL ; }
    else {
        return (uint32_t)-1 ;
    }
}
// "lv_img_cf_t": /^(_?)LV_IMG_CF_/ ,
EMSCRIPTEN_KEEPALIVE char * lv_img_cf_to_name(uint32_t v) {
    if(LV_IMG_CF_UNKNOWN==v){ return "unknown" ; }
    else if(LV_IMG_CF_RAW==v){ return "raw" ; }
    else if(LV_IMG_CF_RAW_ALPHA==v){ return "raw-alpha" ; }
    else if(LV_IMG_CF_RAW_CHROMA_KEYED==v){ return "raw-chroma-keyed" ; }
    else if(LV_IMG_CF_TRUE_COLOR==v){ return "true-color" ; }
    else if(LV_IMG_CF_TRUE_COLOR_ALPHA==v){ return "true-color-alpha" ; }
    else if(LV_IMG_CF_TRUE_COLOR_CHROMA_KEYED==v){ return "true-color-chroma-keyed" ; }
    else if(LV_IMG_CF_INDEXED_1BIT==v){ return "indexed-1bit" ; }
    else if(LV_IMG_CF_INDEXED_2BIT==v){ return "indexed-2bit" ; }
    else if(LV_IMG_CF_INDEXED_4BIT==v){ return "indexed-4bit" ; }
    else if(LV_IMG_CF_INDEXED_8BIT==v){ return "indexed-8bit" ; }
    else if(LV_IMG_CF_ALPHA_1BIT==v){ return "alpha-1bit" ; }
    else if(LV_IMG_CF_ALPHA_2BIT==v){ return "alpha-2bit" ; }
    else if(LV_IMG_CF_ALPHA_4BIT==v){ return "alpha-4bit" ; }
    else if(LV_IMG_CF_ALPHA_8BIT==v){ return "alpha-8bit" ; }
    else if(LV_IMG_CF_RESERVED_15==v){ return "reserved-15" ; }
    else if(LV_IMG_CF_RESERVED_16==v){ return "reserved-16" ; }
    else if(LV_IMG_CF_RESERVED_17==v){ return "reserved-17" ; }
    else if(LV_IMG_CF_RESERVED_18==v){ return "reserved-18" ; }
    else if(LV_IMG_CF_RESERVED_19==v){ return "reserved-19" ; }
    else if(LV_IMG_CF_RESERVED_20==v){ return "reserved-20" ; }
    else if(LV_IMG_CF_RESERVED_21==v){ return "reserved-21" ; }
    else if(LV_IMG_CF_RESERVED_22==v){ return "reserved-22" ; }
    else if(LV_IMG_CF_RESERVED_23==v){ return "reserved-23" ; }
    else if(LV_IMG_CF_USER_ENCODED_0==v){ return "user-encoded-0" ; }
    else if(LV_IMG_CF_USER_ENCODED_1==v){ return "user-encoded-1" ; }
    else if(LV_IMG_CF_USER_ENCODED_2==v){ return "user-encoded-2" ; }
    else if(LV_IMG_CF_USER_ENCODED_3==v){ return "user-encoded-3" ; }
    else if(LV_IMG_CF_USER_ENCODED_4==v){ return "user-encoded-4" ; }
    else if(LV_IMG_CF_USER_ENCODED_5==v){ return "user-encoded-5" ; }
    else if(LV_IMG_CF_USER_ENCODED_6==v){ return "user-encoded-6" ; }
    else if(LV_IMG_CF_USER_ENCODED_7==v){ return "user-encoded-7" ; }
    else {
        return "<unknow>" ;
    }
}
EMSCRIPTEN_KEEPALIVE uint32_t lv_img_cf_to_value(char * n) {
    if(strcmp("unknown", n)==0){ return LV_IMG_CF_UNKNOWN ; }
    else if(strcmp("raw", n)==0){ return LV_IMG_CF_RAW ; }
    else if(strcmp("raw-alpha", n)==0){ return LV_IMG_CF_RAW_ALPHA ; }
    else if(strcmp("raw-chroma-keyed", n)==0){ return LV_IMG_CF_RAW_CHROMA_KEYED ; }
    else if(strcmp("true-color", n)==0){ return LV_IMG_CF_TRUE_COLOR ; }
    else if(strcmp("true-color-alpha", n)==0){ return LV_IMG_CF_TRUE_COLOR_ALPHA ; }
    else if(strcmp("true-color-chroma-keyed", n)==0){ return LV_IMG_CF_TRUE_COLOR_CHROMA_KEYED ; }
    else if(strcmp("indexed-1bit", n)==0){ return LV_IMG_CF_INDEXED_1BIT ; }
    else if(strcmp("indexed-2bit", n)==0){ return LV_IMG_CF_INDEXED_2BIT ; }
    else if(strcmp("indexed-4bit", n)==0){ return LV_IMG_CF_INDEXED_4BIT ; }
    else if(strcmp("indexed-8bit", n)==0){ return LV_IMG_CF_INDEXED_8BIT ; }
    else if(strcmp("alpha-1bit", n)==0){ return LV_IMG_CF_ALPHA_1BIT ; }
    else if(strcmp("alpha-2bit", n)==0){ return LV_IMG_CF_ALPHA_2BIT ; }
    else if(strcmp("alpha-4bit", n)==0){ return LV_IMG_CF_ALPHA_4BIT ; }
    else if(strcmp("alpha-8bit", n)==0){ return LV_IMG_CF_ALPHA_8BIT ; }
    else if(strcmp("reserved-15", n)==0){ return LV_IMG_CF_RESERVED_15 ; }
    else if(strcmp("reserved-16", n)==0){ return LV_IMG_CF_RESERVED_16 ; }
    else if(strcmp("reserved-17", n)==0){ return LV_IMG_CF_RESERVED_17 ; }
    else if(strcmp("reserved-18", n)==0){ return LV_IMG_CF_RESERVED_18 ; }
    else if(strcmp("reserved-19", n)==0){ return LV_IMG_CF_RESERVED_19 ; }
    else if(strcmp("reserved-20", n)==0){ return LV_IMG_CF_RESERVED_20 ; }
    else if(strcmp("reserved-21", n)==0){ return LV_IMG_CF_RESERVED_21 ; }
    else if(strcmp("reserved-22", n)==0){ return LV_IMG_CF_RESERVED_22 ; }
    else if(strcmp("reserved-23", n)==0){ return LV_IMG_CF_RESERVED_23 ; }
    else if(strcmp("user-encoded-0", n)==0){ return LV_IMG_CF_USER_ENCODED_0 ; }
    else if(strcmp("user-encoded-1", n)==0){ return LV_IMG_CF_USER_ENCODED_1 ; }
    else if(strcmp("user-encoded-2", n)==0){ return LV_IMG_CF_USER_ENCODED_2 ; }
    else if(strcmp("user-encoded-3", n)==0){ return LV_IMG_CF_USER_ENCODED_3 ; }
    else if(strcmp("user-encoded-4", n)==0){ return LV_IMG_CF_USER_ENCODED_4 ; }
    else if(strcmp("user-encoded-5", n)==0){ return LV_IMG_CF_USER_ENCODED_5 ; }
    else if(strcmp("user-encoded-6", n)==0){ return LV_IMG_CF_USER_ENCODED_6 ; }
    else if(strcmp("user-encoded-7", n)==0){ return LV_IMG_CF_USER_ENCODED_7 ; }
    else {
        return (uint32_t)-1 ;
    }
}
// "lv_disp_rot_t": /^(_?)LV_DISP_ROT_/ ,
EMSCRIPTEN_KEEPALIVE char * lv_disp_rot_to_name(uint32_t v) {
    if(LV_DISP_ROT_NONE==v){ return "none" ; }
    else if(LV_DISP_ROT_90==v){ return "90" ; }
    else if(LV_DISP_ROT_180==v){ return "180" ; }
    else if(LV_DISP_ROT_270==v){ return "270" ; }
    else {
        return "<unknow>" ;
    }
}
EMSCRIPTEN_KEEPALIVE uint32_t lv_disp_rot_to_value(char * n) {
    if(strcmp("none", n)==0){ return LV_DISP_ROT_NONE ; }
    else if(strcmp("90", n)==0){ return LV_DISP_ROT_90 ; }
    else if(strcmp("180", n)==0){ return LV_DISP_ROT_180 ; }
    else if(strcmp("270", n)==0){ return LV_DISP_ROT_270 ; }
    else {
        return (uint32_t)-1 ;
    }
}
// "lv_indev_type_t": /^(_?)LV_INDEV_TYPE_/ ,
EMSCRIPTEN_KEEPALIVE char * lv_indev_type_to_name(uint32_t v) {
    if(LV_INDEV_TYPE_NONE==v){ return "none" ; }
    else if(LV_INDEV_TYPE_POINTER==v){ return "pointer" ; }
    else if(LV_INDEV_TYPE_KEYPAD==v){ return "keypad" ; }
    else if(LV_INDEV_TYPE_BUTTON==v){ return "button" ; }
    else if(LV_INDEV_TYPE_ENCODER==v){ return "encoder" ; }
    else {
        return "<unknow>" ;
    }
}
EMSCRIPTEN_KEEPALIVE uint32_t lv_indev_type_to_value(char * n) {
    if(strcmp("none", n)==0){ return LV_INDEV_TYPE_NONE ; }
    else if(strcmp("pointer", n)==0){ return LV_INDEV_TYPE_POINTER ; }
    else if(strcmp("keypad", n)==0){ return LV_INDEV_TYPE_KEYPAD ; }
    else if(strcmp("button", n)==0){ return LV_INDEV_TYPE_BUTTON ; }
    else if(strcmp("encoder", n)==0){ return LV_INDEV_TYPE_ENCODER ; }
    else {
        return (uint32_t)-1 ;
    }
}
// "lv_indev_state_t": /^(_?)LV_INDEV_STATE_/ ,
EMSCRIPTEN_KEEPALIVE char * lv_indev_state_to_name(uint32_t v) {
    if(LV_INDEV_STATE_RELEASED==v){ return "released" ; }
    else if(LV_INDEV_STATE_PRESSED==v){ return "pressed" ; }
    else {
        return "<unknow>" ;
    }
}
EMSCRIPTEN_KEEPALIVE uint32_t lv_indev_state_to_value(char * n) {
    if(strcmp("released", n)==0){ return LV_INDEV_STATE_RELEASED ; }
    else if(strcmp("pressed", n)==0){ return LV_INDEV_STATE_PRESSED ; }
    else {
        return (uint32_t)-1 ;
    }
}
// "lv_font_subpx_t": /^(_?)LV_FONT_SUBPX_/ ,
EMSCRIPTEN_KEEPALIVE char * lv_font_subpx_to_name(uint32_t v) {
    if(LV_FONT_SUBPX_NONE==v){ return "none" ; }
    else if(LV_FONT_SUBPX_HOR==v){ return "hor" ; }
    else if(LV_FONT_SUBPX_VER==v){ return "ver" ; }
    else if(LV_FONT_SUBPX_BOTH==v){ return "both" ; }
    else {
        return "<unknow>" ;
    }
}
EMSCRIPTEN_KEEPALIVE uint32_t lv_font_subpx_to_value(char * n) {
    if(strcmp("none", n)==0){ return LV_FONT_SUBPX_NONE ; }
    else if(strcmp("hor", n)==0){ return LV_FONT_SUBPX_HOR ; }
    else if(strcmp("ver", n)==0){ return LV_FONT_SUBPX_VER ; }
    else if(strcmp("both", n)==0){ return LV_FONT_SUBPX_BOTH ; }
    else {
        return (uint32_t)-1 ;
    }
}
// "lv_text_flag_t": /^(_?)LV_TEXT_FLAG_/ ,
EMSCRIPTEN_KEEPALIVE char * lv_text_flag_to_name(uint32_t v) {
    if(LV_TEXT_FLAG_NONE==v){ return "none" ; }
    else if(LV_TEXT_FLAG_RECOLOR==v){ return "recolor" ; }
    else if(LV_TEXT_FLAG_EXPAND==v){ return "expand" ; }
    else if(LV_TEXT_FLAG_FIT==v){ return "fit" ; }
    else {
        return "<unknow>" ;
    }
}
EMSCRIPTEN_KEEPALIVE uint32_t lv_text_flag_to_value(char * n) {
    if(strcmp("none", n)==0){ return LV_TEXT_FLAG_NONE ; }
    else if(strcmp("recolor", n)==0){ return LV_TEXT_FLAG_RECOLOR ; }
    else if(strcmp("expand", n)==0){ return LV_TEXT_FLAG_EXPAND ; }
    else if(strcmp("fit", n)==0){ return LV_TEXT_FLAG_FIT ; }
    else {
        return (uint32_t)-1 ;
    }
}
// "lv_text_cmd_state_t": /^(_?)LV_TEXT_CMD_STATE_/ ,
EMSCRIPTEN_KEEPALIVE char * lv_text_cmd_state_to_name(uint32_t v) {
    if(LV_TEXT_CMD_STATE_WAIT==v){ return "wait" ; }
    else if(LV_TEXT_CMD_STATE_PAR==v){ return "par" ; }
    else if(LV_TEXT_CMD_STATE_IN==v){ return "in" ; }
    else {
        return "<unknow>" ;
    }
}
EMSCRIPTEN_KEEPALIVE uint32_t lv_text_cmd_state_to_value(char * n) {
    if(strcmp("wait", n)==0){ return LV_TEXT_CMD_STATE_WAIT ; }
    else if(strcmp("par", n)==0){ return LV_TEXT_CMD_STATE_PAR ; }
    else if(strcmp("in", n)==0){ return LV_TEXT_CMD_STATE_IN ; }
    else {
        return (uint32_t)-1 ;
    }
}
// "lv_text_align_t": /^(_?)LV_TEXT_ALIGN_/ ,
EMSCRIPTEN_KEEPALIVE char * lv_text_align_to_name(uint32_t v) {
    if(LV_TEXT_ALIGN_AUTO==v){ return "auto" ; }
    else if(LV_TEXT_ALIGN_LEFT==v){ return "left" ; }
    else if(LV_TEXT_ALIGN_CENTER==v){ return "center" ; }
    else if(LV_TEXT_ALIGN_RIGHT==v){ return "right" ; }
    else {
        return "<unknow>" ;
    }
}
EMSCRIPTEN_KEEPALIVE uint32_t lv_text_align_to_value(char * n) {
    if(strcmp("auto", n)==0){ return LV_TEXT_ALIGN_AUTO ; }
    else if(strcmp("left", n)==0){ return LV_TEXT_ALIGN_LEFT ; }
    else if(strcmp("center", n)==0){ return LV_TEXT_ALIGN_CENTER ; }
    else if(strcmp("right", n)==0){ return LV_TEXT_ALIGN_RIGHT ; }
    else {
        return (uint32_t)-1 ;
    }
}
// "lv_base_dir_t": /^(_?)LV_BASE_DIR_/ ,
EMSCRIPTEN_KEEPALIVE char * lv_base_dir_to_name(uint32_t v) {
    if(LV_BASE_DIR_LTR==v){ return "ltr" ; }
    else if(LV_BASE_DIR_RTL==v){ return "rtl" ; }
    else if(LV_BASE_DIR_AUTO==v){ return "auto" ; }
    else if(LV_BASE_DIR_NEUTRAL==v){ return "neutral" ; }
    else if(LV_BASE_DIR_WEAK==v){ return "weak" ; }
    else {
        return "<unknow>" ;
    }
}
EMSCRIPTEN_KEEPALIVE uint32_t lv_base_dir_to_value(char * n) {
    if(strcmp("ltr", n)==0){ return LV_BASE_DIR_LTR ; }
    else if(strcmp("rtl", n)==0){ return LV_BASE_DIR_RTL ; }
    else if(strcmp("auto", n)==0){ return LV_BASE_DIR_AUTO ; }
    else if(strcmp("neutral", n)==0){ return LV_BASE_DIR_NEUTRAL ; }
    else if(strcmp("weak", n)==0){ return LV_BASE_DIR_WEAK ; }
    else {
        return (uint32_t)-1 ;
    }
}
// "lv_blend_mode_t": /^(_?)LV_BLEND_/ ,
EMSCRIPTEN_KEEPALIVE char * lv_blend_mode_to_name(uint32_t v) {
    if(LV_BLEND_MODE_NORMAL==v){ return "mode-normal" ; }
    else if(LV_BLEND_MODE_ADDITIVE==v){ return "mode-additive" ; }
    else if(LV_BLEND_MODE_SUBTRACTIVE==v){ return "mode-subtractive" ; }
    else if(LV_BLEND_MODE_MULTIPLY==v){ return "mode-multiply" ; }
    else {
        return "<unknow>" ;
    }
}
EMSCRIPTEN_KEEPALIVE uint32_t lv_blend_mode_to_value(char * n) {
    if(strcmp("mode-normal", n)==0){ return LV_BLEND_MODE_NORMAL ; }
    else if(strcmp("mode-additive", n)==0){ return LV_BLEND_MODE_ADDITIVE ; }
    else if(strcmp("mode-subtractive", n)==0){ return LV_BLEND_MODE_SUBTRACTIVE ; }
    else if(strcmp("mode-multiply", n)==0){ return LV_BLEND_MODE_MULTIPLY ; }
    else {
        return (uint32_t)-1 ;
    }
}
// "lv_text_decor_t": /^(_?)LV_TEXT_DECOR_/ ,
EMSCRIPTEN_KEEPALIVE char * lv_text_decor_to_name(uint32_t v) {
    if(LV_TEXT_DECOR_NONE==v){ return "none" ; }
    else if(LV_TEXT_DECOR_UNDERLINE==v){ return "underline" ; }
    else if(LV_TEXT_DECOR_STRIKETHROUGH==v){ return "strikethrough" ; }
    else {
        return "<unknow>" ;
    }
}
EMSCRIPTEN_KEEPALIVE uint32_t lv_text_decor_to_value(char * n) {
    if(strcmp("none", n)==0){ return LV_TEXT_DECOR_NONE ; }
    else if(strcmp("underline", n)==0){ return LV_TEXT_DECOR_UNDERLINE ; }
    else if(strcmp("strikethrough", n)==0){ return LV_TEXT_DECOR_STRIKETHROUGH ; }
    else {
        return (uint32_t)-1 ;
    }
}
// "lv_border_side_t": /^(_?)LV_BORDER_SIDE_/ ,
EMSCRIPTEN_KEEPALIVE char * lv_border_side_to_name(uint32_t v) {
    if(LV_BORDER_SIDE_NONE==v){ return "none" ; }
    else if(LV_BORDER_SIDE_BOTTOM==v){ return "bottom" ; }
    else if(LV_BORDER_SIDE_TOP==v){ return "top" ; }
    else if(LV_BORDER_SIDE_LEFT==v){ return "left" ; }
    else if(LV_BORDER_SIDE_RIGHT==v){ return "right" ; }
    else if(LV_BORDER_SIDE_FULL==v){ return "full" ; }
    else if(LV_BORDER_SIDE_INTERNAL==v){ return "internal" ; }
    else {
        return "<unknow>" ;
    }
}
EMSCRIPTEN_KEEPALIVE uint32_t lv_border_side_to_value(char * n) {
    if(strcmp("none", n)==0){ return LV_BORDER_SIDE_NONE ; }
    else if(strcmp("bottom", n)==0){ return LV_BORDER_SIDE_BOTTOM ; }
    else if(strcmp("top", n)==0){ return LV_BORDER_SIDE_TOP ; }
    else if(strcmp("left", n)==0){ return LV_BORDER_SIDE_LEFT ; }
    else if(strcmp("right", n)==0){ return LV_BORDER_SIDE_RIGHT ; }
    else if(strcmp("full", n)==0){ return LV_BORDER_SIDE_FULL ; }
    else if(strcmp("internal", n)==0){ return LV_BORDER_SIDE_INTERNAL ; }
    else {
        return (uint32_t)-1 ;
    }
}
// "lv_grad_dir_t": /^(_?)LV_GRAD_DIR_/ ,
EMSCRIPTEN_KEEPALIVE char * lv_grad_dir_to_name(uint32_t v) {
    if(LV_GRAD_DIR_NONE==v){ return "none" ; }
    else if(LV_GRAD_DIR_VER==v){ return "ver" ; }
    else if(LV_GRAD_DIR_HOR==v){ return "hor" ; }
    else {
        return "<unknow>" ;
    }
}
EMSCRIPTEN_KEEPALIVE uint32_t lv_grad_dir_to_value(char * n) {
    if(strcmp("none", n)==0){ return LV_GRAD_DIR_NONE ; }
    else if(strcmp("ver", n)==0){ return LV_GRAD_DIR_VER ; }
    else if(strcmp("hor", n)==0){ return LV_GRAD_DIR_HOR ; }
    else {
        return (uint32_t)-1 ;
    }
}
// "lv_style_prop_t": /^(_?)LV_STYLE_/ ,
EMSCRIPTEN_KEEPALIVE char * lv_style_prop_to_name(uint32_t v) {
    if(LV_STYLE_PROP_INV==v){ return "prop-inv" ; }
    else if(LV_STYLE_WIDTH==v){ return "width" ; }
    else if(LV_STYLE_MIN_WIDTH==v){ return "min-width" ; }
    else if(LV_STYLE_MAX_WIDTH==v){ return "max-width" ; }
    else if(LV_STYLE_HEIGHT==v){ return "height" ; }
    else if(LV_STYLE_MIN_HEIGHT==v){ return "min-height" ; }
    else if(LV_STYLE_MAX_HEIGHT==v){ return "max-height" ; }
    else if(LV_STYLE_X==v){ return "x" ; }
    else if(LV_STYLE_Y==v){ return "y" ; }
    else if(LV_STYLE_ALIGN==v){ return "align" ; }
    else if(LV_STYLE_TRANSFORM_WIDTH==v){ return "transform-width" ; }
    else if(LV_STYLE_TRANSFORM_HEIGHT==v){ return "transform-height" ; }
    else if(LV_STYLE_TRANSLATE_X==v){ return "translate-x" ; }
    else if(LV_STYLE_TRANSLATE_Y==v){ return "translate-y" ; }
    else if(LV_STYLE_TRANSFORM_ZOOM==v){ return "transform-zoom" ; }
    else if(LV_STYLE_TRANSFORM_ANGLE==v){ return "transform-angle" ; }
    else if(LV_STYLE_PAD_TOP==v){ return "pad-top" ; }
    else if(LV_STYLE_PAD_BOTTOM==v){ return "pad-bottom" ; }
    else if(LV_STYLE_PAD_LEFT==v){ return "pad-left" ; }
    else if(LV_STYLE_PAD_RIGHT==v){ return "pad-right" ; }
    else if(LV_STYLE_PAD_ROW==v){ return "pad-row" ; }
    else if(LV_STYLE_PAD_COLUMN==v){ return "pad-column" ; }
    else if(LV_STYLE_BG_COLOR==v){ return "bg-color" ; }
    else if(LV_STYLE_BG_COLOR_FILTERED==v){ return "bg-color-filtered" ; }
    else if(LV_STYLE_BG_OPA==v){ return "bg-opa" ; }
    else if(LV_STYLE_BG_GRAD_COLOR==v){ return "bg-grad-color" ; }
    else if(LV_STYLE_BG_GRAD_COLOR_FILTERED==v){ return "bg-grad-color-filtered" ; }
    else if(LV_STYLE_BG_GRAD_DIR==v){ return "bg-grad-dir" ; }
    else if(LV_STYLE_BG_MAIN_STOP==v){ return "bg-main-stop" ; }
    else if(LV_STYLE_BG_GRAD_STOP==v){ return "bg-grad-stop" ; }
    else if(LV_STYLE_BG_IMG_SRC==v){ return "bg-img-src" ; }
    else if(LV_STYLE_BG_IMG_OPA==v){ return "bg-img-opa" ; }
    else if(LV_STYLE_BG_IMG_RECOLOR==v){ return "bg-img-recolor" ; }
    else if(LV_STYLE_BG_IMG_RECOLOR_FILTERED==v){ return "bg-img-recolor-filtered" ; }
    else if(LV_STYLE_BG_IMG_RECOLOR_OPA==v){ return "bg-img-recolor-opa" ; }
    else if(LV_STYLE_BG_IMG_TILED==v){ return "bg-img-tiled" ; }
    else if(LV_STYLE_BORDER_COLOR==v){ return "border-color" ; }
    else if(LV_STYLE_BORDER_COLOR_FILTERED==v){ return "border-color-filtered" ; }
    else if(LV_STYLE_BORDER_OPA==v){ return "border-opa" ; }
    else if(LV_STYLE_BORDER_WIDTH==v){ return "border-width" ; }
    else if(LV_STYLE_BORDER_SIDE==v){ return "border-side" ; }
    else if(LV_STYLE_BORDER_POST==v){ return "border-post" ; }
    else if(LV_STYLE_OUTLINE_WIDTH==v){ return "outline-width" ; }
    else if(LV_STYLE_OUTLINE_COLOR==v){ return "outline-color" ; }
    else if(LV_STYLE_OUTLINE_COLOR_FILTERED==v){ return "outline-color-filtered" ; }
    else if(LV_STYLE_OUTLINE_OPA==v){ return "outline-opa" ; }
    else if(LV_STYLE_OUTLINE_PAD==v){ return "outline-pad" ; }
    else if(LV_STYLE_SHADOW_WIDTH==v){ return "shadow-width" ; }
    else if(LV_STYLE_SHADOW_OFS_X==v){ return "shadow-ofs-x" ; }
    else if(LV_STYLE_SHADOW_OFS_Y==v){ return "shadow-ofs-y" ; }
    else if(LV_STYLE_SHADOW_SPREAD==v){ return "shadow-spread" ; }
    else if(LV_STYLE_SHADOW_COLOR==v){ return "shadow-color" ; }
    else if(LV_STYLE_SHADOW_COLOR_FILTERED==v){ return "shadow-color-filtered" ; }
    else if(LV_STYLE_SHADOW_OPA==v){ return "shadow-opa" ; }
    else if(LV_STYLE_IMG_OPA==v){ return "img-opa" ; }
    else if(LV_STYLE_IMG_RECOLOR==v){ return "img-recolor" ; }
    else if(LV_STYLE_IMG_RECOLOR_FILTERED==v){ return "img-recolor-filtered" ; }
    else if(LV_STYLE_IMG_RECOLOR_OPA==v){ return "img-recolor-opa" ; }
    else if(LV_STYLE_LINE_WIDTH==v){ return "line-width" ; }
    else if(LV_STYLE_LINE_DASH_WIDTH==v){ return "line-dash-width" ; }
    else if(LV_STYLE_LINE_DASH_GAP==v){ return "line-dash-gap" ; }
    else if(LV_STYLE_LINE_ROUNDED==v){ return "line-rounded" ; }
    else if(LV_STYLE_LINE_COLOR==v){ return "line-color" ; }
    else if(LV_STYLE_LINE_COLOR_FILTERED==v){ return "line-color-filtered" ; }
    else if(LV_STYLE_LINE_OPA==v){ return "line-opa" ; }
    else if(LV_STYLE_ARC_WIDTH==v){ return "arc-width" ; }
    else if(LV_STYLE_ARC_ROUNDED==v){ return "arc-rounded" ; }
    else if(LV_STYLE_ARC_COLOR==v){ return "arc-color" ; }
    else if(LV_STYLE_ARC_COLOR_FILTERED==v){ return "arc-color-filtered" ; }
    else if(LV_STYLE_ARC_OPA==v){ return "arc-opa" ; }
    else if(LV_STYLE_ARC_IMG_SRC==v){ return "arc-img-src" ; }
    else if(LV_STYLE_TEXT_COLOR==v){ return "text-color" ; }
    else if(LV_STYLE_TEXT_COLOR_FILTERED==v){ return "text-color-filtered" ; }
    else if(LV_STYLE_TEXT_OPA==v){ return "text-opa" ; }
    else if(LV_STYLE_TEXT_FONT==v){ return "text-font" ; }
    else if(LV_STYLE_TEXT_LETTER_SPACE==v){ return "text-letter-space" ; }
    else if(LV_STYLE_TEXT_LINE_SPACE==v){ return "text-line-space" ; }
    else if(LV_STYLE_TEXT_DECOR==v){ return "text-decor" ; }
    else if(LV_STYLE_TEXT_ALIGN==v){ return "text-align" ; }
    else if(LV_STYLE_RADIUS==v){ return "radius" ; }
    else if(LV_STYLE_CLIP_CORNER==v){ return "clip-corner" ; }
    else if(LV_STYLE_OPA==v){ return "opa" ; }
    else if(LV_STYLE_COLOR_FILTER_DSC==v){ return "color-filter-dsc" ; }
    else if(LV_STYLE_COLOR_FILTER_OPA==v){ return "color-filter-opa" ; }
    else if(LV_STYLE_ANIM_TIME==v){ return "anim-time" ; }
    else if(LV_STYLE_ANIM_SPEED==v){ return "anim-speed" ; }
    else if(LV_STYLE_TRANSITION==v){ return "transition" ; }
    else if(LV_STYLE_BLEND_MODE==v){ return "blend-mode" ; }
    else if(LV_STYLE_LAYOUT==v){ return "layout" ; }
    else if(LV_STYLE_BASE_DIR==v){ return "base-dir" ; }
    else if(_LV_STYLE_LAST_BUILT_IN_PROP==v){ return "_last-built-in-prop" ; }
    else if(LV_STYLE_PROP_ANY==v){ return "prop-any" ; }
    else {
        return "<unknow>" ;
    }
}
EMSCRIPTEN_KEEPALIVE uint32_t lv_style_prop_to_value(char * n) {
    if(strcmp("prop-inv", n)==0){ return LV_STYLE_PROP_INV ; }
    else if(strcmp("width", n)==0){ return LV_STYLE_WIDTH ; }
    else if(strcmp("min-width", n)==0){ return LV_STYLE_MIN_WIDTH ; }
    else if(strcmp("max-width", n)==0){ return LV_STYLE_MAX_WIDTH ; }
    else if(strcmp("height", n)==0){ return LV_STYLE_HEIGHT ; }
    else if(strcmp("min-height", n)==0){ return LV_STYLE_MIN_HEIGHT ; }
    else if(strcmp("max-height", n)==0){ return LV_STYLE_MAX_HEIGHT ; }
    else if(strcmp("x", n)==0){ return LV_STYLE_X ; }
    else if(strcmp("y", n)==0){ return LV_STYLE_Y ; }
    else if(strcmp("align", n)==0){ return LV_STYLE_ALIGN ; }
    else if(strcmp("transform-width", n)==0){ return LV_STYLE_TRANSFORM_WIDTH ; }
    else if(strcmp("transform-height", n)==0){ return LV_STYLE_TRANSFORM_HEIGHT ; }
    else if(strcmp("translate-x", n)==0){ return LV_STYLE_TRANSLATE_X ; }
    else if(strcmp("translate-y", n)==0){ return LV_STYLE_TRANSLATE_Y ; }
    else if(strcmp("transform-zoom", n)==0){ return LV_STYLE_TRANSFORM_ZOOM ; }
    else if(strcmp("transform-angle", n)==0){ return LV_STYLE_TRANSFORM_ANGLE ; }
    else if(strcmp("pad-top", n)==0){ return LV_STYLE_PAD_TOP ; }
    else if(strcmp("pad-bottom", n)==0){ return LV_STYLE_PAD_BOTTOM ; }
    else if(strcmp("pad-left", n)==0){ return LV_STYLE_PAD_LEFT ; }
    else if(strcmp("pad-right", n)==0){ return LV_STYLE_PAD_RIGHT ; }
    else if(strcmp("pad-row", n)==0){ return LV_STYLE_PAD_ROW ; }
    else if(strcmp("pad-column", n)==0){ return LV_STYLE_PAD_COLUMN ; }
    else if(strcmp("bg-color", n)==0){ return LV_STYLE_BG_COLOR ; }
    else if(strcmp("bg-color-filtered", n)==0){ return LV_STYLE_BG_COLOR_FILTERED ; }
    else if(strcmp("bg-opa", n)==0){ return LV_STYLE_BG_OPA ; }
    else if(strcmp("bg-grad-color", n)==0){ return LV_STYLE_BG_GRAD_COLOR ; }
    else if(strcmp("bg-grad-color-filtered", n)==0){ return LV_STYLE_BG_GRAD_COLOR_FILTERED ; }
    else if(strcmp("bg-grad-dir", n)==0){ return LV_STYLE_BG_GRAD_DIR ; }
    else if(strcmp("bg-main-stop", n)==0){ return LV_STYLE_BG_MAIN_STOP ; }
    else if(strcmp("bg-grad-stop", n)==0){ return LV_STYLE_BG_GRAD_STOP ; }
    else if(strcmp("bg-img-src", n)==0){ return LV_STYLE_BG_IMG_SRC ; }
    else if(strcmp("bg-img-opa", n)==0){ return LV_STYLE_BG_IMG_OPA ; }
    else if(strcmp("bg-img-recolor", n)==0){ return LV_STYLE_BG_IMG_RECOLOR ; }
    else if(strcmp("bg-img-recolor-filtered", n)==0){ return LV_STYLE_BG_IMG_RECOLOR_FILTERED ; }
    else if(strcmp("bg-img-recolor-opa", n)==0){ return LV_STYLE_BG_IMG_RECOLOR_OPA ; }
    else if(strcmp("bg-img-tiled", n)==0){ return LV_STYLE_BG_IMG_TILED ; }
    else if(strcmp("border-color", n)==0){ return LV_STYLE_BORDER_COLOR ; }
    else if(strcmp("border-color-filtered", n)==0){ return LV_STYLE_BORDER_COLOR_FILTERED ; }
    else if(strcmp("border-opa", n)==0){ return LV_STYLE_BORDER_OPA ; }
    else if(strcmp("border-width", n)==0){ return LV_STYLE_BORDER_WIDTH ; }
    else if(strcmp("border-side", n)==0){ return LV_STYLE_BORDER_SIDE ; }
    else if(strcmp("border-post", n)==0){ return LV_STYLE_BORDER_POST ; }
    else if(strcmp("outline-width", n)==0){ return LV_STYLE_OUTLINE_WIDTH ; }
    else if(strcmp("outline-color", n)==0){ return LV_STYLE_OUTLINE_COLOR ; }
    else if(strcmp("outline-color-filtered", n)==0){ return LV_STYLE_OUTLINE_COLOR_FILTERED ; }
    else if(strcmp("outline-opa", n)==0){ return LV_STYLE_OUTLINE_OPA ; }
    else if(strcmp("outline-pad", n)==0){ return LV_STYLE_OUTLINE_PAD ; }
    else if(strcmp("shadow-width", n)==0){ return LV_STYLE_SHADOW_WIDTH ; }
    else if(strcmp("shadow-ofs-x", n)==0){ return LV_STYLE_SHADOW_OFS_X ; }
    else if(strcmp("shadow-ofs-y", n)==0){ return LV_STYLE_SHADOW_OFS_Y ; }
    else if(strcmp("shadow-spread", n)==0){ return LV_STYLE_SHADOW_SPREAD ; }
    else if(strcmp("shadow-color", n)==0){ return LV_STYLE_SHADOW_COLOR ; }
    else if(strcmp("shadow-color-filtered", n)==0){ return LV_STYLE_SHADOW_COLOR_FILTERED ; }
    else if(strcmp("shadow-opa", n)==0){ return LV_STYLE_SHADOW_OPA ; }
    else if(strcmp("img-opa", n)==0){ return LV_STYLE_IMG_OPA ; }
    else if(strcmp("img-recolor", n)==0){ return LV_STYLE_IMG_RECOLOR ; }
    else if(strcmp("img-recolor-filtered", n)==0){ return LV_STYLE_IMG_RECOLOR_FILTERED ; }
    else if(strcmp("img-recolor-opa", n)==0){ return LV_STYLE_IMG_RECOLOR_OPA ; }
    else if(strcmp("line-width", n)==0){ return LV_STYLE_LINE_WIDTH ; }
    else if(strcmp("line-dash-width", n)==0){ return LV_STYLE_LINE_DASH_WIDTH ; }
    else if(strcmp("line-dash-gap", n)==0){ return LV_STYLE_LINE_DASH_GAP ; }
    else if(strcmp("line-rounded", n)==0){ return LV_STYLE_LINE_ROUNDED ; }
    else if(strcmp("line-color", n)==0){ return LV_STYLE_LINE_COLOR ; }
    else if(strcmp("line-color-filtered", n)==0){ return LV_STYLE_LINE_COLOR_FILTERED ; }
    else if(strcmp("line-opa", n)==0){ return LV_STYLE_LINE_OPA ; }
    else if(strcmp("arc-width", n)==0){ return LV_STYLE_ARC_WIDTH ; }
    else if(strcmp("arc-rounded", n)==0){ return LV_STYLE_ARC_ROUNDED ; }
    else if(strcmp("arc-color", n)==0){ return LV_STYLE_ARC_COLOR ; }
    else if(strcmp("arc-color-filtered", n)==0){ return LV_STYLE_ARC_COLOR_FILTERED ; }
    else if(strcmp("arc-opa", n)==0){ return LV_STYLE_ARC_OPA ; }
    else if(strcmp("arc-img-src", n)==0){ return LV_STYLE_ARC_IMG_SRC ; }
    else if(strcmp("text-color", n)==0){ return LV_STYLE_TEXT_COLOR ; }
    else if(strcmp("text-color-filtered", n)==0){ return LV_STYLE_TEXT_COLOR_FILTERED ; }
    else if(strcmp("text-opa", n)==0){ return LV_STYLE_TEXT_OPA ; }
    else if(strcmp("text-font", n)==0){ return LV_STYLE_TEXT_FONT ; }
    else if(strcmp("text-letter-space", n)==0){ return LV_STYLE_TEXT_LETTER_SPACE ; }
    else if(strcmp("text-line-space", n)==0){ return LV_STYLE_TEXT_LINE_SPACE ; }
    else if(strcmp("text-decor", n)==0){ return LV_STYLE_TEXT_DECOR ; }
    else if(strcmp("text-align", n)==0){ return LV_STYLE_TEXT_ALIGN ; }
    else if(strcmp("radius", n)==0){ return LV_STYLE_RADIUS ; }
    else if(strcmp("clip-corner", n)==0){ return LV_STYLE_CLIP_CORNER ; }
    else if(strcmp("opa", n)==0){ return LV_STYLE_OPA ; }
    else if(strcmp("color-filter-dsc", n)==0){ return LV_STYLE_COLOR_FILTER_DSC ; }
    else if(strcmp("color-filter-opa", n)==0){ return LV_STYLE_COLOR_FILTER_OPA ; }
    else if(strcmp("anim-time", n)==0){ return LV_STYLE_ANIM_TIME ; }
    else if(strcmp("anim-speed", n)==0){ return LV_STYLE_ANIM_SPEED ; }
    else if(strcmp("transition", n)==0){ return LV_STYLE_TRANSITION ; }
    else if(strcmp("blend-mode", n)==0){ return LV_STYLE_BLEND_MODE ; }
    else if(strcmp("layout", n)==0){ return LV_STYLE_LAYOUT ; }
    else if(strcmp("base-dir", n)==0){ return LV_STYLE_BASE_DIR ; }
    else if(strcmp("_last-built-in-prop", n)==0){ return _LV_STYLE_LAST_BUILT_IN_PROP ; }
    else if(strcmp("prop-any", n)==0){ return LV_STYLE_PROP_ANY ; }
    else {
        return (uint32_t)-1 ;
    }
}
// "lv_state_t": /^(_?)LV_STATE_/ ,
EMSCRIPTEN_KEEPALIVE char * lv_state_to_name(uint32_t v) {
    if(LV_STATE_DEFAULT==v){ return "default" ; }
    else if(LV_STATE_CHECKED==v){ return "checked" ; }
    else if(LV_STATE_FOCUSED==v){ return "focused" ; }
    else if(LV_STATE_FOCUS_KEY==v){ return "focus-key" ; }
    else if(LV_STATE_EDITED==v){ return "edited" ; }
    else if(LV_STATE_HOVERED==v){ return "hovered" ; }
    else if(LV_STATE_PRESSED==v){ return "pressed" ; }
    else if(LV_STATE_SCROLLED==v){ return "scrolled" ; }
    else if(LV_STATE_DISABLED==v){ return "disabled" ; }
    else if(LV_STATE_USER_1==v){ return "user-1" ; }
    else if(LV_STATE_USER_2==v){ return "user-2" ; }
    else if(LV_STATE_USER_3==v){ return "user-3" ; }
    else if(LV_STATE_USER_4==v){ return "user-4" ; }
    else if(LV_STATE_ANY==v){ return "any" ; }
    else {
        return "<unknow>" ;
    }
}
EMSCRIPTEN_KEEPALIVE uint32_t lv_state_to_value(char * n) {
    if(strcmp("default", n)==0){ return LV_STATE_DEFAULT ; }
    else if(strcmp("checked", n)==0){ return LV_STATE_CHECKED ; }
    else if(strcmp("focused", n)==0){ return LV_STATE_FOCUSED ; }
    else if(strcmp("focus-key", n)==0){ return LV_STATE_FOCUS_KEY ; }
    else if(strcmp("edited", n)==0){ return LV_STATE_EDITED ; }
    else if(strcmp("hovered", n)==0){ return LV_STATE_HOVERED ; }
    else if(strcmp("pressed", n)==0){ return LV_STATE_PRESSED ; }
    else if(strcmp("scrolled", n)==0){ return LV_STATE_SCROLLED ; }
    else if(strcmp("disabled", n)==0){ return LV_STATE_DISABLED ; }
    else if(strcmp("user-1", n)==0){ return LV_STATE_USER_1 ; }
    else if(strcmp("user-2", n)==0){ return LV_STATE_USER_2 ; }
    else if(strcmp("user-3", n)==0){ return LV_STATE_USER_3 ; }
    else if(strcmp("user-4", n)==0){ return LV_STATE_USER_4 ; }
    else if(strcmp("any", n)==0){ return LV_STATE_ANY ; }
    else {
        return (uint32_t)-1 ;
    }
}
// "lv_part_t": /^(_?)LV_PART_/ ,
EMSCRIPTEN_KEEPALIVE char * lv_part_to_name(uint32_t v) {
    if(LV_PART_MAIN==v){ return "main" ; }
    else if(LV_PART_SCROLLBAR==v){ return "scrollbar" ; }
    else if(LV_PART_INDICATOR==v){ return "indicator" ; }
    else if(LV_PART_KNOB==v){ return "knob" ; }
    else if(LV_PART_SELECTED==v){ return "selected" ; }
    else if(LV_PART_ITEMS==v){ return "items" ; }
    else if(LV_PART_TICKS==v){ return "ticks" ; }
    else if(LV_PART_CURSOR==v){ return "cursor" ; }
    else if(LV_PART_CUSTOM_FIRST==v){ return "custom-first" ; }
    else if(LV_PART_ANY==v){ return "any" ; }
    else {
        return "<unknow>" ;
    }
}
EMSCRIPTEN_KEEPALIVE uint32_t lv_part_to_value(char * n) {
    if(strcmp("main", n)==0){ return LV_PART_MAIN ; }
    else if(strcmp("scrollbar", n)==0){ return LV_PART_SCROLLBAR ; }
    else if(strcmp("indicator", n)==0){ return LV_PART_INDICATOR ; }
    else if(strcmp("knob", n)==0){ return LV_PART_KNOB ; }
    else if(strcmp("selected", n)==0){ return LV_PART_SELECTED ; }
    else if(strcmp("items", n)==0){ return LV_PART_ITEMS ; }
    else if(strcmp("ticks", n)==0){ return LV_PART_TICKS ; }
    else if(strcmp("cursor", n)==0){ return LV_PART_CURSOR ; }
    else if(strcmp("custom-first", n)==0){ return LV_PART_CUSTOM_FIRST ; }
    else if(strcmp("any", n)==0){ return LV_PART_ANY ; }
    else {
        return (uint32_t)-1 ;
    }
}
// "lv_obj_flag_t": /^(_?)LV_OBJ_FLAG_/ ,
EMSCRIPTEN_KEEPALIVE char * lv_obj_flag_to_name(uint32_t v) {
    if(LV_OBJ_FLAG_HIDDEN==v){ return "hidden" ; }
    else if(LV_OBJ_FLAG_CLICKABLE==v){ return "clickable" ; }
    else if(LV_OBJ_FLAG_CLICK_FOCUSABLE==v){ return "click-focusable" ; }
    else if(LV_OBJ_FLAG_CHECKABLE==v){ return "checkable" ; }
    else if(LV_OBJ_FLAG_SCROLLABLE==v){ return "scrollable" ; }
    else if(LV_OBJ_FLAG_SCROLL_ELASTIC==v){ return "scroll-elastic" ; }
    else if(LV_OBJ_FLAG_SCROLL_MOMENTUM==v){ return "scroll-momentum" ; }
    else if(LV_OBJ_FLAG_SCROLL_ONE==v){ return "scroll-one" ; }
    else if(LV_OBJ_FLAG_SCROLL_CHAIN==v){ return "scroll-chain" ; }
    else if(LV_OBJ_FLAG_SCROLL_ON_FOCUS==v){ return "scroll-on-focus" ; }
    else if(LV_OBJ_FLAG_SNAPPABLE==v){ return "snappable" ; }
    else if(LV_OBJ_FLAG_PRESS_LOCK==v){ return "press-lock" ; }
    else if(LV_OBJ_FLAG_EVENT_BUBBLE==v){ return "event-bubble" ; }
    else if(LV_OBJ_FLAG_GESTURE_BUBBLE==v){ return "gesture-bubble" ; }
    else if(LV_OBJ_FLAG_ADV_HITTEST==v){ return "adv-hittest" ; }
    else if(LV_OBJ_FLAG_IGNORE_LAYOUT==v){ return "ignore-layout" ; }
    else if(LV_OBJ_FLAG_FLOATING==v){ return "floating" ; }
    else if(LV_OBJ_FLAG_LAYOUT_1==v){ return "layout-1" ; }
    else if(LV_OBJ_FLAG_LAYOUT_2==v){ return "layout-2" ; }
    else if(LV_OBJ_FLAG_WIDGET_1==v){ return "widget-1" ; }
    else if(LV_OBJ_FLAG_WIDGET_2==v){ return "widget-2" ; }
    else if(LV_OBJ_FLAG_USER_1==v){ return "user-1" ; }
    else if(LV_OBJ_FLAG_USER_2==v){ return "user-2" ; }
    else if(LV_OBJ_FLAG_USER_3==v){ return "user-3" ; }
    else if(LV_OBJ_FLAG_USER_4==v){ return "user-4" ; }
    else {
        return "<unknow>" ;
    }
}
EMSCRIPTEN_KEEPALIVE uint32_t lv_obj_flag_to_value(char * n) {
    if(strcmp("hidden", n)==0){ return LV_OBJ_FLAG_HIDDEN ; }
    else if(strcmp("clickable", n)==0){ return LV_OBJ_FLAG_CLICKABLE ; }
    else if(strcmp("click-focusable", n)==0){ return LV_OBJ_FLAG_CLICK_FOCUSABLE ; }
    else if(strcmp("checkable", n)==0){ return LV_OBJ_FLAG_CHECKABLE ; }
    else if(strcmp("scrollable", n)==0){ return LV_OBJ_FLAG_SCROLLABLE ; }
    else if(strcmp("scroll-elastic", n)==0){ return LV_OBJ_FLAG_SCROLL_ELASTIC ; }
    else if(strcmp("scroll-momentum", n)==0){ return LV_OBJ_FLAG_SCROLL_MOMENTUM ; }
    else if(strcmp("scroll-one", n)==0){ return LV_OBJ_FLAG_SCROLL_ONE ; }
    else if(strcmp("scroll-chain", n)==0){ return LV_OBJ_FLAG_SCROLL_CHAIN ; }
    else if(strcmp("scroll-on-focus", n)==0){ return LV_OBJ_FLAG_SCROLL_ON_FOCUS ; }
    else if(strcmp("snappable", n)==0){ return LV_OBJ_FLAG_SNAPPABLE ; }
    else if(strcmp("press-lock", n)==0){ return LV_OBJ_FLAG_PRESS_LOCK ; }
    else if(strcmp("event-bubble", n)==0){ return LV_OBJ_FLAG_EVENT_BUBBLE ; }
    else if(strcmp("gesture-bubble", n)==0){ return LV_OBJ_FLAG_GESTURE_BUBBLE ; }
    else if(strcmp("adv-hittest", n)==0){ return LV_OBJ_FLAG_ADV_HITTEST ; }
    else if(strcmp("ignore-layout", n)==0){ return LV_OBJ_FLAG_IGNORE_LAYOUT ; }
    else if(strcmp("floating", n)==0){ return LV_OBJ_FLAG_FLOATING ; }
    else if(strcmp("layout-1", n)==0){ return LV_OBJ_FLAG_LAYOUT_1 ; }
    else if(strcmp("layout-2", n)==0){ return LV_OBJ_FLAG_LAYOUT_2 ; }
    else if(strcmp("widget-1", n)==0){ return LV_OBJ_FLAG_WIDGET_1 ; }
    else if(strcmp("widget-2", n)==0){ return LV_OBJ_FLAG_WIDGET_2 ; }
    else if(strcmp("user-1", n)==0){ return LV_OBJ_FLAG_USER_1 ; }
    else if(strcmp("user-2", n)==0){ return LV_OBJ_FLAG_USER_2 ; }
    else if(strcmp("user-3", n)==0){ return LV_OBJ_FLAG_USER_3 ; }
    else if(strcmp("user-4", n)==0){ return LV_OBJ_FLAG_USER_4 ; }
    else {
        return (uint32_t)-1 ;
    }
}
// "lv_obj_draw_part_type_t": /^(_?)LV_OBJ_DRAW_PART_/ ,
EMSCRIPTEN_KEEPALIVE char * lv_obj_draw_part_type_to_name(uint32_t v) {
    if(LV_OBJ_DRAW_PART_RECTANGLE==v){ return "rectangle" ; }
    else if(LV_OBJ_DRAW_PART_BORDER_POST==v){ return "border-post" ; }
    else if(LV_OBJ_DRAW_PART_SCROLLBAR==v){ return "scrollbar" ; }
    else {
        return "<unknow>" ;
    }
}
EMSCRIPTEN_KEEPALIVE uint32_t lv_obj_draw_part_type_to_value(char * n) {
    if(strcmp("rectangle", n)==0){ return LV_OBJ_DRAW_PART_RECTANGLE ; }
    else if(strcmp("border-post", n)==0){ return LV_OBJ_DRAW_PART_BORDER_POST ; }
    else if(strcmp("scrollbar", n)==0){ return LV_OBJ_DRAW_PART_SCROLLBAR ; }
    else {
        return (uint32_t)-1 ;
    }
}
// "lv_obj_tree_walk_res_t": /^(_?)LV_OBJ_TREE_WALK_/ ,
EMSCRIPTEN_KEEPALIVE char * lv_obj_tree_walk_res_to_name(uint32_t v) {
    if(LV_OBJ_TREE_WALK_NEXT==v){ return "next" ; }
    else if(LV_OBJ_TREE_WALK_SKIP_CHILDREN==v){ return "skip-children" ; }
    else if(LV_OBJ_TREE_WALK_END==v){ return "end" ; }
    else {
        return "<unknow>" ;
    }
}
EMSCRIPTEN_KEEPALIVE uint32_t lv_obj_tree_walk_res_to_value(char * n) {
    if(strcmp("next", n)==0){ return LV_OBJ_TREE_WALK_NEXT ; }
    else if(strcmp("skip-children", n)==0){ return LV_OBJ_TREE_WALK_SKIP_CHILDREN ; }
    else if(strcmp("end", n)==0){ return LV_OBJ_TREE_WALK_END ; }
    else {
        return (uint32_t)-1 ;
    }
}
// "lv_scrollbar_mode_t": /^(_?)LV_SCROLLBAR_/ ,
EMSCRIPTEN_KEEPALIVE char * lv_scrollbar_mode_to_name(uint32_t v) {
    if(LV_SCROLLBAR_MODE_OFF==v){ return "mode-off" ; }
    else if(LV_SCROLLBAR_MODE_ON==v){ return "mode-on" ; }
    else if(LV_SCROLLBAR_MODE_ACTIVE==v){ return "mode-active" ; }
    else if(LV_SCROLLBAR_MODE_AUTO==v){ return "mode-auto" ; }
    else {
        return "<unknow>" ;
    }
}
EMSCRIPTEN_KEEPALIVE uint32_t lv_scrollbar_mode_to_value(char * n) {
    if(strcmp("mode-off", n)==0){ return LV_SCROLLBAR_MODE_OFF ; }
    else if(strcmp("mode-on", n)==0){ return LV_SCROLLBAR_MODE_ON ; }
    else if(strcmp("mode-active", n)==0){ return LV_SCROLLBAR_MODE_ACTIVE ; }
    else if(strcmp("mode-auto", n)==0){ return LV_SCROLLBAR_MODE_AUTO ; }
    else {
        return (uint32_t)-1 ;
    }
}
// "lv_scroll_snap_t": /^(_?)LV_SCROLL_SNAP_/ ,
EMSCRIPTEN_KEEPALIVE char * lv_scroll_snap_to_name(uint32_t v) {
    if(LV_SCROLL_SNAP_NONE==v){ return "none" ; }
    else if(LV_SCROLL_SNAP_START==v){ return "start" ; }
    else if(LV_SCROLL_SNAP_END==v){ return "end" ; }
    else if(LV_SCROLL_SNAP_CENTER==v){ return "center" ; }
    else {
        return "<unknow>" ;
    }
}
EMSCRIPTEN_KEEPALIVE uint32_t lv_scroll_snap_to_value(char * n) {
    if(strcmp("none", n)==0){ return LV_SCROLL_SNAP_NONE ; }
    else if(strcmp("start", n)==0){ return LV_SCROLL_SNAP_START ; }
    else if(strcmp("end", n)==0){ return LV_SCROLL_SNAP_END ; }
    else if(strcmp("center", n)==0){ return LV_SCROLL_SNAP_CENTER ; }
    else {
        return (uint32_t)-1 ;
    }
}
// "_lv_style_state_cmp_t": /^(_?)_LV_STYLE_STATE_CMP_/ ,
EMSCRIPTEN_KEEPALIVE char * lv__style_state_cmp_to_name(uint32_t v) {
    if(_LV_STYLE_STATE_CMP_SAME==v){ return "same" ; }
    else if(_LV_STYLE_STATE_CMP_DIFF_REDRAW==v){ return "diff-redraw" ; }
    else if(_LV_STYLE_STATE_CMP_DIFF_DRAW_PAD==v){ return "diff-draw-pad" ; }
    else if(_LV_STYLE_STATE_CMP_DIFF_LAYOUT==v){ return "diff-layout" ; }
    else {
        return "<unknow>" ;
    }
}
EMSCRIPTEN_KEEPALIVE uint32_t lv__style_state_cmp_to_value(char * n) {
    if(strcmp("same", n)==0){ return _LV_STYLE_STATE_CMP_SAME ; }
    else if(strcmp("diff-redraw", n)==0){ return _LV_STYLE_STATE_CMP_DIFF_REDRAW ; }
    else if(strcmp("diff-draw-pad", n)==0){ return _LV_STYLE_STATE_CMP_DIFF_DRAW_PAD ; }
    else if(strcmp("diff-layout", n)==0){ return _LV_STYLE_STATE_CMP_DIFF_LAYOUT ; }
    else {
        return (uint32_t)-1 ;
    }
}
// "lv_fs_res_t": /^(_?)LV_FS_RES_/ ,
EMSCRIPTEN_KEEPALIVE char * lv_fs_res_to_name(uint32_t v) {
    if(LV_FS_RES_OK==v){ return "ok" ; }
    else if(LV_FS_RES_HW_ERR==v){ return "hw-err" ; }
    else if(LV_FS_RES_FS_ERR==v){ return "fs-err" ; }
    else if(LV_FS_RES_NOT_EX==v){ return "not-ex" ; }
    else if(LV_FS_RES_FULL==v){ return "full" ; }
    else if(LV_FS_RES_LOCKED==v){ return "locked" ; }
    else if(LV_FS_RES_DENIED==v){ return "denied" ; }
    else if(LV_FS_RES_BUSY==v){ return "busy" ; }
    else if(LV_FS_RES_TOUT==v){ return "tout" ; }
    else if(LV_FS_RES_NOT_IMP==v){ return "not-imp" ; }
    else if(LV_FS_RES_OUT_OF_MEM==v){ return "out-of-mem" ; }
    else if(LV_FS_RES_INV_PARAM==v){ return "inv-param" ; }
    else if(LV_FS_RES_UNKNOWN==v){ return "unknown" ; }
    else {
        return "<unknow>" ;
    }
}
EMSCRIPTEN_KEEPALIVE uint32_t lv_fs_res_to_value(char * n) {
    if(strcmp("ok", n)==0){ return LV_FS_RES_OK ; }
    else if(strcmp("hw-err", n)==0){ return LV_FS_RES_HW_ERR ; }
    else if(strcmp("fs-err", n)==0){ return LV_FS_RES_FS_ERR ; }
    else if(strcmp("not-ex", n)==0){ return LV_FS_RES_NOT_EX ; }
    else if(strcmp("full", n)==0){ return LV_FS_RES_FULL ; }
    else if(strcmp("locked", n)==0){ return LV_FS_RES_LOCKED ; }
    else if(strcmp("denied", n)==0){ return LV_FS_RES_DENIED ; }
    else if(strcmp("busy", n)==0){ return LV_FS_RES_BUSY ; }
    else if(strcmp("tout", n)==0){ return LV_FS_RES_TOUT ; }
    else if(strcmp("not-imp", n)==0){ return LV_FS_RES_NOT_IMP ; }
    else if(strcmp("out-of-mem", n)==0){ return LV_FS_RES_OUT_OF_MEM ; }
    else if(strcmp("inv-param", n)==0){ return LV_FS_RES_INV_PARAM ; }
    else if(strcmp("unknown", n)==0){ return LV_FS_RES_UNKNOWN ; }
    else {
        return (uint32_t)-1 ;
    }
}
// "lv_fs_mode_t": /^(_?)LV_FS_/ ,
EMSCRIPTEN_KEEPALIVE char * lv_fs_mode_to_name(uint32_t v) {
    if(LV_FS_MODE_WR==v){ return "mode-wr" ; }
    else if(LV_FS_MODE_RD==v){ return "mode-rd" ; }
    else {
        return "<unknow>" ;
    }
}
EMSCRIPTEN_KEEPALIVE uint32_t lv_fs_mode_to_value(char * n) {
    if(strcmp("mode-wr", n)==0){ return LV_FS_MODE_WR ; }
    else if(strcmp("mode-rd", n)==0){ return LV_FS_MODE_RD ; }
    else {
        return (uint32_t)-1 ;
    }
}
// "lv_fs_whence_t": /^(_?)LV_FS_/ ,
EMSCRIPTEN_KEEPALIVE char * lv_fs_whence_to_name(uint32_t v) {
    if(LV_FS_SEEK_SET==v){ return "seek-set" ; }
    else if(LV_FS_SEEK_CUR==v){ return "seek-cur" ; }
    else if(LV_FS_SEEK_END==v){ return "seek-end" ; }
    else {
        return "<unknow>" ;
    }
}
EMSCRIPTEN_KEEPALIVE uint32_t lv_fs_whence_to_value(char * n) {
    if(strcmp("seek-set", n)==0){ return LV_FS_SEEK_SET ; }
    else if(strcmp("seek-cur", n)==0){ return LV_FS_SEEK_CUR ; }
    else if(strcmp("seek-end", n)==0){ return LV_FS_SEEK_END ; }
    else {
        return (uint32_t)-1 ;
    }
}
// "lv_img_src_t": /^(_?)LV_IMG_SRC_/ ,
EMSCRIPTEN_KEEPALIVE char * lv_img_src_to_name(uint32_t v) {
    if(LV_IMG_SRC_VARIABLE==v){ return "variable" ; }
    else if(LV_IMG_SRC_FILE==v){ return "file" ; }
    else if(LV_IMG_SRC_SYMBOL==v){ return "symbol" ; }
    else if(LV_IMG_SRC_UNKNOWN==v){ return "unknown" ; }
    else {
        return "<unknow>" ;
    }
}
EMSCRIPTEN_KEEPALIVE uint32_t lv_img_src_to_value(char * n) {
    if(strcmp("variable", n)==0){ return LV_IMG_SRC_VARIABLE ; }
    else if(strcmp("file", n)==0){ return LV_IMG_SRC_FILE ; }
    else if(strcmp("symbol", n)==0){ return LV_IMG_SRC_SYMBOL ; }
    else if(strcmp("unknown", n)==0){ return LV_IMG_SRC_UNKNOWN ; }
    else {
        return (uint32_t)-1 ;
    }
}
// "lv_draw_mask_res_t": /^(_?)LV_DRAW_MASK_RES_/ ,
EMSCRIPTEN_KEEPALIVE char * lv_draw_mask_res_to_name(uint32_t v) {
    if(LV_DRAW_MASK_RES_TRANSP==v){ return "transp" ; }
    else if(LV_DRAW_MASK_RES_FULL_COVER==v){ return "full-cover" ; }
    else if(LV_DRAW_MASK_RES_CHANGED==v){ return "changed" ; }
    else if(LV_DRAW_MASK_RES_UNKNOWN==v){ return "unknown" ; }
    else {
        return "<unknow>" ;
    }
}
EMSCRIPTEN_KEEPALIVE uint32_t lv_draw_mask_res_to_value(char * n) {
    if(strcmp("transp", n)==0){ return LV_DRAW_MASK_RES_TRANSP ; }
    else if(strcmp("full-cover", n)==0){ return LV_DRAW_MASK_RES_FULL_COVER ; }
    else if(strcmp("changed", n)==0){ return LV_DRAW_MASK_RES_CHANGED ; }
    else if(strcmp("unknown", n)==0){ return LV_DRAW_MASK_RES_UNKNOWN ; }
    else {
        return (uint32_t)-1 ;
    }
}
// "lv_draw_mask_type_t": /^(_?)LV_DRAW_MASK_TYPE_/ ,
EMSCRIPTEN_KEEPALIVE char * lv_draw_mask_type_to_name(uint32_t v) {
    if(LV_DRAW_MASK_TYPE_LINE==v){ return "line" ; }
    else if(LV_DRAW_MASK_TYPE_ANGLE==v){ return "angle" ; }
    else if(LV_DRAW_MASK_TYPE_RADIUS==v){ return "radius" ; }
    else if(LV_DRAW_MASK_TYPE_FADE==v){ return "fade" ; }
    else if(LV_DRAW_MASK_TYPE_MAP==v){ return "map" ; }
    else {
        return "<unknow>" ;
    }
}
EMSCRIPTEN_KEEPALIVE uint32_t lv_draw_mask_type_to_value(char * n) {
    if(strcmp("line", n)==0){ return LV_DRAW_MASK_TYPE_LINE ; }
    else if(strcmp("angle", n)==0){ return LV_DRAW_MASK_TYPE_ANGLE ; }
    else if(strcmp("radius", n)==0){ return LV_DRAW_MASK_TYPE_RADIUS ; }
    else if(strcmp("fade", n)==0){ return LV_DRAW_MASK_TYPE_FADE ; }
    else if(strcmp("map", n)==0){ return LV_DRAW_MASK_TYPE_MAP ; }
    else {
        return (uint32_t)-1 ;
    }
}
// "lv_draw_mask_xcb_t": /^(_?)LV_DRAW_MASK_/ ,
EMSCRIPTEN_KEEPALIVE char * lv_draw_mask_xcb_to_name(uint32_t v) {
    if(LV_DRAW_MASK_LINE_SIDE_LEFT==v){ return "line-side-left" ; }
    else if(LV_DRAW_MASK_LINE_SIDE_RIGHT==v){ return "line-side-right" ; }
    else if(LV_DRAW_MASK_LINE_SIDE_TOP==v){ return "line-side-top" ; }
    else if(LV_DRAW_MASK_LINE_SIDE_BOTTOM==v){ return "line-side-bottom" ; }
    else {
        return "<unknow>" ;
    }
}
EMSCRIPTEN_KEEPALIVE uint32_t lv_draw_mask_xcb_to_value(char * n) {
    if(strcmp("line-side-left", n)==0){ return LV_DRAW_MASK_LINE_SIDE_LEFT ; }
    else if(strcmp("line-side-right", n)==0){ return LV_DRAW_MASK_LINE_SIDE_RIGHT ; }
    else if(strcmp("line-side-top", n)==0){ return LV_DRAW_MASK_LINE_SIDE_TOP ; }
    else if(strcmp("line-side-bottom", n)==0){ return LV_DRAW_MASK_LINE_SIDE_BOTTOM ; }
    else {
        return (uint32_t)-1 ;
    }
}
// "lv_cover_res_t": /^(_?)LV_COVER_RES_/ ,
EMSCRIPTEN_KEEPALIVE char * lv_cover_res_to_name(uint32_t v) {
    if(LV_COVER_RES_COVER==v){ return "cover" ; }
    else if(LV_COVER_RES_NOT_COVER==v){ return "not-cover" ; }
    else if(LV_COVER_RES_MASKED==v){ return "masked" ; }
    else {
        return "<unknow>" ;
    }
}
EMSCRIPTEN_KEEPALIVE uint32_t lv_cover_res_to_value(char * n) {
    if(strcmp("cover", n)==0){ return LV_COVER_RES_COVER ; }
    else if(strcmp("not-cover", n)==0){ return LV_COVER_RES_NOT_COVER ; }
    else if(strcmp("masked", n)==0){ return LV_COVER_RES_MASKED ; }
    else {
        return (uint32_t)-1 ;
    }
}
// "lv_obj_class_editable_t": /^(_?)LV_OBJ_CLASS_EDITABLE_/ ,
EMSCRIPTEN_KEEPALIVE char * lv_obj_class_editable_to_name(uint32_t v) {
    if(LV_OBJ_CLASS_EDITABLE_INHERIT==v){ return "inherit" ; }
    else if(LV_OBJ_CLASS_EDITABLE_TRUE==v){ return "true" ; }
    else if(LV_OBJ_CLASS_EDITABLE_FALSE==v){ return "false" ; }
    else {
        return "<unknow>" ;
    }
}
EMSCRIPTEN_KEEPALIVE uint32_t lv_obj_class_editable_to_value(char * n) {
    if(strcmp("inherit", n)==0){ return LV_OBJ_CLASS_EDITABLE_INHERIT ; }
    else if(strcmp("true", n)==0){ return LV_OBJ_CLASS_EDITABLE_TRUE ; }
    else if(strcmp("false", n)==0){ return LV_OBJ_CLASS_EDITABLE_FALSE ; }
    else {
        return (uint32_t)-1 ;
    }
}
// "lv_obj_class_group_def_t": /^(_?)LV_OBJ_CLASS_GROUP_DEF_/ ,
EMSCRIPTEN_KEEPALIVE char * lv_obj_class_group_def_to_name(uint32_t v) {
    if(LV_OBJ_CLASS_GROUP_DEF_INHERIT==v){ return "inherit" ; }
    else if(LV_OBJ_CLASS_GROUP_DEF_TRUE==v){ return "true" ; }
    else if(LV_OBJ_CLASS_GROUP_DEF_FALSE==v){ return "false" ; }
    else {
        return "<unknow>" ;
    }
}
EMSCRIPTEN_KEEPALIVE uint32_t lv_obj_class_group_def_to_value(char * n) {
    if(strcmp("inherit", n)==0){ return LV_OBJ_CLASS_GROUP_DEF_INHERIT ; }
    else if(strcmp("true", n)==0){ return LV_OBJ_CLASS_GROUP_DEF_TRUE ; }
    else if(strcmp("false", n)==0){ return LV_OBJ_CLASS_GROUP_DEF_FALSE ; }
    else {
        return (uint32_t)-1 ;
    }
}
// "lv_event_code_t": /^(_?)LV_EVENT_/ ,
EMSCRIPTEN_KEEPALIVE char * lv_event_code_to_name(uint32_t v) {
    if(LV_EVENT_ALL==v){ return "all" ; }
    else if(LV_EVENT_PRESSED==v){ return "pressed" ; }
    else if(LV_EVENT_PRESSING==v){ return "pressing" ; }
    else if(LV_EVENT_PRESS_LOST==v){ return "press-lost" ; }
    else if(LV_EVENT_SHORT_CLICKED==v){ return "short-clicked" ; }
    else if(LV_EVENT_LONG_PRESSED==v){ return "long-pressed" ; }
    else if(LV_EVENT_LONG_PRESSED_REPEAT==v){ return "long-pressed-repeat" ; }
    else if(LV_EVENT_CLICKED==v){ return "clicked" ; }
    else if(LV_EVENT_RELEASED==v){ return "released" ; }
    else if(LV_EVENT_SCROLL_BEGIN==v){ return "scroll-begin" ; }
    else if(LV_EVENT_SCROLL_END==v){ return "scroll-end" ; }
    else if(LV_EVENT_SCROLL==v){ return "scroll" ; }
    else if(LV_EVENT_GESTURE==v){ return "gesture" ; }
    else if(LV_EVENT_KEY==v){ return "key" ; }
    else if(LV_EVENT_FOCUSED==v){ return "focused" ; }
    else if(LV_EVENT_DEFOCUSED==v){ return "defocused" ; }
    else if(LV_EVENT_LEAVE==v){ return "leave" ; }
    else if(LV_EVENT_HIT_TEST==v){ return "hit-test" ; }
    else if(LV_EVENT_COVER_CHECK==v){ return "cover-check" ; }
    else if(LV_EVENT_REFR_EXT_DRAW_SIZE==v){ return "refr-ext-draw-size" ; }
    else if(LV_EVENT_DRAW_MAIN_BEGIN==v){ return "draw-main-begin" ; }
    else if(LV_EVENT_DRAW_MAIN==v){ return "draw-main" ; }
    else if(LV_EVENT_DRAW_MAIN_END==v){ return "draw-main-end" ; }
    else if(LV_EVENT_DRAW_POST_BEGIN==v){ return "draw-post-begin" ; }
    else if(LV_EVENT_DRAW_POST==v){ return "draw-post" ; }
    else if(LV_EVENT_DRAW_POST_END==v){ return "draw-post-end" ; }
    else if(LV_EVENT_DRAW_PART_BEGIN==v){ return "draw-part-begin" ; }
    else if(LV_EVENT_DRAW_PART_END==v){ return "draw-part-end" ; }
    else if(LV_EVENT_VALUE_CHANGED==v){ return "value-changed" ; }
    else if(LV_EVENT_INSERT==v){ return "insert" ; }
    else if(LV_EVENT_REFRESH==v){ return "refresh" ; }
    else if(LV_EVENT_READY==v){ return "ready" ; }
    else if(LV_EVENT_CANCEL==v){ return "cancel" ; }
    else if(LV_EVENT_DELETE==v){ return "delete" ; }
    else if(LV_EVENT_CHILD_CHANGED==v){ return "child-changed" ; }
    else if(LV_EVENT_CHILD_CREATED==v){ return "child-created" ; }
    else if(LV_EVENT_CHILD_DELETED==v){ return "child-deleted" ; }
    else if(LV_EVENT_SCREEN_UNLOAD_START==v){ return "screen-unload-start" ; }
    else if(LV_EVENT_SCREEN_LOAD_START==v){ return "screen-load-start" ; }
    else if(LV_EVENT_SCREEN_LOADED==v){ return "screen-loaded" ; }
    else if(LV_EVENT_SCREEN_UNLOADED==v){ return "screen-unloaded" ; }
    else if(LV_EVENT_SIZE_CHANGED==v){ return "size-changed" ; }
    else if(LV_EVENT_STYLE_CHANGED==v){ return "style-changed" ; }
    else if(LV_EVENT_LAYOUT_CHANGED==v){ return "layout-changed" ; }
    else if(LV_EVENT_GET_SELF_SIZE==v){ return "get-self-size" ; }
    else if(_LV_EVENT_LAST==v){ return "_last" ; }
    else {
        return "<unknow>" ;
    }
}
EMSCRIPTEN_KEEPALIVE uint32_t lv_event_code_to_value(char * n) {
    if(strcmp("all", n)==0){ return LV_EVENT_ALL ; }
    else if(strcmp("pressed", n)==0){ return LV_EVENT_PRESSED ; }
    else if(strcmp("pressing", n)==0){ return LV_EVENT_PRESSING ; }
    else if(strcmp("press-lost", n)==0){ return LV_EVENT_PRESS_LOST ; }
    else if(strcmp("short-clicked", n)==0){ return LV_EVENT_SHORT_CLICKED ; }
    else if(strcmp("long-pressed", n)==0){ return LV_EVENT_LONG_PRESSED ; }
    else if(strcmp("long-pressed-repeat", n)==0){ return LV_EVENT_LONG_PRESSED_REPEAT ; }
    else if(strcmp("clicked", n)==0){ return LV_EVENT_CLICKED ; }
    else if(strcmp("released", n)==0){ return LV_EVENT_RELEASED ; }
    else if(strcmp("scroll-begin", n)==0){ return LV_EVENT_SCROLL_BEGIN ; }
    else if(strcmp("scroll-end", n)==0){ return LV_EVENT_SCROLL_END ; }
    else if(strcmp("scroll", n)==0){ return LV_EVENT_SCROLL ; }
    else if(strcmp("gesture", n)==0){ return LV_EVENT_GESTURE ; }
    else if(strcmp("key", n)==0){ return LV_EVENT_KEY ; }
    else if(strcmp("focused", n)==0){ return LV_EVENT_FOCUSED ; }
    else if(strcmp("defocused", n)==0){ return LV_EVENT_DEFOCUSED ; }
    else if(strcmp("leave", n)==0){ return LV_EVENT_LEAVE ; }
    else if(strcmp("hit-test", n)==0){ return LV_EVENT_HIT_TEST ; }
    else if(strcmp("cover-check", n)==0){ return LV_EVENT_COVER_CHECK ; }
    else if(strcmp("refr-ext-draw-size", n)==0){ return LV_EVENT_REFR_EXT_DRAW_SIZE ; }
    else if(strcmp("draw-main-begin", n)==0){ return LV_EVENT_DRAW_MAIN_BEGIN ; }
    else if(strcmp("draw-main", n)==0){ return LV_EVENT_DRAW_MAIN ; }
    else if(strcmp("draw-main-end", n)==0){ return LV_EVENT_DRAW_MAIN_END ; }
    else if(strcmp("draw-post-begin", n)==0){ return LV_EVENT_DRAW_POST_BEGIN ; }
    else if(strcmp("draw-post", n)==0){ return LV_EVENT_DRAW_POST ; }
    else if(strcmp("draw-post-end", n)==0){ return LV_EVENT_DRAW_POST_END ; }
    else if(strcmp("draw-part-begin", n)==0){ return LV_EVENT_DRAW_PART_BEGIN ; }
    else if(strcmp("draw-part-end", n)==0){ return LV_EVENT_DRAW_PART_END ; }
    else if(strcmp("value-changed", n)==0){ return LV_EVENT_VALUE_CHANGED ; }
    else if(strcmp("insert", n)==0){ return LV_EVENT_INSERT ; }
    else if(strcmp("refresh", n)==0){ return LV_EVENT_REFRESH ; }
    else if(strcmp("ready", n)==0){ return LV_EVENT_READY ; }
    else if(strcmp("cancel", n)==0){ return LV_EVENT_CANCEL ; }
    else if(strcmp("delete", n)==0){ return LV_EVENT_DELETE ; }
    else if(strcmp("child-changed", n)==0){ return LV_EVENT_CHILD_CHANGED ; }
    else if(strcmp("child-created", n)==0){ return LV_EVENT_CHILD_CREATED ; }
    else if(strcmp("child-deleted", n)==0){ return LV_EVENT_CHILD_DELETED ; }
    else if(strcmp("screen-unload-start", n)==0){ return LV_EVENT_SCREEN_UNLOAD_START ; }
    else if(strcmp("screen-load-start", n)==0){ return LV_EVENT_SCREEN_LOAD_START ; }
    else if(strcmp("screen-loaded", n)==0){ return LV_EVENT_SCREEN_LOADED ; }
    else if(strcmp("screen-unloaded", n)==0){ return LV_EVENT_SCREEN_UNLOADED ; }
    else if(strcmp("size-changed", n)==0){ return LV_EVENT_SIZE_CHANGED ; }
    else if(strcmp("style-changed", n)==0){ return LV_EVENT_STYLE_CHANGED ; }
    else if(strcmp("layout-changed", n)==0){ return LV_EVENT_LAYOUT_CHANGED ; }
    else if(strcmp("get-self-size", n)==0){ return LV_EVENT_GET_SELF_SIZE ; }
    else if(strcmp("_last", n)==0){ return _LV_EVENT_LAST ; }
    else {
        return (uint32_t)-1 ;
    }
}
// "lv_key_t": /^(_?)LV_KEY_/ ,
EMSCRIPTEN_KEEPALIVE char * lv_key_to_name(uint32_t v) {
    if(LV_KEY_UP==v){ return "up" ; }
    else if(LV_KEY_DOWN==v){ return "down" ; }
    else if(LV_KEY_RIGHT==v){ return "right" ; }
    else if(LV_KEY_LEFT==v){ return "left" ; }
    else if(LV_KEY_ESC==v){ return "esc" ; }
    else if(LV_KEY_DEL==v){ return "del" ; }
    else if(LV_KEY_BACKSPACE==v){ return "backspace" ; }
    else if(LV_KEY_ENTER==v){ return "enter" ; }
    else if(LV_KEY_NEXT==v){ return "next" ; }
    else if(LV_KEY_PREV==v){ return "prev" ; }
    else if(LV_KEY_HOME==v){ return "home" ; }
    else if(LV_KEY_END==v){ return "end" ; }
    else {
        return "<unknow>" ;
    }
}
EMSCRIPTEN_KEEPALIVE uint32_t lv_key_to_value(char * n) {
    if(strcmp("up", n)==0){ return LV_KEY_UP ; }
    else if(strcmp("down", n)==0){ return LV_KEY_DOWN ; }
    else if(strcmp("right", n)==0){ return LV_KEY_RIGHT ; }
    else if(strcmp("left", n)==0){ return LV_KEY_LEFT ; }
    else if(strcmp("esc", n)==0){ return LV_KEY_ESC ; }
    else if(strcmp("del", n)==0){ return LV_KEY_DEL ; }
    else if(strcmp("backspace", n)==0){ return LV_KEY_BACKSPACE ; }
    else if(strcmp("enter", n)==0){ return LV_KEY_ENTER ; }
    else if(strcmp("next", n)==0){ return LV_KEY_NEXT ; }
    else if(strcmp("prev", n)==0){ return LV_KEY_PREV ; }
    else if(strcmp("home", n)==0){ return LV_KEY_HOME ; }
    else if(strcmp("end", n)==0){ return LV_KEY_END ; }
    else {
        return (uint32_t)-1 ;
    }
}
// "lv_group_refocus_policy_t": /^(_?)LV_GROUP_REFOCUS_POLICY_/ ,
EMSCRIPTEN_KEEPALIVE char * lv_group_refocus_policy_to_name(uint32_t v) {
    if(LV_GROUP_REFOCUS_POLICY_NEXT==v){ return "next" ; }
    else if(LV_GROUP_REFOCUS_POLICY_PREV==v){ return "prev" ; }
    else {
        return "<unknow>" ;
    }
}
EMSCRIPTEN_KEEPALIVE uint32_t lv_group_refocus_policy_to_value(char * n) {
    if(strcmp("next", n)==0){ return LV_GROUP_REFOCUS_POLICY_NEXT ; }
    else if(strcmp("prev", n)==0){ return LV_GROUP_REFOCUS_POLICY_PREV ; }
    else {
        return (uint32_t)-1 ;
    }
}
// "lv_scr_load_anim_t": /^(_?)LV_SCR_LOAD_ANIM_/ ,
EMSCRIPTEN_KEEPALIVE char * lv_scr_load_anim_to_name(uint32_t v) {
    if(LV_SCR_LOAD_ANIM_NONE==v){ return "none" ; }
    else if(LV_SCR_LOAD_ANIM_OVER_LEFT==v){ return "over-left" ; }
    else if(LV_SCR_LOAD_ANIM_OVER_RIGHT==v){ return "over-right" ; }
    else if(LV_SCR_LOAD_ANIM_OVER_TOP==v){ return "over-top" ; }
    else if(LV_SCR_LOAD_ANIM_OVER_BOTTOM==v){ return "over-bottom" ; }
    else if(LV_SCR_LOAD_ANIM_MOVE_LEFT==v){ return "move-left" ; }
    else if(LV_SCR_LOAD_ANIM_MOVE_RIGHT==v){ return "move-right" ; }
    else if(LV_SCR_LOAD_ANIM_MOVE_TOP==v){ return "move-top" ; }
    else if(LV_SCR_LOAD_ANIM_MOVE_BOTTOM==v){ return "move-bottom" ; }
    else if(LV_SCR_LOAD_ANIM_FADE_ON==v){ return "fade-on" ; }
    else {
        return "<unknow>" ;
    }
}
EMSCRIPTEN_KEEPALIVE uint32_t lv_scr_load_anim_to_value(char * n) {
    if(strcmp("none", n)==0){ return LV_SCR_LOAD_ANIM_NONE ; }
    else if(strcmp("over-left", n)==0){ return LV_SCR_LOAD_ANIM_OVER_LEFT ; }
    else if(strcmp("over-right", n)==0){ return LV_SCR_LOAD_ANIM_OVER_RIGHT ; }
    else if(strcmp("over-top", n)==0){ return LV_SCR_LOAD_ANIM_OVER_TOP ; }
    else if(strcmp("over-bottom", n)==0){ return LV_SCR_LOAD_ANIM_OVER_BOTTOM ; }
    else if(strcmp("move-left", n)==0){ return LV_SCR_LOAD_ANIM_MOVE_LEFT ; }
    else if(strcmp("move-right", n)==0){ return LV_SCR_LOAD_ANIM_MOVE_RIGHT ; }
    else if(strcmp("move-top", n)==0){ return LV_SCR_LOAD_ANIM_MOVE_TOP ; }
    else if(strcmp("move-bottom", n)==0){ return LV_SCR_LOAD_ANIM_MOVE_BOTTOM ; }
    else if(strcmp("fade-on", n)==0){ return LV_SCR_LOAD_ANIM_FADE_ON ; }
    else {
        return (uint32_t)-1 ;
    }
}
// "lv_font_fmt_txt_cmap_type_t": /^(_?)LV_FONT_FMT_TXT_CMAP_/ ,
EMSCRIPTEN_KEEPALIVE char * lv_font_fmt_txt_cmap_type_to_name(uint32_t v) {
    if(LV_FONT_FMT_TXT_CMAP_FORMAT0_FULL==v){ return "format0-full" ; }
    else if(LV_FONT_FMT_TXT_CMAP_SPARSE_FULL==v){ return "sparse-full" ; }
    else if(LV_FONT_FMT_TXT_CMAP_FORMAT0_TINY==v){ return "format0-tiny" ; }
    else if(LV_FONT_FMT_TXT_CMAP_SPARSE_TINY==v){ return "sparse-tiny" ; }
    else {
        return "<unknow>" ;
    }
}
EMSCRIPTEN_KEEPALIVE uint32_t lv_font_fmt_txt_cmap_type_to_value(char * n) {
    if(strcmp("format0-full", n)==0){ return LV_FONT_FMT_TXT_CMAP_FORMAT0_FULL ; }
    else if(strcmp("sparse-full", n)==0){ return LV_FONT_FMT_TXT_CMAP_SPARSE_FULL ; }
    else if(strcmp("format0-tiny", n)==0){ return LV_FONT_FMT_TXT_CMAP_FORMAT0_TINY ; }
    else if(strcmp("sparse-tiny", n)==0){ return LV_FONT_FMT_TXT_CMAP_SPARSE_TINY ; }
    else {
        return (uint32_t)-1 ;
    }
}
// "lv_font_fmt_txt_bitmap_format_t": /^(_?)LV_FONT_FMT_TXT_/ ,
EMSCRIPTEN_KEEPALIVE char * lv_font_fmt_txt_bitmap_format_to_name(uint32_t v) {
    if(LV_FONT_FMT_TXT_PLAIN==v){ return "plain" ; }
    else if(LV_FONT_FMT_TXT_COMPRESSED==v){ return "compressed" ; }
    else if(LV_FONT_FMT_TXT_COMPRESSED_NO_PREFILTER==v){ return "compressed-no-prefilter" ; }
    else {
        return "<unknow>" ;
    }
}
EMSCRIPTEN_KEEPALIVE uint32_t lv_font_fmt_txt_bitmap_format_to_value(char * n) {
    if(strcmp("plain", n)==0){ return LV_FONT_FMT_TXT_PLAIN ; }
    else if(strcmp("compressed", n)==0){ return LV_FONT_FMT_TXT_COMPRESSED ; }
    else if(strcmp("compressed-no-prefilter", n)==0){ return LV_FONT_FMT_TXT_COMPRESSED_NO_PREFILTER ; }
    else {
        return (uint32_t)-1 ;
    }
}
// "lv_arc_mode_t": /^(_?)LV_ARC_/ ,
EMSCRIPTEN_KEEPALIVE char * lv_arc_mode_to_name(uint32_t v) {
    if(LV_ARC_MODE_NORMAL==v){ return "mode-normal" ; }
    else if(LV_ARC_MODE_SYMMETRICAL==v){ return "mode-symmetrical" ; }
    else if(LV_ARC_MODE_REVERSE==v){ return "mode-reverse" ; }
    else {
        return "<unknow>" ;
    }
}
EMSCRIPTEN_KEEPALIVE uint32_t lv_arc_mode_to_value(char * n) {
    if(strcmp("mode-normal", n)==0){ return LV_ARC_MODE_NORMAL ; }
    else if(strcmp("mode-symmetrical", n)==0){ return LV_ARC_MODE_SYMMETRICAL ; }
    else if(strcmp("mode-reverse", n)==0){ return LV_ARC_MODE_REVERSE ; }
    else {
        return (uint32_t)-1 ;
    }
}
// "lv_arc_draw_part_type_t": /^(_?)LV_ARC_DRAW_PART_/ ,
EMSCRIPTEN_KEEPALIVE char * lv_arc_draw_part_type_to_name(uint32_t v) {
    if(LV_ARC_DRAW_PART_BACKGROUND==v){ return "background" ; }
    else if(LV_ARC_DRAW_PART_FOREGROUND==v){ return "foreground" ; }
    else if(LV_ARC_DRAW_PART_KNOB==v){ return "knob" ; }
    else {
        return "<unknow>" ;
    }
}
EMSCRIPTEN_KEEPALIVE uint32_t lv_arc_draw_part_type_to_value(char * n) {
    if(strcmp("background", n)==0){ return LV_ARC_DRAW_PART_BACKGROUND ; }
    else if(strcmp("foreground", n)==0){ return LV_ARC_DRAW_PART_FOREGROUND ; }
    else if(strcmp("knob", n)==0){ return LV_ARC_DRAW_PART_KNOB ; }
    else {
        return (uint32_t)-1 ;
    }
}
// "lv_img_size_mode_t": /^(_?)LV_IMG_SIZE_/ ,
EMSCRIPTEN_KEEPALIVE char * lv_img_size_mode_to_name(uint32_t v) {
    if(LV_IMG_SIZE_MODE_VIRTUAL==v){ return "mode-virtual" ; }
    else if(LV_IMG_SIZE_MODE_REAL==v){ return "mode-real" ; }
    else {
        return "<unknow>" ;
    }
}
EMSCRIPTEN_KEEPALIVE uint32_t lv_img_size_mode_to_value(char * n) {
    if(strcmp("mode-virtual", n)==0){ return LV_IMG_SIZE_MODE_VIRTUAL ; }
    else if(strcmp("mode-real", n)==0){ return LV_IMG_SIZE_MODE_REAL ; }
    else {
        return (uint32_t)-1 ;
    }
}
// "lv_label_long_mode_t": /^(_?)LV_LABEL_LONG_/ ,
EMSCRIPTEN_KEEPALIVE char * lv_label_long_mode_to_name(uint32_t v) {
    if(LV_LABEL_LONG_WRAP==v){ return "wrap" ; }
    else if(LV_LABEL_LONG_DOT==v){ return "dot" ; }
    else if(LV_LABEL_LONG_SCROLL==v){ return "scroll" ; }
    else if(LV_LABEL_LONG_SCROLL_CIRCULAR==v){ return "scroll-circular" ; }
    else if(LV_LABEL_LONG_CLIP==v){ return "clip" ; }
    else {
        return "<unknow>" ;
    }
}
EMSCRIPTEN_KEEPALIVE uint32_t lv_label_long_mode_to_value(char * n) {
    if(strcmp("wrap", n)==0){ return LV_LABEL_LONG_WRAP ; }
    else if(strcmp("dot", n)==0){ return LV_LABEL_LONG_DOT ; }
    else if(strcmp("scroll", n)==0){ return LV_LABEL_LONG_SCROLL ; }
    else if(strcmp("scroll-circular", n)==0){ return LV_LABEL_LONG_SCROLL_CIRCULAR ; }
    else if(strcmp("clip", n)==0){ return LV_LABEL_LONG_CLIP ; }
    else {
        return (uint32_t)-1 ;
    }
}
// "lv_table_cell_ctrl_t": /^(_?)LV_TABLE_CELL_/ ,
EMSCRIPTEN_KEEPALIVE char * lv_table_cell_ctrl_to_name(uint32_t v) {
    if(LV_TABLE_CELL_CTRL_MERGE_RIGHT==v){ return "ctrl-merge-right" ; }
    else if(LV_TABLE_CELL_CTRL_TEXT_CROP==v){ return "ctrl-text-crop" ; }
    else if(LV_TABLE_CELL_CTRL_CUSTOM_1==v){ return "ctrl-custom-1" ; }
    else if(LV_TABLE_CELL_CTRL_CUSTOM_2==v){ return "ctrl-custom-2" ; }
    else if(LV_TABLE_CELL_CTRL_CUSTOM_3==v){ return "ctrl-custom-3" ; }
    else if(LV_TABLE_CELL_CTRL_CUSTOM_4==v){ return "ctrl-custom-4" ; }
    else {
        return "<unknow>" ;
    }
}
EMSCRIPTEN_KEEPALIVE uint32_t lv_table_cell_ctrl_to_value(char * n) {
    if(strcmp("ctrl-merge-right", n)==0){ return LV_TABLE_CELL_CTRL_MERGE_RIGHT ; }
    else if(strcmp("ctrl-text-crop", n)==0){ return LV_TABLE_CELL_CTRL_TEXT_CROP ; }
    else if(strcmp("ctrl-custom-1", n)==0){ return LV_TABLE_CELL_CTRL_CUSTOM_1 ; }
    else if(strcmp("ctrl-custom-2", n)==0){ return LV_TABLE_CELL_CTRL_CUSTOM_2 ; }
    else if(strcmp("ctrl-custom-3", n)==0){ return LV_TABLE_CELL_CTRL_CUSTOM_3 ; }
    else if(strcmp("ctrl-custom-4", n)==0){ return LV_TABLE_CELL_CTRL_CUSTOM_4 ; }
    else {
        return (uint32_t)-1 ;
    }
}
// "lv_table_draw_part_type_t": /^(_?)LV_TABLE_DRAW_PART_/ ,
EMSCRIPTEN_KEEPALIVE char * lv_table_draw_part_type_to_name(uint32_t v) {
    if(LV_TABLE_DRAW_PART_CELL==v){ return "cell" ; }
    else {
        return "<unknow>" ;
    }
}
EMSCRIPTEN_KEEPALIVE uint32_t lv_table_draw_part_type_to_value(char * n) {
    if(strcmp("cell", n)==0){ return LV_TABLE_DRAW_PART_CELL ; }
    else {
        return (uint32_t)-1 ;
    }
}
// "lv_checkbox_draw_part_type_t": /^(_?)LV_CHECKBOX_DRAW_PART_/ ,
EMSCRIPTEN_KEEPALIVE char * lv_checkbox_draw_part_type_to_name(uint32_t v) {
    if(LV_CHECKBOX_DRAW_PART_BOX==v){ return "box" ; }
    else {
        return "<unknow>" ;
    }
}
EMSCRIPTEN_KEEPALIVE uint32_t lv_checkbox_draw_part_type_to_value(char * n) {
    if(strcmp("box", n)==0){ return LV_CHECKBOX_DRAW_PART_BOX ; }
    else {
        return (uint32_t)-1 ;
    }
}
// "lv_bar_mode_t": /^(_?)LV_BAR_/ ,
EMSCRIPTEN_KEEPALIVE char * lv_bar_mode_to_name(uint32_t v) {
    if(LV_BAR_MODE_NORMAL==v){ return "mode-normal" ; }
    else if(LV_BAR_MODE_SYMMETRICAL==v){ return "mode-symmetrical" ; }
    else if(LV_BAR_MODE_RANGE==v){ return "mode-range" ; }
    else {
        return "<unknow>" ;
    }
}
EMSCRIPTEN_KEEPALIVE uint32_t lv_bar_mode_to_value(char * n) {
    if(strcmp("mode-normal", n)==0){ return LV_BAR_MODE_NORMAL ; }
    else if(strcmp("mode-symmetrical", n)==0){ return LV_BAR_MODE_SYMMETRICAL ; }
    else if(strcmp("mode-range", n)==0){ return LV_BAR_MODE_RANGE ; }
    else {
        return (uint32_t)-1 ;
    }
}
// "lv_bar_draw_part_type_t": /^(_?)LV_BAR_DRAW_PART_/ ,
EMSCRIPTEN_KEEPALIVE char * lv_bar_draw_part_type_to_name(uint32_t v) {
    if(LV_BAR_DRAW_PART_INDICATOR==v){ return "indicator" ; }
    else {
        return "<unknow>" ;
    }
}
EMSCRIPTEN_KEEPALIVE uint32_t lv_bar_draw_part_type_to_value(char * n) {
    if(strcmp("indicator", n)==0){ return LV_BAR_DRAW_PART_INDICATOR ; }
    else {
        return (uint32_t)-1 ;
    }
}
// "lv_slider_mode_t": /^(_?)LV_SLIDER_/ ,
EMSCRIPTEN_KEEPALIVE char * lv_slider_mode_to_name(uint32_t v) {
    if(LV_SLIDER_MODE_NORMAL==v){ return "mode-normal" ; }
    else if(LV_SLIDER_MODE_SYMMETRICAL==v){ return "mode-symmetrical" ; }
    else if(LV_SLIDER_MODE_RANGE==v){ return "mode-range" ; }
    else {
        return "<unknow>" ;
    }
}
EMSCRIPTEN_KEEPALIVE uint32_t lv_slider_mode_to_value(char * n) {
    if(strcmp("mode-normal", n)==0){ return LV_SLIDER_MODE_NORMAL ; }
    else if(strcmp("mode-symmetrical", n)==0){ return LV_SLIDER_MODE_SYMMETRICAL ; }
    else if(strcmp("mode-range", n)==0){ return LV_SLIDER_MODE_RANGE ; }
    else {
        return (uint32_t)-1 ;
    }
}
// "lv_slider_draw_part_type_t": /^(_?)LV_SLIDER_DRAW_PART_/ ,
EMSCRIPTEN_KEEPALIVE char * lv_slider_draw_part_type_to_name(uint32_t v) {
    if(LV_SLIDER_DRAW_PART_KNOB==v){ return "knob" ; }
    else if(LV_SLIDER_DRAW_PART_KNOB_LEFT==v){ return "knob-left" ; }
    else {
        return "<unknow>" ;
    }
}
EMSCRIPTEN_KEEPALIVE uint32_t lv_slider_draw_part_type_to_value(char * n) {
    if(strcmp("knob", n)==0){ return LV_SLIDER_DRAW_PART_KNOB ; }
    else if(strcmp("knob-left", n)==0){ return LV_SLIDER_DRAW_PART_KNOB_LEFT ; }
    else {
        return (uint32_t)-1 ;
    }
}
// "lv_btnmatrix_ctrl_t": /^(_?)LV_BTNMATRIX_/ ,
EMSCRIPTEN_KEEPALIVE char * lv_btnmatrix_ctrl_to_name(uint32_t v) {
    if(_LV_BTNMATRIX_WIDTH==v){ return "_width" ; }
    else if(LV_BTNMATRIX_CTRL_HIDDEN==v){ return "ctrl-hidden" ; }
    else if(LV_BTNMATRIX_CTRL_NO_REPEAT==v){ return "ctrl-no-repeat" ; }
    else if(LV_BTNMATRIX_CTRL_DISABLED==v){ return "ctrl-disabled" ; }
    else if(LV_BTNMATRIX_CTRL_CHECKABLE==v){ return "ctrl-checkable" ; }
    else if(LV_BTNMATRIX_CTRL_CHECKED==v){ return "ctrl-checked" ; }
    else if(LV_BTNMATRIX_CTRL_CLICK_TRIG==v){ return "ctrl-click-trig" ; }
    else if(LV_BTNMATRIX_CTRL_POPOVER==v){ return "ctrl-popover" ; }
    else if(LV_BTNMATRIX_CTRL_RECOLOR==v){ return "ctrl-recolor" ; }
    else if(_LV_BTNMATRIX_CTRL_RESERVED==v){ return "_ctrl-reserved" ; }
    else if(LV_BTNMATRIX_CTRL_CUSTOM_1==v){ return "ctrl-custom-1" ; }
    else if(LV_BTNMATRIX_CTRL_CUSTOM_2==v){ return "ctrl-custom-2" ; }
    else {
        return "<unknow>" ;
    }
}
EMSCRIPTEN_KEEPALIVE uint32_t lv_btnmatrix_ctrl_to_value(char * n) {
    if(strcmp("_width", n)==0){ return _LV_BTNMATRIX_WIDTH ; }
    else if(strcmp("ctrl-hidden", n)==0){ return LV_BTNMATRIX_CTRL_HIDDEN ; }
    else if(strcmp("ctrl-no-repeat", n)==0){ return LV_BTNMATRIX_CTRL_NO_REPEAT ; }
    else if(strcmp("ctrl-disabled", n)==0){ return LV_BTNMATRIX_CTRL_DISABLED ; }
    else if(strcmp("ctrl-checkable", n)==0){ return LV_BTNMATRIX_CTRL_CHECKABLE ; }
    else if(strcmp("ctrl-checked", n)==0){ return LV_BTNMATRIX_CTRL_CHECKED ; }
    else if(strcmp("ctrl-click-trig", n)==0){ return LV_BTNMATRIX_CTRL_CLICK_TRIG ; }
    else if(strcmp("ctrl-popover", n)==0){ return LV_BTNMATRIX_CTRL_POPOVER ; }
    else if(strcmp("ctrl-recolor", n)==0){ return LV_BTNMATRIX_CTRL_RECOLOR ; }
    else if(strcmp("_ctrl-reserved", n)==0){ return _LV_BTNMATRIX_CTRL_RESERVED ; }
    else if(strcmp("ctrl-custom-1", n)==0){ return LV_BTNMATRIX_CTRL_CUSTOM_1 ; }
    else if(strcmp("ctrl-custom-2", n)==0){ return LV_BTNMATRIX_CTRL_CUSTOM_2 ; }
    else {
        return (uint32_t)-1 ;
    }
}
// "lv_btnmatrix_draw_part_type_t": /^(_?)LV_BTNMATRIX_DRAW_PART_/ ,
EMSCRIPTEN_KEEPALIVE char * lv_btnmatrix_draw_part_type_to_name(uint32_t v) {
    if(LV_BTNMATRIX_DRAW_PART_BTN==v){ return "btn" ; }
    else {
        return "<unknow>" ;
    }
}
EMSCRIPTEN_KEEPALIVE uint32_t lv_btnmatrix_draw_part_type_to_value(char * n) {
    if(strcmp("btn", n)==0){ return LV_BTNMATRIX_DRAW_PART_BTN ; }
    else {
        return (uint32_t)-1 ;
    }
}
// "lv_roller_mode_t": /^(_?)LV_ROLLER_/ ,
EMSCRIPTEN_KEEPALIVE char * lv_roller_mode_to_name(uint32_t v) {
    if(LV_ROLLER_MODE_NORMAL==v){ return "mode-normal" ; }
    else if(LV_ROLLER_MODE_INFINITE==v){ return "mode-infinite" ; }
    else {
        return "<unknow>" ;
    }
}
EMSCRIPTEN_KEEPALIVE uint32_t lv_roller_mode_to_value(char * n) {
    if(strcmp("mode-normal", n)==0){ return LV_ROLLER_MODE_NORMAL ; }
    else if(strcmp("mode-infinite", n)==0){ return LV_ROLLER_MODE_INFINITE ; }
    else {
        return (uint32_t)-1 ;
    }
}
// "lv_animimg_part_t": /^(_?)LV_ANIM_IMG_PART_/ ,
EMSCRIPTEN_KEEPALIVE char * lv_animimg_part_to_name(uint32_t v) {
    if(LV_ANIM_IMG_PART_MAIN==v){ return "main" ; }
    else {
        return "<unknow>" ;
    }
}
EMSCRIPTEN_KEEPALIVE uint32_t lv_animimg_part_to_value(char * n) {
    if(strcmp("main", n)==0){ return LV_ANIM_IMG_PART_MAIN ; }
    else {
        return (uint32_t)-1 ;
    }
}
// "lv_chart_type_t": /^(_?)LV_CHART_TYPE_/ ,
EMSCRIPTEN_KEEPALIVE char * lv_chart_type_to_name(uint32_t v) {
    if(LV_CHART_TYPE_NONE==v){ return "none" ; }
    else if(LV_CHART_TYPE_LINE==v){ return "line" ; }
    else if(LV_CHART_TYPE_BAR==v){ return "bar" ; }
    else if(LV_CHART_TYPE_SCATTER==v){ return "scatter" ; }
    else {
        return "<unknow>" ;
    }
}
EMSCRIPTEN_KEEPALIVE uint32_t lv_chart_type_to_value(char * n) {
    if(strcmp("none", n)==0){ return LV_CHART_TYPE_NONE ; }
    else if(strcmp("line", n)==0){ return LV_CHART_TYPE_LINE ; }
    else if(strcmp("bar", n)==0){ return LV_CHART_TYPE_BAR ; }
    else if(strcmp("scatter", n)==0){ return LV_CHART_TYPE_SCATTER ; }
    else {
        return (uint32_t)-1 ;
    }
}
// "lv_chart_update_mode_t": /^(_?)LV_CHART_UPDATE_/ ,
EMSCRIPTEN_KEEPALIVE char * lv_chart_update_mode_to_name(uint32_t v) {
    if(LV_CHART_UPDATE_MODE_SHIFT==v){ return "mode-shift" ; }
    else if(LV_CHART_UPDATE_MODE_CIRCULAR==v){ return "mode-circular" ; }
    else {
        return "<unknow>" ;
    }
}
EMSCRIPTEN_KEEPALIVE uint32_t lv_chart_update_mode_to_value(char * n) {
    if(strcmp("mode-shift", n)==0){ return LV_CHART_UPDATE_MODE_SHIFT ; }
    else if(strcmp("mode-circular", n)==0){ return LV_CHART_UPDATE_MODE_CIRCULAR ; }
    else {
        return (uint32_t)-1 ;
    }
}
// "lv_chart_axis_t": /^(_?)LV_CHART_AXIS_/ ,
EMSCRIPTEN_KEEPALIVE char * lv_chart_axis_to_name(uint32_t v) {
    if(LV_CHART_AXIS_PRIMARY_Y==v){ return "primary-y" ; }
    else if(LV_CHART_AXIS_SECONDARY_Y==v){ return "secondary-y" ; }
    else if(LV_CHART_AXIS_PRIMARY_X==v){ return "primary-x" ; }
    else if(LV_CHART_AXIS_SECONDARY_X==v){ return "secondary-x" ; }
    else if(_LV_CHART_AXIS_LAST==v){ return "_last" ; }
    else {
        return "<unknow>" ;
    }
}
EMSCRIPTEN_KEEPALIVE uint32_t lv_chart_axis_to_value(char * n) {
    if(strcmp("primary-y", n)==0){ return LV_CHART_AXIS_PRIMARY_Y ; }
    else if(strcmp("secondary-y", n)==0){ return LV_CHART_AXIS_SECONDARY_Y ; }
    else if(strcmp("primary-x", n)==0){ return LV_CHART_AXIS_PRIMARY_X ; }
    else if(strcmp("secondary-x", n)==0){ return LV_CHART_AXIS_SECONDARY_X ; }
    else if(strcmp("_last", n)==0){ return _LV_CHART_AXIS_LAST ; }
    else {
        return (uint32_t)-1 ;
    }
}
// "lv_chart_draw_part_type_t": /^(_?)LV_CHART_DRAW_PART_/ ,
EMSCRIPTEN_KEEPALIVE char * lv_chart_draw_part_type_to_name(uint32_t v) {
    if(LV_CHART_DRAW_PART_DIV_LINE_INIT==v){ return "div-line-init" ; }
    else if(LV_CHART_DRAW_PART_DIV_LINE_HOR==v){ return "div-line-hor" ; }
    else if(LV_CHART_DRAW_PART_DIV_LINE_VER==v){ return "div-line-ver" ; }
    else if(LV_CHART_DRAW_PART_LINE_AND_POINT==v){ return "line-and-point" ; }
    else if(LV_CHART_DRAW_PART_BAR==v){ return "bar" ; }
    else if(LV_CHART_DRAW_PART_CURSOR==v){ return "cursor" ; }
    else if(LV_CHART_DRAW_PART_TICK_LABEL==v){ return "tick-label" ; }
    else {
        return "<unknow>" ;
    }
}
EMSCRIPTEN_KEEPALIVE uint32_t lv_chart_draw_part_type_to_value(char * n) {
    if(strcmp("div-line-init", n)==0){ return LV_CHART_DRAW_PART_DIV_LINE_INIT ; }
    else if(strcmp("div-line-hor", n)==0){ return LV_CHART_DRAW_PART_DIV_LINE_HOR ; }
    else if(strcmp("div-line-ver", n)==0){ return LV_CHART_DRAW_PART_DIV_LINE_VER ; }
    else if(strcmp("line-and-point", n)==0){ return LV_CHART_DRAW_PART_LINE_AND_POINT ; }
    else if(strcmp("bar", n)==0){ return LV_CHART_DRAW_PART_BAR ; }
    else if(strcmp("cursor", n)==0){ return LV_CHART_DRAW_PART_CURSOR ; }
    else if(strcmp("tick-label", n)==0){ return LV_CHART_DRAW_PART_TICK_LABEL ; }
    else {
        return (uint32_t)-1 ;
    }
}
// "lv_keyboard_mode_t": /^(_?)LV_KEYBOARD_/ ,
EMSCRIPTEN_KEEPALIVE char * lv_keyboard_mode_to_name(uint32_t v) {
    if(LV_KEYBOARD_MODE_TEXT_LOWER==v){ return "mode-text-lower" ; }
    else if(LV_KEYBOARD_MODE_TEXT_UPPER==v){ return "mode-text-upper" ; }
    else if(LV_KEYBOARD_MODE_SPECIAL==v){ return "mode-special" ; }
    else if(LV_KEYBOARD_MODE_NUMBER==v){ return "mode-number" ; }
    else if(LV_KEYBOARD_MODE_USER_1==v){ return "mode-user-1" ; }
    else if(LV_KEYBOARD_MODE_USER_2==v){ return "mode-user-2" ; }
    else if(LV_KEYBOARD_MODE_USER_3==v){ return "mode-user-3" ; }
    else if(LV_KEYBOARD_MODE_USER_4==v){ return "mode-user-4" ; }
    else {
        return "<unknow>" ;
    }
}
EMSCRIPTEN_KEEPALIVE uint32_t lv_keyboard_mode_to_value(char * n) {
    if(strcmp("mode-text-lower", n)==0){ return LV_KEYBOARD_MODE_TEXT_LOWER ; }
    else if(strcmp("mode-text-upper", n)==0){ return LV_KEYBOARD_MODE_TEXT_UPPER ; }
    else if(strcmp("mode-special", n)==0){ return LV_KEYBOARD_MODE_SPECIAL ; }
    else if(strcmp("mode-number", n)==0){ return LV_KEYBOARD_MODE_NUMBER ; }
    else if(strcmp("mode-user-1", n)==0){ return LV_KEYBOARD_MODE_USER_1 ; }
    else if(strcmp("mode-user-2", n)==0){ return LV_KEYBOARD_MODE_USER_2 ; }
    else if(strcmp("mode-user-3", n)==0){ return LV_KEYBOARD_MODE_USER_3 ; }
    else if(strcmp("mode-user-4", n)==0){ return LV_KEYBOARD_MODE_USER_4 ; }
    else {
        return (uint32_t)-1 ;
    }
}
// "lv_flex_align_t": /^(_?)LV_FLEX_ALIGN_/ ,
EMSCRIPTEN_KEEPALIVE char * lv_flex_align_to_name(uint32_t v) {
    if(LV_FLEX_ALIGN_START==v){ return "start" ; }
    else if(LV_FLEX_ALIGN_END==v){ return "end" ; }
    else if(LV_FLEX_ALIGN_CENTER==v){ return "center" ; }
    else if(LV_FLEX_ALIGN_SPACE_EVENLY==v){ return "space-evenly" ; }
    else if(LV_FLEX_ALIGN_SPACE_AROUND==v){ return "space-around" ; }
    else if(LV_FLEX_ALIGN_SPACE_BETWEEN==v){ return "space-between" ; }
    else {
        return "<unknow>" ;
    }
}
EMSCRIPTEN_KEEPALIVE uint32_t lv_flex_align_to_value(char * n) {
    if(strcmp("start", n)==0){ return LV_FLEX_ALIGN_START ; }
    else if(strcmp("end", n)==0){ return LV_FLEX_ALIGN_END ; }
    else if(strcmp("center", n)==0){ return LV_FLEX_ALIGN_CENTER ; }
    else if(strcmp("space-evenly", n)==0){ return LV_FLEX_ALIGN_SPACE_EVENLY ; }
    else if(strcmp("space-around", n)==0){ return LV_FLEX_ALIGN_SPACE_AROUND ; }
    else if(strcmp("space-between", n)==0){ return LV_FLEX_ALIGN_SPACE_BETWEEN ; }
    else {
        return (uint32_t)-1 ;
    }
}
// "lv_flex_flow_t": /^(_?)LV_FLEX_FLOW_/ ,
EMSCRIPTEN_KEEPALIVE char * lv_flex_flow_to_name(uint32_t v) {
    if(LV_FLEX_FLOW_ROW==v){ return "row" ; }
    else if(LV_FLEX_FLOW_COLUMN==v){ return "column" ; }
    else if(LV_FLEX_FLOW_ROW_WRAP==v){ return "row-wrap" ; }
    else if(LV_FLEX_FLOW_ROW_REVERSE==v){ return "row-reverse" ; }
    else if(LV_FLEX_FLOW_ROW_WRAP_REVERSE==v){ return "row-wrap-reverse" ; }
    else if(LV_FLEX_FLOW_COLUMN_WRAP==v){ return "column-wrap" ; }
    else if(LV_FLEX_FLOW_COLUMN_REVERSE==v){ return "column-reverse" ; }
    else if(LV_FLEX_FLOW_COLUMN_WRAP_REVERSE==v){ return "column-wrap-reverse" ; }
    else {
        return "<unknow>" ;
    }
}
EMSCRIPTEN_KEEPALIVE uint32_t lv_flex_flow_to_value(char * n) {
    if(strcmp("row", n)==0){ return LV_FLEX_FLOW_ROW ; }
    else if(strcmp("column", n)==0){ return LV_FLEX_FLOW_COLUMN ; }
    else if(strcmp("row-wrap", n)==0){ return LV_FLEX_FLOW_ROW_WRAP ; }
    else if(strcmp("row-reverse", n)==0){ return LV_FLEX_FLOW_ROW_REVERSE ; }
    else if(strcmp("row-wrap-reverse", n)==0){ return LV_FLEX_FLOW_ROW_WRAP_REVERSE ; }
    else if(strcmp("column-wrap", n)==0){ return LV_FLEX_FLOW_COLUMN_WRAP ; }
    else if(strcmp("column-reverse", n)==0){ return LV_FLEX_FLOW_COLUMN_REVERSE ; }
    else if(strcmp("column-wrap-reverse", n)==0){ return LV_FLEX_FLOW_COLUMN_WRAP_REVERSE ; }
    else {
        return (uint32_t)-1 ;
    }
}
// "lv_meter_indicator_type_t": /^(_?)LV_METER_INDICATOR_TYPE_/ ,
EMSCRIPTEN_KEEPALIVE char * lv_meter_indicator_type_to_name(uint32_t v) {
    if(LV_METER_INDICATOR_TYPE_NEEDLE_IMG==v){ return "needle-img" ; }
    else if(LV_METER_INDICATOR_TYPE_NEEDLE_LINE==v){ return "needle-line" ; }
    else if(LV_METER_INDICATOR_TYPE_SCALE_LINES==v){ return "scale-lines" ; }
    else if(LV_METER_INDICATOR_TYPE_ARC==v){ return "arc" ; }
    else {
        return "<unknow>" ;
    }
}
EMSCRIPTEN_KEEPALIVE uint32_t lv_meter_indicator_type_to_value(char * n) {
    if(strcmp("needle-img", n)==0){ return LV_METER_INDICATOR_TYPE_NEEDLE_IMG ; }
    else if(strcmp("needle-line", n)==0){ return LV_METER_INDICATOR_TYPE_NEEDLE_LINE ; }
    else if(strcmp("scale-lines", n)==0){ return LV_METER_INDICATOR_TYPE_SCALE_LINES ; }
    else if(strcmp("arc", n)==0){ return LV_METER_INDICATOR_TYPE_ARC ; }
    else {
        return (uint32_t)-1 ;
    }
}
// "lv_meter_draw_part_type_t": /^(_?)LV_METER_DRAW_PART_/ ,
EMSCRIPTEN_KEEPALIVE char * lv_meter_draw_part_type_to_name(uint32_t v) {
    if(LV_METER_DRAW_PART_ARC==v){ return "arc" ; }
    else if(LV_METER_DRAW_PART_NEEDLE_LINE==v){ return "needle-line" ; }
    else if(LV_METER_DRAW_PART_NEEDLE_IMG==v){ return "needle-img" ; }
    else if(LV_METER_DRAW_PART_TICK==v){ return "tick" ; }
    else {
        return "<unknow>" ;
    }
}
EMSCRIPTEN_KEEPALIVE uint32_t lv_meter_draw_part_type_to_value(char * n) {
    if(strcmp("arc", n)==0){ return LV_METER_DRAW_PART_ARC ; }
    else if(strcmp("needle-line", n)==0){ return LV_METER_DRAW_PART_NEEDLE_LINE ; }
    else if(strcmp("needle-img", n)==0){ return LV_METER_DRAW_PART_NEEDLE_IMG ; }
    else if(strcmp("tick", n)==0){ return LV_METER_DRAW_PART_TICK ; }
    else {
        return (uint32_t)-1 ;
    }
}
// "lv_colorwheel_mode_t": /^(_?)LV_COLORWHEEL_/ ,
EMSCRIPTEN_KEEPALIVE char * lv_colorwheel_mode_to_name(uint32_t v) {
    if(LV_COLORWHEEL_MODE_HUE==v){ return "mode-hue" ; }
    else if(LV_COLORWHEEL_MODE_SATURATION==v){ return "mode-saturation" ; }
    else if(LV_COLORWHEEL_MODE_VALUE==v){ return "mode-value" ; }
    else {
        return "<unknow>" ;
    }
}
EMSCRIPTEN_KEEPALIVE uint32_t lv_colorwheel_mode_to_value(char * n) {
    if(strcmp("mode-hue", n)==0){ return LV_COLORWHEEL_MODE_HUE ; }
    else if(strcmp("mode-saturation", n)==0){ return LV_COLORWHEEL_MODE_SATURATION ; }
    else if(strcmp("mode-value", n)==0){ return LV_COLORWHEEL_MODE_VALUE ; }
    else {
        return (uint32_t)-1 ;
    }
}
// "lv_led_draw_part_type_t": /^(_?)LV_LED_DRAW_PART_/ ,
EMSCRIPTEN_KEEPALIVE char * lv_led_draw_part_type_to_name(uint32_t v) {
    if(LV_LED_DRAW_PART_RECTANGLE==v){ return "rectangle" ; }
    else {
        return "<unknow>" ;
    }
}
EMSCRIPTEN_KEEPALIVE uint32_t lv_led_draw_part_type_to_value(char * n) {
    if(strcmp("rectangle", n)==0){ return LV_LED_DRAW_PART_RECTANGLE ; }
    else {
        return (uint32_t)-1 ;
    }
}
// "lv_imgbtn_state_t": /^(_?)LV_IMGBTN_STATE_/ ,
EMSCRIPTEN_KEEPALIVE char * lv_imgbtn_state_to_name(uint32_t v) {
    if(LV_IMGBTN_STATE_RELEASED==v){ return "released" ; }
    else if(LV_IMGBTN_STATE_PRESSED==v){ return "pressed" ; }
    else if(LV_IMGBTN_STATE_DISABLED==v){ return "disabled" ; }
    else if(LV_IMGBTN_STATE_CHECKED_RELEASED==v){ return "checked-released" ; }
    else if(LV_IMGBTN_STATE_CHECKED_PRESSED==v){ return "checked-pressed" ; }
    else if(LV_IMGBTN_STATE_CHECKED_DISABLED==v){ return "checked-disabled" ; }
    else if(_LV_IMGBTN_STATE_NUM==v){ return "_num" ; }
    else {
        return "<unknow>" ;
    }
}
EMSCRIPTEN_KEEPALIVE uint32_t lv_imgbtn_state_to_value(char * n) {
    if(strcmp("released", n)==0){ return LV_IMGBTN_STATE_RELEASED ; }
    else if(strcmp("pressed", n)==0){ return LV_IMGBTN_STATE_PRESSED ; }
    else if(strcmp("disabled", n)==0){ return LV_IMGBTN_STATE_DISABLED ; }
    else if(strcmp("checked-released", n)==0){ return LV_IMGBTN_STATE_CHECKED_RELEASED ; }
    else if(strcmp("checked-pressed", n)==0){ return LV_IMGBTN_STATE_CHECKED_PRESSED ; }
    else if(strcmp("checked-disabled", n)==0){ return LV_IMGBTN_STATE_CHECKED_DISABLED ; }
    else if(strcmp("_num", n)==0){ return _LV_IMGBTN_STATE_NUM ; }
    else {
        return (uint32_t)-1 ;
    }
}
// "lv_span_overflow_t": /^(_?)LV_SPAN_OVERFLOW_/ ,
EMSCRIPTEN_KEEPALIVE char * lv_span_overflow_to_name(uint32_t v) {
    if(LV_SPAN_OVERFLOW_CLIP==v){ return "clip" ; }
    else if(LV_SPAN_OVERFLOW_ELLIPSIS==v){ return "ellipsis" ; }
    else {
        return "<unknow>" ;
    }
}
EMSCRIPTEN_KEEPALIVE uint32_t lv_span_overflow_to_value(char * n) {
    if(strcmp("clip", n)==0){ return LV_SPAN_OVERFLOW_CLIP ; }
    else if(strcmp("ellipsis", n)==0){ return LV_SPAN_OVERFLOW_ELLIPSIS ; }
    else {
        return (uint32_t)-1 ;
    }
}
// "lv_span_mode_t": /^(_?)LV_SPAN_/ ,
EMSCRIPTEN_KEEPALIVE char * lv_span_mode_to_name(uint32_t v) {
    if(LV_SPAN_MODE_FIXED==v){ return "mode-fixed" ; }
    else if(LV_SPAN_MODE_EXPAND==v){ return "mode-expand" ; }
    else if(LV_SPAN_MODE_BREAK==v){ return "mode-break" ; }
    else {
        return "<unknow>" ;
    }
}
EMSCRIPTEN_KEEPALIVE uint32_t lv_span_mode_to_value(char * n) {
    if(strcmp("mode-fixed", n)==0){ return LV_SPAN_MODE_FIXED ; }
    else if(strcmp("mode-expand", n)==0){ return LV_SPAN_MODE_EXPAND ; }
    else if(strcmp("mode-break", n)==0){ return LV_SPAN_MODE_BREAK ; }
    else {
        return (uint32_t)-1 ;
    }
}
// "lv_grid_align_t": /^(_?)LV_GRID_ALIGN_/ ,
EMSCRIPTEN_KEEPALIVE char * lv_grid_align_to_name(uint32_t v) {
    if(LV_GRID_ALIGN_START==v){ return "start" ; }
    else if(LV_GRID_ALIGN_CENTER==v){ return "center" ; }
    else if(LV_GRID_ALIGN_END==v){ return "end" ; }
    else if(LV_GRID_ALIGN_STRETCH==v){ return "stretch" ; }
    else if(LV_GRID_ALIGN_SPACE_EVENLY==v){ return "space-evenly" ; }
    else if(LV_GRID_ALIGN_SPACE_AROUND==v){ return "space-around" ; }
    else if(LV_GRID_ALIGN_SPACE_BETWEEN==v){ return "space-between" ; }
    else {
        return "<unknow>" ;
    }
}
EMSCRIPTEN_KEEPALIVE uint32_t lv_grid_align_to_value(char * n) {
    if(strcmp("start", n)==0){ return LV_GRID_ALIGN_START ; }
    else if(strcmp("center", n)==0){ return LV_GRID_ALIGN_CENTER ; }
    else if(strcmp("end", n)==0){ return LV_GRID_ALIGN_END ; }
    else if(strcmp("stretch", n)==0){ return LV_GRID_ALIGN_STRETCH ; }
    else if(strcmp("space-evenly", n)==0){ return LV_GRID_ALIGN_SPACE_EVENLY ; }
    else if(strcmp("space-around", n)==0){ return LV_GRID_ALIGN_SPACE_AROUND ; }
    else if(strcmp("space-between", n)==0){ return LV_GRID_ALIGN_SPACE_BETWEEN ; }
    else {
        return (uint32_t)-1 ;
    }
}
// AUTO GENERATE CODE END [CONST MAPPING] --------
