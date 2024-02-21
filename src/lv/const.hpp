#pragma once
#include "LV.hpp"
#include "quickjs/quickjs.h"

namespace be::lv {

// AUTO GENERATE CODE START [CONST MAPPING] --------

bool lv_flex_flow_str_to_const(const char * str, lv_flex_flow_t* out) ;
bool lv_flex_flow_jsstr_to_const(JSContext *ctx, JSValue jsstr, lv_flex_flow_t* out) ;
const char * lv_flex_flow_const_to_str(lv_flex_flow_t code) ;
JSValue lv_flex_flow_const_to_jsstr(JSContext *ctx, lv_flex_flow_t code) ;

bool lv_flex_align_str_to_const(const char * str, lv_flex_align_t* out) ;
bool lv_flex_align_jsstr_to_const(JSContext *ctx, JSValue jsstr, lv_flex_align_t* out) ;
const char * lv_flex_align_const_to_str(lv_flex_align_t code) ;
JSValue lv_flex_align_const_to_jsstr(JSContext *ctx, lv_flex_align_t code) ;

bool lv_align_str_to_const(const char * str, lv_align_t* out) ;
bool lv_align_jsstr_to_const(JSContext *ctx, JSValue jsstr, lv_align_t* out) ;
const char * lv_align_const_to_str(lv_align_t code) ;
JSValue lv_align_const_to_jsstr(JSContext *ctx, lv_align_t code) ;

bool lv_border_side_str_to_const(const char * str, lv_border_side_t* out) ;
bool lv_border_side_jsstr_to_const(JSContext *ctx, JSValue jsstr, lv_border_side_t* out) ;
const char * lv_border_side_const_to_str(lv_border_side_t code) ;
JSValue lv_border_side_const_to_jsstr(JSContext *ctx, lv_border_side_t code) ;

bool lv_text_align_str_to_const(const char * str, lv_text_align_t* out) ;
bool lv_text_align_jsstr_to_const(JSContext *ctx, JSValue jsstr, lv_text_align_t* out) ;
const char * lv_text_align_const_to_str(lv_text_align_t code) ;
JSValue lv_text_align_const_to_jsstr(JSContext *ctx, lv_text_align_t code) ;

bool lv_base_dir_str_to_const(const char * str, lv_base_dir_t* out) ;
bool lv_base_dir_jsstr_to_const(JSContext *ctx, JSValue jsstr, lv_base_dir_t* out) ;
const char * lv_base_dir_const_to_str(lv_base_dir_t code) ;
JSValue lv_base_dir_const_to_jsstr(JSContext *ctx, lv_base_dir_t code) ;

bool lv_scrollbar_mode_str_to_const(const char * str, lv_scrollbar_mode_t* out) ;
bool lv_scrollbar_mode_jsstr_to_const(JSContext *ctx, JSValue jsstr, lv_scrollbar_mode_t* out) ;
const char * lv_scrollbar_mode_const_to_str(lv_scrollbar_mode_t code) ;
JSValue lv_scrollbar_mode_const_to_jsstr(JSContext *ctx, lv_scrollbar_mode_t code) ;

bool lv_dir_str_to_const(const char * str, lv_dir_t* out) ;
bool lv_dir_jsstr_to_const(JSContext *ctx, JSValue jsstr, lv_dir_t* out) ;
const char * lv_dir_const_to_str(lv_dir_t code) ;
JSValue lv_dir_const_to_jsstr(JSContext *ctx, lv_dir_t code) ;

bool lv_scroll_snap_str_to_const(const char * str, lv_scroll_snap_t* out) ;
bool lv_scroll_snap_jsstr_to_const(JSContext *ctx, JSValue jsstr, lv_scroll_snap_t* out) ;
const char * lv_scroll_snap_const_to_str(lv_scroll_snap_t code) ;
JSValue lv_scroll_snap_const_to_jsstr(JSContext *ctx, lv_scroll_snap_t code) ;

bool lv__obj_flag_str_to_const(const char * str, lv_obj_flag_t* out) ;
bool lv__obj_flag_jsstr_to_const(JSContext *ctx, JSValue jsstr, lv_obj_flag_t* out) ;
const char * lv__obj_flag_const_to_str(lv_obj_flag_t code) ;
JSValue lv__obj_flag_const_to_jsstr(JSContext *ctx, lv_obj_flag_t code) ;

bool lv_state_str_to_const(const char * str, lv_state_t* out) ;
bool lv_state_jsstr_to_const(JSContext *ctx, JSValue jsstr, lv_state_t* out) ;
const char * lv_state_const_to_str(lv_state_t code) ;
JSValue lv_state_const_to_jsstr(JSContext *ctx, lv_state_t code) ;

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

bool lv_grad_dir_str_to_const(const char * str, lv_grad_dir_t* out) ;
bool lv_grad_dir_jsstr_to_const(JSContext *ctx, JSValue jsstr, lv_grad_dir_t* out) ;
const char * lv_grad_dir_const_to_str(lv_grad_dir_t code) ;
JSValue lv_grad_dir_const_to_jsstr(JSContext *ctx, lv_grad_dir_t code) ;

bool lv_text_decor_str_to_const(const char * str, lv_text_decor_t* out) ;
bool lv_text_decor_jsstr_to_const(JSContext *ctx, JSValue jsstr, lv_text_decor_t* out) ;
const char * lv_text_decor_const_to_str(lv_text_decor_t code) ;
JSValue lv_text_decor_const_to_jsstr(JSContext *ctx, lv_text_decor_t code) ;

bool lv_blend_mode_str_to_const(const char * str, lv_blend_mode_t* out) ;
bool lv_blend_mode_jsstr_to_const(JSContext *ctx, JSValue jsstr, lv_blend_mode_t* out) ;
const char * lv_blend_mode_const_to_str(lv_blend_mode_t code) ;
JSValue lv_blend_mode_const_to_jsstr(JSContext *ctx, lv_blend_mode_t code) ;

bool lv_grid_align_str_to_const(const char * str, lv_grid_align_t* out) ;
bool lv_grid_align_jsstr_to_const(JSContext *ctx, JSValue jsstr, lv_grid_align_t* out) ;
const char * lv_grid_align_const_to_str(lv_grid_align_t code) ;
JSValue lv_grid_align_const_to_jsstr(JSContext *ctx, lv_grid_align_t code) ;

bool lv_opa_str_to_const(const char * str, lv_opa_t* out) ;
bool lv_opa_jsstr_to_const(JSContext *ctx, JSValue jsstr, lv_opa_t* out) ;
const char * lv_opa_const_to_str(lv_opa_t code) ;
JSValue lv_opa_const_to_jsstr(JSContext *ctx, lv_opa_t code) ;

bool lv_keyboard_mode_str_to_const(const char * str, lv_keyboard_mode_t* out) ;
bool lv_keyboard_mode_jsstr_to_const(JSContext *ctx, JSValue jsstr, lv_keyboard_mode_t* out) ;
const char * lv_keyboard_mode_const_to_str(lv_keyboard_mode_t code) ;
JSValue lv_keyboard_mode_const_to_jsstr(JSContext *ctx, lv_keyboard_mode_t code) ;

bool lv_scale_mode_str_to_const(const char * str, lv_scale_mode_t* out) ;
bool lv_scale_mode_jsstr_to_const(JSContext *ctx, JSValue jsstr, lv_scale_mode_t* out) ;
const char * lv_scale_mode_const_to_str(lv_scale_mode_t code) ;
JSValue lv_scale_mode_const_to_jsstr(JSContext *ctx, lv_scale_mode_t code) ;

bool lv_table_cell_ctrl_str_to_const(const char * str, lv_table_cell_ctrl_t* out) ;
bool lv_table_cell_ctrl_jsstr_to_const(JSContext *ctx, JSValue jsstr, lv_table_cell_ctrl_t* out) ;
const char * lv_table_cell_ctrl_const_to_str(lv_table_cell_ctrl_t code) ;
JSValue lv_table_cell_ctrl_const_to_jsstr(JSContext *ctx, lv_table_cell_ctrl_t code) ;

bool lv_arc_mode_str_to_const(const char * str, lv_arc_mode_t* out) ;
bool lv_arc_mode_jsstr_to_const(JSContext *ctx, JSValue jsstr, lv_arc_mode_t* out) ;
const char * lv_arc_mode_const_to_str(lv_arc_mode_t code) ;
JSValue lv_arc_mode_const_to_jsstr(JSContext *ctx, lv_arc_mode_t code) ;

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

bool lv_image_align_str_to_const(const char * str, lv_image_align_t* out) ;
bool lv_image_align_jsstr_to_const(JSContext *ctx, JSValue jsstr, lv_image_align_t* out) ;
const char * lv_image_align_const_to_str(lv_image_align_t code) ;
JSValue lv_image_align_const_to_jsstr(JSContext *ctx, lv_image_align_t code) ;

bool lv_label_long_mode_str_to_const(const char * str, lv_label_long_mode_t* out) ;
bool lv_label_long_mode_jsstr_to_const(JSContext *ctx, JSValue jsstr, lv_label_long_mode_t* out) ;
const char * lv_label_long_mode_const_to_str(lv_label_long_mode_t code) ;
JSValue lv_label_long_mode_const_to_jsstr(JSContext *ctx, lv_label_long_mode_t code) ;

bool lv_slider_mode_str_to_const(const char * str, lv_slider_mode_t* out) ;
bool lv_slider_mode_jsstr_to_const(JSContext *ctx, JSValue jsstr, lv_slider_mode_t* out) ;
const char * lv_slider_mode_const_to_str(lv_slider_mode_t code) ;
JSValue lv_slider_mode_const_to_jsstr(JSContext *ctx, lv_slider_mode_t code) ;

bool lv_bar_mode_str_to_const(const char * str, lv_bar_mode_t* out) ;
bool lv_bar_mode_jsstr_to_const(JSContext *ctx, JSValue jsstr, lv_bar_mode_t* out) ;
const char * lv_bar_mode_const_to_str(lv_bar_mode_t code) ;
JSValue lv_bar_mode_const_to_jsstr(JSContext *ctx, lv_bar_mode_t code) ;

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

bool lv_roller_mode_str_to_const(const char * str, lv_roller_mode_t* out) ;
bool lv_roller_mode_jsstr_to_const(JSContext *ctx, JSValue jsstr, lv_roller_mode_t* out) ;
const char * lv_roller_mode_const_to_str(lv_roller_mode_t code) ;
JSValue lv_roller_mode_const_to_jsstr(JSContext *ctx, lv_roller_mode_t code) ;

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