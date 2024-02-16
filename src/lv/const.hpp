#pragma once
#include "LV.hpp"
#include "quickjs/quickjs.h"

namespace be::lv {

// AUTO GENERATE CODE START [CONST MAPPING] --------

bool lv_align_str_to_const(const char * str, lv_align_t* out) ;
bool lv_align_jsstr_to_const(JSContext *ctx, JSValue jsstr, lv_align_t* out) ;
const char * lv_align_const_to_str(lv_align_t code) ;
JSValue lv_align_const_to_jsstr(JSContext *ctx, lv_align_t code) ;

bool lv_dir_str_to_const(const char * str, lv_dir_t* out) ;
bool lv_dir_jsstr_to_const(JSContext *ctx, JSValue jsstr, lv_dir_t* out) ;
const char * lv_dir_const_to_str(lv_dir_t code) ;
JSValue lv_dir_const_to_jsstr(JSContext *ctx, lv_dir_t code) ;

bool lv_scroll_snap_str_to_const(const char * str, lv_scroll_snap_t* out) ;
bool lv_scroll_snap_jsstr_to_const(JSContext *ctx, JSValue jsstr, lv_scroll_snap_t* out) ;
const char * lv_scroll_snap_const_to_str(lv_scroll_snap_t code) ;
JSValue lv_scroll_snap_const_to_jsstr(JSContext *ctx, lv_scroll_snap_t code) ;

bool lv_flex_flow_str_to_const(const char * str, lv_flex_flow_t* out) ;
bool lv_flex_flow_jsstr_to_const(JSContext *ctx, JSValue jsstr, lv_flex_flow_t* out) ;
const char * lv_flex_flow_const_to_str(lv_flex_flow_t code) ;
JSValue lv_flex_flow_const_to_jsstr(JSContext *ctx, lv_flex_flow_t code) ;

bool lv_event_code_str_to_const(const char * str, lv_event_code_t* out) ;
bool lv_event_code_jsstr_to_const(JSContext *ctx, JSValue jsstr, lv_event_code_t* out) ;
const char * lv_event_code_const_to_str(lv_event_code_t code) ;
JSValue lv_event_code_const_to_jsstr(JSContext *ctx, lv_event_code_t code) ;

bool lv_anim_enable_str_to_const(const char * str, lv_anim_enable_t* out) ;
bool lv_anim_enable_jsstr_to_const(JSContext *ctx, JSValue jsstr, lv_anim_enable_t* out) ;
const char * lv_anim_enable_const_to_str(lv_anim_enable_t code) ;
JSValue lv_anim_enable_const_to_jsstr(JSContext *ctx, lv_anim_enable_t code) ;

bool lv_style_prop_str_to_const(const char * str, lv_style_prop_t* out) ;
bool lv_style_prop_jsstr_to_const(JSContext *ctx, JSValue jsstr, lv_style_prop_t* out) ;
const char * lv_style_prop_const_to_str(lv_style_prop_t code) ;
JSValue lv_style_prop_const_to_jsstr(JSContext *ctx, lv_style_prop_t code) ;

bool lv_keyboard_mode_str_to_const(const char * str, lv_keyboard_mode_t* out) ;
bool lv_keyboard_mode_jsstr_to_const(JSContext *ctx, JSValue jsstr, lv_keyboard_mode_t* out) ;
const char * lv_keyboard_mode_const_to_str(lv_keyboard_mode_t code) ;
JSValue lv_keyboard_mode_const_to_jsstr(JSContext *ctx, lv_keyboard_mode_t code) ;

bool lv_scale_mode_str_to_const(const char * str, lv_scale_mode_t* out) ;
bool lv_scale_mode_jsstr_to_const(JSContext *ctx, JSValue jsstr, lv_scale_mode_t* out) ;
const char * lv_scale_mode_const_to_str(lv_scale_mode_t code) ;
JSValue lv_scale_mode_const_to_jsstr(JSContext *ctx, lv_scale_mode_t code) ;

bool lv_buttonmatrix_ctrl_str_to_const(const char * str, lv_buttonmatrix_ctrl_t* out) ;
bool lv_buttonmatrix_ctrl_jsstr_to_const(JSContext *ctx, JSValue jsstr, lv_buttonmatrix_ctrl_t* out) ;
const char * lv_buttonmatrix_ctrl_const_to_str(lv_buttonmatrix_ctrl_t code) ;
JSValue lv_buttonmatrix_ctrl_const_to_jsstr(JSContext *ctx, lv_buttonmatrix_ctrl_t code) ;

bool lv_chart_type_str_to_const(const char * str, lv_chart_type_t* out) ;
bool lv_chart_type_jsstr_to_const(JSContext *ctx, JSValue jsstr, lv_chart_type_t* out) ;
const char * lv_chart_type_const_to_str(lv_chart_type_t code) ;
JSValue lv_chart_type_const_to_jsstr(JSContext *ctx, lv_chart_type_t code) ;

bool lv_chart_update_mode_str_to_const(const char * str, lv_chart_update_mode_t* out) ;
bool lv_chart_update_mode_jsstr_to_const(JSContext *ctx, JSValue jsstr, lv_chart_update_mode_t* out) ;
const char * lv_chart_update_mode_const_to_str(lv_chart_update_mode_t code) ;
JSValue lv_chart_update_mode_const_to_jsstr(JSContext *ctx, lv_chart_update_mode_t code) ;

bool lv_chart_axis_str_to_const(const char * str, lv_chart_axis_t* out) ;
bool lv_chart_axis_jsstr_to_const(JSContext *ctx, JSValue jsstr, lv_chart_axis_t* out) ;
const char * lv_chart_axis_const_to_str(lv_chart_axis_t code) ;
JSValue lv_chart_axis_const_to_jsstr(JSContext *ctx, lv_chart_axis_t code) ;

bool lv_blend_mode_str_to_const(const char * str, lv_blend_mode_t* out) ;
bool lv_blend_mode_jsstr_to_const(JSContext *ctx, JSValue jsstr, lv_blend_mode_t* out) ;
const char * lv_blend_mode_const_to_str(lv_blend_mode_t code) ;
JSValue lv_blend_mode_const_to_jsstr(JSContext *ctx, lv_blend_mode_t code) ;

bool lv_image_align_str_to_const(const char * str, lv_image_align_t* out) ;
bool lv_image_align_jsstr_to_const(JSContext *ctx, JSValue jsstr, lv_image_align_t* out) ;
const char * lv_image_align_const_to_str(lv_image_align_t code) ;
JSValue lv_image_align_const_to_jsstr(JSContext *ctx, lv_image_align_t code) ;

bool lv_imagebutton_state_str_to_const(const char * str, lv_imagebutton_state_t* out) ;
bool lv_imagebutton_state_jsstr_to_const(JSContext *ctx, JSValue jsstr, lv_imagebutton_state_t* out) ;
const char * lv_imagebutton_state_const_to_str(lv_imagebutton_state_t code) ;
JSValue lv_imagebutton_state_const_to_jsstr(JSContext *ctx, lv_imagebutton_state_t code) ;

bool lv_menu_mode_header_str_to_const(const char * str, lv_menu_mode_header_t* out) ;
bool lv_menu_mode_header_jsstr_to_const(JSContext *ctx, JSValue jsstr, lv_menu_mode_header_t* out) ;
const char * lv_menu_mode_header_const_to_str(lv_menu_mode_header_t code) ;
JSValue lv_menu_mode_header_const_to_jsstr(JSContext *ctx, lv_menu_mode_header_t code) ;

bool lv_menu_mode_root_back_button_str_to_const(const char * str, lv_menu_mode_root_back_button_t* out) ;
bool lv_menu_mode_root_back_button_jsstr_to_const(JSContext *ctx, JSValue jsstr, lv_menu_mode_root_back_button_t* out) ;
const char * lv_menu_mode_root_back_button_const_to_str(lv_menu_mode_root_back_button_t code) ;
JSValue lv_menu_mode_root_back_button_const_to_jsstr(JSContext *ctx, lv_menu_mode_root_back_button_t code) ;

bool lv_span_overflow_str_to_const(const char * str, lv_span_overflow_t* out) ;
bool lv_span_overflow_jsstr_to_const(JSContext *ctx, JSValue jsstr, lv_span_overflow_t* out) ;
const char * lv_span_overflow_const_to_str(lv_span_overflow_t code) ;
JSValue lv_span_overflow_const_to_jsstr(JSContext *ctx, lv_span_overflow_t code) ;

bool lv_span_mode_str_to_const(const char * str, lv_span_mode_t* out) ;
bool lv_span_mode_jsstr_to_const(JSContext *ctx, JSValue jsstr, lv_span_mode_t* out) ;
const char * lv_span_mode_const_to_str(lv_span_mode_t code) ;
JSValue lv_span_mode_const_to_jsstr(JSContext *ctx, lv_span_mode_t code) ;
// AUTO GENERATE CODE END [CONST MAPPING] --------

}