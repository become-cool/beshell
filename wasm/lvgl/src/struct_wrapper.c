#include <stdlib.h>
#include <emscripten.h>
#include "lv_conf.h"
#include "lvgl.h"
#include "../../main/debug.h"

// AUTO GENERATE CODE START [API STRUCT] --------
// struct lv_timer_t wasm api ----------
// fields getter
EMSCRIPTEN_KEEPALIVE uint32_t lv_timer_get_period(lv_timer_t * p) { return p->period ; }
EMSCRIPTEN_KEEPALIVE uint32_t lv_timer_get_last_run(lv_timer_t * p) { return p->last_run ; }
EMSCRIPTEN_KEEPALIVE lv_timer_cb_t lv_timer_get_timer_cb(lv_timer_t * p) { return p->timer_cb ; }
EMSCRIPTEN_KEEPALIVE void * lv_timer_get_user_data(lv_timer_t * p) { return p->user_data ; }
EMSCRIPTEN_KEEPALIVE int32_t lv_timer_get_repeat_count(lv_timer_t * p) { return p->repeat_count ; }
EMSCRIPTEN_KEEPALIVE uint32_t lv_timer_get_paused(lv_timer_t * p) { return p->paused ; }
// fields setter
EMSCRIPTEN_KEEPALIVE void lv_timer_set_last_run(lv_timer_t * p, uint32_t v) { p->last_run = v ; }
EMSCRIPTEN_KEEPALIVE void lv_timer_set_timer_cb(lv_timer_t * p, lv_timer_cb_t v) { p->timer_cb = v ; }
EMSCRIPTEN_KEEPALIVE void lv_timer_set_user_data(lv_timer_t * p, void * v) { p->user_data = v ; }
EMSCRIPTEN_KEEPALIVE void lv_timer_set_paused(lv_timer_t * p, uint32_t v) { p->paused = v ; }


// struct lv_sqrt_res_t wasm api ----------
EMSCRIPTEN_KEEPALIVE lv_sqrt_res_t * lv_sqrt_res_create() {
    lv_sqrt_res_t * p = malloc(sizeof(lv_sqrt_res_t)) ;
    memset(p, 0, sizeof(lv_sqrt_res_t)) ;
    return p ;
}
// fields getter
EMSCRIPTEN_KEEPALIVE uint16_t lv_sqrt_res_get_i(lv_sqrt_res_t * p) { return p->i ; }
EMSCRIPTEN_KEEPALIVE uint16_t lv_sqrt_res_get_f(lv_sqrt_res_t * p) { return p->f ; }
// fields setter
EMSCRIPTEN_KEEPALIVE void lv_sqrt_res_set_i(lv_sqrt_res_t * p, uint16_t v) { p->i = v ; }
EMSCRIPTEN_KEEPALIVE void lv_sqrt_res_set_f(lv_sqrt_res_t * p, uint16_t v) { p->f = v ; }


// struct lv_mem_monitor_t wasm api ----------
EMSCRIPTEN_KEEPALIVE lv_mem_monitor_t * lv_mem_monitor_create() {
    lv_mem_monitor_t * p = malloc(sizeof(lv_mem_monitor_t)) ;
    memset(p, 0, sizeof(lv_mem_monitor_t)) ;
    return p ;
}
// fields getter
EMSCRIPTEN_KEEPALIVE uint32_t lv_mem_monitor_get_total_size(lv_mem_monitor_t * p) { return p->total_size ; }
EMSCRIPTEN_KEEPALIVE uint32_t lv_mem_monitor_get_free_cnt(lv_mem_monitor_t * p) { return p->free_cnt ; }
EMSCRIPTEN_KEEPALIVE uint32_t lv_mem_monitor_get_free_size(lv_mem_monitor_t * p) { return p->free_size ; }
EMSCRIPTEN_KEEPALIVE uint32_t lv_mem_monitor_get_free_biggest_size(lv_mem_monitor_t * p) { return p->free_biggest_size ; }
EMSCRIPTEN_KEEPALIVE uint32_t lv_mem_monitor_get_used_cnt(lv_mem_monitor_t * p) { return p->used_cnt ; }
EMSCRIPTEN_KEEPALIVE uint32_t lv_mem_monitor_get_max_used(lv_mem_monitor_t * p) { return p->max_used ; }
EMSCRIPTEN_KEEPALIVE uint8_t lv_mem_monitor_get_used_pct(lv_mem_monitor_t * p) { return p->used_pct ; }
EMSCRIPTEN_KEEPALIVE uint8_t lv_mem_monitor_get_frag_pct(lv_mem_monitor_t * p) { return p->frag_pct ; }
// fields setter
EMSCRIPTEN_KEEPALIVE void lv_mem_monitor_set_total_size(lv_mem_monitor_t * p, uint32_t v) { p->total_size = v ; }
EMSCRIPTEN_KEEPALIVE void lv_mem_monitor_set_free_cnt(lv_mem_monitor_t * p, uint32_t v) { p->free_cnt = v ; }
EMSCRIPTEN_KEEPALIVE void lv_mem_monitor_set_free_size(lv_mem_monitor_t * p, uint32_t v) { p->free_size = v ; }
EMSCRIPTEN_KEEPALIVE void lv_mem_monitor_set_free_biggest_size(lv_mem_monitor_t * p, uint32_t v) { p->free_biggest_size = v ; }
EMSCRIPTEN_KEEPALIVE void lv_mem_monitor_set_used_cnt(lv_mem_monitor_t * p, uint32_t v) { p->used_cnt = v ; }
EMSCRIPTEN_KEEPALIVE void lv_mem_monitor_set_max_used(lv_mem_monitor_t * p, uint32_t v) { p->max_used = v ; }
EMSCRIPTEN_KEEPALIVE void lv_mem_monitor_set_used_pct(lv_mem_monitor_t * p, uint8_t v) { p->used_pct = v ; }
EMSCRIPTEN_KEEPALIVE void lv_mem_monitor_set_frag_pct(lv_mem_monitor_t * p, uint8_t v) { p->frag_pct = v ; }


// struct lv_mem_buf_t wasm api ----------
EMSCRIPTEN_KEEPALIVE lv_mem_buf_t * lv_mem_buf_create() {
    lv_mem_buf_t * p = malloc(sizeof(lv_mem_buf_t)) ;
    memset(p, 0, sizeof(lv_mem_buf_t)) ;
    return p ;
}
// fields getter
EMSCRIPTEN_KEEPALIVE void * lv_mem_buf_get_p(lv_mem_buf_t * p) { return p->p ; }
EMSCRIPTEN_KEEPALIVE uint16_t lv_mem_buf_get_size(lv_mem_buf_t * p) { return p->size ; }
EMSCRIPTEN_KEEPALIVE uint8_t lv_mem_buf_get_used(lv_mem_buf_t * p) { return p->used ; }
// fields setter
EMSCRIPTEN_KEEPALIVE void lv_mem_buf_set_p(lv_mem_buf_t * p, void * v) { p->p = v ; }
EMSCRIPTEN_KEEPALIVE void lv_mem_buf_set_size(lv_mem_buf_t * p, uint16_t v) { p->size = v ; }
EMSCRIPTEN_KEEPALIVE void lv_mem_buf_set_used(lv_mem_buf_t * p, uint8_t v) { p->used = v ; }


// struct lv_anim_t wasm api ----------
EMSCRIPTEN_KEEPALIVE lv_anim_t * lv_anim_create() {
    lv_anim_t * p = malloc(sizeof(lv_anim_t)) ;
    memset(p, 0, sizeof(lv_anim_t)) ;
    return p ;
}
// fields getter
EMSCRIPTEN_KEEPALIVE void * lv_anim_get_var(lv_anim_t * p) { return p->var ; }
EMSCRIPTEN_KEEPALIVE lv_anim_exec_xcb_t lv_anim_get_exec_cb(lv_anim_t * p) { return p->exec_cb ; }
EMSCRIPTEN_KEEPALIVE lv_anim_start_cb_t lv_anim_get_start_cb(lv_anim_t * p) { return p->start_cb ; }
EMSCRIPTEN_KEEPALIVE lv_anim_ready_cb_t lv_anim_get_ready_cb(lv_anim_t * p) { return p->ready_cb ; }
EMSCRIPTEN_KEEPALIVE lv_anim_get_value_cb_t lv_anim_get_get_value_cb(lv_anim_t * p) { return p->get_value_cb ; }
EMSCRIPTEN_KEEPALIVE lv_anim_path_cb_t lv_anim_get_path_cb(lv_anim_t * p) { return p->path_cb ; }
EMSCRIPTEN_KEEPALIVE int32_t lv_anim_get_start_value(lv_anim_t * p) { return p->start_value ; }
EMSCRIPTEN_KEEPALIVE int32_t lv_anim_get_current_value(lv_anim_t * p) { return p->current_value ; }
EMSCRIPTEN_KEEPALIVE int32_t lv_anim_get_end_value(lv_anim_t * p) { return p->end_value ; }
EMSCRIPTEN_KEEPALIVE int32_t lv_anim_get_time(lv_anim_t * p) { return p->time ; }
EMSCRIPTEN_KEEPALIVE int32_t lv_anim_get_act_time(lv_anim_t * p) { return p->act_time ; }
EMSCRIPTEN_KEEPALIVE uint32_t lv_anim_get_playback_delay(lv_anim_t * p) { return p->playback_delay ; }
EMSCRIPTEN_KEEPALIVE uint32_t lv_anim_get_playback_time(lv_anim_t * p) { return p->playback_time ; }
EMSCRIPTEN_KEEPALIVE uint32_t lv_anim_get_repeat_delay(lv_anim_t * p) { return p->repeat_delay ; }
EMSCRIPTEN_KEEPALIVE uint16_t lv_anim_get_repeat_cnt(lv_anim_t * p) { return p->repeat_cnt ; }
EMSCRIPTEN_KEEPALIVE uint8_t lv_anim_get_early_apply(lv_anim_t * p) { return p->early_apply ; }
EMSCRIPTEN_KEEPALIVE uint8_t lv_anim_get_playback_now(lv_anim_t * p) { return p->playback_now ; }
EMSCRIPTEN_KEEPALIVE uint8_t lv_anim_get_run_round(lv_anim_t * p) { return p->run_round ; }
EMSCRIPTEN_KEEPALIVE uint8_t lv_anim_get_start_cb_called(lv_anim_t * p) { return p->start_cb_called ; }
// fields setter
EMSCRIPTEN_KEEPALIVE void lv_anim_set_start_value(lv_anim_t * p, int32_t v) { p->start_value = v ; }
EMSCRIPTEN_KEEPALIVE void lv_anim_set_current_value(lv_anim_t * p, int32_t v) { p->current_value = v ; }
EMSCRIPTEN_KEEPALIVE void lv_anim_set_end_value(lv_anim_t * p, int32_t v) { p->end_value = v ; }
EMSCRIPTEN_KEEPALIVE void lv_anim_set_act_time(lv_anim_t * p, int32_t v) { p->act_time = v ; }
EMSCRIPTEN_KEEPALIVE void lv_anim_set_repeat_cnt(lv_anim_t * p, uint16_t v) { p->repeat_cnt = v ; }
EMSCRIPTEN_KEEPALIVE void lv_anim_set_playback_now(lv_anim_t * p, uint8_t v) { p->playback_now = v ; }
EMSCRIPTEN_KEEPALIVE void lv_anim_set_run_round(lv_anim_t * p, uint8_t v) { p->run_round = v ; }
EMSCRIPTEN_KEEPALIVE void lv_anim_set_start_cb_called(lv_anim_t * p, uint8_t v) { p->start_cb_called = v ; }


// struct lv_vaformat_t wasm api ----------
EMSCRIPTEN_KEEPALIVE lv_vaformat_t * lv_vaformat_create() {
    lv_vaformat_t * p = malloc(sizeof(lv_vaformat_t)) ;
    memset(p, 0, sizeof(lv_vaformat_t)) ;
    return p ;
}
// fields getter
EMSCRIPTEN_KEEPALIVE const char * lv_vaformat_get_fmt(lv_vaformat_t * p) { return p->fmt ; }
EMSCRIPTEN_KEEPALIVE va_list * lv_vaformat_get_va(lv_vaformat_t * p) { return p->va ; }
// fields setter
EMSCRIPTEN_KEEPALIVE void lv_vaformat_set_fmt(lv_vaformat_t * p, const char * v) { p->fmt = v ; }
EMSCRIPTEN_KEEPALIVE void lv_vaformat_set_va(lv_vaformat_t * p, va_list * v) { p->va = v ; }


// struct lv_color_hsv_t wasm api ----------
EMSCRIPTEN_KEEPALIVE lv_color_hsv_t * lv_color_hsv_create() {
    lv_color_hsv_t * p = malloc(sizeof(lv_color_hsv_t)) ;
    memset(p, 0, sizeof(lv_color_hsv_t)) ;
    return p ;
}
// fields getter
EMSCRIPTEN_KEEPALIVE uint16_t lv_color_hsv_get_h(lv_color_hsv_t * p) { return p->h ; }
EMSCRIPTEN_KEEPALIVE uint8_t lv_color_hsv_get_s(lv_color_hsv_t * p) { return p->s ; }
EMSCRIPTEN_KEEPALIVE uint8_t lv_color_hsv_get_v(lv_color_hsv_t * p) { return p->v ; }
// fields setter
EMSCRIPTEN_KEEPALIVE void lv_color_hsv_set_h(lv_color_hsv_t * p, uint16_t v) { p->h = v ; }
EMSCRIPTEN_KEEPALIVE void lv_color_hsv_set_s(lv_color_hsv_t * p, uint8_t v) { p->s = v ; }
EMSCRIPTEN_KEEPALIVE void lv_color_hsv_set_v(lv_color_hsv_t * p, uint8_t v) { p->v = v ; }


// struct lv_color_filter_dsc_t wasm api ----------
EMSCRIPTEN_KEEPALIVE lv_color_filter_dsc_t * lv_color_filter_dsc_create() {
    lv_color_filter_dsc_t * p = malloc(sizeof(lv_color_filter_dsc_t)) ;
    memset(p, 0, sizeof(lv_color_filter_dsc_t)) ;
    return p ;
}
// fields getter
EMSCRIPTEN_KEEPALIVE lv_color_filter_cb_t lv_color_filter_dsc_get_filter_cb(lv_color_filter_dsc_t * p) { return p->filter_cb ; }
EMSCRIPTEN_KEEPALIVE void * lv_color_filter_dsc_get_user_data(lv_color_filter_dsc_t * p) { return p->user_data ; }
// fields setter
EMSCRIPTEN_KEEPALIVE void lv_color_filter_dsc_set_filter_cb(lv_color_filter_dsc_t * p, lv_color_filter_cb_t v) { p->filter_cb = v ; }
EMSCRIPTEN_KEEPALIVE void lv_color_filter_dsc_set_user_data(lv_color_filter_dsc_t * p, void * v) { p->user_data = v ; }


// struct lv_point_t wasm api ----------
EMSCRIPTEN_KEEPALIVE lv_point_t * lv_point_create() {
    lv_point_t * p = malloc(sizeof(lv_point_t)) ;
    memset(p, 0, sizeof(lv_point_t)) ;
    return p ;
}
// fields getter
EMSCRIPTEN_KEEPALIVE lv_coord_t lv_point_get_x(lv_point_t * p) { return p->x ; }
EMSCRIPTEN_KEEPALIVE lv_coord_t lv_point_get_y(lv_point_t * p) { return p->y ; }
// fields setter
EMSCRIPTEN_KEEPALIVE void lv_point_set_x(lv_point_t * p, lv_coord_t v) { p->x = v ; }
EMSCRIPTEN_KEEPALIVE void lv_point_set_y(lv_point_t * p, lv_coord_t v) { p->y = v ; }


// struct lv_area_t wasm api ----------
EMSCRIPTEN_KEEPALIVE lv_area_t * lv_area_create() {
    lv_area_t * p = malloc(sizeof(lv_area_t)) ;
    memset(p, 0, sizeof(lv_area_t)) ;
    return p ;
}
// fields getter
EMSCRIPTEN_KEEPALIVE lv_coord_t lv_area_get_x1(lv_area_t * p) { return p->x1 ; }
EMSCRIPTEN_KEEPALIVE lv_coord_t lv_area_get_y1(lv_area_t * p) { return p->y1 ; }
EMSCRIPTEN_KEEPALIVE lv_coord_t lv_area_get_x2(lv_area_t * p) { return p->x2 ; }
EMSCRIPTEN_KEEPALIVE lv_coord_t lv_area_get_y2(lv_area_t * p) { return p->y2 ; }
// fields setter
EMSCRIPTEN_KEEPALIVE void lv_area_set_x1(lv_area_t * p, lv_coord_t v) { p->x1 = v ; }
EMSCRIPTEN_KEEPALIVE void lv_area_set_y1(lv_area_t * p, lv_coord_t v) { p->y1 = v ; }
EMSCRIPTEN_KEEPALIVE void lv_area_set_x2(lv_area_t * p, lv_coord_t v) { p->x2 = v ; }
EMSCRIPTEN_KEEPALIVE void lv_area_set_y2(lv_area_t * p, lv_coord_t v) { p->y2 = v ; }


// struct lv_img_header_t wasm api ----------
EMSCRIPTEN_KEEPALIVE lv_img_header_t * lv_img_header_create() {
    lv_img_header_t * p = malloc(sizeof(lv_img_header_t)) ;
    memset(p, 0, sizeof(lv_img_header_t)) ;
    return p ;
}
// fields getter
EMSCRIPTEN_KEEPALIVE uint32_t lv_img_header_get_cf(lv_img_header_t * p) { return p->cf ; }
EMSCRIPTEN_KEEPALIVE uint32_t lv_img_header_get_always_zero(lv_img_header_t * p) { return p->always_zero ; }
EMSCRIPTEN_KEEPALIVE uint32_t lv_img_header_get_reserved(lv_img_header_t * p) { return p->reserved ; }
EMSCRIPTEN_KEEPALIVE uint32_t lv_img_header_get_w(lv_img_header_t * p) { return p->w ; }
EMSCRIPTEN_KEEPALIVE uint32_t lv_img_header_get_h(lv_img_header_t * p) { return p->h ; }
// fields setter
EMSCRIPTEN_KEEPALIVE void lv_img_header_set_cf(lv_img_header_t * p, uint32_t v) { p->cf = v ; }
EMSCRIPTEN_KEEPALIVE void lv_img_header_set_always_zero(lv_img_header_t * p, uint32_t v) { p->always_zero = v ; }
EMSCRIPTEN_KEEPALIVE void lv_img_header_set_reserved(lv_img_header_t * p, uint32_t v) { p->reserved = v ; }
EMSCRIPTEN_KEEPALIVE void lv_img_header_set_w(lv_img_header_t * p, uint32_t v) { p->w = v ; }
EMSCRIPTEN_KEEPALIVE void lv_img_header_set_h(lv_img_header_t * p, uint32_t v) { p->h = v ; }


// struct lv_img_dsc_t wasm api ----------
EMSCRIPTEN_KEEPALIVE lv_img_dsc_t * lv_img_dsc_create() {
    lv_img_dsc_t * p = malloc(sizeof(lv_img_dsc_t)) ;
    memset(p, 0, sizeof(lv_img_dsc_t)) ;
    return p ;
}
// fields getter
EMSCRIPTEN_KEEPALIVE lv_img_header_t lv_img_dsc_get_header(lv_img_dsc_t * p) { return p->header ; }
EMSCRIPTEN_KEEPALIVE uint32_t lv_img_dsc_get_data_size(lv_img_dsc_t * p) { return p->data_size ; }
EMSCRIPTEN_KEEPALIVE const uint8_t * lv_img_dsc_get_data(lv_img_dsc_t * p) { return p->data ; }
// fields setter
EMSCRIPTEN_KEEPALIVE void lv_img_dsc_set_header(lv_img_dsc_t * p, lv_img_header_t v) { p->header = v ; }
EMSCRIPTEN_KEEPALIVE void lv_img_dsc_set_data_size(lv_img_dsc_t * p, uint32_t v) { p->data_size = v ; }
EMSCRIPTEN_KEEPALIVE void lv_img_dsc_set_data(lv_img_dsc_t * p, const uint8_t * v) { p->data = v ; }


// struct lv_img_transform_dsc_t wasm api ----------
EMSCRIPTEN_KEEPALIVE lv_img_transform_dsc_t * lv_img_transform_dsc_create() {
    lv_img_transform_dsc_t * p = malloc(sizeof(lv_img_transform_dsc_t)) ;
    memset(p, 0, sizeof(lv_img_transform_dsc_t)) ;
    return p ;
}


// struct lv_ll_t wasm api ----------
EMSCRIPTEN_KEEPALIVE lv_ll_t * lv_ll_create() {
    lv_ll_t * p = malloc(sizeof(lv_ll_t)) ;
    memset(p, 0, sizeof(lv_ll_t)) ;
    return p ;
}
// fields getter
EMSCRIPTEN_KEEPALIVE uint32_t lv_ll_get_n_size(lv_ll_t * p) { return p->n_size ; }
EMSCRIPTEN_KEEPALIVE lv_ll_node_t * lv_ll_get_head(lv_ll_t * p) { return p->head ; }
EMSCRIPTEN_KEEPALIVE lv_ll_node_t * lv_ll_get_tail(lv_ll_t * p) { return p->tail ; }
// fields setter
EMSCRIPTEN_KEEPALIVE void lv_ll_set_n_size(lv_ll_t * p, uint32_t v) { p->n_size = v ; }
EMSCRIPTEN_KEEPALIVE void lv_ll_set_head(lv_ll_t * p, lv_ll_node_t * v) { p->head = v ; }
EMSCRIPTEN_KEEPALIVE void lv_ll_set_tail(lv_ll_t * p, lv_ll_node_t * v) { p->tail = v ; }


// struct lv_disp_draw_buf_t wasm api ----------
EMSCRIPTEN_KEEPALIVE lv_disp_draw_buf_t * lv_disp_draw_buf_create() {
    lv_disp_draw_buf_t * p = malloc(sizeof(lv_disp_draw_buf_t)) ;
    memset(p, 0, sizeof(lv_disp_draw_buf_t)) ;
    return p ;
}
// fields getter
EMSCRIPTEN_KEEPALIVE void * lv_disp_draw_buf_get_buf1(lv_disp_draw_buf_t * p) { return p->buf1 ; }
EMSCRIPTEN_KEEPALIVE void * lv_disp_draw_buf_get_buf2(lv_disp_draw_buf_t * p) { return p->buf2 ; }
EMSCRIPTEN_KEEPALIVE void * lv_disp_draw_buf_get_buf_act(lv_disp_draw_buf_t * p) { return p->buf_act ; }
EMSCRIPTEN_KEEPALIVE uint32_t lv_disp_draw_buf_get_size(lv_disp_draw_buf_t * p) { return p->size ; }
EMSCRIPTEN_KEEPALIVE lv_area_t lv_disp_draw_buf_get_area(lv_disp_draw_buf_t * p) { return p->area ; }
EMSCRIPTEN_KEEPALIVE volatile int lv_disp_draw_buf_get_flushing(lv_disp_draw_buf_t * p) { return p->flushing ; }
EMSCRIPTEN_KEEPALIVE volatile int lv_disp_draw_buf_get_flushing_last(lv_disp_draw_buf_t * p) { return p->flushing_last ; }
EMSCRIPTEN_KEEPALIVE volatile uint32_t lv_disp_draw_buf_get_last_area(lv_disp_draw_buf_t * p) { return p->last_area ; }
EMSCRIPTEN_KEEPALIVE volatile uint32_t lv_disp_draw_buf_get_last_part(lv_disp_draw_buf_t * p) { return p->last_part ; }
// fields setter
EMSCRIPTEN_KEEPALIVE void lv_disp_draw_buf_set_buf1(lv_disp_draw_buf_t * p, void * v) { p->buf1 = v ; }
EMSCRIPTEN_KEEPALIVE void lv_disp_draw_buf_set_buf2(lv_disp_draw_buf_t * p, void * v) { p->buf2 = v ; }
EMSCRIPTEN_KEEPALIVE void lv_disp_draw_buf_set_buf_act(lv_disp_draw_buf_t * p, void * v) { p->buf_act = v ; }
EMSCRIPTEN_KEEPALIVE void lv_disp_draw_buf_set_size(lv_disp_draw_buf_t * p, uint32_t v) { p->size = v ; }
EMSCRIPTEN_KEEPALIVE void lv_disp_draw_buf_set_area(lv_disp_draw_buf_t * p, lv_area_t v) { p->area = v ; }
EMSCRIPTEN_KEEPALIVE void lv_disp_draw_buf_set_flushing(lv_disp_draw_buf_t * p, volatile int v) { p->flushing = v ; }
EMSCRIPTEN_KEEPALIVE void lv_disp_draw_buf_set_flushing_last(lv_disp_draw_buf_t * p, volatile int v) { p->flushing_last = v ; }
EMSCRIPTEN_KEEPALIVE void lv_disp_draw_buf_set_last_area(lv_disp_draw_buf_t * p, volatile uint32_t v) { p->last_area = v ; }
EMSCRIPTEN_KEEPALIVE void lv_disp_draw_buf_set_last_part(lv_disp_draw_buf_t * p, volatile uint32_t v) { p->last_part = v ; }


// struct lv_disp_drv_t wasm api ----------
EMSCRIPTEN_KEEPALIVE lv_disp_drv_t * lv_disp_drv_create() {
    lv_disp_drv_t * p = malloc(sizeof(lv_disp_drv_t)) ;
    memset(p, 0, sizeof(lv_disp_drv_t)) ;
    return p ;
}
// fields getter
EMSCRIPTEN_KEEPALIVE lv_coord_t lv_disp_drv_get_hor_res(lv_disp_drv_t * p) { return p->hor_res ; }
EMSCRIPTEN_KEEPALIVE lv_coord_t lv_disp_drv_get_ver_res(lv_disp_drv_t * p) { return p->ver_res ; }
EMSCRIPTEN_KEEPALIVE lv_coord_t lv_disp_drv_get_physical_hor_res(lv_disp_drv_t * p) { return p->physical_hor_res ; }
EMSCRIPTEN_KEEPALIVE lv_coord_t lv_disp_drv_get_physical_ver_res(lv_disp_drv_t * p) { return p->physical_ver_res ; }
EMSCRIPTEN_KEEPALIVE lv_coord_t lv_disp_drv_get_offset_x(lv_disp_drv_t * p) { return p->offset_x ; }
EMSCRIPTEN_KEEPALIVE lv_coord_t lv_disp_drv_get_offset_y(lv_disp_drv_t * p) { return p->offset_y ; }
EMSCRIPTEN_KEEPALIVE lv_disp_draw_buf_t * lv_disp_drv_get_draw_buf(lv_disp_drv_t * p) { return p->draw_buf ; }
EMSCRIPTEN_KEEPALIVE uint32_t lv_disp_drv_get_direct_mode(lv_disp_drv_t * p) { return p->direct_mode ; }
EMSCRIPTEN_KEEPALIVE uint32_t lv_disp_drv_get_full_refresh(lv_disp_drv_t * p) { return p->full_refresh ; }
EMSCRIPTEN_KEEPALIVE uint32_t lv_disp_drv_get_sw_rotate(lv_disp_drv_t * p) { return p->sw_rotate ; }
EMSCRIPTEN_KEEPALIVE uint32_t lv_disp_drv_get_antialiasing(lv_disp_drv_t * p) { return p->antialiasing ; }
EMSCRIPTEN_KEEPALIVE uint32_t lv_disp_drv_get_rotated(lv_disp_drv_t * p) { return p->rotated ; }
EMSCRIPTEN_KEEPALIVE uint32_t lv_disp_drv_get_screen_transp(lv_disp_drv_t * p) { return p->screen_transp ; }
EMSCRIPTEN_KEEPALIVE uint32_t lv_disp_drv_get_dpi(lv_disp_drv_t * p) { return p->dpi ; }
EMSCRIPTEN_KEEPALIVE void * lv_disp_drv_get_flush_cb(lv_disp_drv_t * p) { return p->flush_cb ; }
EMSCRIPTEN_KEEPALIVE void * lv_disp_drv_get_rounder_cb(lv_disp_drv_t * p) { return p->rounder_cb ; }
EMSCRIPTEN_KEEPALIVE void * lv_disp_drv_get_set_px_cb(lv_disp_drv_t * p) { return p->set_px_cb ; }
EMSCRIPTEN_KEEPALIVE void * lv_disp_drv_get_monitor_cb(lv_disp_drv_t * p) { return p->monitor_cb ; }
EMSCRIPTEN_KEEPALIVE void * lv_disp_drv_get_wait_cb(lv_disp_drv_t * p) { return p->wait_cb ; }
EMSCRIPTEN_KEEPALIVE void * lv_disp_drv_get_clean_dcache_cb(lv_disp_drv_t * p) { return p->clean_dcache_cb ; }
EMSCRIPTEN_KEEPALIVE void * lv_disp_drv_get_gpu_wait_cb(lv_disp_drv_t * p) { return p->gpu_wait_cb ; }
EMSCRIPTEN_KEEPALIVE void * lv_disp_drv_get_drv_update_cb(lv_disp_drv_t * p) { return p->drv_update_cb ; }
EMSCRIPTEN_KEEPALIVE void * lv_disp_drv_get_gpu_fill_cb(lv_disp_drv_t * p) { return p->gpu_fill_cb ; }
EMSCRIPTEN_KEEPALIVE lv_color_t lv_disp_drv_get_color_chroma_key(lv_disp_drv_t * p) { return p->color_chroma_key ; }
EMSCRIPTEN_KEEPALIVE void * lv_disp_drv_get_user_data(lv_disp_drv_t * p) { return p->user_data ; }
// fields setter
EMSCRIPTEN_KEEPALIVE void lv_disp_drv_set_hor_res(lv_disp_drv_t * p, lv_coord_t v) { p->hor_res = v ; }
EMSCRIPTEN_KEEPALIVE void lv_disp_drv_set_ver_res(lv_disp_drv_t * p, lv_coord_t v) { p->ver_res = v ; }
EMSCRIPTEN_KEEPALIVE void lv_disp_drv_set_physical_hor_res(lv_disp_drv_t * p, lv_coord_t v) { p->physical_hor_res = v ; }
EMSCRIPTEN_KEEPALIVE void lv_disp_drv_set_physical_ver_res(lv_disp_drv_t * p, lv_coord_t v) { p->physical_ver_res = v ; }
EMSCRIPTEN_KEEPALIVE void lv_disp_drv_set_offset_x(lv_disp_drv_t * p, lv_coord_t v) { p->offset_x = v ; }
EMSCRIPTEN_KEEPALIVE void lv_disp_drv_set_offset_y(lv_disp_drv_t * p, lv_coord_t v) { p->offset_y = v ; }
EMSCRIPTEN_KEEPALIVE void lv_disp_drv_set_draw_buf(lv_disp_drv_t * p, lv_disp_draw_buf_t * v) { p->draw_buf = v ; }
EMSCRIPTEN_KEEPALIVE void lv_disp_drv_set_direct_mode(lv_disp_drv_t * p, uint32_t v) { p->direct_mode = v ; }
EMSCRIPTEN_KEEPALIVE void lv_disp_drv_set_full_refresh(lv_disp_drv_t * p, uint32_t v) { p->full_refresh = v ; }
EMSCRIPTEN_KEEPALIVE void lv_disp_drv_set_sw_rotate(lv_disp_drv_t * p, uint32_t v) { p->sw_rotate = v ; }
EMSCRIPTEN_KEEPALIVE void lv_disp_drv_set_antialiasing(lv_disp_drv_t * p, uint32_t v) { p->antialiasing = v ; }
EMSCRIPTEN_KEEPALIVE void lv_disp_drv_set_rotated(lv_disp_drv_t * p, uint32_t v) { p->rotated = v ; }
EMSCRIPTEN_KEEPALIVE void lv_disp_drv_set_screen_transp(lv_disp_drv_t * p, uint32_t v) { p->screen_transp = v ; }
EMSCRIPTEN_KEEPALIVE void lv_disp_drv_set_dpi(lv_disp_drv_t * p, uint32_t v) { p->dpi = v ; }
EMSCRIPTEN_KEEPALIVE void lv_disp_drv_set_flush_cb(lv_disp_drv_t * p, void * v) { p->flush_cb = v ; }
EMSCRIPTEN_KEEPALIVE void lv_disp_drv_set_rounder_cb(lv_disp_drv_t * p, void * v) { p->rounder_cb = v ; }
EMSCRIPTEN_KEEPALIVE void lv_disp_drv_set_set_px_cb(lv_disp_drv_t * p, void * v) { p->set_px_cb = v ; }
EMSCRIPTEN_KEEPALIVE void lv_disp_drv_set_monitor_cb(lv_disp_drv_t * p, void * v) { p->monitor_cb = v ; }
EMSCRIPTEN_KEEPALIVE void lv_disp_drv_set_wait_cb(lv_disp_drv_t * p, void * v) { p->wait_cb = v ; }
EMSCRIPTEN_KEEPALIVE void lv_disp_drv_set_clean_dcache_cb(lv_disp_drv_t * p, void * v) { p->clean_dcache_cb = v ; }
EMSCRIPTEN_KEEPALIVE void lv_disp_drv_set_gpu_wait_cb(lv_disp_drv_t * p, void * v) { p->gpu_wait_cb = v ; }
EMSCRIPTEN_KEEPALIVE void lv_disp_drv_set_drv_update_cb(lv_disp_drv_t * p, void * v) { p->drv_update_cb = v ; }
EMSCRIPTEN_KEEPALIVE void lv_disp_drv_set_gpu_fill_cb(lv_disp_drv_t * p, void * v) { p->gpu_fill_cb = v ; }
EMSCRIPTEN_KEEPALIVE void lv_disp_drv_set_color_chroma_key(lv_disp_drv_t * p, lv_color_t v) { p->color_chroma_key = v ; }
EMSCRIPTEN_KEEPALIVE void lv_disp_drv_set_user_data(lv_disp_drv_t * p, void * v) { p->user_data = v ; }


// struct lv_disp_t wasm api ----------
EMSCRIPTEN_KEEPALIVE lv_disp_t * lv_disp_create() {
    lv_disp_t * p = malloc(sizeof(lv_disp_t)) ;
    memset(p, 0, sizeof(lv_disp_t)) ;
    return p ;
}
// fields getter
EMSCRIPTEN_KEEPALIVE struct _lv_disp_drv_t * lv_disp_get_driver(lv_disp_t * p) { return p->driver ; }
EMSCRIPTEN_KEEPALIVE lv_timer_t * lv_disp_get_refr_timer(lv_disp_t * p) { return p->refr_timer ; }
EMSCRIPTEN_KEEPALIVE struct _lv_obj_t ** lv_disp_get_screens(lv_disp_t * p) { return p->screens ; }
EMSCRIPTEN_KEEPALIVE struct _lv_obj_t * lv_disp_get_act_scr(lv_disp_t * p) { return p->act_scr ; }
EMSCRIPTEN_KEEPALIVE struct _lv_obj_t * lv_disp_get_prev_scr(lv_disp_t * p) { return p->prev_scr ; }
EMSCRIPTEN_KEEPALIVE struct _lv_obj_t * lv_disp_get_scr_to_load(lv_disp_t * p) { return p->scr_to_load ; }
EMSCRIPTEN_KEEPALIVE struct _lv_obj_t * lv_disp_get_top_layer(lv_disp_t * p) { return p->top_layer ; }
EMSCRIPTEN_KEEPALIVE struct _lv_obj_t * lv_disp_get_sys_layer(lv_disp_t * p) { return p->sys_layer ; }
EMSCRIPTEN_KEEPALIVE uint32_t lv_disp_get_screen_cnt(lv_disp_t * p) { return p->screen_cnt ; }
EMSCRIPTEN_KEEPALIVE uint8_t lv_disp_get_del_prev(lv_disp_t * p) { return p->del_prev ; }
EMSCRIPTEN_KEEPALIVE lv_opa_t lv_disp_get_bg_opa(lv_disp_t * p) { return p->bg_opa ; }
EMSCRIPTEN_KEEPALIVE lv_color_t lv_disp_get_bg_color(lv_disp_t * p) { return p->bg_color ; }
EMSCRIPTEN_KEEPALIVE const void * lv_disp_get_bg_img(lv_disp_t * p) { return p->bg_img ; }
EMSCRIPTEN_KEEPALIVE void * lv_disp_get_bg_fn(lv_disp_t * p) { return p->bg_fn ; }
EMSCRIPTEN_KEEPALIVE lv_area_t * lv_disp_get_inv_areas(lv_disp_t * p) { return p->inv_areas ; }
EMSCRIPTEN_KEEPALIVE uint8_t * lv_disp_get_inv_area_joined(lv_disp_t * p) { return p->inv_area_joined ; }
EMSCRIPTEN_KEEPALIVE uint16_t lv_disp_get_inv_p(lv_disp_t * p) { return p->inv_p ; }
EMSCRIPTEN_KEEPALIVE uint32_t lv_disp_get_last_activity_time(lv_disp_t * p) { return p->last_activity_time ; }
// fields setter
EMSCRIPTEN_KEEPALIVE void lv_disp_set_driver(lv_disp_t * p, struct _lv_disp_drv_t * v) { p->driver = v ; }
EMSCRIPTEN_KEEPALIVE void lv_disp_set_refr_timer(lv_disp_t * p, lv_timer_t * v) { p->refr_timer = v ; }
EMSCRIPTEN_KEEPALIVE void lv_disp_set_screens(lv_disp_t * p, struct _lv_obj_t ** v) { p->screens = v ; }
EMSCRIPTEN_KEEPALIVE void lv_disp_set_act_scr(lv_disp_t * p, struct _lv_obj_t * v) { p->act_scr = v ; }
EMSCRIPTEN_KEEPALIVE void lv_disp_set_prev_scr(lv_disp_t * p, struct _lv_obj_t * v) { p->prev_scr = v ; }
EMSCRIPTEN_KEEPALIVE void lv_disp_set_scr_to_load(lv_disp_t * p, struct _lv_obj_t * v) { p->scr_to_load = v ; }
EMSCRIPTEN_KEEPALIVE void lv_disp_set_top_layer(lv_disp_t * p, struct _lv_obj_t * v) { p->top_layer = v ; }
EMSCRIPTEN_KEEPALIVE void lv_disp_set_sys_layer(lv_disp_t * p, struct _lv_obj_t * v) { p->sys_layer = v ; }
EMSCRIPTEN_KEEPALIVE void lv_disp_set_screen_cnt(lv_disp_t * p, uint32_t v) { p->screen_cnt = v ; }
EMSCRIPTEN_KEEPALIVE void lv_disp_set_del_prev(lv_disp_t * p, uint8_t v) { p->del_prev = v ; }
EMSCRIPTEN_KEEPALIVE void lv_disp_set_bg_img(lv_disp_t * p, const void * v) { p->bg_img = v ; }
EMSCRIPTEN_KEEPALIVE void lv_disp_set_bg_fn(lv_disp_t * p, void * v) { p->bg_fn = v ; }
EMSCRIPTEN_KEEPALIVE void lv_disp_set_inv_p(lv_disp_t * p, uint16_t v) { p->inv_p = v ; }
EMSCRIPTEN_KEEPALIVE void lv_disp_set_last_activity_time(lv_disp_t * p, uint32_t v) { p->last_activity_time = v ; }


// struct lv_indev_data_t wasm api ----------
EMSCRIPTEN_KEEPALIVE lv_indev_data_t * lv_indev_data_create() {
    lv_indev_data_t * p = malloc(sizeof(lv_indev_data_t)) ;
    memset(p, 0, sizeof(lv_indev_data_t)) ;
    return p ;
}
// fields getter
EMSCRIPTEN_KEEPALIVE lv_point_t lv_indev_data_get_point(lv_indev_data_t * p) { return p->point ; }
EMSCRIPTEN_KEEPALIVE uint32_t lv_indev_data_get_key(lv_indev_data_t * p) { return p->key ; }
EMSCRIPTEN_KEEPALIVE uint32_t lv_indev_data_get_btn_id(lv_indev_data_t * p) { return p->btn_id ; }
EMSCRIPTEN_KEEPALIVE int16_t lv_indev_data_get_enc_diff(lv_indev_data_t * p) { return p->enc_diff ; }
EMSCRIPTEN_KEEPALIVE lv_indev_state_t lv_indev_data_get_state(lv_indev_data_t * p) { return p->state ; }
EMSCRIPTEN_KEEPALIVE bool lv_indev_data_get_continue_reading(lv_indev_data_t * p) { return p->continue_reading ; }
// fields setter
EMSCRIPTEN_KEEPALIVE void lv_indev_data_set_point(lv_indev_data_t * p, lv_point_t v) { p->point = v ; }
EMSCRIPTEN_KEEPALIVE void lv_indev_data_set_key(lv_indev_data_t * p, uint32_t v) { p->key = v ; }
EMSCRIPTEN_KEEPALIVE void lv_indev_data_set_btn_id(lv_indev_data_t * p, uint32_t v) { p->btn_id = v ; }
EMSCRIPTEN_KEEPALIVE void lv_indev_data_set_enc_diff(lv_indev_data_t * p, int16_t v) { p->enc_diff = v ; }
EMSCRIPTEN_KEEPALIVE void lv_indev_data_set_state(lv_indev_data_t * p, lv_indev_state_t v) { p->state = v ; }
EMSCRIPTEN_KEEPALIVE void lv_indev_data_set_continue_reading(lv_indev_data_t * p, bool v) { p->continue_reading = v ; }


// struct lv_indev_drv_t wasm api ----------
EMSCRIPTEN_KEEPALIVE lv_indev_drv_t * lv_indev_drv_create() {
    lv_indev_drv_t * p = malloc(sizeof(lv_indev_drv_t)) ;
    memset(p, 0, sizeof(lv_indev_drv_t)) ;
    return p ;
}
// fields getter
EMSCRIPTEN_KEEPALIVE lv_indev_type_t lv_indev_drv_get_type(lv_indev_drv_t * p) { return p->type ; }
EMSCRIPTEN_KEEPALIVE void * lv_indev_drv_get_read_cb(lv_indev_drv_t * p) { return p->read_cb ; }
EMSCRIPTEN_KEEPALIVE void * lv_indev_drv_get_feedback_cb(lv_indev_drv_t * p) { return p->feedback_cb ; }
EMSCRIPTEN_KEEPALIVE void * lv_indev_drv_get_user_data(lv_indev_drv_t * p) { return p->user_data ; }
EMSCRIPTEN_KEEPALIVE struct _lv_disp_t * lv_indev_drv_get_disp(lv_indev_drv_t * p) { return p->disp ; }
EMSCRIPTEN_KEEPALIVE lv_timer_t * lv_indev_drv_get_read_timer(lv_indev_drv_t * p) { return p->read_timer ; }
EMSCRIPTEN_KEEPALIVE uint8_t lv_indev_drv_get_scroll_limit(lv_indev_drv_t * p) { return p->scroll_limit ; }
EMSCRIPTEN_KEEPALIVE uint8_t lv_indev_drv_get_scroll_throw(lv_indev_drv_t * p) { return p->scroll_throw ; }
EMSCRIPTEN_KEEPALIVE uint8_t lv_indev_drv_get_gesture_min_velocity(lv_indev_drv_t * p) { return p->gesture_min_velocity ; }
EMSCRIPTEN_KEEPALIVE uint8_t lv_indev_drv_get_gesture_limit(lv_indev_drv_t * p) { return p->gesture_limit ; }
EMSCRIPTEN_KEEPALIVE uint16_t lv_indev_drv_get_long_press_time(lv_indev_drv_t * p) { return p->long_press_time ; }
EMSCRIPTEN_KEEPALIVE uint16_t lv_indev_drv_get_long_press_repeat_time(lv_indev_drv_t * p) { return p->long_press_repeat_time ; }
// fields setter
EMSCRIPTEN_KEEPALIVE void lv_indev_drv_set_type(lv_indev_drv_t * p, lv_indev_type_t v) { p->type = v ; }
EMSCRIPTEN_KEEPALIVE void lv_indev_drv_set_read_cb(lv_indev_drv_t * p, void * v) { p->read_cb = v ; }
EMSCRIPTEN_KEEPALIVE void lv_indev_drv_set_feedback_cb(lv_indev_drv_t * p, void * v) { p->feedback_cb = v ; }
EMSCRIPTEN_KEEPALIVE void lv_indev_drv_set_user_data(lv_indev_drv_t * p, void * v) { p->user_data = v ; }
EMSCRIPTEN_KEEPALIVE void lv_indev_drv_set_disp(lv_indev_drv_t * p, struct _lv_disp_t * v) { p->disp = v ; }
EMSCRIPTEN_KEEPALIVE void lv_indev_drv_set_read_timer(lv_indev_drv_t * p, lv_timer_t * v) { p->read_timer = v ; }
EMSCRIPTEN_KEEPALIVE void lv_indev_drv_set_scroll_limit(lv_indev_drv_t * p, uint8_t v) { p->scroll_limit = v ; }
EMSCRIPTEN_KEEPALIVE void lv_indev_drv_set_scroll_throw(lv_indev_drv_t * p, uint8_t v) { p->scroll_throw = v ; }
EMSCRIPTEN_KEEPALIVE void lv_indev_drv_set_gesture_min_velocity(lv_indev_drv_t * p, uint8_t v) { p->gesture_min_velocity = v ; }
EMSCRIPTEN_KEEPALIVE void lv_indev_drv_set_gesture_limit(lv_indev_drv_t * p, uint8_t v) { p->gesture_limit = v ; }
EMSCRIPTEN_KEEPALIVE void lv_indev_drv_set_long_press_time(lv_indev_drv_t * p, uint16_t v) { p->long_press_time = v ; }
EMSCRIPTEN_KEEPALIVE void lv_indev_drv_set_long_press_repeat_time(lv_indev_drv_t * p, uint16_t v) { p->long_press_repeat_time = v ; }


// struct _lv_indev_proc_t wasm api ----------
EMSCRIPTEN_KEEPALIVE _lv_indev_proc_t * _lv_indev_proc_create() {
    _lv_indev_proc_t * p = malloc(sizeof(_lv_indev_proc_t)) ;
    memset(p, 0, sizeof(_lv_indev_proc_t)) ;
    return p ;
}
// fields getter
EMSCRIPTEN_KEEPALIVE lv_indev_state_t _lv_indev_proc_get_state(_lv_indev_proc_t * p) { return p->state ; }
EMSCRIPTEN_KEEPALIVE uint8_t _lv_indev_proc_get_long_pr_sent(_lv_indev_proc_t * p) { return p->long_pr_sent ; }
EMSCRIPTEN_KEEPALIVE uint8_t _lv_indev_proc_get_reset_query(_lv_indev_proc_t * p) { return p->reset_query ; }
EMSCRIPTEN_KEEPALIVE uint8_t _lv_indev_proc_get_disabled(_lv_indev_proc_t * p) { return p->disabled ; }
EMSCRIPTEN_KEEPALIVE uint8_t _lv_indev_proc_get_wait_until_release(_lv_indev_proc_t * p) { return p->wait_until_release ; }
EMSCRIPTEN_KEEPALIVE uint32_t _lv_indev_proc_get_pr_timestamp(_lv_indev_proc_t * p) { return p->pr_timestamp ; }
EMSCRIPTEN_KEEPALIVE uint32_t _lv_indev_proc_get_longpr_rep_timestamp(_lv_indev_proc_t * p) { return p->longpr_rep_timestamp ; }
// fields setter
EMSCRIPTEN_KEEPALIVE void _lv_indev_proc_set_state(_lv_indev_proc_t * p, lv_indev_state_t v) { p->state = v ; }
EMSCRIPTEN_KEEPALIVE void _lv_indev_proc_set_long_pr_sent(_lv_indev_proc_t * p, uint8_t v) { p->long_pr_sent = v ; }
EMSCRIPTEN_KEEPALIVE void _lv_indev_proc_set_reset_query(_lv_indev_proc_t * p, uint8_t v) { p->reset_query = v ; }
EMSCRIPTEN_KEEPALIVE void _lv_indev_proc_set_disabled(_lv_indev_proc_t * p, uint8_t v) { p->disabled = v ; }
EMSCRIPTEN_KEEPALIVE void _lv_indev_proc_set_wait_until_release(_lv_indev_proc_t * p, uint8_t v) { p->wait_until_release = v ; }
EMSCRIPTEN_KEEPALIVE void _lv_indev_proc_set_pr_timestamp(_lv_indev_proc_t * p, uint32_t v) { p->pr_timestamp = v ; }
EMSCRIPTEN_KEEPALIVE void _lv_indev_proc_set_longpr_rep_timestamp(_lv_indev_proc_t * p, uint32_t v) { p->longpr_rep_timestamp = v ; }


// struct lv_indev_t wasm api ----------
EMSCRIPTEN_KEEPALIVE lv_indev_t * lv_indev_create() {
    lv_indev_t * p = malloc(sizeof(lv_indev_t)) ;
    memset(p, 0, sizeof(lv_indev_t)) ;
    return p ;
}
// fields getter
EMSCRIPTEN_KEEPALIVE struct _lv_indev_drv_t * lv_indev_get_driver(lv_indev_t * p) { return p->driver ; }
EMSCRIPTEN_KEEPALIVE _lv_indev_proc_t lv_indev_get_proc(lv_indev_t * p) { return p->proc ; }
EMSCRIPTEN_KEEPALIVE struct _lv_obj_t * lv_indev_get_cursor(lv_indev_t * p) { return p->cursor ; }
EMSCRIPTEN_KEEPALIVE struct _lv_group_t * lv_indev_get_group(lv_indev_t * p) { return p->group ; }
EMSCRIPTEN_KEEPALIVE const lv_point_t * lv_indev_get_btn_points(lv_indev_t * p) { return p->btn_points ; }
// fields setter
EMSCRIPTEN_KEEPALIVE void lv_indev_set_driver(lv_indev_t * p, struct _lv_indev_drv_t * v) { p->driver = v ; }
EMSCRIPTEN_KEEPALIVE void lv_indev_set_proc(lv_indev_t * p, _lv_indev_proc_t v) { p->proc = v ; }
EMSCRIPTEN_KEEPALIVE void lv_indev_set_btn_points(lv_indev_t * p, const lv_point_t * v) { p->btn_points = v ; }


// struct lv_font_glyph_dsc_t wasm api ----------
EMSCRIPTEN_KEEPALIVE lv_font_glyph_dsc_t * lv_font_glyph_dsc_create() {
    lv_font_glyph_dsc_t * p = malloc(sizeof(lv_font_glyph_dsc_t)) ;
    memset(p, 0, sizeof(lv_font_glyph_dsc_t)) ;
    return p ;
}
// fields getter
EMSCRIPTEN_KEEPALIVE uint16_t lv_font_glyph_dsc_get_adv_w(lv_font_glyph_dsc_t * p) { return p->adv_w ; }
EMSCRIPTEN_KEEPALIVE uint16_t lv_font_glyph_dsc_get_box_w(lv_font_glyph_dsc_t * p) { return p->box_w ; }
EMSCRIPTEN_KEEPALIVE uint16_t lv_font_glyph_dsc_get_box_h(lv_font_glyph_dsc_t * p) { return p->box_h ; }
EMSCRIPTEN_KEEPALIVE int16_t lv_font_glyph_dsc_get_ofs_x(lv_font_glyph_dsc_t * p) { return p->ofs_x ; }
EMSCRIPTEN_KEEPALIVE int16_t lv_font_glyph_dsc_get_ofs_y(lv_font_glyph_dsc_t * p) { return p->ofs_y ; }
EMSCRIPTEN_KEEPALIVE uint8_t lv_font_glyph_dsc_get_bpp(lv_font_glyph_dsc_t * p) { return p->bpp ; }
// fields setter
EMSCRIPTEN_KEEPALIVE void lv_font_glyph_dsc_set_adv_w(lv_font_glyph_dsc_t * p, uint16_t v) { p->adv_w = v ; }
EMSCRIPTEN_KEEPALIVE void lv_font_glyph_dsc_set_box_w(lv_font_glyph_dsc_t * p, uint16_t v) { p->box_w = v ; }
EMSCRIPTEN_KEEPALIVE void lv_font_glyph_dsc_set_box_h(lv_font_glyph_dsc_t * p, uint16_t v) { p->box_h = v ; }
EMSCRIPTEN_KEEPALIVE void lv_font_glyph_dsc_set_ofs_x(lv_font_glyph_dsc_t * p, int16_t v) { p->ofs_x = v ; }
EMSCRIPTEN_KEEPALIVE void lv_font_glyph_dsc_set_ofs_y(lv_font_glyph_dsc_t * p, int16_t v) { p->ofs_y = v ; }
EMSCRIPTEN_KEEPALIVE void lv_font_glyph_dsc_set_bpp(lv_font_glyph_dsc_t * p, uint8_t v) { p->bpp = v ; }


// struct lv_font_t wasm api ----------
EMSCRIPTEN_KEEPALIVE lv_font_t * lv_font_create() {
    lv_font_t * p = malloc(sizeof(lv_font_t)) ;
    memset(p, 0, sizeof(lv_font_t)) ;
    return p ;
}
// fields getter
EMSCRIPTEN_KEEPALIVE void * lv_font_get_get_glyph_dsc(lv_font_t * p) { return p->get_glyph_dsc ; }
EMSCRIPTEN_KEEPALIVE void * lv_font_get_get_glyph_bitmap(lv_font_t * p) { return p->get_glyph_bitmap ; }
EMSCRIPTEN_KEEPALIVE lv_coord_t lv_font_get_base_line(lv_font_t * p) { return p->base_line ; }
EMSCRIPTEN_KEEPALIVE uint8_t lv_font_get_subpx(lv_font_t * p) { return p->subpx ; }
EMSCRIPTEN_KEEPALIVE int8_t lv_font_get_underline_position(lv_font_t * p) { return p->underline_position ; }
EMSCRIPTEN_KEEPALIVE int8_t lv_font_get_underline_thickness(lv_font_t * p) { return p->underline_thickness ; }
EMSCRIPTEN_KEEPALIVE const void * lv_font_get_dsc(lv_font_t * p) { return p->dsc ; }
EMSCRIPTEN_KEEPALIVE void * lv_font_get_user_data(lv_font_t * p) { return p->user_data ; }
// fields setter
EMSCRIPTEN_KEEPALIVE void lv_font_set_get_glyph_dsc(lv_font_t * p, void * v) { p->get_glyph_dsc = v ; }
EMSCRIPTEN_KEEPALIVE void lv_font_set_get_glyph_bitmap(lv_font_t * p, void * v) { p->get_glyph_bitmap = v ; }
EMSCRIPTEN_KEEPALIVE void lv_font_set_line_height(lv_font_t * p, lv_coord_t v) { p->line_height = v ; }
EMSCRIPTEN_KEEPALIVE void lv_font_set_base_line(lv_font_t * p, lv_coord_t v) { p->base_line = v ; }
EMSCRIPTEN_KEEPALIVE void lv_font_set_subpx(lv_font_t * p, uint8_t v) { p->subpx = v ; }
EMSCRIPTEN_KEEPALIVE void lv_font_set_underline_position(lv_font_t * p, int8_t v) { p->underline_position = v ; }
EMSCRIPTEN_KEEPALIVE void lv_font_set_underline_thickness(lv_font_t * p, int8_t v) { p->underline_thickness = v ; }
EMSCRIPTEN_KEEPALIVE void lv_font_set_dsc(lv_font_t * p, const void * v) { p->dsc = v ; }
EMSCRIPTEN_KEEPALIVE void lv_font_set_user_data(lv_font_t * p, void * v) { p->user_data = v ; }


// struct lv_style_transition_dsc_t wasm api ----------
EMSCRIPTEN_KEEPALIVE lv_style_transition_dsc_t * lv_style_transition_dsc_create() {
    lv_style_transition_dsc_t * p = malloc(sizeof(lv_style_transition_dsc_t)) ;
    memset(p, 0, sizeof(lv_style_transition_dsc_t)) ;
    return p ;
}
// fields getter
EMSCRIPTEN_KEEPALIVE const lv_style_prop_t * lv_style_transition_dsc_get_props(lv_style_transition_dsc_t * p) { return p->props ; }
EMSCRIPTEN_KEEPALIVE void * lv_style_transition_dsc_get_user_data(lv_style_transition_dsc_t * p) { return p->user_data ; }
EMSCRIPTEN_KEEPALIVE lv_anim_path_cb_t lv_style_transition_dsc_get_path_xcb(lv_style_transition_dsc_t * p) { return p->path_xcb ; }
EMSCRIPTEN_KEEPALIVE uint32_t lv_style_transition_dsc_get_time(lv_style_transition_dsc_t * p) { return p->time ; }
EMSCRIPTEN_KEEPALIVE uint32_t lv_style_transition_dsc_get_delay(lv_style_transition_dsc_t * p) { return p->delay ; }
// fields setter
EMSCRIPTEN_KEEPALIVE void lv_style_transition_dsc_set_props(lv_style_transition_dsc_t * p, const lv_style_prop_t * v) { p->props = v ; }
EMSCRIPTEN_KEEPALIVE void lv_style_transition_dsc_set_user_data(lv_style_transition_dsc_t * p, void * v) { p->user_data = v ; }
EMSCRIPTEN_KEEPALIVE void lv_style_transition_dsc_set_path_xcb(lv_style_transition_dsc_t * p, lv_anim_path_cb_t v) { p->path_xcb = v ; }
EMSCRIPTEN_KEEPALIVE void lv_style_transition_dsc_set_time(lv_style_transition_dsc_t * p, uint32_t v) { p->time = v ; }
EMSCRIPTEN_KEEPALIVE void lv_style_transition_dsc_set_delay(lv_style_transition_dsc_t * p, uint32_t v) { p->delay = v ; }


// struct lv_style_const_prop_t wasm api ----------
EMSCRIPTEN_KEEPALIVE lv_style_const_prop_t * lv_style_const_prop_create() {
    lv_style_const_prop_t * p = malloc(sizeof(lv_style_const_prop_t)) ;
    memset(p, 0, sizeof(lv_style_const_prop_t)) ;
    return p ;
}
// fields getter
EMSCRIPTEN_KEEPALIVE lv_style_prop_t lv_style_const_prop_get_prop(lv_style_const_prop_t * p) { return p->prop ; }
EMSCRIPTEN_KEEPALIVE lv_style_value_t lv_style_const_prop_get_value(lv_style_const_prop_t * p) { return p->value ; }
// fields setter
EMSCRIPTEN_KEEPALIVE void lv_style_const_prop_set_prop(lv_style_const_prop_t * p, lv_style_prop_t v) { p->prop = v ; }
EMSCRIPTEN_KEEPALIVE void lv_style_const_prop_set_value(lv_style_const_prop_t * p, lv_style_value_t v) { p->value = v ; }


// struct lv_style_t wasm api ----------
EMSCRIPTEN_KEEPALIVE lv_style_t * lv_style_create() {
    lv_style_t * p = malloc(sizeof(lv_style_t)) ;
    memset(p, 0, sizeof(lv_style_t)) ;
    return p ;
}
// fields getter
EMSCRIPTEN_KEEPALIVE uint16_t lv_style_get_prop1(lv_style_t * p) { return p->prop1 ; }
EMSCRIPTEN_KEEPALIVE uint16_t lv_style_get_is_const(lv_style_t * p) { return p->is_const ; }
EMSCRIPTEN_KEEPALIVE uint8_t lv_style_get_has_group(lv_style_t * p) { return p->has_group ; }
EMSCRIPTEN_KEEPALIVE uint8_t lv_style_get_prop_cnt(lv_style_t * p) { return p->prop_cnt ; }
// fields setter
EMSCRIPTEN_KEEPALIVE void lv_style_set_prop1(lv_style_t * p, uint16_t v) { p->prop1 = v ; }
EMSCRIPTEN_KEEPALIVE void lv_style_set_is_const(lv_style_t * p, uint16_t v) { p->is_const = v ; }
EMSCRIPTEN_KEEPALIVE void lv_style_set_has_group(lv_style_t * p, uint8_t v) { p->has_group = v ; }
EMSCRIPTEN_KEEPALIVE void lv_style_set_prop_cnt(lv_style_t * p, uint8_t v) { p->prop_cnt = v ; }


// struct lv_layout_dsc_t wasm api ----------
EMSCRIPTEN_KEEPALIVE lv_layout_dsc_t * lv_layout_dsc_create() {
    lv_layout_dsc_t * p = malloc(sizeof(lv_layout_dsc_t)) ;
    memset(p, 0, sizeof(lv_layout_dsc_t)) ;
    return p ;
}
// fields getter
EMSCRIPTEN_KEEPALIVE lv_layout_update_cb_t lv_layout_dsc_get_cb(lv_layout_dsc_t * p) { return p->cb ; }
EMSCRIPTEN_KEEPALIVE void * lv_layout_dsc_get_user_data(lv_layout_dsc_t * p) { return p->user_data ; }
// fields setter
EMSCRIPTEN_KEEPALIVE void lv_layout_dsc_set_cb(lv_layout_dsc_t * p, lv_layout_update_cb_t v) { p->cb = v ; }
EMSCRIPTEN_KEEPALIVE void lv_layout_dsc_set_user_data(lv_layout_dsc_t * p, void * v) { p->user_data = v ; }


// struct _lv_obj_style_t wasm api ----------
EMSCRIPTEN_KEEPALIVE _lv_obj_style_t * _lv_obj_style_create() {
    _lv_obj_style_t * p = malloc(sizeof(_lv_obj_style_t)) ;
    memset(p, 0, sizeof(_lv_obj_style_t)) ;
    return p ;
}
// fields getter
EMSCRIPTEN_KEEPALIVE lv_style_t * _lv_obj_style_get_style(_lv_obj_style_t * p) { return p->style ; }
EMSCRIPTEN_KEEPALIVE uint32_t _lv_obj_style_get_selector(_lv_obj_style_t * p) { return p->selector ; }
EMSCRIPTEN_KEEPALIVE uint32_t _lv_obj_style_get_is_local(_lv_obj_style_t * p) { return p->is_local ; }
EMSCRIPTEN_KEEPALIVE uint32_t _lv_obj_style_get_is_trans(_lv_obj_style_t * p) { return p->is_trans ; }
// fields setter
EMSCRIPTEN_KEEPALIVE void _lv_obj_style_set_style(_lv_obj_style_t * p, lv_style_t * v) { p->style = v ; }
EMSCRIPTEN_KEEPALIVE void _lv_obj_style_set_selector(_lv_obj_style_t * p, uint32_t v) { p->selector = v ; }
EMSCRIPTEN_KEEPALIVE void _lv_obj_style_set_is_local(_lv_obj_style_t * p, uint32_t v) { p->is_local = v ; }
EMSCRIPTEN_KEEPALIVE void _lv_obj_style_set_is_trans(_lv_obj_style_t * p, uint32_t v) { p->is_trans = v ; }


// struct _lv_obj_style_transition_dsc_t wasm api ----------
EMSCRIPTEN_KEEPALIVE _lv_obj_style_transition_dsc_t * _lv_obj_style_transition_dsc_create() {
    _lv_obj_style_transition_dsc_t * p = malloc(sizeof(_lv_obj_style_transition_dsc_t)) ;
    memset(p, 0, sizeof(_lv_obj_style_transition_dsc_t)) ;
    return p ;
}
// fields getter
EMSCRIPTEN_KEEPALIVE uint16_t _lv_obj_style_transition_dsc_get_time(_lv_obj_style_transition_dsc_t * p) { return p->time ; }
EMSCRIPTEN_KEEPALIVE uint16_t _lv_obj_style_transition_dsc_get_delay(_lv_obj_style_transition_dsc_t * p) { return p->delay ; }
EMSCRIPTEN_KEEPALIVE lv_style_selector_t _lv_obj_style_transition_dsc_get_selector(_lv_obj_style_transition_dsc_t * p) { return p->selector ; }
EMSCRIPTEN_KEEPALIVE lv_style_prop_t _lv_obj_style_transition_dsc_get_prop(_lv_obj_style_transition_dsc_t * p) { return p->prop ; }
EMSCRIPTEN_KEEPALIVE lv_anim_path_cb_t _lv_obj_style_transition_dsc_get_path_cb(_lv_obj_style_transition_dsc_t * p) { return p->path_cb ; }
EMSCRIPTEN_KEEPALIVE void * _lv_obj_style_transition_dsc_get_user_data(_lv_obj_style_transition_dsc_t * p) { return p->user_data ; }
// fields setter
EMSCRIPTEN_KEEPALIVE void _lv_obj_style_transition_dsc_set_time(_lv_obj_style_transition_dsc_t * p, uint16_t v) { p->time = v ; }
EMSCRIPTEN_KEEPALIVE void _lv_obj_style_transition_dsc_set_delay(_lv_obj_style_transition_dsc_t * p, uint16_t v) { p->delay = v ; }
EMSCRIPTEN_KEEPALIVE void _lv_obj_style_transition_dsc_set_selector(_lv_obj_style_transition_dsc_t * p, lv_style_selector_t v) { p->selector = v ; }
EMSCRIPTEN_KEEPALIVE void _lv_obj_style_transition_dsc_set_prop(_lv_obj_style_transition_dsc_t * p, lv_style_prop_t v) { p->prop = v ; }
EMSCRIPTEN_KEEPALIVE void _lv_obj_style_transition_dsc_set_path_cb(_lv_obj_style_transition_dsc_t * p, lv_anim_path_cb_t v) { p->path_cb = v ; }
EMSCRIPTEN_KEEPALIVE void _lv_obj_style_transition_dsc_set_user_data(_lv_obj_style_transition_dsc_t * p, void * v) { p->user_data = v ; }


// struct lv_fs_drv_t wasm api ----------
EMSCRIPTEN_KEEPALIVE lv_fs_drv_t * lv_fs_drv_create() {
    lv_fs_drv_t * p = malloc(sizeof(lv_fs_drv_t)) ;
    memset(p, 0, sizeof(lv_fs_drv_t)) ;
    return p ;
}
// fields getter
EMSCRIPTEN_KEEPALIVE char lv_fs_drv_get_letter(lv_fs_drv_t * p) { return p->letter ; }
EMSCRIPTEN_KEEPALIVE void * lv_fs_drv_get_ready_cb(lv_fs_drv_t * p) { return p->ready_cb ; }
EMSCRIPTEN_KEEPALIVE void * lv_fs_drv_get_open_cb(lv_fs_drv_t * p) { return p->open_cb ; }
EMSCRIPTEN_KEEPALIVE void * lv_fs_drv_get_close_cb(lv_fs_drv_t * p) { return p->close_cb ; }
EMSCRIPTEN_KEEPALIVE void * lv_fs_drv_get_read_cb(lv_fs_drv_t * p) { return p->read_cb ; }
EMSCRIPTEN_KEEPALIVE void * lv_fs_drv_get_write_cb(lv_fs_drv_t * p) { return p->write_cb ; }
EMSCRIPTEN_KEEPALIVE void * lv_fs_drv_get_seek_cb(lv_fs_drv_t * p) { return p->seek_cb ; }
EMSCRIPTEN_KEEPALIVE void * lv_fs_drv_get_tell_cb(lv_fs_drv_t * p) { return p->tell_cb ; }
EMSCRIPTEN_KEEPALIVE void * lv_fs_drv_get_dir_open_cb(lv_fs_drv_t * p) { return p->dir_open_cb ; }
EMSCRIPTEN_KEEPALIVE void * lv_fs_drv_get_dir_read_cb(lv_fs_drv_t * p) { return p->dir_read_cb ; }
EMSCRIPTEN_KEEPALIVE void * lv_fs_drv_get_dir_close_cb(lv_fs_drv_t * p) { return p->dir_close_cb ; }
EMSCRIPTEN_KEEPALIVE void * lv_fs_drv_get_user_data(lv_fs_drv_t * p) { return p->user_data ; }
// fields setter
EMSCRIPTEN_KEEPALIVE void lv_fs_drv_set_letter(lv_fs_drv_t * p, char v) { p->letter = v ; }
EMSCRIPTEN_KEEPALIVE void lv_fs_drv_set_ready_cb(lv_fs_drv_t * p, void * v) { p->ready_cb = v ; }
EMSCRIPTEN_KEEPALIVE void lv_fs_drv_set_open_cb(lv_fs_drv_t * p, void * v) { p->open_cb = v ; }
EMSCRIPTEN_KEEPALIVE void lv_fs_drv_set_close_cb(lv_fs_drv_t * p, void * v) { p->close_cb = v ; }
EMSCRIPTEN_KEEPALIVE void lv_fs_drv_set_read_cb(lv_fs_drv_t * p, void * v) { p->read_cb = v ; }
EMSCRIPTEN_KEEPALIVE void lv_fs_drv_set_write_cb(lv_fs_drv_t * p, void * v) { p->write_cb = v ; }
EMSCRIPTEN_KEEPALIVE void lv_fs_drv_set_seek_cb(lv_fs_drv_t * p, void * v) { p->seek_cb = v ; }
EMSCRIPTEN_KEEPALIVE void lv_fs_drv_set_tell_cb(lv_fs_drv_t * p, void * v) { p->tell_cb = v ; }
EMSCRIPTEN_KEEPALIVE void lv_fs_drv_set_dir_open_cb(lv_fs_drv_t * p, void * v) { p->dir_open_cb = v ; }
EMSCRIPTEN_KEEPALIVE void lv_fs_drv_set_dir_read_cb(lv_fs_drv_t * p, void * v) { p->dir_read_cb = v ; }
EMSCRIPTEN_KEEPALIVE void lv_fs_drv_set_dir_close_cb(lv_fs_drv_t * p, void * v) { p->dir_close_cb = v ; }
EMSCRIPTEN_KEEPALIVE void lv_fs_drv_set_user_data(lv_fs_drv_t * p, void * v) { p->user_data = v ; }


// struct lv_fs_file_t wasm api ----------
EMSCRIPTEN_KEEPALIVE lv_fs_file_t * lv_fs_file_create() {
    lv_fs_file_t * p = malloc(sizeof(lv_fs_file_t)) ;
    memset(p, 0, sizeof(lv_fs_file_t)) ;
    return p ;
}
// fields getter
EMSCRIPTEN_KEEPALIVE void * lv_fs_file_get_file_d(lv_fs_file_t * p) { return p->file_d ; }
EMSCRIPTEN_KEEPALIVE lv_fs_drv_t * lv_fs_file_get_drv(lv_fs_file_t * p) { return p->drv ; }
// fields setter
EMSCRIPTEN_KEEPALIVE void lv_fs_file_set_file_d(lv_fs_file_t * p, void * v) { p->file_d = v ; }
EMSCRIPTEN_KEEPALIVE void lv_fs_file_set_drv(lv_fs_file_t * p, lv_fs_drv_t * v) { p->drv = v ; }


// struct lv_fs_dir_t wasm api ----------
EMSCRIPTEN_KEEPALIVE lv_fs_dir_t * lv_fs_dir_create() {
    lv_fs_dir_t * p = malloc(sizeof(lv_fs_dir_t)) ;
    memset(p, 0, sizeof(lv_fs_dir_t)) ;
    return p ;
}
// fields getter
EMSCRIPTEN_KEEPALIVE void * lv_fs_dir_get_dir_d(lv_fs_dir_t * p) { return p->dir_d ; }
EMSCRIPTEN_KEEPALIVE lv_fs_drv_t * lv_fs_dir_get_drv(lv_fs_dir_t * p) { return p->drv ; }
// fields setter
EMSCRIPTEN_KEEPALIVE void lv_fs_dir_set_dir_d(lv_fs_dir_t * p, void * v) { p->dir_d = v ; }
EMSCRIPTEN_KEEPALIVE void lv_fs_dir_set_drv(lv_fs_dir_t * p, lv_fs_drv_t * v) { p->drv = v ; }


// struct lv_img_decoder_t wasm api ----------
// fields getter
EMSCRIPTEN_KEEPALIVE lv_img_decoder_info_f_t lv_img_decoder_get_info_cb(lv_img_decoder_t * p) { return p->info_cb ; }
EMSCRIPTEN_KEEPALIVE lv_img_decoder_open_f_t lv_img_decoder_get_open_cb(lv_img_decoder_t * p) { return p->open_cb ; }
EMSCRIPTEN_KEEPALIVE lv_img_decoder_read_line_f_t lv_img_decoder_get_read_line_cb(lv_img_decoder_t * p) { return p->read_line_cb ; }
EMSCRIPTEN_KEEPALIVE lv_img_decoder_close_f_t lv_img_decoder_get_close_cb(lv_img_decoder_t * p) { return p->close_cb ; }
EMSCRIPTEN_KEEPALIVE void * lv_img_decoder_get_user_data(lv_img_decoder_t * p) { return p->user_data ; }
// fields setter
EMSCRIPTEN_KEEPALIVE void lv_img_decoder_set_user_data(lv_img_decoder_t * p, void * v) { p->user_data = v ; }


// struct lv_img_decoder_dsc_t wasm api ----------
EMSCRIPTEN_KEEPALIVE lv_img_decoder_dsc_t * lv_img_decoder_dsc_create() {
    lv_img_decoder_dsc_t * p = malloc(sizeof(lv_img_decoder_dsc_t)) ;
    memset(p, 0, sizeof(lv_img_decoder_dsc_t)) ;
    return p ;
}
// fields getter
EMSCRIPTEN_KEEPALIVE lv_img_decoder_t * lv_img_decoder_dsc_get_decoder(lv_img_decoder_dsc_t * p) { return p->decoder ; }
EMSCRIPTEN_KEEPALIVE const void * lv_img_decoder_dsc_get_src(lv_img_decoder_dsc_t * p) { return p->src ; }
EMSCRIPTEN_KEEPALIVE lv_color_t lv_img_decoder_dsc_get_color(lv_img_decoder_dsc_t * p) { return p->color ; }
EMSCRIPTEN_KEEPALIVE int32_t lv_img_decoder_dsc_get_frame_id(lv_img_decoder_dsc_t * p) { return p->frame_id ; }
EMSCRIPTEN_KEEPALIVE lv_img_src_t lv_img_decoder_dsc_get_src_type(lv_img_decoder_dsc_t * p) { return p->src_type ; }
EMSCRIPTEN_KEEPALIVE lv_img_header_t lv_img_decoder_dsc_get_header(lv_img_decoder_dsc_t * p) { return p->header ; }
EMSCRIPTEN_KEEPALIVE const uint8_t * lv_img_decoder_dsc_get_img_data(lv_img_decoder_dsc_t * p) { return p->img_data ; }
EMSCRIPTEN_KEEPALIVE uint32_t lv_img_decoder_dsc_get_time_to_open(lv_img_decoder_dsc_t * p) { return p->time_to_open ; }
EMSCRIPTEN_KEEPALIVE const char * lv_img_decoder_dsc_get_error_msg(lv_img_decoder_dsc_t * p) { return p->error_msg ; }
EMSCRIPTEN_KEEPALIVE void * lv_img_decoder_dsc_get_user_data(lv_img_decoder_dsc_t * p) { return p->user_data ; }
// fields setter
EMSCRIPTEN_KEEPALIVE void lv_img_decoder_dsc_set_decoder(lv_img_decoder_dsc_t * p, lv_img_decoder_t * v) { p->decoder = v ; }
EMSCRIPTEN_KEEPALIVE void lv_img_decoder_dsc_set_src(lv_img_decoder_dsc_t * p, const void * v) { p->src = v ; }
EMSCRIPTEN_KEEPALIVE void lv_img_decoder_dsc_set_color(lv_img_decoder_dsc_t * p, lv_color_t v) { p->color = v ; }
EMSCRIPTEN_KEEPALIVE void lv_img_decoder_dsc_set_frame_id(lv_img_decoder_dsc_t * p, int32_t v) { p->frame_id = v ; }
EMSCRIPTEN_KEEPALIVE void lv_img_decoder_dsc_set_src_type(lv_img_decoder_dsc_t * p, lv_img_src_t v) { p->src_type = v ; }
EMSCRIPTEN_KEEPALIVE void lv_img_decoder_dsc_set_header(lv_img_decoder_dsc_t * p, lv_img_header_t v) { p->header = v ; }
EMSCRIPTEN_KEEPALIVE void lv_img_decoder_dsc_set_img_data(lv_img_decoder_dsc_t * p, const uint8_t * v) { p->img_data = v ; }
EMSCRIPTEN_KEEPALIVE void lv_img_decoder_dsc_set_time_to_open(lv_img_decoder_dsc_t * p, uint32_t v) { p->time_to_open = v ; }
EMSCRIPTEN_KEEPALIVE void lv_img_decoder_dsc_set_error_msg(lv_img_decoder_dsc_t * p, const char * v) { p->error_msg = v ; }
EMSCRIPTEN_KEEPALIVE void lv_img_decoder_dsc_set_user_data(lv_img_decoder_dsc_t * p, void * v) { p->user_data = v ; }


// struct _lv_img_cache_entry_t wasm api ----------
EMSCRIPTEN_KEEPALIVE _lv_img_cache_entry_t * _lv_img_cache_entry_create() {
    _lv_img_cache_entry_t * p = malloc(sizeof(_lv_img_cache_entry_t)) ;
    memset(p, 0, sizeof(_lv_img_cache_entry_t)) ;
    return p ;
}
// fields getter
EMSCRIPTEN_KEEPALIVE lv_img_decoder_dsc_t _lv_img_cache_entry_get_dec_dsc(_lv_img_cache_entry_t * p) { return p->dec_dsc ; }
EMSCRIPTEN_KEEPALIVE int32_t _lv_img_cache_entry_get_life(_lv_img_cache_entry_t * p) { return p->life ; }
// fields setter
EMSCRIPTEN_KEEPALIVE void _lv_img_cache_entry_set_dec_dsc(_lv_img_cache_entry_t * p, lv_img_decoder_dsc_t v) { p->dec_dsc = v ; }
EMSCRIPTEN_KEEPALIVE void _lv_img_cache_entry_set_life(_lv_img_cache_entry_t * p, int32_t v) { p->life = v ; }


// struct _lv_draw_mask_saved_t wasm api ----------
EMSCRIPTEN_KEEPALIVE _lv_draw_mask_saved_t * _lv_draw_mask_saved_create() {
    _lv_draw_mask_saved_t * p = malloc(sizeof(_lv_draw_mask_saved_t)) ;
    memset(p, 0, sizeof(_lv_draw_mask_saved_t)) ;
    return p ;
}
// fields getter
EMSCRIPTEN_KEEPALIVE void * _lv_draw_mask_saved_get_param(_lv_draw_mask_saved_t * p) { return p->param ; }
EMSCRIPTEN_KEEPALIVE void * _lv_draw_mask_saved_get_custom_id(_lv_draw_mask_saved_t * p) { return p->custom_id ; }
// fields setter
EMSCRIPTEN_KEEPALIVE void _lv_draw_mask_saved_set_param(_lv_draw_mask_saved_t * p, void * v) { p->param = v ; }
EMSCRIPTEN_KEEPALIVE void _lv_draw_mask_saved_set_custom_id(_lv_draw_mask_saved_t * p, void * v) { p->custom_id = v ; }


// struct _lv_draw_mask_common_dsc_t wasm api ----------
EMSCRIPTEN_KEEPALIVE _lv_draw_mask_common_dsc_t * _lv_draw_mask_common_dsc_create() {
    _lv_draw_mask_common_dsc_t * p = malloc(sizeof(_lv_draw_mask_common_dsc_t)) ;
    memset(p, 0, sizeof(_lv_draw_mask_common_dsc_t)) ;
    return p ;
}
// fields getter
EMSCRIPTEN_KEEPALIVE lv_draw_mask_xcb_t _lv_draw_mask_common_dsc_get_cb(_lv_draw_mask_common_dsc_t * p) { return p->cb ; }
EMSCRIPTEN_KEEPALIVE lv_draw_mask_type_t _lv_draw_mask_common_dsc_get_type(_lv_draw_mask_common_dsc_t * p) { return p->type ; }
// fields setter
EMSCRIPTEN_KEEPALIVE void _lv_draw_mask_common_dsc_set_cb(_lv_draw_mask_common_dsc_t * p, lv_draw_mask_xcb_t v) { p->cb = v ; }
EMSCRIPTEN_KEEPALIVE void _lv_draw_mask_common_dsc_set_type(_lv_draw_mask_common_dsc_t * p, lv_draw_mask_type_t v) { p->type = v ; }


// struct lv_draw_mask_line_param_t wasm api ----------
EMSCRIPTEN_KEEPALIVE lv_draw_mask_line_param_t * lv_draw_mask_line_param_create() {
    lv_draw_mask_line_param_t * p = malloc(sizeof(lv_draw_mask_line_param_t)) ;
    memset(p, 0, sizeof(lv_draw_mask_line_param_t)) ;
    return p ;
}
// fields getter
EMSCRIPTEN_KEEPALIVE _lv_draw_mask_common_dsc_t lv_draw_mask_line_param_get_dsc(lv_draw_mask_line_param_t * p) { return p->dsc ; }
EMSCRIPTEN_KEEPALIVE lv_point_t lv_draw_mask_line_param_get_origo(lv_draw_mask_line_param_t * p) { return p->origo ; }
EMSCRIPTEN_KEEPALIVE int32_t lv_draw_mask_line_param_get_xy_steep(lv_draw_mask_line_param_t * p) { return p->xy_steep ; }
EMSCRIPTEN_KEEPALIVE int32_t lv_draw_mask_line_param_get_yx_steep(lv_draw_mask_line_param_t * p) { return p->yx_steep ; }
EMSCRIPTEN_KEEPALIVE int32_t lv_draw_mask_line_param_get_steep(lv_draw_mask_line_param_t * p) { return p->steep ; }
EMSCRIPTEN_KEEPALIVE int32_t lv_draw_mask_line_param_get_spx(lv_draw_mask_line_param_t * p) { return p->spx ; }
EMSCRIPTEN_KEEPALIVE uint8_t lv_draw_mask_line_param_get_flat(lv_draw_mask_line_param_t * p) { return p->flat ; }
EMSCRIPTEN_KEEPALIVE uint8_t lv_draw_mask_line_param_get_inv(lv_draw_mask_line_param_t * p) { return p->inv ; }
// fields setter
EMSCRIPTEN_KEEPALIVE void lv_draw_mask_line_param_set_dsc(lv_draw_mask_line_param_t * p, _lv_draw_mask_common_dsc_t v) { p->dsc = v ; }
EMSCRIPTEN_KEEPALIVE void lv_draw_mask_line_param_set_origo(lv_draw_mask_line_param_t * p, lv_point_t v) { p->origo = v ; }
EMSCRIPTEN_KEEPALIVE void lv_draw_mask_line_param_set_xy_steep(lv_draw_mask_line_param_t * p, int32_t v) { p->xy_steep = v ; }
EMSCRIPTEN_KEEPALIVE void lv_draw_mask_line_param_set_yx_steep(lv_draw_mask_line_param_t * p, int32_t v) { p->yx_steep = v ; }
EMSCRIPTEN_KEEPALIVE void lv_draw_mask_line_param_set_steep(lv_draw_mask_line_param_t * p, int32_t v) { p->steep = v ; }
EMSCRIPTEN_KEEPALIVE void lv_draw_mask_line_param_set_spx(lv_draw_mask_line_param_t * p, int32_t v) { p->spx = v ; }
EMSCRIPTEN_KEEPALIVE void lv_draw_mask_line_param_set_flat(lv_draw_mask_line_param_t * p, uint8_t v) { p->flat = v ; }
EMSCRIPTEN_KEEPALIVE void lv_draw_mask_line_param_set_inv(lv_draw_mask_line_param_t * p, uint8_t v) { p->inv = v ; }


// struct lv_draw_mask_angle_param_t wasm api ----------
EMSCRIPTEN_KEEPALIVE lv_draw_mask_angle_param_t * lv_draw_mask_angle_param_create() {
    lv_draw_mask_angle_param_t * p = malloc(sizeof(lv_draw_mask_angle_param_t)) ;
    memset(p, 0, sizeof(lv_draw_mask_angle_param_t)) ;
    return p ;
}
// fields getter
EMSCRIPTEN_KEEPALIVE _lv_draw_mask_common_dsc_t lv_draw_mask_angle_param_get_dsc(lv_draw_mask_angle_param_t * p) { return p->dsc ; }
EMSCRIPTEN_KEEPALIVE lv_draw_mask_line_param_t lv_draw_mask_angle_param_get_start_line(lv_draw_mask_angle_param_t * p) { return p->start_line ; }
EMSCRIPTEN_KEEPALIVE lv_draw_mask_line_param_t lv_draw_mask_angle_param_get_end_line(lv_draw_mask_angle_param_t * p) { return p->end_line ; }
EMSCRIPTEN_KEEPALIVE uint16_t lv_draw_mask_angle_param_get_delta_deg(lv_draw_mask_angle_param_t * p) { return p->delta_deg ; }
// fields setter
EMSCRIPTEN_KEEPALIVE void lv_draw_mask_angle_param_set_dsc(lv_draw_mask_angle_param_t * p, _lv_draw_mask_common_dsc_t v) { p->dsc = v ; }
EMSCRIPTEN_KEEPALIVE void lv_draw_mask_angle_param_set_start_line(lv_draw_mask_angle_param_t * p, lv_draw_mask_line_param_t v) { p->start_line = v ; }
EMSCRIPTEN_KEEPALIVE void lv_draw_mask_angle_param_set_end_line(lv_draw_mask_angle_param_t * p, lv_draw_mask_line_param_t v) { p->end_line = v ; }
EMSCRIPTEN_KEEPALIVE void lv_draw_mask_angle_param_set_delta_deg(lv_draw_mask_angle_param_t * p, uint16_t v) { p->delta_deg = v ; }


// struct _lv_draw_mask_radius_circle_dsc_t wasm api ----------
EMSCRIPTEN_KEEPALIVE _lv_draw_mask_radius_circle_dsc_t * _lv_draw_mask_radius_circle_dsc_create() {
    _lv_draw_mask_radius_circle_dsc_t * p = malloc(sizeof(_lv_draw_mask_radius_circle_dsc_t)) ;
    memset(p, 0, sizeof(_lv_draw_mask_radius_circle_dsc_t)) ;
    return p ;
}
// fields getter
EMSCRIPTEN_KEEPALIVE uint8_t * _lv_draw_mask_radius_circle_dsc_get_buf(_lv_draw_mask_radius_circle_dsc_t * p) { return p->buf ; }
EMSCRIPTEN_KEEPALIVE lv_opa_t * _lv_draw_mask_radius_circle_dsc_get_cir_opa(_lv_draw_mask_radius_circle_dsc_t * p) { return p->cir_opa ; }
EMSCRIPTEN_KEEPALIVE uint16_t * _lv_draw_mask_radius_circle_dsc_get_x_start_on_y(_lv_draw_mask_radius_circle_dsc_t * p) { return p->x_start_on_y ; }
EMSCRIPTEN_KEEPALIVE uint16_t * _lv_draw_mask_radius_circle_dsc_get_opa_start_on_y(_lv_draw_mask_radius_circle_dsc_t * p) { return p->opa_start_on_y ; }
EMSCRIPTEN_KEEPALIVE int32_t _lv_draw_mask_radius_circle_dsc_get_life(_lv_draw_mask_radius_circle_dsc_t * p) { return p->life ; }
EMSCRIPTEN_KEEPALIVE uint32_t _lv_draw_mask_radius_circle_dsc_get_used_cnt(_lv_draw_mask_radius_circle_dsc_t * p) { return p->used_cnt ; }
EMSCRIPTEN_KEEPALIVE lv_coord_t _lv_draw_mask_radius_circle_dsc_get_radius(_lv_draw_mask_radius_circle_dsc_t * p) { return p->radius ; }
// fields setter
EMSCRIPTEN_KEEPALIVE void _lv_draw_mask_radius_circle_dsc_set_buf(_lv_draw_mask_radius_circle_dsc_t * p, uint8_t * v) { p->buf = v ; }
EMSCRIPTEN_KEEPALIVE void _lv_draw_mask_radius_circle_dsc_set_cir_opa(_lv_draw_mask_radius_circle_dsc_t * p, lv_opa_t * v) { p->cir_opa = v ; }
EMSCRIPTEN_KEEPALIVE void _lv_draw_mask_radius_circle_dsc_set_x_start_on_y(_lv_draw_mask_radius_circle_dsc_t * p, uint16_t * v) { p->x_start_on_y = v ; }
EMSCRIPTEN_KEEPALIVE void _lv_draw_mask_radius_circle_dsc_set_opa_start_on_y(_lv_draw_mask_radius_circle_dsc_t * p, uint16_t * v) { p->opa_start_on_y = v ; }
EMSCRIPTEN_KEEPALIVE void _lv_draw_mask_radius_circle_dsc_set_life(_lv_draw_mask_radius_circle_dsc_t * p, int32_t v) { p->life = v ; }
EMSCRIPTEN_KEEPALIVE void _lv_draw_mask_radius_circle_dsc_set_used_cnt(_lv_draw_mask_radius_circle_dsc_t * p, uint32_t v) { p->used_cnt = v ; }
EMSCRIPTEN_KEEPALIVE void _lv_draw_mask_radius_circle_dsc_set_radius(_lv_draw_mask_radius_circle_dsc_t * p, lv_coord_t v) { p->radius = v ; }


// struct lv_draw_mask_radius_param_t wasm api ----------
EMSCRIPTEN_KEEPALIVE lv_draw_mask_radius_param_t * lv_draw_mask_radius_param_create() {
    lv_draw_mask_radius_param_t * p = malloc(sizeof(lv_draw_mask_radius_param_t)) ;
    memset(p, 0, sizeof(lv_draw_mask_radius_param_t)) ;
    return p ;
}
// fields getter
EMSCRIPTEN_KEEPALIVE _lv_draw_mask_common_dsc_t lv_draw_mask_radius_param_get_dsc(lv_draw_mask_radius_param_t * p) { return p->dsc ; }
EMSCRIPTEN_KEEPALIVE _lv_draw_mask_radius_circle_dsc_t * lv_draw_mask_radius_param_get_circle(lv_draw_mask_radius_param_t * p) { return p->circle ; }
// fields setter
EMSCRIPTEN_KEEPALIVE void lv_draw_mask_radius_param_set_dsc(lv_draw_mask_radius_param_t * p, _lv_draw_mask_common_dsc_t v) { p->dsc = v ; }
EMSCRIPTEN_KEEPALIVE void lv_draw_mask_radius_param_set_circle(lv_draw_mask_radius_param_t * p, _lv_draw_mask_radius_circle_dsc_t * v) { p->circle = v ; }


// struct lv_draw_mask_fade_param_t wasm api ----------
EMSCRIPTEN_KEEPALIVE lv_draw_mask_fade_param_t * lv_draw_mask_fade_param_create() {
    lv_draw_mask_fade_param_t * p = malloc(sizeof(lv_draw_mask_fade_param_t)) ;
    memset(p, 0, sizeof(lv_draw_mask_fade_param_t)) ;
    return p ;
}
// fields getter
EMSCRIPTEN_KEEPALIVE _lv_draw_mask_common_dsc_t lv_draw_mask_fade_param_get_dsc(lv_draw_mask_fade_param_t * p) { return p->dsc ; }
// fields setter
EMSCRIPTEN_KEEPALIVE void lv_draw_mask_fade_param_set_dsc(lv_draw_mask_fade_param_t * p, _lv_draw_mask_common_dsc_t v) { p->dsc = v ; }


// struct lv_draw_mask_map_param_t wasm api ----------
EMSCRIPTEN_KEEPALIVE lv_draw_mask_map_param_t * lv_draw_mask_map_param_create() {
    lv_draw_mask_map_param_t * p = malloc(sizeof(lv_draw_mask_map_param_t)) ;
    memset(p, 0, sizeof(lv_draw_mask_map_param_t)) ;
    return p ;
}
// fields getter
EMSCRIPTEN_KEEPALIVE _lv_draw_mask_common_dsc_t lv_draw_mask_map_param_get_dsc(lv_draw_mask_map_param_t * p) { return p->dsc ; }
// fields setter
EMSCRIPTEN_KEEPALIVE void lv_draw_mask_map_param_set_dsc(lv_draw_mask_map_param_t * p, _lv_draw_mask_common_dsc_t v) { p->dsc = v ; }


// struct lv_draw_rect_dsc_t wasm api ----------
EMSCRIPTEN_KEEPALIVE lv_draw_rect_dsc_t * lv_draw_rect_dsc_create() {
    lv_draw_rect_dsc_t * p = malloc(sizeof(lv_draw_rect_dsc_t)) ;
    memset(p, 0, sizeof(lv_draw_rect_dsc_t)) ;
    return p ;
}
// fields getter
EMSCRIPTEN_KEEPALIVE lv_coord_t lv_draw_rect_dsc_get_radius(lv_draw_rect_dsc_t * p) { return p->radius ; }
EMSCRIPTEN_KEEPALIVE lv_blend_mode_t lv_draw_rect_dsc_get_blend_mode(lv_draw_rect_dsc_t * p) { return p->blend_mode ; }
EMSCRIPTEN_KEEPALIVE lv_color_t lv_draw_rect_dsc_get_bg_color(lv_draw_rect_dsc_t * p) { return p->bg_color ; }
EMSCRIPTEN_KEEPALIVE lv_color_t lv_draw_rect_dsc_get_bg_grad_color(lv_draw_rect_dsc_t * p) { return p->bg_grad_color ; }
EMSCRIPTEN_KEEPALIVE uint8_t lv_draw_rect_dsc_get_bg_main_color_stop(lv_draw_rect_dsc_t * p) { return p->bg_main_color_stop ; }
EMSCRIPTEN_KEEPALIVE uint8_t lv_draw_rect_dsc_get_bg_grad_color_stop(lv_draw_rect_dsc_t * p) { return p->bg_grad_color_stop ; }
EMSCRIPTEN_KEEPALIVE lv_opa_t lv_draw_rect_dsc_get_bg_opa(lv_draw_rect_dsc_t * p) { return p->bg_opa ; }
EMSCRIPTEN_KEEPALIVE lv_grad_dir_t lv_draw_rect_dsc_get_bg_grad_dir(lv_draw_rect_dsc_t * p) { return p->bg_grad_dir ; }
EMSCRIPTEN_KEEPALIVE const void * lv_draw_rect_dsc_get_bg_img_src(lv_draw_rect_dsc_t * p) { return p->bg_img_src ; }
EMSCRIPTEN_KEEPALIVE const void * lv_draw_rect_dsc_get_bg_img_symbol_font(lv_draw_rect_dsc_t * p) { return p->bg_img_symbol_font ; }
EMSCRIPTEN_KEEPALIVE lv_color_t lv_draw_rect_dsc_get_bg_img_recolor(lv_draw_rect_dsc_t * p) { return p->bg_img_recolor ; }
EMSCRIPTEN_KEEPALIVE lv_opa_t lv_draw_rect_dsc_get_bg_img_opa(lv_draw_rect_dsc_t * p) { return p->bg_img_opa ; }
EMSCRIPTEN_KEEPALIVE lv_opa_t lv_draw_rect_dsc_get_bg_img_recolor_opa(lv_draw_rect_dsc_t * p) { return p->bg_img_recolor_opa ; }
EMSCRIPTEN_KEEPALIVE uint8_t lv_draw_rect_dsc_get_bg_img_tiled(lv_draw_rect_dsc_t * p) { return p->bg_img_tiled ; }
EMSCRIPTEN_KEEPALIVE lv_color_t lv_draw_rect_dsc_get_border_color(lv_draw_rect_dsc_t * p) { return p->border_color ; }
EMSCRIPTEN_KEEPALIVE lv_coord_t lv_draw_rect_dsc_get_border_width(lv_draw_rect_dsc_t * p) { return p->border_width ; }
EMSCRIPTEN_KEEPALIVE lv_opa_t lv_draw_rect_dsc_get_border_opa(lv_draw_rect_dsc_t * p) { return p->border_opa ; }
EMSCRIPTEN_KEEPALIVE uint8_t lv_draw_rect_dsc_get_border_post(lv_draw_rect_dsc_t * p) { return p->border_post ; }
EMSCRIPTEN_KEEPALIVE lv_border_side_t lv_draw_rect_dsc_get_border_side(lv_draw_rect_dsc_t * p) { return p->border_side ; }
EMSCRIPTEN_KEEPALIVE lv_color_t lv_draw_rect_dsc_get_outline_color(lv_draw_rect_dsc_t * p) { return p->outline_color ; }
EMSCRIPTEN_KEEPALIVE lv_coord_t lv_draw_rect_dsc_get_outline_width(lv_draw_rect_dsc_t * p) { return p->outline_width ; }
EMSCRIPTEN_KEEPALIVE lv_coord_t lv_draw_rect_dsc_get_outline_pad(lv_draw_rect_dsc_t * p) { return p->outline_pad ; }
EMSCRIPTEN_KEEPALIVE lv_opa_t lv_draw_rect_dsc_get_outline_opa(lv_draw_rect_dsc_t * p) { return p->outline_opa ; }
EMSCRIPTEN_KEEPALIVE lv_color_t lv_draw_rect_dsc_get_shadow_color(lv_draw_rect_dsc_t * p) { return p->shadow_color ; }
EMSCRIPTEN_KEEPALIVE lv_coord_t lv_draw_rect_dsc_get_shadow_width(lv_draw_rect_dsc_t * p) { return p->shadow_width ; }
EMSCRIPTEN_KEEPALIVE lv_coord_t lv_draw_rect_dsc_get_shadow_ofs_x(lv_draw_rect_dsc_t * p) { return p->shadow_ofs_x ; }
EMSCRIPTEN_KEEPALIVE lv_coord_t lv_draw_rect_dsc_get_shadow_ofs_y(lv_draw_rect_dsc_t * p) { return p->shadow_ofs_y ; }
EMSCRIPTEN_KEEPALIVE lv_coord_t lv_draw_rect_dsc_get_shadow_spread(lv_draw_rect_dsc_t * p) { return p->shadow_spread ; }
EMSCRIPTEN_KEEPALIVE lv_opa_t lv_draw_rect_dsc_get_shadow_opa(lv_draw_rect_dsc_t * p) { return p->shadow_opa ; }
// fields setter
EMSCRIPTEN_KEEPALIVE void lv_draw_rect_dsc_set_radius(lv_draw_rect_dsc_t * p, lv_coord_t v) { p->radius = v ; }
EMSCRIPTEN_KEEPALIVE void lv_draw_rect_dsc_set_blend_mode(lv_draw_rect_dsc_t * p, lv_blend_mode_t v) { p->blend_mode = v ; }
EMSCRIPTEN_KEEPALIVE void lv_draw_rect_dsc_set_bg_color(lv_draw_rect_dsc_t * p, lv_color_t v) { p->bg_color = v ; }
EMSCRIPTEN_KEEPALIVE void lv_draw_rect_dsc_set_bg_grad_color(lv_draw_rect_dsc_t * p, lv_color_t v) { p->bg_grad_color = v ; }
EMSCRIPTEN_KEEPALIVE void lv_draw_rect_dsc_set_bg_main_color_stop(lv_draw_rect_dsc_t * p, uint8_t v) { p->bg_main_color_stop = v ; }
EMSCRIPTEN_KEEPALIVE void lv_draw_rect_dsc_set_bg_grad_color_stop(lv_draw_rect_dsc_t * p, uint8_t v) { p->bg_grad_color_stop = v ; }
EMSCRIPTEN_KEEPALIVE void lv_draw_rect_dsc_set_bg_opa(lv_draw_rect_dsc_t * p, lv_opa_t v) { p->bg_opa = v ; }
EMSCRIPTEN_KEEPALIVE void lv_draw_rect_dsc_set_bg_grad_dir(lv_draw_rect_dsc_t * p, lv_grad_dir_t v) { p->bg_grad_dir = v ; }
EMSCRIPTEN_KEEPALIVE void lv_draw_rect_dsc_set_bg_img_src(lv_draw_rect_dsc_t * p, const void * v) { p->bg_img_src = v ; }
EMSCRIPTEN_KEEPALIVE void lv_draw_rect_dsc_set_bg_img_symbol_font(lv_draw_rect_dsc_t * p, const void * v) { p->bg_img_symbol_font = v ; }
EMSCRIPTEN_KEEPALIVE void lv_draw_rect_dsc_set_bg_img_recolor(lv_draw_rect_dsc_t * p, lv_color_t v) { p->bg_img_recolor = v ; }
EMSCRIPTEN_KEEPALIVE void lv_draw_rect_dsc_set_bg_img_opa(lv_draw_rect_dsc_t * p, lv_opa_t v) { p->bg_img_opa = v ; }
EMSCRIPTEN_KEEPALIVE void lv_draw_rect_dsc_set_bg_img_recolor_opa(lv_draw_rect_dsc_t * p, lv_opa_t v) { p->bg_img_recolor_opa = v ; }
EMSCRIPTEN_KEEPALIVE void lv_draw_rect_dsc_set_bg_img_tiled(lv_draw_rect_dsc_t * p, uint8_t v) { p->bg_img_tiled = v ; }
EMSCRIPTEN_KEEPALIVE void lv_draw_rect_dsc_set_border_color(lv_draw_rect_dsc_t * p, lv_color_t v) { p->border_color = v ; }
EMSCRIPTEN_KEEPALIVE void lv_draw_rect_dsc_set_border_width(lv_draw_rect_dsc_t * p, lv_coord_t v) { p->border_width = v ; }
EMSCRIPTEN_KEEPALIVE void lv_draw_rect_dsc_set_border_opa(lv_draw_rect_dsc_t * p, lv_opa_t v) { p->border_opa = v ; }
EMSCRIPTEN_KEEPALIVE void lv_draw_rect_dsc_set_border_post(lv_draw_rect_dsc_t * p, uint8_t v) { p->border_post = v ; }
EMSCRIPTEN_KEEPALIVE void lv_draw_rect_dsc_set_border_side(lv_draw_rect_dsc_t * p, lv_border_side_t v) { p->border_side = v ; }
EMSCRIPTEN_KEEPALIVE void lv_draw_rect_dsc_set_outline_color(lv_draw_rect_dsc_t * p, lv_color_t v) { p->outline_color = v ; }
EMSCRIPTEN_KEEPALIVE void lv_draw_rect_dsc_set_outline_width(lv_draw_rect_dsc_t * p, lv_coord_t v) { p->outline_width = v ; }
EMSCRIPTEN_KEEPALIVE void lv_draw_rect_dsc_set_outline_pad(lv_draw_rect_dsc_t * p, lv_coord_t v) { p->outline_pad = v ; }
EMSCRIPTEN_KEEPALIVE void lv_draw_rect_dsc_set_outline_opa(lv_draw_rect_dsc_t * p, lv_opa_t v) { p->outline_opa = v ; }
EMSCRIPTEN_KEEPALIVE void lv_draw_rect_dsc_set_shadow_color(lv_draw_rect_dsc_t * p, lv_color_t v) { p->shadow_color = v ; }
EMSCRIPTEN_KEEPALIVE void lv_draw_rect_dsc_set_shadow_width(lv_draw_rect_dsc_t * p, lv_coord_t v) { p->shadow_width = v ; }
EMSCRIPTEN_KEEPALIVE void lv_draw_rect_dsc_set_shadow_ofs_x(lv_draw_rect_dsc_t * p, lv_coord_t v) { p->shadow_ofs_x = v ; }
EMSCRIPTEN_KEEPALIVE void lv_draw_rect_dsc_set_shadow_ofs_y(lv_draw_rect_dsc_t * p, lv_coord_t v) { p->shadow_ofs_y = v ; }
EMSCRIPTEN_KEEPALIVE void lv_draw_rect_dsc_set_shadow_spread(lv_draw_rect_dsc_t * p, lv_coord_t v) { p->shadow_spread = v ; }
EMSCRIPTEN_KEEPALIVE void lv_draw_rect_dsc_set_shadow_opa(lv_draw_rect_dsc_t * p, lv_opa_t v) { p->shadow_opa = v ; }


// struct lv_draw_label_dsc_t wasm api ----------
EMSCRIPTEN_KEEPALIVE lv_draw_label_dsc_t * lv_draw_label_dsc_create() {
    lv_draw_label_dsc_t * p = malloc(sizeof(lv_draw_label_dsc_t)) ;
    memset(p, 0, sizeof(lv_draw_label_dsc_t)) ;
    return p ;
}
// fields getter
EMSCRIPTEN_KEEPALIVE const lv_font_t * lv_draw_label_dsc_get_font(lv_draw_label_dsc_t * p) { return p->font ; }
EMSCRIPTEN_KEEPALIVE uint32_t lv_draw_label_dsc_get_sel_start(lv_draw_label_dsc_t * p) { return p->sel_start ; }
EMSCRIPTEN_KEEPALIVE uint32_t lv_draw_label_dsc_get_sel_end(lv_draw_label_dsc_t * p) { return p->sel_end ; }
EMSCRIPTEN_KEEPALIVE lv_color_t lv_draw_label_dsc_get_color(lv_draw_label_dsc_t * p) { return p->color ; }
EMSCRIPTEN_KEEPALIVE lv_color_t lv_draw_label_dsc_get_sel_color(lv_draw_label_dsc_t * p) { return p->sel_color ; }
EMSCRIPTEN_KEEPALIVE lv_color_t lv_draw_label_dsc_get_sel_bg_color(lv_draw_label_dsc_t * p) { return p->sel_bg_color ; }
EMSCRIPTEN_KEEPALIVE lv_coord_t lv_draw_label_dsc_get_line_space(lv_draw_label_dsc_t * p) { return p->line_space ; }
EMSCRIPTEN_KEEPALIVE lv_coord_t lv_draw_label_dsc_get_letter_space(lv_draw_label_dsc_t * p) { return p->letter_space ; }
EMSCRIPTEN_KEEPALIVE lv_coord_t lv_draw_label_dsc_get_ofs_x(lv_draw_label_dsc_t * p) { return p->ofs_x ; }
EMSCRIPTEN_KEEPALIVE lv_coord_t lv_draw_label_dsc_get_ofs_y(lv_draw_label_dsc_t * p) { return p->ofs_y ; }
EMSCRIPTEN_KEEPALIVE lv_opa_t lv_draw_label_dsc_get_opa(lv_draw_label_dsc_t * p) { return p->opa ; }
EMSCRIPTEN_KEEPALIVE lv_base_dir_t lv_draw_label_dsc_get_bidi_dir(lv_draw_label_dsc_t * p) { return p->bidi_dir ; }
EMSCRIPTEN_KEEPALIVE lv_text_align_t lv_draw_label_dsc_get_align(lv_draw_label_dsc_t * p) { return p->align ; }
EMSCRIPTEN_KEEPALIVE lv_text_flag_t lv_draw_label_dsc_get_flag(lv_draw_label_dsc_t * p) { return p->flag ; }
EMSCRIPTEN_KEEPALIVE lv_text_decor_t lv_draw_label_dsc_get_decor(lv_draw_label_dsc_t * p) { return p->decor ; }
EMSCRIPTEN_KEEPALIVE lv_blend_mode_t lv_draw_label_dsc_get_blend_mode(lv_draw_label_dsc_t * p) { return p->blend_mode ; }
// fields setter
EMSCRIPTEN_KEEPALIVE void lv_draw_label_dsc_set_font(lv_draw_label_dsc_t * p, const lv_font_t * v) { p->font = v ; }
EMSCRIPTEN_KEEPALIVE void lv_draw_label_dsc_set_sel_start(lv_draw_label_dsc_t * p, uint32_t v) { p->sel_start = v ; }
EMSCRIPTEN_KEEPALIVE void lv_draw_label_dsc_set_sel_end(lv_draw_label_dsc_t * p, uint32_t v) { p->sel_end = v ; }
EMSCRIPTEN_KEEPALIVE void lv_draw_label_dsc_set_color(lv_draw_label_dsc_t * p, lv_color_t v) { p->color = v ; }
EMSCRIPTEN_KEEPALIVE void lv_draw_label_dsc_set_sel_color(lv_draw_label_dsc_t * p, lv_color_t v) { p->sel_color = v ; }
EMSCRIPTEN_KEEPALIVE void lv_draw_label_dsc_set_sel_bg_color(lv_draw_label_dsc_t * p, lv_color_t v) { p->sel_bg_color = v ; }
EMSCRIPTEN_KEEPALIVE void lv_draw_label_dsc_set_line_space(lv_draw_label_dsc_t * p, lv_coord_t v) { p->line_space = v ; }
EMSCRIPTEN_KEEPALIVE void lv_draw_label_dsc_set_letter_space(lv_draw_label_dsc_t * p, lv_coord_t v) { p->letter_space = v ; }
EMSCRIPTEN_KEEPALIVE void lv_draw_label_dsc_set_ofs_x(lv_draw_label_dsc_t * p, lv_coord_t v) { p->ofs_x = v ; }
EMSCRIPTEN_KEEPALIVE void lv_draw_label_dsc_set_ofs_y(lv_draw_label_dsc_t * p, lv_coord_t v) { p->ofs_y = v ; }
EMSCRIPTEN_KEEPALIVE void lv_draw_label_dsc_set_opa(lv_draw_label_dsc_t * p, lv_opa_t v) { p->opa = v ; }
EMSCRIPTEN_KEEPALIVE void lv_draw_label_dsc_set_bidi_dir(lv_draw_label_dsc_t * p, lv_base_dir_t v) { p->bidi_dir = v ; }
EMSCRIPTEN_KEEPALIVE void lv_draw_label_dsc_set_align(lv_draw_label_dsc_t * p, lv_text_align_t v) { p->align = v ; }
EMSCRIPTEN_KEEPALIVE void lv_draw_label_dsc_set_flag(lv_draw_label_dsc_t * p, lv_text_flag_t v) { p->flag = v ; }
EMSCRIPTEN_KEEPALIVE void lv_draw_label_dsc_set_decor(lv_draw_label_dsc_t * p, lv_text_decor_t v) { p->decor = v ; }
EMSCRIPTEN_KEEPALIVE void lv_draw_label_dsc_set_blend_mode(lv_draw_label_dsc_t * p, lv_blend_mode_t v) { p->blend_mode = v ; }


// struct lv_draw_label_hint_t wasm api ----------
EMSCRIPTEN_KEEPALIVE lv_draw_label_hint_t * lv_draw_label_hint_create() {
    lv_draw_label_hint_t * p = malloc(sizeof(lv_draw_label_hint_t)) ;
    memset(p, 0, sizeof(lv_draw_label_hint_t)) ;
    return p ;
}
// fields getter
EMSCRIPTEN_KEEPALIVE int32_t lv_draw_label_hint_get_line_start(lv_draw_label_hint_t * p) { return p->line_start ; }
EMSCRIPTEN_KEEPALIVE int32_t lv_draw_label_hint_get_y(lv_draw_label_hint_t * p) { return p->y ; }
EMSCRIPTEN_KEEPALIVE int32_t lv_draw_label_hint_get_coord_y(lv_draw_label_hint_t * p) { return p->coord_y ; }
// fields setter
EMSCRIPTEN_KEEPALIVE void lv_draw_label_hint_set_line_start(lv_draw_label_hint_t * p, int32_t v) { p->line_start = v ; }
EMSCRIPTEN_KEEPALIVE void lv_draw_label_hint_set_y(lv_draw_label_hint_t * p, int32_t v) { p->y = v ; }
EMSCRIPTEN_KEEPALIVE void lv_draw_label_hint_set_coord_y(lv_draw_label_hint_t * p, int32_t v) { p->coord_y = v ; }


// struct lv_draw_img_dsc_t wasm api ----------
EMSCRIPTEN_KEEPALIVE lv_draw_img_dsc_t * lv_draw_img_dsc_create() {
    lv_draw_img_dsc_t * p = malloc(sizeof(lv_draw_img_dsc_t)) ;
    memset(p, 0, sizeof(lv_draw_img_dsc_t)) ;
    return p ;
}
// fields getter
EMSCRIPTEN_KEEPALIVE uint16_t lv_draw_img_dsc_get_angle(lv_draw_img_dsc_t * p) { return p->angle ; }
EMSCRIPTEN_KEEPALIVE uint16_t lv_draw_img_dsc_get_zoom(lv_draw_img_dsc_t * p) { return p->zoom ; }
EMSCRIPTEN_KEEPALIVE lv_point_t lv_draw_img_dsc_get_pivot(lv_draw_img_dsc_t * p) { return p->pivot ; }
EMSCRIPTEN_KEEPALIVE lv_color_t lv_draw_img_dsc_get_recolor(lv_draw_img_dsc_t * p) { return p->recolor ; }
EMSCRIPTEN_KEEPALIVE lv_opa_t lv_draw_img_dsc_get_recolor_opa(lv_draw_img_dsc_t * p) { return p->recolor_opa ; }
EMSCRIPTEN_KEEPALIVE lv_opa_t lv_draw_img_dsc_get_opa(lv_draw_img_dsc_t * p) { return p->opa ; }
EMSCRIPTEN_KEEPALIVE lv_blend_mode_t lv_draw_img_dsc_get_blend_mode(lv_draw_img_dsc_t * p) { return p->blend_mode ; }
EMSCRIPTEN_KEEPALIVE int32_t lv_draw_img_dsc_get_frame_id(lv_draw_img_dsc_t * p) { return p->frame_id ; }
EMSCRIPTEN_KEEPALIVE uint8_t lv_draw_img_dsc_get_antialias(lv_draw_img_dsc_t * p) { return p->antialias ; }
// fields setter
EMSCRIPTEN_KEEPALIVE void lv_draw_img_dsc_set_angle(lv_draw_img_dsc_t * p, uint16_t v) { p->angle = v ; }
EMSCRIPTEN_KEEPALIVE void lv_draw_img_dsc_set_zoom(lv_draw_img_dsc_t * p, uint16_t v) { p->zoom = v ; }
EMSCRIPTEN_KEEPALIVE void lv_draw_img_dsc_set_pivot(lv_draw_img_dsc_t * p, lv_point_t v) { p->pivot = v ; }
EMSCRIPTEN_KEEPALIVE void lv_draw_img_dsc_set_recolor(lv_draw_img_dsc_t * p, lv_color_t v) { p->recolor = v ; }
EMSCRIPTEN_KEEPALIVE void lv_draw_img_dsc_set_recolor_opa(lv_draw_img_dsc_t * p, lv_opa_t v) { p->recolor_opa = v ; }
EMSCRIPTEN_KEEPALIVE void lv_draw_img_dsc_set_opa(lv_draw_img_dsc_t * p, lv_opa_t v) { p->opa = v ; }
EMSCRIPTEN_KEEPALIVE void lv_draw_img_dsc_set_blend_mode(lv_draw_img_dsc_t * p, lv_blend_mode_t v) { p->blend_mode = v ; }
EMSCRIPTEN_KEEPALIVE void lv_draw_img_dsc_set_frame_id(lv_draw_img_dsc_t * p, int32_t v) { p->frame_id = v ; }
EMSCRIPTEN_KEEPALIVE void lv_draw_img_dsc_set_antialias(lv_draw_img_dsc_t * p, uint8_t v) { p->antialias = v ; }


// struct lv_draw_line_dsc_t wasm api ----------
EMSCRIPTEN_KEEPALIVE lv_draw_line_dsc_t * lv_draw_line_dsc_create() {
    lv_draw_line_dsc_t * p = malloc(sizeof(lv_draw_line_dsc_t)) ;
    memset(p, 0, sizeof(lv_draw_line_dsc_t)) ;
    return p ;
}
// fields getter
EMSCRIPTEN_KEEPALIVE lv_color_t lv_draw_line_dsc_get_color(lv_draw_line_dsc_t * p) { return p->color ; }
EMSCRIPTEN_KEEPALIVE lv_coord_t lv_draw_line_dsc_get_width(lv_draw_line_dsc_t * p) { return p->width ; }
EMSCRIPTEN_KEEPALIVE lv_coord_t lv_draw_line_dsc_get_dash_width(lv_draw_line_dsc_t * p) { return p->dash_width ; }
EMSCRIPTEN_KEEPALIVE lv_coord_t lv_draw_line_dsc_get_dash_gap(lv_draw_line_dsc_t * p) { return p->dash_gap ; }
EMSCRIPTEN_KEEPALIVE lv_opa_t lv_draw_line_dsc_get_opa(lv_draw_line_dsc_t * p) { return p->opa ; }
EMSCRIPTEN_KEEPALIVE lv_blend_mode_t lv_draw_line_dsc_get_blend_mode(lv_draw_line_dsc_t * p) { return p->blend_mode ; }
EMSCRIPTEN_KEEPALIVE uint8_t lv_draw_line_dsc_get_round_start(lv_draw_line_dsc_t * p) { return p->round_start ; }
EMSCRIPTEN_KEEPALIVE uint8_t lv_draw_line_dsc_get_round_end(lv_draw_line_dsc_t * p) { return p->round_end ; }
EMSCRIPTEN_KEEPALIVE uint8_t lv_draw_line_dsc_get_raw_end(lv_draw_line_dsc_t * p) { return p->raw_end ; }
// fields setter
EMSCRIPTEN_KEEPALIVE void lv_draw_line_dsc_set_color(lv_draw_line_dsc_t * p, lv_color_t v) { p->color = v ; }
EMSCRIPTEN_KEEPALIVE void lv_draw_line_dsc_set_width(lv_draw_line_dsc_t * p, lv_coord_t v) { p->width = v ; }
EMSCRIPTEN_KEEPALIVE void lv_draw_line_dsc_set_dash_width(lv_draw_line_dsc_t * p, lv_coord_t v) { p->dash_width = v ; }
EMSCRIPTEN_KEEPALIVE void lv_draw_line_dsc_set_dash_gap(lv_draw_line_dsc_t * p, lv_coord_t v) { p->dash_gap = v ; }
EMSCRIPTEN_KEEPALIVE void lv_draw_line_dsc_set_opa(lv_draw_line_dsc_t * p, lv_opa_t v) { p->opa = v ; }
EMSCRIPTEN_KEEPALIVE void lv_draw_line_dsc_set_blend_mode(lv_draw_line_dsc_t * p, lv_blend_mode_t v) { p->blend_mode = v ; }
EMSCRIPTEN_KEEPALIVE void lv_draw_line_dsc_set_round_start(lv_draw_line_dsc_t * p, uint8_t v) { p->round_start = v ; }
EMSCRIPTEN_KEEPALIVE void lv_draw_line_dsc_set_round_end(lv_draw_line_dsc_t * p, uint8_t v) { p->round_end = v ; }
EMSCRIPTEN_KEEPALIVE void lv_draw_line_dsc_set_raw_end(lv_draw_line_dsc_t * p, uint8_t v) { p->raw_end = v ; }


// struct lv_draw_arc_dsc_t wasm api ----------
EMSCRIPTEN_KEEPALIVE lv_draw_arc_dsc_t * lv_draw_arc_dsc_create() {
    lv_draw_arc_dsc_t * p = malloc(sizeof(lv_draw_arc_dsc_t)) ;
    memset(p, 0, sizeof(lv_draw_arc_dsc_t)) ;
    return p ;
}
// fields getter
EMSCRIPTEN_KEEPALIVE lv_color_t lv_draw_arc_dsc_get_color(lv_draw_arc_dsc_t * p) { return p->color ; }
EMSCRIPTEN_KEEPALIVE lv_coord_t lv_draw_arc_dsc_get_width(lv_draw_arc_dsc_t * p) { return p->width ; }
EMSCRIPTEN_KEEPALIVE const void * lv_draw_arc_dsc_get_img_src(lv_draw_arc_dsc_t * p) { return p->img_src ; }
EMSCRIPTEN_KEEPALIVE lv_opa_t lv_draw_arc_dsc_get_opa(lv_draw_arc_dsc_t * p) { return p->opa ; }
EMSCRIPTEN_KEEPALIVE lv_blend_mode_t lv_draw_arc_dsc_get_blend_mode(lv_draw_arc_dsc_t * p) { return p->blend_mode ; }
EMSCRIPTEN_KEEPALIVE uint8_t lv_draw_arc_dsc_get_rounded(lv_draw_arc_dsc_t * p) { return p->rounded ; }
// fields setter
EMSCRIPTEN_KEEPALIVE void lv_draw_arc_dsc_set_color(lv_draw_arc_dsc_t * p, lv_color_t v) { p->color = v ; }
EMSCRIPTEN_KEEPALIVE void lv_draw_arc_dsc_set_width(lv_draw_arc_dsc_t * p, lv_coord_t v) { p->width = v ; }
EMSCRIPTEN_KEEPALIVE void lv_draw_arc_dsc_set_img_src(lv_draw_arc_dsc_t * p, const void * v) { p->img_src = v ; }
EMSCRIPTEN_KEEPALIVE void lv_draw_arc_dsc_set_opa(lv_draw_arc_dsc_t * p, lv_opa_t v) { p->opa = v ; }
EMSCRIPTEN_KEEPALIVE void lv_draw_arc_dsc_set_blend_mode(lv_draw_arc_dsc_t * p, lv_blend_mode_t v) { p->blend_mode = v ; }
EMSCRIPTEN_KEEPALIVE void lv_draw_arc_dsc_set_rounded(lv_draw_arc_dsc_t * p, uint8_t v) { p->rounded = v ; }


// struct lv_obj_draw_part_dsc_t wasm api ----------
EMSCRIPTEN_KEEPALIVE lv_obj_draw_part_dsc_t * lv_obj_draw_part_dsc_create() {
    lv_obj_draw_part_dsc_t * p = malloc(sizeof(lv_obj_draw_part_dsc_t)) ;
    memset(p, 0, sizeof(lv_obj_draw_part_dsc_t)) ;
    return p ;
}
// fields getter
EMSCRIPTEN_KEEPALIVE const lv_area_t * lv_obj_draw_part_dsc_get_clip_area(lv_obj_draw_part_dsc_t * p) { return p->clip_area ; }
EMSCRIPTEN_KEEPALIVE const struct _lv_obj_class_t * lv_obj_draw_part_dsc_get_class_p(lv_obj_draw_part_dsc_t * p) { return p->class_p ; }
EMSCRIPTEN_KEEPALIVE uint32_t lv_obj_draw_part_dsc_get_type(lv_obj_draw_part_dsc_t * p) { return p->type ; }
EMSCRIPTEN_KEEPALIVE lv_area_t * lv_obj_draw_part_dsc_get_draw_area(lv_obj_draw_part_dsc_t * p) { return p->draw_area ; }
EMSCRIPTEN_KEEPALIVE lv_draw_rect_dsc_t * lv_obj_draw_part_dsc_get_rect_dsc(lv_obj_draw_part_dsc_t * p) { return p->rect_dsc ; }
EMSCRIPTEN_KEEPALIVE lv_draw_label_dsc_t * lv_obj_draw_part_dsc_get_label_dsc(lv_obj_draw_part_dsc_t * p) { return p->label_dsc ; }
EMSCRIPTEN_KEEPALIVE lv_draw_line_dsc_t * lv_obj_draw_part_dsc_get_line_dsc(lv_obj_draw_part_dsc_t * p) { return p->line_dsc ; }
EMSCRIPTEN_KEEPALIVE lv_draw_img_dsc_t * lv_obj_draw_part_dsc_get_img_dsc(lv_obj_draw_part_dsc_t * p) { return p->img_dsc ; }
EMSCRIPTEN_KEEPALIVE lv_draw_arc_dsc_t * lv_obj_draw_part_dsc_get_arc_dsc(lv_obj_draw_part_dsc_t * p) { return p->arc_dsc ; }
EMSCRIPTEN_KEEPALIVE const lv_point_t * lv_obj_draw_part_dsc_get_p1(lv_obj_draw_part_dsc_t * p) { return p->p1 ; }
EMSCRIPTEN_KEEPALIVE const lv_point_t * lv_obj_draw_part_dsc_get_p2(lv_obj_draw_part_dsc_t * p) { return p->p2 ; }
EMSCRIPTEN_KEEPALIVE char * lv_obj_draw_part_dsc_get_text(lv_obj_draw_part_dsc_t * p) { return p->text ; }
EMSCRIPTEN_KEEPALIVE uint32_t lv_obj_draw_part_dsc_get_text_length(lv_obj_draw_part_dsc_t * p) { return p->text_length ; }
EMSCRIPTEN_KEEPALIVE uint32_t lv_obj_draw_part_dsc_get_part(lv_obj_draw_part_dsc_t * p) { return p->part ; }
EMSCRIPTEN_KEEPALIVE uint32_t lv_obj_draw_part_dsc_get_id(lv_obj_draw_part_dsc_t * p) { return p->id ; }
EMSCRIPTEN_KEEPALIVE lv_coord_t lv_obj_draw_part_dsc_get_radius(lv_obj_draw_part_dsc_t * p) { return p->radius ; }
EMSCRIPTEN_KEEPALIVE int32_t lv_obj_draw_part_dsc_get_value(lv_obj_draw_part_dsc_t * p) { return p->value ; }
EMSCRIPTEN_KEEPALIVE const void * lv_obj_draw_part_dsc_get_sub_part_ptr(lv_obj_draw_part_dsc_t * p) { return p->sub_part_ptr ; }
// fields setter
EMSCRIPTEN_KEEPALIVE void lv_obj_draw_part_dsc_set_clip_area(lv_obj_draw_part_dsc_t * p, const lv_area_t * v) { p->clip_area = v ; }
EMSCRIPTEN_KEEPALIVE void lv_obj_draw_part_dsc_set_class_p(lv_obj_draw_part_dsc_t * p, const struct _lv_obj_class_t * v) { p->class_p = v ; }
EMSCRIPTEN_KEEPALIVE void lv_obj_draw_part_dsc_set_type(lv_obj_draw_part_dsc_t * p, uint32_t v) { p->type = v ; }
EMSCRIPTEN_KEEPALIVE void lv_obj_draw_part_dsc_set_draw_area(lv_obj_draw_part_dsc_t * p, lv_area_t * v) { p->draw_area = v ; }
EMSCRIPTEN_KEEPALIVE void lv_obj_draw_part_dsc_set_rect_dsc(lv_obj_draw_part_dsc_t * p, lv_draw_rect_dsc_t * v) { p->rect_dsc = v ; }
EMSCRIPTEN_KEEPALIVE void lv_obj_draw_part_dsc_set_label_dsc(lv_obj_draw_part_dsc_t * p, lv_draw_label_dsc_t * v) { p->label_dsc = v ; }
EMSCRIPTEN_KEEPALIVE void lv_obj_draw_part_dsc_set_line_dsc(lv_obj_draw_part_dsc_t * p, lv_draw_line_dsc_t * v) { p->line_dsc = v ; }
EMSCRIPTEN_KEEPALIVE void lv_obj_draw_part_dsc_set_img_dsc(lv_obj_draw_part_dsc_t * p, lv_draw_img_dsc_t * v) { p->img_dsc = v ; }
EMSCRIPTEN_KEEPALIVE void lv_obj_draw_part_dsc_set_arc_dsc(lv_obj_draw_part_dsc_t * p, lv_draw_arc_dsc_t * v) { p->arc_dsc = v ; }
EMSCRIPTEN_KEEPALIVE void lv_obj_draw_part_dsc_set_p1(lv_obj_draw_part_dsc_t * p, const lv_point_t * v) { p->p1 = v ; }
EMSCRIPTEN_KEEPALIVE void lv_obj_draw_part_dsc_set_p2(lv_obj_draw_part_dsc_t * p, const lv_point_t * v) { p->p2 = v ; }
EMSCRIPTEN_KEEPALIVE void lv_obj_draw_part_dsc_set_text(lv_obj_draw_part_dsc_t * p, char * v) { p->text = v ; }
EMSCRIPTEN_KEEPALIVE void lv_obj_draw_part_dsc_set_text_length(lv_obj_draw_part_dsc_t * p, uint32_t v) { p->text_length = v ; }
EMSCRIPTEN_KEEPALIVE void lv_obj_draw_part_dsc_set_part(lv_obj_draw_part_dsc_t * p, uint32_t v) { p->part = v ; }
EMSCRIPTEN_KEEPALIVE void lv_obj_draw_part_dsc_set_id(lv_obj_draw_part_dsc_t * p, uint32_t v) { p->id = v ; }
EMSCRIPTEN_KEEPALIVE void lv_obj_draw_part_dsc_set_radius(lv_obj_draw_part_dsc_t * p, lv_coord_t v) { p->radius = v ; }
EMSCRIPTEN_KEEPALIVE void lv_obj_draw_part_dsc_set_value(lv_obj_draw_part_dsc_t * p, int32_t v) { p->value = v ; }
EMSCRIPTEN_KEEPALIVE void lv_obj_draw_part_dsc_set_sub_part_ptr(lv_obj_draw_part_dsc_t * p, const void * v) { p->sub_part_ptr = v ; }


// struct lv_obj_class_t wasm api ----------
EMSCRIPTEN_KEEPALIVE lv_obj_class_t * lv_obj_class_create() {
    lv_obj_class_t * p = malloc(sizeof(lv_obj_class_t)) ;
    memset(p, 0, sizeof(lv_obj_class_t)) ;
    return p ;
}
// fields getter
EMSCRIPTEN_KEEPALIVE const struct _lv_obj_class_t * lv_obj_class_get_base_class(lv_obj_class_t * p) { return p->base_class ; }
EMSCRIPTEN_KEEPALIVE void * lv_obj_class_get_constructor_cb(lv_obj_class_t * p) { return p->constructor_cb ; }
EMSCRIPTEN_KEEPALIVE void * lv_obj_class_get_destructor_cb(lv_obj_class_t * p) { return p->destructor_cb ; }
EMSCRIPTEN_KEEPALIVE void * lv_obj_class_get_user_data(lv_obj_class_t * p) { return p->user_data ; }
EMSCRIPTEN_KEEPALIVE void * lv_obj_class_get_event_cb(lv_obj_class_t * p) { return p->event_cb ; }
EMSCRIPTEN_KEEPALIVE lv_coord_t lv_obj_class_get_width_def(lv_obj_class_t * p) { return p->width_def ; }
EMSCRIPTEN_KEEPALIVE lv_coord_t lv_obj_class_get_height_def(lv_obj_class_t * p) { return p->height_def ; }
EMSCRIPTEN_KEEPALIVE uint32_t lv_obj_class_get_editable(lv_obj_class_t * p) { return p->editable ; }
EMSCRIPTEN_KEEPALIVE uint32_t lv_obj_class_get_group_def(lv_obj_class_t * p) { return p->group_def ; }
EMSCRIPTEN_KEEPALIVE uint32_t lv_obj_class_get_instance_size(lv_obj_class_t * p) { return p->instance_size ; }
// fields setter
EMSCRIPTEN_KEEPALIVE void lv_obj_class_set_base_class(lv_obj_class_t * p, const struct _lv_obj_class_t * v) { p->base_class = v ; }
EMSCRIPTEN_KEEPALIVE void lv_obj_class_set_constructor_cb(lv_obj_class_t * p, void * v) { p->constructor_cb = v ; }
EMSCRIPTEN_KEEPALIVE void lv_obj_class_set_destructor_cb(lv_obj_class_t * p, void * v) { p->destructor_cb = v ; }
EMSCRIPTEN_KEEPALIVE void lv_obj_class_set_user_data(lv_obj_class_t * p, void * v) { p->user_data = v ; }
EMSCRIPTEN_KEEPALIVE void lv_obj_class_set_event_cb(lv_obj_class_t * p, void * v) { p->event_cb = v ; }
EMSCRIPTEN_KEEPALIVE void lv_obj_class_set_width_def(lv_obj_class_t * p, lv_coord_t v) { p->width_def = v ; }
EMSCRIPTEN_KEEPALIVE void lv_obj_class_set_height_def(lv_obj_class_t * p, lv_coord_t v) { p->height_def = v ; }
EMSCRIPTEN_KEEPALIVE void lv_obj_class_set_editable(lv_obj_class_t * p, uint32_t v) { p->editable = v ; }
EMSCRIPTEN_KEEPALIVE void lv_obj_class_set_group_def(lv_obj_class_t * p, uint32_t v) { p->group_def = v ; }
EMSCRIPTEN_KEEPALIVE void lv_obj_class_set_instance_size(lv_obj_class_t * p, uint32_t v) { p->instance_size = v ; }


// struct lv_event_t wasm api ----------
EMSCRIPTEN_KEEPALIVE lv_event_t * lv_event_create() {
    lv_event_t * p = malloc(sizeof(lv_event_t)) ;
    memset(p, 0, sizeof(lv_event_t)) ;
    return p ;
}
// fields getter
EMSCRIPTEN_KEEPALIVE struct _lv_event_t * lv_event_get_prev(lv_event_t * p) { return p->prev ; }
EMSCRIPTEN_KEEPALIVE uint8_t lv_event_get_deleted(lv_event_t * p) { return p->deleted ; }
// fields setter
EMSCRIPTEN_KEEPALIVE void lv_event_set_target(lv_event_t * p, struct _lv_obj_t * v) { p->target = v ; }
EMSCRIPTEN_KEEPALIVE void lv_event_set_current_target(lv_event_t * p, struct _lv_obj_t * v) { p->current_target = v ; }
EMSCRIPTEN_KEEPALIVE void lv_event_set_code(lv_event_t * p, lv_event_code_t v) { p->code = v ; }
EMSCRIPTEN_KEEPALIVE void lv_event_set_user_data(lv_event_t * p, void * v) { p->user_data = v ; }
EMSCRIPTEN_KEEPALIVE void lv_event_set_param(lv_event_t * p, void * v) { p->param = v ; }
EMSCRIPTEN_KEEPALIVE void lv_event_set_prev(lv_event_t * p, struct _lv_event_t * v) { p->prev = v ; }
EMSCRIPTEN_KEEPALIVE void lv_event_set_deleted(lv_event_t * p, uint8_t v) { p->deleted = v ; }


// struct lv_hit_test_info_t wasm api ----------
EMSCRIPTEN_KEEPALIVE lv_hit_test_info_t * lv_hit_test_info_create() {
    lv_hit_test_info_t * p = malloc(sizeof(lv_hit_test_info_t)) ;
    memset(p, 0, sizeof(lv_hit_test_info_t)) ;
    return p ;
}
// fields getter
EMSCRIPTEN_KEEPALIVE const lv_point_t * lv_hit_test_info_get_point(lv_hit_test_info_t * p) { return p->point ; }
EMSCRIPTEN_KEEPALIVE bool lv_hit_test_info_get_res(lv_hit_test_info_t * p) { return p->res ; }
// fields setter
EMSCRIPTEN_KEEPALIVE void lv_hit_test_info_set_point(lv_hit_test_info_t * p, const lv_point_t * v) { p->point = v ; }
EMSCRIPTEN_KEEPALIVE void lv_hit_test_info_set_res(lv_hit_test_info_t * p, bool v) { p->res = v ; }


// struct lv_cover_check_info_t wasm api ----------
EMSCRIPTEN_KEEPALIVE lv_cover_check_info_t * lv_cover_check_info_create() {
    lv_cover_check_info_t * p = malloc(sizeof(lv_cover_check_info_t)) ;
    memset(p, 0, sizeof(lv_cover_check_info_t)) ;
    return p ;
}
// fields getter
EMSCRIPTEN_KEEPALIVE lv_cover_res_t lv_cover_check_info_get_res(lv_cover_check_info_t * p) { return p->res ; }
EMSCRIPTEN_KEEPALIVE const lv_area_t * lv_cover_check_info_get_area(lv_cover_check_info_t * p) { return p->area ; }
// fields setter
EMSCRIPTEN_KEEPALIVE void lv_cover_check_info_set_res(lv_cover_check_info_t * p, lv_cover_res_t v) { p->res = v ; }
EMSCRIPTEN_KEEPALIVE void lv_cover_check_info_set_area(lv_cover_check_info_t * p, const lv_area_t * v) { p->area = v ; }


// struct lv_group_t wasm api ----------
// fields getter
EMSCRIPTEN_KEEPALIVE lv_ll_t lv_group_get_obj_ll(lv_group_t * p) { return p->obj_ll ; }
EMSCRIPTEN_KEEPALIVE struct _lv_obj_t ** lv_group_get_obj_focus(lv_group_t * p) { return p->obj_focus ; }
EMSCRIPTEN_KEEPALIVE void * lv_group_get_user_data(lv_group_t * p) { return p->user_data ; }
EMSCRIPTEN_KEEPALIVE uint8_t lv_group_get_frozen(lv_group_t * p) { return p->frozen ; }
EMSCRIPTEN_KEEPALIVE uint8_t lv_group_get_refocus_policy(lv_group_t * p) { return p->refocus_policy ; }
// fields setter
EMSCRIPTEN_KEEPALIVE void lv_group_set_obj_ll(lv_group_t * p, lv_ll_t v) { p->obj_ll = v ; }
EMSCRIPTEN_KEEPALIVE void lv_group_set_obj_focus(lv_group_t * p, struct _lv_obj_t ** v) { p->obj_focus = v ; }
EMSCRIPTEN_KEEPALIVE void lv_group_set_user_data(lv_group_t * p, void * v) { p->user_data = v ; }
EMSCRIPTEN_KEEPALIVE void lv_group_set_frozen(lv_group_t * p, uint8_t v) { p->frozen = v ; }


// struct _lv_obj_spec_attr_t wasm api ----------
EMSCRIPTEN_KEEPALIVE _lv_obj_spec_attr_t * _lv_obj_spec_attr_create() {
    _lv_obj_spec_attr_t * p = malloc(sizeof(_lv_obj_spec_attr_t)) ;
    memset(p, 0, sizeof(_lv_obj_spec_attr_t)) ;
    return p ;
}
// fields getter
EMSCRIPTEN_KEEPALIVE struct _lv_obj_t ** _lv_obj_spec_attr_get_children(_lv_obj_spec_attr_t * p) { return p->children ; }
EMSCRIPTEN_KEEPALIVE uint32_t _lv_obj_spec_attr_get_child_cnt(_lv_obj_spec_attr_t * p) { return p->child_cnt ; }
EMSCRIPTEN_KEEPALIVE lv_group_t * _lv_obj_spec_attr_get_group_p(_lv_obj_spec_attr_t * p) { return p->group_p ; }
EMSCRIPTEN_KEEPALIVE struct _lv_event_dsc_t * _lv_obj_spec_attr_get_event_dsc(_lv_obj_spec_attr_t * p) { return p->event_dsc ; }
EMSCRIPTEN_KEEPALIVE lv_point_t _lv_obj_spec_attr_get_scroll(_lv_obj_spec_attr_t * p) { return p->scroll ; }
EMSCRIPTEN_KEEPALIVE lv_coord_t _lv_obj_spec_attr_get_ext_click_pad(_lv_obj_spec_attr_t * p) { return p->ext_click_pad ; }
EMSCRIPTEN_KEEPALIVE lv_coord_t _lv_obj_spec_attr_get_ext_draw_size(_lv_obj_spec_attr_t * p) { return p->ext_draw_size ; }
EMSCRIPTEN_KEEPALIVE lv_scrollbar_mode_t _lv_obj_spec_attr_get_scrollbar_mode(_lv_obj_spec_attr_t * p) { return p->scrollbar_mode ; }
EMSCRIPTEN_KEEPALIVE lv_scroll_snap_t _lv_obj_spec_attr_get_scroll_snap_x(_lv_obj_spec_attr_t * p) { return p->scroll_snap_x ; }
EMSCRIPTEN_KEEPALIVE lv_scroll_snap_t _lv_obj_spec_attr_get_scroll_snap_y(_lv_obj_spec_attr_t * p) { return p->scroll_snap_y ; }
EMSCRIPTEN_KEEPALIVE lv_dir_t _lv_obj_spec_attr_get_scroll_dir(_lv_obj_spec_attr_t * p) { return p->scroll_dir ; }
EMSCRIPTEN_KEEPALIVE uint8_t _lv_obj_spec_attr_get_event_dsc_cnt(_lv_obj_spec_attr_t * p) { return p->event_dsc_cnt ; }
// fields setter
EMSCRIPTEN_KEEPALIVE void _lv_obj_spec_attr_set_children(_lv_obj_spec_attr_t * p, struct _lv_obj_t ** v) { p->children = v ; }
EMSCRIPTEN_KEEPALIVE void _lv_obj_spec_attr_set_child_cnt(_lv_obj_spec_attr_t * p, uint32_t v) { p->child_cnt = v ; }
EMSCRIPTEN_KEEPALIVE void _lv_obj_spec_attr_set_group_p(_lv_obj_spec_attr_t * p, lv_group_t * v) { p->group_p = v ; }
EMSCRIPTEN_KEEPALIVE void _lv_obj_spec_attr_set_event_dsc(_lv_obj_spec_attr_t * p, struct _lv_event_dsc_t * v) { p->event_dsc = v ; }
EMSCRIPTEN_KEEPALIVE void _lv_obj_spec_attr_set_scroll(_lv_obj_spec_attr_t * p, lv_point_t v) { p->scroll = v ; }
EMSCRIPTEN_KEEPALIVE void _lv_obj_spec_attr_set_ext_click_pad(_lv_obj_spec_attr_t * p, lv_coord_t v) { p->ext_click_pad = v ; }
EMSCRIPTEN_KEEPALIVE void _lv_obj_spec_attr_set_ext_draw_size(_lv_obj_spec_attr_t * p, lv_coord_t v) { p->ext_draw_size = v ; }
EMSCRIPTEN_KEEPALIVE void _lv_obj_spec_attr_set_scrollbar_mode(_lv_obj_spec_attr_t * p, lv_scrollbar_mode_t v) { p->scrollbar_mode = v ; }
EMSCRIPTEN_KEEPALIVE void _lv_obj_spec_attr_set_scroll_snap_x(_lv_obj_spec_attr_t * p, lv_scroll_snap_t v) { p->scroll_snap_x = v ; }
EMSCRIPTEN_KEEPALIVE void _lv_obj_spec_attr_set_scroll_snap_y(_lv_obj_spec_attr_t * p, lv_scroll_snap_t v) { p->scroll_snap_y = v ; }
EMSCRIPTEN_KEEPALIVE void _lv_obj_spec_attr_set_scroll_dir(_lv_obj_spec_attr_t * p, lv_dir_t v) { p->scroll_dir = v ; }
EMSCRIPTEN_KEEPALIVE void _lv_obj_spec_attr_set_event_dsc_cnt(_lv_obj_spec_attr_t * p, uint8_t v) { p->event_dsc_cnt = v ; }


// struct lv_obj_t wasm api ----------
// fields getter
EMSCRIPTEN_KEEPALIVE const lv_obj_class_t * lv_obj_get_class_p(lv_obj_t * p) { return p->class_p ; }
EMSCRIPTEN_KEEPALIVE _lv_obj_spec_attr_t * lv_obj_get_spec_attr(lv_obj_t * p) { return p->spec_attr ; }
EMSCRIPTEN_KEEPALIVE _lv_obj_style_t * lv_obj_get_styles(lv_obj_t * p) { return p->styles ; }
EMSCRIPTEN_KEEPALIVE lv_obj_flag_t lv_obj_get_flags(lv_obj_t * p) { return p->flags ; }
EMSCRIPTEN_KEEPALIVE uint16_t lv_obj_get_layout_inv(lv_obj_t * p) { return p->layout_inv ; }
EMSCRIPTEN_KEEPALIVE uint16_t lv_obj_get_scr_layout_inv(lv_obj_t * p) { return p->scr_layout_inv ; }
EMSCRIPTEN_KEEPALIVE uint16_t lv_obj_get_skip_trans(lv_obj_t * p) { return p->skip_trans ; }
EMSCRIPTEN_KEEPALIVE uint16_t lv_obj_get_style_cnt(lv_obj_t * p) { return p->style_cnt ; }
EMSCRIPTEN_KEEPALIVE uint16_t lv_obj_get_h_layout(lv_obj_t * p) { return p->h_layout ; }
EMSCRIPTEN_KEEPALIVE uint16_t lv_obj_get_w_layout(lv_obj_t * p) { return p->w_layout ; }
// fields setter
EMSCRIPTEN_KEEPALIVE void lv_obj_set_class_p(lv_obj_t * p, const lv_obj_class_t * v) { p->class_p = v ; }
EMSCRIPTEN_KEEPALIVE void lv_obj_set_spec_attr(lv_obj_t * p, _lv_obj_spec_attr_t * v) { p->spec_attr = v ; }
EMSCRIPTEN_KEEPALIVE void lv_obj_set_styles(lv_obj_t * p, _lv_obj_style_t * v) { p->styles = v ; }
EMSCRIPTEN_KEEPALIVE void lv_obj_set_coords(lv_obj_t * p, lv_area_t v) { p->coords = v ; }
EMSCRIPTEN_KEEPALIVE void lv_obj_set_flags(lv_obj_t * p, lv_obj_flag_t v) { p->flags = v ; }
EMSCRIPTEN_KEEPALIVE void lv_obj_set_state(lv_obj_t * p, lv_state_t v) { p->state = v ; }
EMSCRIPTEN_KEEPALIVE void lv_obj_set_layout_inv(lv_obj_t * p, uint16_t v) { p->layout_inv = v ; }
EMSCRIPTEN_KEEPALIVE void lv_obj_set_scr_layout_inv(lv_obj_t * p, uint16_t v) { p->scr_layout_inv = v ; }
EMSCRIPTEN_KEEPALIVE void lv_obj_set_skip_trans(lv_obj_t * p, uint16_t v) { p->skip_trans = v ; }
EMSCRIPTEN_KEEPALIVE void lv_obj_set_style_cnt(lv_obj_t * p, uint16_t v) { p->style_cnt = v ; }
EMSCRIPTEN_KEEPALIVE void lv_obj_set_h_layout(lv_obj_t * p, uint16_t v) { p->h_layout = v ; }
EMSCRIPTEN_KEEPALIVE void lv_obj_set_w_layout(lv_obj_t * p, uint16_t v) { p->w_layout = v ; }


// struct lv_theme_t wasm api ----------
EMSCRIPTEN_KEEPALIVE lv_theme_t * lv_theme_create() {
    lv_theme_t * p = malloc(sizeof(lv_theme_t)) ;
    memset(p, 0, sizeof(lv_theme_t)) ;
    return p ;
}
// fields getter
EMSCRIPTEN_KEEPALIVE lv_theme_apply_cb_t lv_theme_get_apply_cb(lv_theme_t * p) { return p->apply_cb ; }
EMSCRIPTEN_KEEPALIVE struct _lv_theme_t * lv_theme_get_parent(lv_theme_t * p) { return p->parent ; }
EMSCRIPTEN_KEEPALIVE void * lv_theme_get_user_data(lv_theme_t * p) { return p->user_data ; }
EMSCRIPTEN_KEEPALIVE struct _lv_disp_t * lv_theme_get_disp(lv_theme_t * p) { return p->disp ; }
EMSCRIPTEN_KEEPALIVE uint32_t lv_theme_get_flags(lv_theme_t * p) { return p->flags ; }
// fields setter
EMSCRIPTEN_KEEPALIVE void lv_theme_set_user_data(lv_theme_t * p, void * v) { p->user_data = v ; }
EMSCRIPTEN_KEEPALIVE void lv_theme_set_disp(lv_theme_t * p, struct _lv_disp_t * v) { p->disp = v ; }
EMSCRIPTEN_KEEPALIVE void lv_theme_set_color_primary(lv_theme_t * p, lv_color_t v) { p->color_primary = v ; }
EMSCRIPTEN_KEEPALIVE void lv_theme_set_color_secondary(lv_theme_t * p, lv_color_t v) { p->color_secondary = v ; }
EMSCRIPTEN_KEEPALIVE void lv_theme_set_font_small(lv_theme_t * p, const lv_font_t * v) { p->font_small = v ; }
EMSCRIPTEN_KEEPALIVE void lv_theme_set_font_normal(lv_theme_t * p, const lv_font_t * v) { p->font_normal = v ; }
EMSCRIPTEN_KEEPALIVE void lv_theme_set_font_large(lv_theme_t * p, const lv_font_t * v) { p->font_large = v ; }
EMSCRIPTEN_KEEPALIVE void lv_theme_set_flags(lv_theme_t * p, uint32_t v) { p->flags = v ; }


// struct lv_font_fmt_txt_glyph_dsc_t wasm api ----------
EMSCRIPTEN_KEEPALIVE lv_font_fmt_txt_glyph_dsc_t * lv_font_fmt_txt_glyph_dsc_create() {
    lv_font_fmt_txt_glyph_dsc_t * p = malloc(sizeof(lv_font_fmt_txt_glyph_dsc_t)) ;
    memset(p, 0, sizeof(lv_font_fmt_txt_glyph_dsc_t)) ;
    return p ;
}
// fields getter
EMSCRIPTEN_KEEPALIVE uint32_t lv_font_fmt_txt_glyph_dsc_get_bitmap_index(lv_font_fmt_txt_glyph_dsc_t * p) { return p->bitmap_index ; }
EMSCRIPTEN_KEEPALIVE uint32_t lv_font_fmt_txt_glyph_dsc_get_adv_w(lv_font_fmt_txt_glyph_dsc_t * p) { return p->adv_w ; }
EMSCRIPTEN_KEEPALIVE uint8_t lv_font_fmt_txt_glyph_dsc_get_box_w(lv_font_fmt_txt_glyph_dsc_t * p) { return p->box_w ; }
EMSCRIPTEN_KEEPALIVE uint8_t lv_font_fmt_txt_glyph_dsc_get_box_h(lv_font_fmt_txt_glyph_dsc_t * p) { return p->box_h ; }
EMSCRIPTEN_KEEPALIVE int8_t lv_font_fmt_txt_glyph_dsc_get_ofs_x(lv_font_fmt_txt_glyph_dsc_t * p) { return p->ofs_x ; }
EMSCRIPTEN_KEEPALIVE int8_t lv_font_fmt_txt_glyph_dsc_get_ofs_y(lv_font_fmt_txt_glyph_dsc_t * p) { return p->ofs_y ; }
// fields setter
EMSCRIPTEN_KEEPALIVE void lv_font_fmt_txt_glyph_dsc_set_bitmap_index(lv_font_fmt_txt_glyph_dsc_t * p, uint32_t v) { p->bitmap_index = v ; }
EMSCRIPTEN_KEEPALIVE void lv_font_fmt_txt_glyph_dsc_set_adv_w(lv_font_fmt_txt_glyph_dsc_t * p, uint32_t v) { p->adv_w = v ; }
EMSCRIPTEN_KEEPALIVE void lv_font_fmt_txt_glyph_dsc_set_box_w(lv_font_fmt_txt_glyph_dsc_t * p, uint8_t v) { p->box_w = v ; }
EMSCRIPTEN_KEEPALIVE void lv_font_fmt_txt_glyph_dsc_set_box_h(lv_font_fmt_txt_glyph_dsc_t * p, uint8_t v) { p->box_h = v ; }
EMSCRIPTEN_KEEPALIVE void lv_font_fmt_txt_glyph_dsc_set_ofs_x(lv_font_fmt_txt_glyph_dsc_t * p, int8_t v) { p->ofs_x = v ; }
EMSCRIPTEN_KEEPALIVE void lv_font_fmt_txt_glyph_dsc_set_ofs_y(lv_font_fmt_txt_glyph_dsc_t * p, int8_t v) { p->ofs_y = v ; }


// struct lv_font_fmt_txt_cmap_t wasm api ----------
EMSCRIPTEN_KEEPALIVE lv_font_fmt_txt_cmap_t * lv_font_fmt_txt_cmap_create() {
    lv_font_fmt_txt_cmap_t * p = malloc(sizeof(lv_font_fmt_txt_cmap_t)) ;
    memset(p, 0, sizeof(lv_font_fmt_txt_cmap_t)) ;
    return p ;
}
// fields getter
EMSCRIPTEN_KEEPALIVE uint32_t lv_font_fmt_txt_cmap_get_range_start(lv_font_fmt_txt_cmap_t * p) { return p->range_start ; }
EMSCRIPTEN_KEEPALIVE uint16_t lv_font_fmt_txt_cmap_get_range_length(lv_font_fmt_txt_cmap_t * p) { return p->range_length ; }
EMSCRIPTEN_KEEPALIVE uint16_t lv_font_fmt_txt_cmap_get_glyph_id_start(lv_font_fmt_txt_cmap_t * p) { return p->glyph_id_start ; }
EMSCRIPTEN_KEEPALIVE const uint16_t * lv_font_fmt_txt_cmap_get_unicode_list(lv_font_fmt_txt_cmap_t * p) { return p->unicode_list ; }
EMSCRIPTEN_KEEPALIVE const void * lv_font_fmt_txt_cmap_get_glyph_id_ofs_list(lv_font_fmt_txt_cmap_t * p) { return p->glyph_id_ofs_list ; }
EMSCRIPTEN_KEEPALIVE uint16_t lv_font_fmt_txt_cmap_get_list_length(lv_font_fmt_txt_cmap_t * p) { return p->list_length ; }
EMSCRIPTEN_KEEPALIVE lv_font_fmt_txt_cmap_type_t lv_font_fmt_txt_cmap_get_type(lv_font_fmt_txt_cmap_t * p) { return p->type ; }
// fields setter
EMSCRIPTEN_KEEPALIVE void lv_font_fmt_txt_cmap_set_range_start(lv_font_fmt_txt_cmap_t * p, uint32_t v) { p->range_start = v ; }
EMSCRIPTEN_KEEPALIVE void lv_font_fmt_txt_cmap_set_range_length(lv_font_fmt_txt_cmap_t * p, uint16_t v) { p->range_length = v ; }
EMSCRIPTEN_KEEPALIVE void lv_font_fmt_txt_cmap_set_glyph_id_start(lv_font_fmt_txt_cmap_t * p, uint16_t v) { p->glyph_id_start = v ; }
EMSCRIPTEN_KEEPALIVE void lv_font_fmt_txt_cmap_set_unicode_list(lv_font_fmt_txt_cmap_t * p, const uint16_t * v) { p->unicode_list = v ; }
EMSCRIPTEN_KEEPALIVE void lv_font_fmt_txt_cmap_set_glyph_id_ofs_list(lv_font_fmt_txt_cmap_t * p, const void * v) { p->glyph_id_ofs_list = v ; }
EMSCRIPTEN_KEEPALIVE void lv_font_fmt_txt_cmap_set_list_length(lv_font_fmt_txt_cmap_t * p, uint16_t v) { p->list_length = v ; }
EMSCRIPTEN_KEEPALIVE void lv_font_fmt_txt_cmap_set_type(lv_font_fmt_txt_cmap_t * p, lv_font_fmt_txt_cmap_type_t v) { p->type = v ; }


// struct lv_font_fmt_txt_kern_pair_t wasm api ----------
EMSCRIPTEN_KEEPALIVE lv_font_fmt_txt_kern_pair_t * lv_font_fmt_txt_kern_pair_create() {
    lv_font_fmt_txt_kern_pair_t * p = malloc(sizeof(lv_font_fmt_txt_kern_pair_t)) ;
    memset(p, 0, sizeof(lv_font_fmt_txt_kern_pair_t)) ;
    return p ;
}
// fields getter
EMSCRIPTEN_KEEPALIVE const void * lv_font_fmt_txt_kern_pair_get_glyph_ids(lv_font_fmt_txt_kern_pair_t * p) { return p->glyph_ids ; }
EMSCRIPTEN_KEEPALIVE const int8_t * lv_font_fmt_txt_kern_pair_get_values(lv_font_fmt_txt_kern_pair_t * p) { return p->values ; }
EMSCRIPTEN_KEEPALIVE uint32_t lv_font_fmt_txt_kern_pair_get_pair_cnt(lv_font_fmt_txt_kern_pair_t * p) { return p->pair_cnt ; }
EMSCRIPTEN_KEEPALIVE uint32_t lv_font_fmt_txt_kern_pair_get_glyph_ids_size(lv_font_fmt_txt_kern_pair_t * p) { return p->glyph_ids_size ; }
// fields setter
EMSCRIPTEN_KEEPALIVE void lv_font_fmt_txt_kern_pair_set_glyph_ids(lv_font_fmt_txt_kern_pair_t * p, const void * v) { p->glyph_ids = v ; }
EMSCRIPTEN_KEEPALIVE void lv_font_fmt_txt_kern_pair_set_values(lv_font_fmt_txt_kern_pair_t * p, const int8_t * v) { p->values = v ; }
EMSCRIPTEN_KEEPALIVE void lv_font_fmt_txt_kern_pair_set_pair_cnt(lv_font_fmt_txt_kern_pair_t * p, uint32_t v) { p->pair_cnt = v ; }
EMSCRIPTEN_KEEPALIVE void lv_font_fmt_txt_kern_pair_set_glyph_ids_size(lv_font_fmt_txt_kern_pair_t * p, uint32_t v) { p->glyph_ids_size = v ; }


// struct lv_font_fmt_txt_kern_classes_t wasm api ----------
EMSCRIPTEN_KEEPALIVE lv_font_fmt_txt_kern_classes_t * lv_font_fmt_txt_kern_classes_create() {
    lv_font_fmt_txt_kern_classes_t * p = malloc(sizeof(lv_font_fmt_txt_kern_classes_t)) ;
    memset(p, 0, sizeof(lv_font_fmt_txt_kern_classes_t)) ;
    return p ;
}
// fields getter
EMSCRIPTEN_KEEPALIVE const int8_t * lv_font_fmt_txt_kern_classes_get_class_pair_values(lv_font_fmt_txt_kern_classes_t * p) { return p->class_pair_values ; }
EMSCRIPTEN_KEEPALIVE const uint8_t * lv_font_fmt_txt_kern_classes_get_left_class_mapping(lv_font_fmt_txt_kern_classes_t * p) { return p->left_class_mapping ; }
EMSCRIPTEN_KEEPALIVE const uint8_t * lv_font_fmt_txt_kern_classes_get_right_class_mapping(lv_font_fmt_txt_kern_classes_t * p) { return p->right_class_mapping ; }
EMSCRIPTEN_KEEPALIVE uint8_t lv_font_fmt_txt_kern_classes_get_left_class_cnt(lv_font_fmt_txt_kern_classes_t * p) { return p->left_class_cnt ; }
EMSCRIPTEN_KEEPALIVE uint8_t lv_font_fmt_txt_kern_classes_get_right_class_cnt(lv_font_fmt_txt_kern_classes_t * p) { return p->right_class_cnt ; }
// fields setter
EMSCRIPTEN_KEEPALIVE void lv_font_fmt_txt_kern_classes_set_class_pair_values(lv_font_fmt_txt_kern_classes_t * p, const int8_t * v) { p->class_pair_values = v ; }
EMSCRIPTEN_KEEPALIVE void lv_font_fmt_txt_kern_classes_set_left_class_mapping(lv_font_fmt_txt_kern_classes_t * p, const uint8_t * v) { p->left_class_mapping = v ; }
EMSCRIPTEN_KEEPALIVE void lv_font_fmt_txt_kern_classes_set_right_class_mapping(lv_font_fmt_txt_kern_classes_t * p, const uint8_t * v) { p->right_class_mapping = v ; }
EMSCRIPTEN_KEEPALIVE void lv_font_fmt_txt_kern_classes_set_left_class_cnt(lv_font_fmt_txt_kern_classes_t * p, uint8_t v) { p->left_class_cnt = v ; }
EMSCRIPTEN_KEEPALIVE void lv_font_fmt_txt_kern_classes_set_right_class_cnt(lv_font_fmt_txt_kern_classes_t * p, uint8_t v) { p->right_class_cnt = v ; }


// struct lv_font_fmt_txt_glyph_cache_t wasm api ----------
EMSCRIPTEN_KEEPALIVE lv_font_fmt_txt_glyph_cache_t * lv_font_fmt_txt_glyph_cache_create() {
    lv_font_fmt_txt_glyph_cache_t * p = malloc(sizeof(lv_font_fmt_txt_glyph_cache_t)) ;
    memset(p, 0, sizeof(lv_font_fmt_txt_glyph_cache_t)) ;
    return p ;
}
// fields getter
EMSCRIPTEN_KEEPALIVE uint32_t lv_font_fmt_txt_glyph_cache_get_last_letter(lv_font_fmt_txt_glyph_cache_t * p) { return p->last_letter ; }
EMSCRIPTEN_KEEPALIVE uint32_t lv_font_fmt_txt_glyph_cache_get_last_glyph_id(lv_font_fmt_txt_glyph_cache_t * p) { return p->last_glyph_id ; }
// fields setter
EMSCRIPTEN_KEEPALIVE void lv_font_fmt_txt_glyph_cache_set_last_letter(lv_font_fmt_txt_glyph_cache_t * p, uint32_t v) { p->last_letter = v ; }
EMSCRIPTEN_KEEPALIVE void lv_font_fmt_txt_glyph_cache_set_last_glyph_id(lv_font_fmt_txt_glyph_cache_t * p, uint32_t v) { p->last_glyph_id = v ; }


// struct lv_font_fmt_txt_dsc_t wasm api ----------
EMSCRIPTEN_KEEPALIVE lv_font_fmt_txt_dsc_t * lv_font_fmt_txt_dsc_create() {
    lv_font_fmt_txt_dsc_t * p = malloc(sizeof(lv_font_fmt_txt_dsc_t)) ;
    memset(p, 0, sizeof(lv_font_fmt_txt_dsc_t)) ;
    return p ;
}
// fields getter
EMSCRIPTEN_KEEPALIVE const uint8_t * lv_font_fmt_txt_dsc_get_glyph_bitmap(lv_font_fmt_txt_dsc_t * p) { return p->glyph_bitmap ; }
EMSCRIPTEN_KEEPALIVE const lv_font_fmt_txt_glyph_dsc_t * lv_font_fmt_txt_dsc_get_glyph_dsc(lv_font_fmt_txt_dsc_t * p) { return p->glyph_dsc ; }
EMSCRIPTEN_KEEPALIVE const lv_font_fmt_txt_cmap_t * lv_font_fmt_txt_dsc_get_cmaps(lv_font_fmt_txt_dsc_t * p) { return p->cmaps ; }
EMSCRIPTEN_KEEPALIVE const void * lv_font_fmt_txt_dsc_get_kern_dsc(lv_font_fmt_txt_dsc_t * p) { return p->kern_dsc ; }
EMSCRIPTEN_KEEPALIVE uint16_t lv_font_fmt_txt_dsc_get_kern_scale(lv_font_fmt_txt_dsc_t * p) { return p->kern_scale ; }
EMSCRIPTEN_KEEPALIVE uint16_t lv_font_fmt_txt_dsc_get_cmap_num(lv_font_fmt_txt_dsc_t * p) { return p->cmap_num ; }
EMSCRIPTEN_KEEPALIVE uint16_t lv_font_fmt_txt_dsc_get_bpp(lv_font_fmt_txt_dsc_t * p) { return p->bpp ; }
EMSCRIPTEN_KEEPALIVE uint16_t lv_font_fmt_txt_dsc_get_kern_classes(lv_font_fmt_txt_dsc_t * p) { return p->kern_classes ; }
EMSCRIPTEN_KEEPALIVE uint16_t lv_font_fmt_txt_dsc_get_bitmap_format(lv_font_fmt_txt_dsc_t * p) { return p->bitmap_format ; }
EMSCRIPTEN_KEEPALIVE lv_font_fmt_txt_glyph_cache_t * lv_font_fmt_txt_dsc_get_cache(lv_font_fmt_txt_dsc_t * p) { return p->cache ; }
// fields setter
EMSCRIPTEN_KEEPALIVE void lv_font_fmt_txt_dsc_set_glyph_bitmap(lv_font_fmt_txt_dsc_t * p, const uint8_t * v) { p->glyph_bitmap = v ; }
EMSCRIPTEN_KEEPALIVE void lv_font_fmt_txt_dsc_set_glyph_dsc(lv_font_fmt_txt_dsc_t * p, const lv_font_fmt_txt_glyph_dsc_t * v) { p->glyph_dsc = v ; }
EMSCRIPTEN_KEEPALIVE void lv_font_fmt_txt_dsc_set_cmaps(lv_font_fmt_txt_dsc_t * p, const lv_font_fmt_txt_cmap_t * v) { p->cmaps = v ; }
EMSCRIPTEN_KEEPALIVE void lv_font_fmt_txt_dsc_set_kern_dsc(lv_font_fmt_txt_dsc_t * p, const void * v) { p->kern_dsc = v ; }
EMSCRIPTEN_KEEPALIVE void lv_font_fmt_txt_dsc_set_kern_scale(lv_font_fmt_txt_dsc_t * p, uint16_t v) { p->kern_scale = v ; }
EMSCRIPTEN_KEEPALIVE void lv_font_fmt_txt_dsc_set_cmap_num(lv_font_fmt_txt_dsc_t * p, uint16_t v) { p->cmap_num = v ; }
EMSCRIPTEN_KEEPALIVE void lv_font_fmt_txt_dsc_set_bpp(lv_font_fmt_txt_dsc_t * p, uint16_t v) { p->bpp = v ; }
EMSCRIPTEN_KEEPALIVE void lv_font_fmt_txt_dsc_set_kern_classes(lv_font_fmt_txt_dsc_t * p, uint16_t v) { p->kern_classes = v ; }
EMSCRIPTEN_KEEPALIVE void lv_font_fmt_txt_dsc_set_bitmap_format(lv_font_fmt_txt_dsc_t * p, uint16_t v) { p->bitmap_format = v ; }
EMSCRIPTEN_KEEPALIVE void lv_font_fmt_txt_dsc_set_cache(lv_font_fmt_txt_dsc_t * p, lv_font_fmt_txt_glyph_cache_t * v) { p->cache = v ; }


// struct lv_arc_t wasm api ----------
// fields getter
EMSCRIPTEN_KEEPALIVE lv_obj_t lv_arc_get_obj(lv_arc_t * p) { return p->obj ; }
EMSCRIPTEN_KEEPALIVE uint16_t lv_arc_get_rotation(lv_arc_t * p) { return p->rotation ; }
EMSCRIPTEN_KEEPALIVE uint16_t lv_arc_get_indic_angle_start(lv_arc_t * p) { return p->indic_angle_start ; }
EMSCRIPTEN_KEEPALIVE uint16_t lv_arc_get_indic_angle_end(lv_arc_t * p) { return p->indic_angle_end ; }
EMSCRIPTEN_KEEPALIVE uint16_t lv_arc_get_dragging(lv_arc_t * p) { return p->dragging ; }
EMSCRIPTEN_KEEPALIVE uint16_t lv_arc_get_type(lv_arc_t * p) { return p->type ; }
EMSCRIPTEN_KEEPALIVE uint16_t lv_arc_get_min_close(lv_arc_t * p) { return p->min_close ; }
EMSCRIPTEN_KEEPALIVE uint16_t lv_arc_get_chg_rate(lv_arc_t * p) { return p->chg_rate ; }
EMSCRIPTEN_KEEPALIVE uint32_t lv_arc_get_last_tick(lv_arc_t * p) { return p->last_tick ; }
EMSCRIPTEN_KEEPALIVE int16_t lv_arc_get_last_angle(lv_arc_t * p) { return p->last_angle ; }
// fields setter
EMSCRIPTEN_KEEPALIVE void lv_arc_set_obj(lv_arc_t * p, lv_obj_t v) { p->obj = v ; }
EMSCRIPTEN_KEEPALIVE void lv_arc_set_indic_angle_start(lv_arc_t * p, uint16_t v) { p->indic_angle_start = v ; }
EMSCRIPTEN_KEEPALIVE void lv_arc_set_indic_angle_end(lv_arc_t * p, uint16_t v) { p->indic_angle_end = v ; }
EMSCRIPTEN_KEEPALIVE void lv_arc_set_bg_angle_start(lv_arc_t * p, uint16_t v) { p->bg_angle_start = v ; }
EMSCRIPTEN_KEEPALIVE void lv_arc_set_bg_angle_end(lv_arc_t * p, uint16_t v) { p->bg_angle_end = v ; }
EMSCRIPTEN_KEEPALIVE void lv_arc_set_min_value(lv_arc_t * p, int16_t v) { p->min_value = v ; }
EMSCRIPTEN_KEEPALIVE void lv_arc_set_max_value(lv_arc_t * p, int16_t v) { p->max_value = v ; }
EMSCRIPTEN_KEEPALIVE void lv_arc_set_dragging(lv_arc_t * p, uint16_t v) { p->dragging = v ; }
EMSCRIPTEN_KEEPALIVE void lv_arc_set_type(lv_arc_t * p, uint16_t v) { p->type = v ; }
EMSCRIPTEN_KEEPALIVE void lv_arc_set_min_close(lv_arc_t * p, uint16_t v) { p->min_close = v ; }
EMSCRIPTEN_KEEPALIVE void lv_arc_set_chg_rate(lv_arc_t * p, uint16_t v) { p->chg_rate = v ; }
EMSCRIPTEN_KEEPALIVE void lv_arc_set_last_tick(lv_arc_t * p, uint32_t v) { p->last_tick = v ; }
EMSCRIPTEN_KEEPALIVE void lv_arc_set_last_angle(lv_arc_t * p, int16_t v) { p->last_angle = v ; }


// struct lv_btn_t wasm api ----------
// fields getter
EMSCRIPTEN_KEEPALIVE lv_obj_t lv_btn_get_obj(lv_btn_t * p) { return p->obj ; }
// fields setter
EMSCRIPTEN_KEEPALIVE void lv_btn_set_obj(lv_btn_t * p, lv_obj_t v) { p->obj = v ; }


// struct lv_img_t wasm api ----------
// fields getter
EMSCRIPTEN_KEEPALIVE lv_obj_t lv_img_get_obj(lv_img_t * p) { return p->obj ; }
EMSCRIPTEN_KEEPALIVE lv_point_t lv_img_get_offset(lv_img_t * p) { return p->offset ; }
EMSCRIPTEN_KEEPALIVE lv_coord_t lv_img_get_w(lv_img_t * p) { return p->w ; }
EMSCRIPTEN_KEEPALIVE lv_coord_t lv_img_get_h(lv_img_t * p) { return p->h ; }
EMSCRIPTEN_KEEPALIVE uint8_t lv_img_get_src_type(lv_img_t * p) { return p->src_type ; }
EMSCRIPTEN_KEEPALIVE uint8_t lv_img_get_cf(lv_img_t * p) { return p->cf ; }
EMSCRIPTEN_KEEPALIVE uint8_t lv_img_get_obj_size_mode(lv_img_t * p) { return p->obj_size_mode ; }
// fields setter
EMSCRIPTEN_KEEPALIVE void lv_img_set_obj(lv_img_t * p, lv_obj_t v) { p->obj = v ; }
EMSCRIPTEN_KEEPALIVE void lv_img_set_offset(lv_img_t * p, lv_point_t v) { p->offset = v ; }
EMSCRIPTEN_KEEPALIVE void lv_img_set_w(lv_img_t * p, lv_coord_t v) { p->w = v ; }
EMSCRIPTEN_KEEPALIVE void lv_img_set_h(lv_img_t * p, lv_coord_t v) { p->h = v ; }
EMSCRIPTEN_KEEPALIVE void lv_img_set_src_type(lv_img_t * p, uint8_t v) { p->src_type = v ; }
EMSCRIPTEN_KEEPALIVE void lv_img_set_cf(lv_img_t * p, uint8_t v) { p->cf = v ; }
EMSCRIPTEN_KEEPALIVE void lv_img_set_obj_size_mode(lv_img_t * p, uint8_t v) { p->obj_size_mode = v ; }


// struct lv_label_t wasm api ----------
// fields getter
EMSCRIPTEN_KEEPALIVE lv_obj_t lv_label_get_obj(lv_label_t * p) { return p->obj ; }
EMSCRIPTEN_KEEPALIVE uint32_t lv_label_get_dot_end(lv_label_t * p) { return p->dot_end ; }
EMSCRIPTEN_KEEPALIVE lv_draw_label_hint_t lv_label_get_hint(lv_label_t * p) { return p->hint ; }
EMSCRIPTEN_KEEPALIVE uint32_t lv_label_get_sel_start(lv_label_t * p) { return p->sel_start ; }
EMSCRIPTEN_KEEPALIVE uint32_t lv_label_get_sel_end(lv_label_t * p) { return p->sel_end ; }
EMSCRIPTEN_KEEPALIVE lv_point_t lv_label_get_offset(lv_label_t * p) { return p->offset ; }
EMSCRIPTEN_KEEPALIVE uint8_t lv_label_get_static_txt(lv_label_t * p) { return p->static_txt ; }
EMSCRIPTEN_KEEPALIVE uint8_t lv_label_get_expand(lv_label_t * p) { return p->expand ; }
EMSCRIPTEN_KEEPALIVE uint8_t lv_label_get_dot_tmp_alloc(lv_label_t * p) { return p->dot_tmp_alloc ; }
// fields setter
EMSCRIPTEN_KEEPALIVE void lv_label_set_obj(lv_label_t * p, lv_obj_t v) { p->obj = v ; }
EMSCRIPTEN_KEEPALIVE void lv_label_set_dot_end(lv_label_t * p, uint32_t v) { p->dot_end = v ; }
EMSCRIPTEN_KEEPALIVE void lv_label_set_hint(lv_label_t * p, lv_draw_label_hint_t v) { p->hint = v ; }
EMSCRIPTEN_KEEPALIVE void lv_label_set_sel_start(lv_label_t * p, uint32_t v) { p->sel_start = v ; }
EMSCRIPTEN_KEEPALIVE void lv_label_set_sel_end(lv_label_t * p, uint32_t v) { p->sel_end = v ; }
EMSCRIPTEN_KEEPALIVE void lv_label_set_offset(lv_label_t * p, lv_point_t v) { p->offset = v ; }
EMSCRIPTEN_KEEPALIVE void lv_label_set_static_txt(lv_label_t * p, uint8_t v) { p->static_txt = v ; }
EMSCRIPTEN_KEEPALIVE void lv_label_set_expand(lv_label_t * p, uint8_t v) { p->expand = v ; }
EMSCRIPTEN_KEEPALIVE void lv_label_set_dot_tmp_alloc(lv_label_t * p, uint8_t v) { p->dot_tmp_alloc = v ; }


// struct lv_line_t wasm api ----------
// fields getter
EMSCRIPTEN_KEEPALIVE lv_obj_t lv_line_get_obj(lv_line_t * p) { return p->obj ; }
EMSCRIPTEN_KEEPALIVE const lv_point_t * lv_line_get_point_array(lv_line_t * p) { return p->point_array ; }
EMSCRIPTEN_KEEPALIVE uint16_t lv_line_get_point_num(lv_line_t * p) { return p->point_num ; }
EMSCRIPTEN_KEEPALIVE uint8_t lv_line_get_y_inv(lv_line_t * p) { return p->y_inv ; }
// fields setter
EMSCRIPTEN_KEEPALIVE void lv_line_set_obj(lv_line_t * p, lv_obj_t v) { p->obj = v ; }
EMSCRIPTEN_KEEPALIVE void lv_line_set_point_array(lv_line_t * p, const lv_point_t * v) { p->point_array = v ; }
EMSCRIPTEN_KEEPALIVE void lv_line_set_point_num(lv_line_t * p, uint16_t v) { p->point_num = v ; }
EMSCRIPTEN_KEEPALIVE void lv_line_set_y_inv(lv_line_t * p, uint8_t v) { p->y_inv = v ; }


// struct lv_table_t wasm api ----------
// fields getter
EMSCRIPTEN_KEEPALIVE lv_obj_t lv_table_get_obj(lv_table_t * p) { return p->obj ; }
EMSCRIPTEN_KEEPALIVE char ** lv_table_get_cell_data(lv_table_t * p) { return p->cell_data ; }
EMSCRIPTEN_KEEPALIVE lv_coord_t * lv_table_get_row_h(lv_table_t * p) { return p->row_h ; }
EMSCRIPTEN_KEEPALIVE lv_coord_t * lv_table_get_col_w(lv_table_t * p) { return p->col_w ; }
EMSCRIPTEN_KEEPALIVE uint16_t lv_table_get_col_act(lv_table_t * p) { return p->col_act ; }
EMSCRIPTEN_KEEPALIVE uint16_t lv_table_get_row_act(lv_table_t * p) { return p->row_act ; }
// fields setter
EMSCRIPTEN_KEEPALIVE void lv_table_set_obj(lv_table_t * p, lv_obj_t v) { p->obj = v ; }
EMSCRIPTEN_KEEPALIVE void lv_table_set_cell_data(lv_table_t * p, char ** v) { p->cell_data = v ; }
EMSCRIPTEN_KEEPALIVE void lv_table_set_row_h(lv_table_t * p, lv_coord_t * v) { p->row_h = v ; }
EMSCRIPTEN_KEEPALIVE void lv_table_set_col_w(lv_table_t * p, lv_coord_t * v) { p->col_w = v ; }
EMSCRIPTEN_KEEPALIVE void lv_table_set_col_act(lv_table_t * p, uint16_t v) { p->col_act = v ; }
EMSCRIPTEN_KEEPALIVE void lv_table_set_row_act(lv_table_t * p, uint16_t v) { p->row_act = v ; }


// struct lv_checkbox_t wasm api ----------
// fields getter
EMSCRIPTEN_KEEPALIVE lv_obj_t lv_checkbox_get_obj(lv_checkbox_t * p) { return p->obj ; }
EMSCRIPTEN_KEEPALIVE char * lv_checkbox_get_txt(lv_checkbox_t * p) { return p->txt ; }
EMSCRIPTEN_KEEPALIVE uint32_t lv_checkbox_get_static_txt(lv_checkbox_t * p) { return p->static_txt ; }
// fields setter
EMSCRIPTEN_KEEPALIVE void lv_checkbox_set_obj(lv_checkbox_t * p, lv_obj_t v) { p->obj = v ; }
EMSCRIPTEN_KEEPALIVE void lv_checkbox_set_txt(lv_checkbox_t * p, char * v) { p->txt = v ; }
EMSCRIPTEN_KEEPALIVE void lv_checkbox_set_static_txt(lv_checkbox_t * p, uint32_t v) { p->static_txt = v ; }


// struct _lv_bar_anim_t wasm api ----------
EMSCRIPTEN_KEEPALIVE _lv_bar_anim_t * _lv_bar_anim_create() {
    _lv_bar_anim_t * p = malloc(sizeof(_lv_bar_anim_t)) ;
    memset(p, 0, sizeof(_lv_bar_anim_t)) ;
    return p ;
}
// fields getter
EMSCRIPTEN_KEEPALIVE lv_obj_t * _lv_bar_anim_get_bar(_lv_bar_anim_t * p) { return p->bar ; }
EMSCRIPTEN_KEEPALIVE int32_t _lv_bar_anim_get_anim_start(_lv_bar_anim_t * p) { return p->anim_start ; }
EMSCRIPTEN_KEEPALIVE int32_t _lv_bar_anim_get_anim_end(_lv_bar_anim_t * p) { return p->anim_end ; }
EMSCRIPTEN_KEEPALIVE int32_t _lv_bar_anim_get_anim_state(_lv_bar_anim_t * p) { return p->anim_state ; }
// fields setter
EMSCRIPTEN_KEEPALIVE void _lv_bar_anim_set_bar(_lv_bar_anim_t * p, lv_obj_t * v) { p->bar = v ; }
EMSCRIPTEN_KEEPALIVE void _lv_bar_anim_set_anim_start(_lv_bar_anim_t * p, int32_t v) { p->anim_start = v ; }
EMSCRIPTEN_KEEPALIVE void _lv_bar_anim_set_anim_end(_lv_bar_anim_t * p, int32_t v) { p->anim_end = v ; }
EMSCRIPTEN_KEEPALIVE void _lv_bar_anim_set_anim_state(_lv_bar_anim_t * p, int32_t v) { p->anim_state = v ; }


// struct lv_bar_t wasm api ----------
// fields getter
EMSCRIPTEN_KEEPALIVE lv_obj_t lv_bar_get_obj(lv_bar_t * p) { return p->obj ; }
EMSCRIPTEN_KEEPALIVE int32_t lv_bar_get_cur_value(lv_bar_t * p) { return p->cur_value ; }
EMSCRIPTEN_KEEPALIVE lv_area_t lv_bar_get_indic_area(lv_bar_t * p) { return p->indic_area ; }
EMSCRIPTEN_KEEPALIVE _lv_bar_anim_t lv_bar_get_cur_value_anim(lv_bar_t * p) { return p->cur_value_anim ; }
EMSCRIPTEN_KEEPALIVE _lv_bar_anim_t lv_bar_get_start_value_anim(lv_bar_t * p) { return p->start_value_anim ; }
// fields setter
EMSCRIPTEN_KEEPALIVE void lv_bar_set_obj(lv_bar_t * p, lv_obj_t v) { p->obj = v ; }
EMSCRIPTEN_KEEPALIVE void lv_bar_set_cur_value(lv_bar_t * p, int32_t v) { p->cur_value = v ; }
EMSCRIPTEN_KEEPALIVE void lv_bar_set_min_value(lv_bar_t * p, int32_t v) { p->min_value = v ; }
EMSCRIPTEN_KEEPALIVE void lv_bar_set_max_value(lv_bar_t * p, int32_t v) { p->max_value = v ; }
EMSCRIPTEN_KEEPALIVE void lv_bar_set_indic_area(lv_bar_t * p, lv_area_t v) { p->indic_area = v ; }
EMSCRIPTEN_KEEPALIVE void lv_bar_set_cur_value_anim(lv_bar_t * p, _lv_bar_anim_t v) { p->cur_value_anim = v ; }
EMSCRIPTEN_KEEPALIVE void lv_bar_set_start_value_anim(lv_bar_t * p, _lv_bar_anim_t v) { p->start_value_anim = v ; }


// struct lv_slider_t wasm api ----------
// fields getter
EMSCRIPTEN_KEEPALIVE lv_bar_t lv_slider_get_bar(lv_slider_t * p) { return p->bar ; }
EMSCRIPTEN_KEEPALIVE lv_area_t lv_slider_get_left_knob_area(lv_slider_t * p) { return p->left_knob_area ; }
EMSCRIPTEN_KEEPALIVE lv_area_t lv_slider_get_right_knob_area(lv_slider_t * p) { return p->right_knob_area ; }
EMSCRIPTEN_KEEPALIVE int32_t * lv_slider_get_value_to_set(lv_slider_t * p) { return p->value_to_set ; }
EMSCRIPTEN_KEEPALIVE uint8_t lv_slider_get_dragging(lv_slider_t * p) { return p->dragging ; }
EMSCRIPTEN_KEEPALIVE uint8_t lv_slider_get_left_knob_focus(lv_slider_t * p) { return p->left_knob_focus ; }
// fields setter
EMSCRIPTEN_KEEPALIVE void lv_slider_set_bar(lv_slider_t * p, lv_bar_t v) { p->bar = v ; }
EMSCRIPTEN_KEEPALIVE void lv_slider_set_left_knob_area(lv_slider_t * p, lv_area_t v) { p->left_knob_area = v ; }
EMSCRIPTEN_KEEPALIVE void lv_slider_set_right_knob_area(lv_slider_t * p, lv_area_t v) { p->right_knob_area = v ; }
EMSCRIPTEN_KEEPALIVE void lv_slider_set_value_to_set(lv_slider_t * p, int32_t * v) { p->value_to_set = v ; }
EMSCRIPTEN_KEEPALIVE void lv_slider_set_dragging(lv_slider_t * p, uint8_t v) { p->dragging = v ; }
EMSCRIPTEN_KEEPALIVE void lv_slider_set_left_knob_focus(lv_slider_t * p, uint8_t v) { p->left_knob_focus = v ; }


// struct lv_btnmatrix_t wasm api ----------
// fields getter
EMSCRIPTEN_KEEPALIVE lv_obj_t lv_btnmatrix_get_obj(lv_btnmatrix_t * p) { return p->obj ; }
EMSCRIPTEN_KEEPALIVE const char ** lv_btnmatrix_get_map_p(lv_btnmatrix_t * p) { return p->map_p ; }
EMSCRIPTEN_KEEPALIVE lv_area_t * lv_btnmatrix_get_button_areas(lv_btnmatrix_t * p) { return p->button_areas ; }
EMSCRIPTEN_KEEPALIVE lv_btnmatrix_ctrl_t * lv_btnmatrix_get_ctrl_bits(lv_btnmatrix_t * p) { return p->ctrl_bits ; }
EMSCRIPTEN_KEEPALIVE uint16_t lv_btnmatrix_get_btn_cnt(lv_btnmatrix_t * p) { return p->btn_cnt ; }
EMSCRIPTEN_KEEPALIVE uint16_t lv_btnmatrix_get_row_cnt(lv_btnmatrix_t * p) { return p->row_cnt ; }
EMSCRIPTEN_KEEPALIVE uint16_t lv_btnmatrix_get_btn_id_sel(lv_btnmatrix_t * p) { return p->btn_id_sel ; }
EMSCRIPTEN_KEEPALIVE uint8_t lv_btnmatrix_get_one_check(lv_btnmatrix_t * p) { return p->one_check ; }
// fields setter
EMSCRIPTEN_KEEPALIVE void lv_btnmatrix_set_obj(lv_btnmatrix_t * p, lv_obj_t v) { p->obj = v ; }
EMSCRIPTEN_KEEPALIVE void lv_btnmatrix_set_map_p(lv_btnmatrix_t * p, const char ** v) { p->map_p = v ; }
EMSCRIPTEN_KEEPALIVE void lv_btnmatrix_set_button_areas(lv_btnmatrix_t * p, lv_area_t * v) { p->button_areas = v ; }
EMSCRIPTEN_KEEPALIVE void lv_btnmatrix_set_ctrl_bits(lv_btnmatrix_t * p, lv_btnmatrix_ctrl_t * v) { p->ctrl_bits = v ; }
EMSCRIPTEN_KEEPALIVE void lv_btnmatrix_set_btn_cnt(lv_btnmatrix_t * p, uint16_t v) { p->btn_cnt = v ; }
EMSCRIPTEN_KEEPALIVE void lv_btnmatrix_set_row_cnt(lv_btnmatrix_t * p, uint16_t v) { p->row_cnt = v ; }
EMSCRIPTEN_KEEPALIVE void lv_btnmatrix_set_btn_id_sel(lv_btnmatrix_t * p, uint16_t v) { p->btn_id_sel = v ; }
EMSCRIPTEN_KEEPALIVE void lv_btnmatrix_set_one_check(lv_btnmatrix_t * p, uint8_t v) { p->one_check = v ; }


// struct lv_dropdown_t wasm api ----------
// fields getter
EMSCRIPTEN_KEEPALIVE lv_obj_t lv_dropdown_get_obj(lv_dropdown_t * p) { return p->obj ; }
EMSCRIPTEN_KEEPALIVE uint16_t lv_dropdown_get_sel_opt_id(lv_dropdown_t * p) { return p->sel_opt_id ; }
EMSCRIPTEN_KEEPALIVE uint16_t lv_dropdown_get_sel_opt_id_orig(lv_dropdown_t * p) { return p->sel_opt_id_orig ; }
EMSCRIPTEN_KEEPALIVE uint16_t lv_dropdown_get_pr_opt_id(lv_dropdown_t * p) { return p->pr_opt_id ; }
EMSCRIPTEN_KEEPALIVE uint8_t lv_dropdown_get_static_txt(lv_dropdown_t * p) { return p->static_txt ; }
// fields setter
EMSCRIPTEN_KEEPALIVE void lv_dropdown_set_obj(lv_dropdown_t * p, lv_obj_t v) { p->obj = v ; }
EMSCRIPTEN_KEEPALIVE void lv_dropdown_set_list(lv_dropdown_t * p, lv_obj_t * v) { p->list = v ; }
EMSCRIPTEN_KEEPALIVE void lv_dropdown_set_option_cnt(lv_dropdown_t * p, uint16_t v) { p->option_cnt = v ; }
EMSCRIPTEN_KEEPALIVE void lv_dropdown_set_sel_opt_id(lv_dropdown_t * p, uint16_t v) { p->sel_opt_id = v ; }
EMSCRIPTEN_KEEPALIVE void lv_dropdown_set_sel_opt_id_orig(lv_dropdown_t * p, uint16_t v) { p->sel_opt_id_orig = v ; }
EMSCRIPTEN_KEEPALIVE void lv_dropdown_set_pr_opt_id(lv_dropdown_t * p, uint16_t v) { p->pr_opt_id = v ; }
EMSCRIPTEN_KEEPALIVE void lv_dropdown_set_static_txt(lv_dropdown_t * p, uint8_t v) { p->static_txt = v ; }


// struct lv_dropdown_list_t wasm api ----------
EMSCRIPTEN_KEEPALIVE lv_dropdown_list_t * lv_dropdown_list_create() {
    lv_dropdown_list_t * p = malloc(sizeof(lv_dropdown_list_t)) ;
    memset(p, 0, sizeof(lv_dropdown_list_t)) ;
    return p ;
}
// fields getter
EMSCRIPTEN_KEEPALIVE lv_obj_t lv_dropdown_list_get_obj(lv_dropdown_list_t * p) { return p->obj ; }
EMSCRIPTEN_KEEPALIVE lv_obj_t * lv_dropdown_list_get_dropdown(lv_dropdown_list_t * p) { return p->dropdown ; }
// fields setter
EMSCRIPTEN_KEEPALIVE void lv_dropdown_list_set_obj(lv_dropdown_list_t * p, lv_obj_t v) { p->obj = v ; }
EMSCRIPTEN_KEEPALIVE void lv_dropdown_list_set_dropdown(lv_dropdown_list_t * p, lv_obj_t * v) { p->dropdown = v ; }


// struct lv_roller_t wasm api ----------
// fields getter
EMSCRIPTEN_KEEPALIVE lv_obj_t lv_roller_get_obj(lv_roller_t * p) { return p->obj ; }
EMSCRIPTEN_KEEPALIVE uint16_t lv_roller_get_sel_opt_id(lv_roller_t * p) { return p->sel_opt_id ; }
EMSCRIPTEN_KEEPALIVE uint16_t lv_roller_get_sel_opt_id_ori(lv_roller_t * p) { return p->sel_opt_id_ori ; }
EMSCRIPTEN_KEEPALIVE lv_roller_mode_t lv_roller_get_mode(lv_roller_t * p) { return p->mode ; }
EMSCRIPTEN_KEEPALIVE uint32_t lv_roller_get_moved(lv_roller_t * p) { return p->moved ; }
// fields setter
EMSCRIPTEN_KEEPALIVE void lv_roller_set_obj(lv_roller_t * p, lv_obj_t v) { p->obj = v ; }
EMSCRIPTEN_KEEPALIVE void lv_roller_set_option_cnt(lv_roller_t * p, uint16_t v) { p->option_cnt = v ; }
EMSCRIPTEN_KEEPALIVE void lv_roller_set_sel_opt_id(lv_roller_t * p, uint16_t v) { p->sel_opt_id = v ; }
EMSCRIPTEN_KEEPALIVE void lv_roller_set_sel_opt_id_ori(lv_roller_t * p, uint16_t v) { p->sel_opt_id_ori = v ; }
EMSCRIPTEN_KEEPALIVE void lv_roller_set_mode(lv_roller_t * p, lv_roller_mode_t v) { p->mode = v ; }
EMSCRIPTEN_KEEPALIVE void lv_roller_set_moved(lv_roller_t * p, uint32_t v) { p->moved = v ; }


// struct lv_textarea_t wasm api ----------
// fields getter
EMSCRIPTEN_KEEPALIVE lv_obj_t lv_textarea_get_obj(lv_textarea_t * p) { return p->obj ; }
EMSCRIPTEN_KEEPALIVE char * lv_textarea_get_placeholder_txt(lv_textarea_t * p) { return p->placeholder_txt ; }
EMSCRIPTEN_KEEPALIVE char * lv_textarea_get_pwd_tmp(lv_textarea_t * p) { return p->pwd_tmp ; }
EMSCRIPTEN_KEEPALIVE uint16_t lv_textarea_get_pwd_show_time(lv_textarea_t * p) { return p->pwd_show_time ; }
EMSCRIPTEN_KEEPALIVE uint32_t lv_textarea_get_sel_start(lv_textarea_t * p) { return p->sel_start ; }
EMSCRIPTEN_KEEPALIVE uint32_t lv_textarea_get_sel_end(lv_textarea_t * p) { return p->sel_end ; }
EMSCRIPTEN_KEEPALIVE uint8_t lv_textarea_get_text_sel_in_prog(lv_textarea_t * p) { return p->text_sel_in_prog ; }
EMSCRIPTEN_KEEPALIVE uint8_t lv_textarea_get_text_sel_en(lv_textarea_t * p) { return p->text_sel_en ; }
EMSCRIPTEN_KEEPALIVE uint8_t lv_textarea_get_pwd_mode(lv_textarea_t * p) { return p->pwd_mode ; }
// fields setter
EMSCRIPTEN_KEEPALIVE void lv_textarea_set_obj(lv_textarea_t * p, lv_obj_t v) { p->obj = v ; }
EMSCRIPTEN_KEEPALIVE void lv_textarea_set_label(lv_textarea_t * p, lv_obj_t * v) { p->label = v ; }
EMSCRIPTEN_KEEPALIVE void lv_textarea_set_placeholder_txt(lv_textarea_t * p, char * v) { p->placeholder_txt = v ; }
EMSCRIPTEN_KEEPALIVE void lv_textarea_set_pwd_tmp(lv_textarea_t * p, char * v) { p->pwd_tmp = v ; }
EMSCRIPTEN_KEEPALIVE void lv_textarea_set_pwd_show_time(lv_textarea_t * p, uint16_t v) { p->pwd_show_time = v ; }
EMSCRIPTEN_KEEPALIVE void lv_textarea_set_sel_start(lv_textarea_t * p, uint32_t v) { p->sel_start = v ; }
EMSCRIPTEN_KEEPALIVE void lv_textarea_set_sel_end(lv_textarea_t * p, uint32_t v) { p->sel_end = v ; }
EMSCRIPTEN_KEEPALIVE void lv_textarea_set_text_sel_in_prog(lv_textarea_t * p, uint8_t v) { p->text_sel_in_prog = v ; }
EMSCRIPTEN_KEEPALIVE void lv_textarea_set_text_sel_en(lv_textarea_t * p, uint8_t v) { p->text_sel_en = v ; }
EMSCRIPTEN_KEEPALIVE void lv_textarea_set_pwd_mode(lv_textarea_t * p, uint8_t v) { p->pwd_mode = v ; }


// struct lv_canvas_t wasm api ----------
// fields getter
EMSCRIPTEN_KEEPALIVE lv_img_dsc_t lv_canvas_get_dsc(lv_canvas_t * p) { return p->dsc ; }
// fields setter
EMSCRIPTEN_KEEPALIVE void lv_canvas_set_img(lv_canvas_t * p, lv_img_t v) { p->img = v ; }
EMSCRIPTEN_KEEPALIVE void lv_canvas_set_dsc(lv_canvas_t * p, lv_img_dsc_t v) { p->dsc = v ; }


// struct lv_switch_t wasm api ----------
// fields getter
EMSCRIPTEN_KEEPALIVE lv_obj_t lv_switch_get_obj(lv_switch_t * p) { return p->obj ; }
EMSCRIPTEN_KEEPALIVE int32_t lv_switch_get_anim_state(lv_switch_t * p) { return p->anim_state ; }
// fields setter
EMSCRIPTEN_KEEPALIVE void lv_switch_set_obj(lv_switch_t * p, lv_obj_t v) { p->obj = v ; }
EMSCRIPTEN_KEEPALIVE void lv_switch_set_anim_state(lv_switch_t * p, int32_t v) { p->anim_state = v ; }


// struct lv_animimg_t wasm api ----------
// fields getter
EMSCRIPTEN_KEEPALIVE lv_img_t lv_animimg_get_img(lv_animimg_t * p) { return p->img ; }
EMSCRIPTEN_KEEPALIVE lv_anim_t lv_animimg_get_anim(lv_animimg_t * p) { return p->anim ; }
EMSCRIPTEN_KEEPALIVE lv_img_dsc_t ** lv_animimg_get_dsc(lv_animimg_t * p) { return p->dsc ; }
EMSCRIPTEN_KEEPALIVE int8_t lv_animimg_get_pic_count(lv_animimg_t * p) { return p->pic_count ; }
// fields setter
EMSCRIPTEN_KEEPALIVE void lv_animimg_set_img(lv_animimg_t * p, lv_img_t v) { p->img = v ; }
EMSCRIPTEN_KEEPALIVE void lv_animimg_set_anim(lv_animimg_t * p, lv_anim_t v) { p->anim = v ; }
EMSCRIPTEN_KEEPALIVE void lv_animimg_set_dsc(lv_animimg_t * p, lv_img_dsc_t ** v) { p->dsc = v ; }
EMSCRIPTEN_KEEPALIVE void lv_animimg_set_pic_count(lv_animimg_t * p, int8_t v) { p->pic_count = v ; }


// struct lv_calendar_date_t wasm api ----------
EMSCRIPTEN_KEEPALIVE lv_calendar_date_t * lv_calendar_date_create() {
    lv_calendar_date_t * p = malloc(sizeof(lv_calendar_date_t)) ;
    memset(p, 0, sizeof(lv_calendar_date_t)) ;
    return p ;
}
// fields getter
EMSCRIPTEN_KEEPALIVE uint16_t lv_calendar_date_get_year(lv_calendar_date_t * p) { return p->year ; }
EMSCRIPTEN_KEEPALIVE int8_t lv_calendar_date_get_month(lv_calendar_date_t * p) { return p->month ; }
EMSCRIPTEN_KEEPALIVE int8_t lv_calendar_date_get_day(lv_calendar_date_t * p) { return p->day ; }
// fields setter
EMSCRIPTEN_KEEPALIVE void lv_calendar_date_set_year(lv_calendar_date_t * p, uint16_t v) { p->year = v ; }
EMSCRIPTEN_KEEPALIVE void lv_calendar_date_set_month(lv_calendar_date_t * p, int8_t v) { p->month = v ; }
EMSCRIPTEN_KEEPALIVE void lv_calendar_date_set_day(lv_calendar_date_t * p, int8_t v) { p->day = v ; }


// struct lv_calendar_t wasm api ----------
// fields getter
EMSCRIPTEN_KEEPALIVE lv_obj_t lv_calendar_get_obj(lv_calendar_t * p) { return p->obj ; }
EMSCRIPTEN_KEEPALIVE lv_obj_t * lv_calendar_get_btnm(lv_calendar_t * p) { return p->btnm ; }
EMSCRIPTEN_KEEPALIVE lv_calendar_date_t lv_calendar_get_today(lv_calendar_t * p) { return p->today ; }
EMSCRIPTEN_KEEPALIVE const char ** lv_calendar_get_map(lv_calendar_t * p) { return p->map ; }
// fields setter
EMSCRIPTEN_KEEPALIVE void lv_calendar_set_obj(lv_calendar_t * p, lv_obj_t v) { p->obj = v ; }
EMSCRIPTEN_KEEPALIVE void lv_calendar_set_btnm(lv_calendar_t * p, lv_obj_t * v) { p->btnm = v ; }
EMSCRIPTEN_KEEPALIVE void lv_calendar_set_today(lv_calendar_t * p, lv_calendar_date_t v) { p->today = v ; }
EMSCRIPTEN_KEEPALIVE void lv_calendar_set_highlighted_dates_num(lv_calendar_t * p, uint16_t v) { p->highlighted_dates_num = v ; }


// struct lv_chart_series_t wasm api ----------
EMSCRIPTEN_KEEPALIVE lv_chart_series_t * lv_chart_series_create() {
    lv_chart_series_t * p = malloc(sizeof(lv_chart_series_t)) ;
    memset(p, 0, sizeof(lv_chart_series_t)) ;
    return p ;
}
// fields getter
EMSCRIPTEN_KEEPALIVE lv_coord_t * lv_chart_series_get_x_points(lv_chart_series_t * p) { return p->x_points ; }
EMSCRIPTEN_KEEPALIVE lv_coord_t * lv_chart_series_get_y_points(lv_chart_series_t * p) { return p->y_points ; }
EMSCRIPTEN_KEEPALIVE lv_color_t lv_chart_series_get_color(lv_chart_series_t * p) { return p->color ; }
EMSCRIPTEN_KEEPALIVE uint16_t lv_chart_series_get_start_point(lv_chart_series_t * p) { return p->start_point ; }
EMSCRIPTEN_KEEPALIVE uint8_t lv_chart_series_get_hidden(lv_chart_series_t * p) { return p->hidden ; }
EMSCRIPTEN_KEEPALIVE uint8_t lv_chart_series_get_x_ext_buf_assigned(lv_chart_series_t * p) { return p->x_ext_buf_assigned ; }
EMSCRIPTEN_KEEPALIVE uint8_t lv_chart_series_get_y_ext_buf_assigned(lv_chart_series_t * p) { return p->y_ext_buf_assigned ; }
EMSCRIPTEN_KEEPALIVE uint8_t lv_chart_series_get_x_axis_sec(lv_chart_series_t * p) { return p->x_axis_sec ; }
EMSCRIPTEN_KEEPALIVE uint8_t lv_chart_series_get_y_axis_sec(lv_chart_series_t * p) { return p->y_axis_sec ; }
// fields setter
EMSCRIPTEN_KEEPALIVE void lv_chart_series_set_x_points(lv_chart_series_t * p, lv_coord_t * v) { p->x_points = v ; }
EMSCRIPTEN_KEEPALIVE void lv_chart_series_set_y_points(lv_chart_series_t * p, lv_coord_t * v) { p->y_points = v ; }
EMSCRIPTEN_KEEPALIVE void lv_chart_series_set_color(lv_chart_series_t * p, lv_color_t v) { p->color = v ; }
EMSCRIPTEN_KEEPALIVE void lv_chart_series_set_start_point(lv_chart_series_t * p, uint16_t v) { p->start_point = v ; }
EMSCRIPTEN_KEEPALIVE void lv_chart_series_set_hidden(lv_chart_series_t * p, uint8_t v) { p->hidden = v ; }
EMSCRIPTEN_KEEPALIVE void lv_chart_series_set_x_ext_buf_assigned(lv_chart_series_t * p, uint8_t v) { p->x_ext_buf_assigned = v ; }
EMSCRIPTEN_KEEPALIVE void lv_chart_series_set_y_ext_buf_assigned(lv_chart_series_t * p, uint8_t v) { p->y_ext_buf_assigned = v ; }
EMSCRIPTEN_KEEPALIVE void lv_chart_series_set_x_axis_sec(lv_chart_series_t * p, uint8_t v) { p->x_axis_sec = v ; }
EMSCRIPTEN_KEEPALIVE void lv_chart_series_set_y_axis_sec(lv_chart_series_t * p, uint8_t v) { p->y_axis_sec = v ; }


// struct lv_chart_cursor_t wasm api ----------
EMSCRIPTEN_KEEPALIVE lv_chart_cursor_t * lv_chart_cursor_create() {
    lv_chart_cursor_t * p = malloc(sizeof(lv_chart_cursor_t)) ;
    memset(p, 0, sizeof(lv_chart_cursor_t)) ;
    return p ;
}
// fields getter
EMSCRIPTEN_KEEPALIVE lv_point_t lv_chart_cursor_get_pos(lv_chart_cursor_t * p) { return p->pos ; }
EMSCRIPTEN_KEEPALIVE uint16_t lv_chart_cursor_get_point_id(lv_chart_cursor_t * p) { return p->point_id ; }
EMSCRIPTEN_KEEPALIVE lv_color_t lv_chart_cursor_get_color(lv_chart_cursor_t * p) { return p->color ; }
EMSCRIPTEN_KEEPALIVE lv_chart_series_t * lv_chart_cursor_get_ser(lv_chart_cursor_t * p) { return p->ser ; }
EMSCRIPTEN_KEEPALIVE lv_dir_t lv_chart_cursor_get_dir(lv_chart_cursor_t * p) { return p->dir ; }
EMSCRIPTEN_KEEPALIVE uint8_t lv_chart_cursor_get_pos_set(lv_chart_cursor_t * p) { return p->pos_set ; }
// fields setter
EMSCRIPTEN_KEEPALIVE void lv_chart_cursor_set_pos(lv_chart_cursor_t * p, lv_point_t v) { p->pos = v ; }
EMSCRIPTEN_KEEPALIVE void lv_chart_cursor_set_point_id(lv_chart_cursor_t * p, uint16_t v) { p->point_id = v ; }
EMSCRIPTEN_KEEPALIVE void lv_chart_cursor_set_color(lv_chart_cursor_t * p, lv_color_t v) { p->color = v ; }
EMSCRIPTEN_KEEPALIVE void lv_chart_cursor_set_ser(lv_chart_cursor_t * p, lv_chart_series_t * v) { p->ser = v ; }
EMSCRIPTEN_KEEPALIVE void lv_chart_cursor_set_dir(lv_chart_cursor_t * p, lv_dir_t v) { p->dir = v ; }
EMSCRIPTEN_KEEPALIVE void lv_chart_cursor_set_pos_set(lv_chart_cursor_t * p, uint8_t v) { p->pos_set = v ; }


// struct lv_chart_tick_dsc_t wasm api ----------
EMSCRIPTEN_KEEPALIVE lv_chart_tick_dsc_t * lv_chart_tick_dsc_create() {
    lv_chart_tick_dsc_t * p = malloc(sizeof(lv_chart_tick_dsc_t)) ;
    memset(p, 0, sizeof(lv_chart_tick_dsc_t)) ;
    return p ;
}
// fields getter
EMSCRIPTEN_KEEPALIVE lv_coord_t lv_chart_tick_dsc_get_major_len(lv_chart_tick_dsc_t * p) { return p->major_len ; }
EMSCRIPTEN_KEEPALIVE lv_coord_t lv_chart_tick_dsc_get_minor_len(lv_chart_tick_dsc_t * p) { return p->minor_len ; }
EMSCRIPTEN_KEEPALIVE lv_coord_t lv_chart_tick_dsc_get_draw_size(lv_chart_tick_dsc_t * p) { return p->draw_size ; }
EMSCRIPTEN_KEEPALIVE uint32_t lv_chart_tick_dsc_get_minor_cnt(lv_chart_tick_dsc_t * p) { return p->minor_cnt ; }
EMSCRIPTEN_KEEPALIVE uint32_t lv_chart_tick_dsc_get_major_cnt(lv_chart_tick_dsc_t * p) { return p->major_cnt ; }
EMSCRIPTEN_KEEPALIVE uint32_t lv_chart_tick_dsc_get_label_en(lv_chart_tick_dsc_t * p) { return p->label_en ; }
// fields setter
EMSCRIPTEN_KEEPALIVE void lv_chart_tick_dsc_set_major_len(lv_chart_tick_dsc_t * p, lv_coord_t v) { p->major_len = v ; }
EMSCRIPTEN_KEEPALIVE void lv_chart_tick_dsc_set_minor_len(lv_chart_tick_dsc_t * p, lv_coord_t v) { p->minor_len = v ; }
EMSCRIPTEN_KEEPALIVE void lv_chart_tick_dsc_set_draw_size(lv_chart_tick_dsc_t * p, lv_coord_t v) { p->draw_size = v ; }
EMSCRIPTEN_KEEPALIVE void lv_chart_tick_dsc_set_minor_cnt(lv_chart_tick_dsc_t * p, uint32_t v) { p->minor_cnt = v ; }
EMSCRIPTEN_KEEPALIVE void lv_chart_tick_dsc_set_major_cnt(lv_chart_tick_dsc_t * p, uint32_t v) { p->major_cnt = v ; }
EMSCRIPTEN_KEEPALIVE void lv_chart_tick_dsc_set_label_en(lv_chart_tick_dsc_t * p, uint32_t v) { p->label_en = v ; }


// struct lv_chart_t wasm api ----------
// fields getter
EMSCRIPTEN_KEEPALIVE lv_obj_t lv_chart_get_obj(lv_chart_t * p) { return p->obj ; }
EMSCRIPTEN_KEEPALIVE lv_ll_t lv_chart_get_series_ll(lv_chart_t * p) { return p->series_ll ; }
EMSCRIPTEN_KEEPALIVE lv_ll_t lv_chart_get_cursor_ll(lv_chart_t * p) { return p->cursor_ll ; }
EMSCRIPTEN_KEEPALIVE lv_chart_tick_dsc_t * lv_chart_get_tick(lv_chart_t * p) { return p->tick ; }
EMSCRIPTEN_KEEPALIVE lv_coord_t * lv_chart_get_ymin(lv_chart_t * p) { return p->ymin ; }
EMSCRIPTEN_KEEPALIVE lv_coord_t * lv_chart_get_ymax(lv_chart_t * p) { return p->ymax ; }
EMSCRIPTEN_KEEPALIVE lv_coord_t * lv_chart_get_xmin(lv_chart_t * p) { return p->xmin ; }
EMSCRIPTEN_KEEPALIVE lv_coord_t * lv_chart_get_xmax(lv_chart_t * p) { return p->xmax ; }
EMSCRIPTEN_KEEPALIVE uint16_t lv_chart_get_pressed_point_id(lv_chart_t * p) { return p->pressed_point_id ; }
EMSCRIPTEN_KEEPALIVE uint16_t lv_chart_get_hdiv_cnt(lv_chart_t * p) { return p->hdiv_cnt ; }
EMSCRIPTEN_KEEPALIVE uint16_t lv_chart_get_vdiv_cnt(lv_chart_t * p) { return p->vdiv_cnt ; }
EMSCRIPTEN_KEEPALIVE uint16_t lv_chart_get_point_cnt(lv_chart_t * p) { return p->point_cnt ; }
EMSCRIPTEN_KEEPALIVE lv_chart_update_mode_t lv_chart_get_update_mode(lv_chart_t * p) { return p->update_mode ; }
// fields setter
EMSCRIPTEN_KEEPALIVE void lv_chart_set_obj(lv_chart_t * p, lv_obj_t v) { p->obj = v ; }
EMSCRIPTEN_KEEPALIVE void lv_chart_set_series_ll(lv_chart_t * p, lv_ll_t v) { p->series_ll = v ; }
EMSCRIPTEN_KEEPALIVE void lv_chart_set_cursor_ll(lv_chart_t * p, lv_ll_t v) { p->cursor_ll = v ; }
EMSCRIPTEN_KEEPALIVE void lv_chart_set_pressed_point_id(lv_chart_t * p, uint16_t v) { p->pressed_point_id = v ; }
EMSCRIPTEN_KEEPALIVE void lv_chart_set_hdiv_cnt(lv_chart_t * p, uint16_t v) { p->hdiv_cnt = v ; }
EMSCRIPTEN_KEEPALIVE void lv_chart_set_vdiv_cnt(lv_chart_t * p, uint16_t v) { p->vdiv_cnt = v ; }
EMSCRIPTEN_KEEPALIVE void lv_chart_set_point_cnt(lv_chart_t * p, uint16_t v) { p->point_cnt = v ; }


// struct lv_keyboard_t wasm api ----------
// fields getter
EMSCRIPTEN_KEEPALIVE lv_btnmatrix_t lv_keyboard_get_btnm(lv_keyboard_t * p) { return p->btnm ; }
EMSCRIPTEN_KEEPALIVE lv_obj_t * lv_keyboard_get_ta(lv_keyboard_t * p) { return p->ta ; }
EMSCRIPTEN_KEEPALIVE uint8_t lv_keyboard_get_popovers(lv_keyboard_t * p) { return p->popovers ; }
// fields setter
EMSCRIPTEN_KEEPALIVE void lv_keyboard_set_btnm(lv_keyboard_t * p, lv_btnmatrix_t v) { p->btnm = v ; }
EMSCRIPTEN_KEEPALIVE void lv_keyboard_set_ta(lv_keyboard_t * p, lv_obj_t * v) { p->ta = v ; }


// struct lv_msgbox_t wasm api ----------
// fields getter
EMSCRIPTEN_KEEPALIVE lv_obj_t lv_msgbox_get_obj(lv_msgbox_t * p) { return p->obj ; }
// fields setter
EMSCRIPTEN_KEEPALIVE void lv_msgbox_set_obj(lv_msgbox_t * p, lv_obj_t v) { p->obj = v ; }
EMSCRIPTEN_KEEPALIVE void lv_msgbox_set_title(lv_msgbox_t * p, lv_obj_t * v) { p->title = v ; }
EMSCRIPTEN_KEEPALIVE void lv_msgbox_set_close_btn(lv_msgbox_t * p, lv_obj_t * v) { p->close_btn = v ; }
EMSCRIPTEN_KEEPALIVE void lv_msgbox_set_content(lv_msgbox_t * p, lv_obj_t * v) { p->content = v ; }
EMSCRIPTEN_KEEPALIVE void lv_msgbox_set_text(lv_msgbox_t * p, lv_obj_t * v) { p->text = v ; }
EMSCRIPTEN_KEEPALIVE void lv_msgbox_set_btns(lv_msgbox_t * p, lv_obj_t * v) { p->btns = v ; }


// struct lv_meter_scale_t wasm api ----------
EMSCRIPTEN_KEEPALIVE lv_meter_scale_t * lv_meter_scale_create() {
    lv_meter_scale_t * p = malloc(sizeof(lv_meter_scale_t)) ;
    memset(p, 0, sizeof(lv_meter_scale_t)) ;
    return p ;
}
// fields getter
EMSCRIPTEN_KEEPALIVE lv_color_t lv_meter_scale_get_tick_color(lv_meter_scale_t * p) { return p->tick_color ; }
EMSCRIPTEN_KEEPALIVE uint16_t lv_meter_scale_get_tick_cnt(lv_meter_scale_t * p) { return p->tick_cnt ; }
EMSCRIPTEN_KEEPALIVE uint16_t lv_meter_scale_get_tick_length(lv_meter_scale_t * p) { return p->tick_length ; }
EMSCRIPTEN_KEEPALIVE uint16_t lv_meter_scale_get_tick_width(lv_meter_scale_t * p) { return p->tick_width ; }
EMSCRIPTEN_KEEPALIVE lv_color_t lv_meter_scale_get_tick_major_color(lv_meter_scale_t * p) { return p->tick_major_color ; }
EMSCRIPTEN_KEEPALIVE uint16_t lv_meter_scale_get_tick_major_nth(lv_meter_scale_t * p) { return p->tick_major_nth ; }
EMSCRIPTEN_KEEPALIVE uint16_t lv_meter_scale_get_tick_major_length(lv_meter_scale_t * p) { return p->tick_major_length ; }
EMSCRIPTEN_KEEPALIVE uint16_t lv_meter_scale_get_tick_major_width(lv_meter_scale_t * p) { return p->tick_major_width ; }
EMSCRIPTEN_KEEPALIVE int16_t lv_meter_scale_get_label_gap(lv_meter_scale_t * p) { return p->label_gap ; }
EMSCRIPTEN_KEEPALIVE int16_t lv_meter_scale_get_label_color(lv_meter_scale_t * p) { return p->label_color ; }
EMSCRIPTEN_KEEPALIVE int32_t lv_meter_scale_get_min(lv_meter_scale_t * p) { return p->min ; }
EMSCRIPTEN_KEEPALIVE int32_t lv_meter_scale_get_max(lv_meter_scale_t * p) { return p->max ; }
EMSCRIPTEN_KEEPALIVE int16_t lv_meter_scale_get_r_mod(lv_meter_scale_t * p) { return p->r_mod ; }
EMSCRIPTEN_KEEPALIVE uint16_t lv_meter_scale_get_angle_range(lv_meter_scale_t * p) { return p->angle_range ; }
EMSCRIPTEN_KEEPALIVE int16_t lv_meter_scale_get_rotation(lv_meter_scale_t * p) { return p->rotation ; }
// fields setter
EMSCRIPTEN_KEEPALIVE void lv_meter_scale_set_tick_color(lv_meter_scale_t * p, lv_color_t v) { p->tick_color = v ; }
EMSCRIPTEN_KEEPALIVE void lv_meter_scale_set_tick_cnt(lv_meter_scale_t * p, uint16_t v) { p->tick_cnt = v ; }
EMSCRIPTEN_KEEPALIVE void lv_meter_scale_set_tick_length(lv_meter_scale_t * p, uint16_t v) { p->tick_length = v ; }
EMSCRIPTEN_KEEPALIVE void lv_meter_scale_set_tick_width(lv_meter_scale_t * p, uint16_t v) { p->tick_width = v ; }
EMSCRIPTEN_KEEPALIVE void lv_meter_scale_set_tick_major_color(lv_meter_scale_t * p, lv_color_t v) { p->tick_major_color = v ; }
EMSCRIPTEN_KEEPALIVE void lv_meter_scale_set_tick_major_nth(lv_meter_scale_t * p, uint16_t v) { p->tick_major_nth = v ; }
EMSCRIPTEN_KEEPALIVE void lv_meter_scale_set_tick_major_length(lv_meter_scale_t * p, uint16_t v) { p->tick_major_length = v ; }
EMSCRIPTEN_KEEPALIVE void lv_meter_scale_set_tick_major_width(lv_meter_scale_t * p, uint16_t v) { p->tick_major_width = v ; }
EMSCRIPTEN_KEEPALIVE void lv_meter_scale_set_label_gap(lv_meter_scale_t * p, int16_t v) { p->label_gap = v ; }
EMSCRIPTEN_KEEPALIVE void lv_meter_scale_set_label_color(lv_meter_scale_t * p, int16_t v) { p->label_color = v ; }
EMSCRIPTEN_KEEPALIVE void lv_meter_scale_set_min(lv_meter_scale_t * p, int32_t v) { p->min = v ; }
EMSCRIPTEN_KEEPALIVE void lv_meter_scale_set_max(lv_meter_scale_t * p, int32_t v) { p->max = v ; }
EMSCRIPTEN_KEEPALIVE void lv_meter_scale_set_r_mod(lv_meter_scale_t * p, int16_t v) { p->r_mod = v ; }
EMSCRIPTEN_KEEPALIVE void lv_meter_scale_set_angle_range(lv_meter_scale_t * p, uint16_t v) { p->angle_range = v ; }
EMSCRIPTEN_KEEPALIVE void lv_meter_scale_set_rotation(lv_meter_scale_t * p, int16_t v) { p->rotation = v ; }


// struct lv_meter_indicator_t wasm api ----------
EMSCRIPTEN_KEEPALIVE lv_meter_indicator_t * lv_meter_indicator_create() {
    lv_meter_indicator_t * p = malloc(sizeof(lv_meter_indicator_t)) ;
    memset(p, 0, sizeof(lv_meter_indicator_t)) ;
    return p ;
}
// fields getter
EMSCRIPTEN_KEEPALIVE lv_meter_scale_t * lv_meter_indicator_get_scale(lv_meter_indicator_t * p) { return p->scale ; }
EMSCRIPTEN_KEEPALIVE lv_meter_indicator_type_t lv_meter_indicator_get_type(lv_meter_indicator_t * p) { return p->type ; }
EMSCRIPTEN_KEEPALIVE lv_opa_t lv_meter_indicator_get_opa(lv_meter_indicator_t * p) { return p->opa ; }
EMSCRIPTEN_KEEPALIVE int32_t lv_meter_indicator_get_start_value(lv_meter_indicator_t * p) { return p->start_value ; }
EMSCRIPTEN_KEEPALIVE int32_t lv_meter_indicator_get_end_value(lv_meter_indicator_t * p) { return p->end_value ; }
// fields setter
EMSCRIPTEN_KEEPALIVE void lv_meter_indicator_set_scale(lv_meter_indicator_t * p, lv_meter_scale_t * v) { p->scale = v ; }
EMSCRIPTEN_KEEPALIVE void lv_meter_indicator_set_type(lv_meter_indicator_t * p, lv_meter_indicator_type_t v) { p->type = v ; }
EMSCRIPTEN_KEEPALIVE void lv_meter_indicator_set_opa(lv_meter_indicator_t * p, lv_opa_t v) { p->opa = v ; }
EMSCRIPTEN_KEEPALIVE void lv_meter_indicator_set_start_value(lv_meter_indicator_t * p, int32_t v) { p->start_value = v ; }
EMSCRIPTEN_KEEPALIVE void lv_meter_indicator_set_end_value(lv_meter_indicator_t * p, int32_t v) { p->end_value = v ; }


// struct lv_meter_t wasm api ----------
// fields getter
EMSCRIPTEN_KEEPALIVE lv_obj_t lv_meter_get_obj(lv_meter_t * p) { return p->obj ; }
EMSCRIPTEN_KEEPALIVE lv_ll_t lv_meter_get_scale_ll(lv_meter_t * p) { return p->scale_ll ; }
EMSCRIPTEN_KEEPALIVE lv_ll_t lv_meter_get_indicator_ll(lv_meter_t * p) { return p->indicator_ll ; }
// fields setter
EMSCRIPTEN_KEEPALIVE void lv_meter_set_obj(lv_meter_t * p, lv_obj_t v) { p->obj = v ; }
EMSCRIPTEN_KEEPALIVE void lv_meter_set_scale_ll(lv_meter_t * p, lv_ll_t v) { p->scale_ll = v ; }
EMSCRIPTEN_KEEPALIVE void lv_meter_set_indicator_ll(lv_meter_t * p, lv_ll_t v) { p->indicator_ll = v ; }


// struct lv_spinbox_t wasm api ----------
// fields getter
EMSCRIPTEN_KEEPALIVE lv_textarea_t lv_spinbox_get_ta(lv_spinbox_t * p) { return p->ta ; }
EMSCRIPTEN_KEEPALIVE int32_t lv_spinbox_get_range_max(lv_spinbox_t * p) { return p->range_max ; }
EMSCRIPTEN_KEEPALIVE int32_t lv_spinbox_get_range_min(lv_spinbox_t * p) { return p->range_min ; }
EMSCRIPTEN_KEEPALIVE uint16_t lv_spinbox_get_digit_count(lv_spinbox_t * p) { return p->digit_count ; }
EMSCRIPTEN_KEEPALIVE uint16_t lv_spinbox_get_dec_point_pos(lv_spinbox_t * p) { return p->dec_point_pos ; }
EMSCRIPTEN_KEEPALIVE uint16_t lv_spinbox_get_digit_step_dir(lv_spinbox_t * p) { return p->digit_step_dir ; }
// fields setter
EMSCRIPTEN_KEEPALIVE void lv_spinbox_set_ta(lv_spinbox_t * p, lv_textarea_t v) { p->ta = v ; }
EMSCRIPTEN_KEEPALIVE void lv_spinbox_set_range_max(lv_spinbox_t * p, int32_t v) { p->range_max = v ; }
EMSCRIPTEN_KEEPALIVE void lv_spinbox_set_range_min(lv_spinbox_t * p, int32_t v) { p->range_min = v ; }
EMSCRIPTEN_KEEPALIVE void lv_spinbox_set_digit_count(lv_spinbox_t * p, uint16_t v) { p->digit_count = v ; }
EMSCRIPTEN_KEEPALIVE void lv_spinbox_set_dec_point_pos(lv_spinbox_t * p, uint16_t v) { p->dec_point_pos = v ; }
EMSCRIPTEN_KEEPALIVE void lv_spinbox_set_digit_step_dir(lv_spinbox_t * p, uint16_t v) { p->digit_step_dir = v ; }


// struct lv_tabview_t wasm api ----------
// fields getter
EMSCRIPTEN_KEEPALIVE lv_obj_t lv_tabview_get_obj(lv_tabview_t * p) { return p->obj ; }
EMSCRIPTEN_KEEPALIVE char ** lv_tabview_get_map(lv_tabview_t * p) { return p->map ; }
EMSCRIPTEN_KEEPALIVE uint16_t lv_tabview_get_tab_cnt(lv_tabview_t * p) { return p->tab_cnt ; }
EMSCRIPTEN_KEEPALIVE uint16_t lv_tabview_get_tab_cur(lv_tabview_t * p) { return p->tab_cur ; }
EMSCRIPTEN_KEEPALIVE lv_dir_t lv_tabview_get_tab_pos(lv_tabview_t * p) { return p->tab_pos ; }
// fields setter
EMSCRIPTEN_KEEPALIVE void lv_tabview_set_obj(lv_tabview_t * p, lv_obj_t v) { p->obj = v ; }
EMSCRIPTEN_KEEPALIVE void lv_tabview_set_map(lv_tabview_t * p, char ** v) { p->map = v ; }
EMSCRIPTEN_KEEPALIVE void lv_tabview_set_tab_cnt(lv_tabview_t * p, uint16_t v) { p->tab_cnt = v ; }
EMSCRIPTEN_KEEPALIVE void lv_tabview_set_tab_cur(lv_tabview_t * p, uint16_t v) { p->tab_cur = v ; }
EMSCRIPTEN_KEEPALIVE void lv_tabview_set_tab_pos(lv_tabview_t * p, lv_dir_t v) { p->tab_pos = v ; }


// struct lv_tileview_t wasm api ----------
// fields getter
EMSCRIPTEN_KEEPALIVE lv_obj_t lv_tileview_get_obj(lv_tileview_t * p) { return p->obj ; }
// fields setter
EMSCRIPTEN_KEEPALIVE void lv_tileview_set_obj(lv_tileview_t * p, lv_obj_t v) { p->obj = v ; }
EMSCRIPTEN_KEEPALIVE void lv_tileview_set_tile_act(lv_tileview_t * p, lv_obj_t * v) { p->tile_act = v ; }


// struct lv_tileview_tile_t wasm api ----------
EMSCRIPTEN_KEEPALIVE lv_tileview_tile_t * lv_tileview_tile_create() {
    lv_tileview_tile_t * p = malloc(sizeof(lv_tileview_tile_t)) ;
    memset(p, 0, sizeof(lv_tileview_tile_t)) ;
    return p ;
}
// fields getter
EMSCRIPTEN_KEEPALIVE lv_obj_t lv_tileview_tile_get_obj(lv_tileview_tile_t * p) { return p->obj ; }
EMSCRIPTEN_KEEPALIVE lv_dir_t lv_tileview_tile_get_dir(lv_tileview_tile_t * p) { return p->dir ; }
// fields setter
EMSCRIPTEN_KEEPALIVE void lv_tileview_tile_set_obj(lv_tileview_tile_t * p, lv_obj_t v) { p->obj = v ; }
EMSCRIPTEN_KEEPALIVE void lv_tileview_tile_set_dir(lv_tileview_tile_t * p, lv_dir_t v) { p->dir = v ; }


// struct lv_win_t wasm api ----------
// fields getter
EMSCRIPTEN_KEEPALIVE lv_obj_t lv_win_get_obj(lv_win_t * p) { return p->obj ; }
// fields setter
EMSCRIPTEN_KEEPALIVE void lv_win_set_obj(lv_win_t * p, lv_obj_t v) { p->obj = v ; }


// struct lv_colorwheel_t wasm api ----------
// fields getter
EMSCRIPTEN_KEEPALIVE lv_obj_t lv_colorwheel_get_obj(lv_colorwheel_t * p) { return p->obj ; }
EMSCRIPTEN_KEEPALIVE uint32_t lv_colorwheel_get_last_click_time(lv_colorwheel_t * p) { return p->last_click_time ; }
EMSCRIPTEN_KEEPALIVE uint32_t lv_colorwheel_get_last_change_time(lv_colorwheel_t * p) { return p->last_change_time ; }
EMSCRIPTEN_KEEPALIVE lv_point_t lv_colorwheel_get_last_press_point(lv_colorwheel_t * p) { return p->last_press_point ; }
EMSCRIPTEN_KEEPALIVE lv_colorwheel_mode_t lv_colorwheel_get_mode(lv_colorwheel_t * p) { return p->mode ; }
EMSCRIPTEN_KEEPALIVE uint8_t lv_colorwheel_get_mode_fixed(lv_colorwheel_t * p) { return p->mode_fixed ; }
// fields setter
EMSCRIPTEN_KEEPALIVE void lv_colorwheel_set_obj(lv_colorwheel_t * p, lv_obj_t v) { p->obj = v ; }
EMSCRIPTEN_KEEPALIVE void lv_colorwheel_set_last_click_time(lv_colorwheel_t * p, uint32_t v) { p->last_click_time = v ; }
EMSCRIPTEN_KEEPALIVE void lv_colorwheel_set_last_change_time(lv_colorwheel_t * p, uint32_t v) { p->last_change_time = v ; }
EMSCRIPTEN_KEEPALIVE void lv_colorwheel_set_last_press_point(lv_colorwheel_t * p, lv_point_t v) { p->last_press_point = v ; }


// struct lv_led_t wasm api ----------
// fields getter
EMSCRIPTEN_KEEPALIVE lv_obj_t lv_led_get_obj(lv_led_t * p) { return p->obj ; }
EMSCRIPTEN_KEEPALIVE lv_color_t lv_led_get_color(lv_led_t * p) { return p->color ; }
EMSCRIPTEN_KEEPALIVE uint8_t lv_led_get_bright(lv_led_t * p) { return p->bright ; }
// fields setter
EMSCRIPTEN_KEEPALIVE void lv_led_set_obj(lv_led_t * p, lv_obj_t v) { p->obj = v ; }
EMSCRIPTEN_KEEPALIVE void lv_led_set_bright(lv_led_t * p, uint8_t v) { p->bright = v ; }


// struct lv_imgbtn_t wasm api ----------
// fields getter
EMSCRIPTEN_KEEPALIVE lv_obj_t lv_imgbtn_get_obj(lv_imgbtn_t * p) { return p->obj ; }
EMSCRIPTEN_KEEPALIVE const void ** lv_imgbtn_get_img_src_mid(lv_imgbtn_t * p) { return p->img_src_mid ; }
EMSCRIPTEN_KEEPALIVE const void ** lv_imgbtn_get_img_src_left(lv_imgbtn_t * p) { return p->img_src_left ; }
EMSCRIPTEN_KEEPALIVE const void ** lv_imgbtn_get_img_src_right(lv_imgbtn_t * p) { return p->img_src_right ; }
EMSCRIPTEN_KEEPALIVE lv_img_cf_t lv_imgbtn_get_act_cf(lv_imgbtn_t * p) { return p->act_cf ; }
// fields setter
EMSCRIPTEN_KEEPALIVE void lv_imgbtn_set_obj(lv_imgbtn_t * p, lv_obj_t v) { p->obj = v ; }
EMSCRIPTEN_KEEPALIVE void lv_imgbtn_set_act_cf(lv_imgbtn_t * p, lv_img_cf_t v) { p->act_cf = v ; }


// struct lv_span_t wasm api ----------
EMSCRIPTEN_KEEPALIVE lv_span_t * lv_span_create() {
    lv_span_t * p = malloc(sizeof(lv_span_t)) ;
    memset(p, 0, sizeof(lv_span_t)) ;
    return p ;
}
// fields getter
EMSCRIPTEN_KEEPALIVE char * lv_span_get_txt(lv_span_t * p) { return p->txt ; }
EMSCRIPTEN_KEEPALIVE lv_obj_t * lv_span_get_spangroup(lv_span_t * p) { return p->spangroup ; }
EMSCRIPTEN_KEEPALIVE lv_style_t lv_span_get_style(lv_span_t * p) { return p->style ; }
EMSCRIPTEN_KEEPALIVE uint8_t lv_span_get_static_flag(lv_span_t * p) { return p->static_flag ; }
// fields setter
EMSCRIPTEN_KEEPALIVE void lv_span_set_txt(lv_span_t * p, char * v) { p->txt = v ; }
EMSCRIPTEN_KEEPALIVE void lv_span_set_spangroup(lv_span_t * p, lv_obj_t * v) { p->spangroup = v ; }
EMSCRIPTEN_KEEPALIVE void lv_span_set_style(lv_span_t * p, lv_style_t v) { p->style = v ; }
EMSCRIPTEN_KEEPALIVE void lv_span_set_static_flag(lv_span_t * p, uint8_t v) { p->static_flag = v ; }


// struct lv_spangroup_t wasm api ----------
// fields getter
EMSCRIPTEN_KEEPALIVE lv_obj_t lv_spangroup_get_obj(lv_spangroup_t * p) { return p->obj ; }
EMSCRIPTEN_KEEPALIVE lv_coord_t lv_spangroup_get_cache_w(lv_spangroup_t * p) { return p->cache_w ; }
EMSCRIPTEN_KEEPALIVE lv_coord_t lv_spangroup_get_cache_h(lv_spangroup_t * p) { return p->cache_h ; }
EMSCRIPTEN_KEEPALIVE lv_ll_t lv_spangroup_get_child_ll(lv_spangroup_t * p) { return p->child_ll ; }
EMSCRIPTEN_KEEPALIVE uint8_t lv_spangroup_get_refresh(lv_spangroup_t * p) { return p->refresh ; }
// fields setter
EMSCRIPTEN_KEEPALIVE void lv_spangroup_set_obj(lv_spangroup_t * p, lv_obj_t v) { p->obj = v ; }
EMSCRIPTEN_KEEPALIVE void lv_spangroup_set_cache_w(lv_spangroup_t * p, lv_coord_t v) { p->cache_w = v ; }
EMSCRIPTEN_KEEPALIVE void lv_spangroup_set_cache_h(lv_spangroup_t * p, lv_coord_t v) { p->cache_h = v ; }
EMSCRIPTEN_KEEPALIVE void lv_spangroup_set_child_ll(lv_spangroup_t * p, lv_ll_t v) { p->child_ll = v ; }
EMSCRIPTEN_KEEPALIVE void lv_spangroup_set_refresh(lv_spangroup_t * p, uint8_t v) { p->refresh = v ; }
// AUTO GENERATE CODE END [API STRUCT] --------



EMSCRIPTEN_KEEPALIVE size_t lv_disp_drv_meta() {
    return sizeof(struct {
        int32_t num;         /**< Number integer number (opacity, enums, booleans or "normal" numbers)*/
        const void * ptr;    /**< Constant pointers  (font, cone text, etc)*/
        lv_color_t color;    /**< Colors*/
    }) ;
}