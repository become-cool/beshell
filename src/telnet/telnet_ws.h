#ifndef _TELNET_WS_H
#define _TELNET_WS_H

#include "quickjs-libc.h"
// #include "lvgl.h"
// #include "lv_conf.h"
#include "mongoose.h"

#ifdef PLATFORM_ESP32
// #include "disp_st77xx.h"
#include <freertos/task.h>
#include <freertos/queue.h>
#include <freertos/semphr.h>
#include "tjpg.h"
#endif

bool telnet_ws_response(struct mg_connection *c, int ev, void *ev_data, void *fn_data) ;

// 投屏
typedef struct {
    uint16_t x ;
    uint16_t y ;
    uint8_t * raw ;
    uint32_t rawlen ;
} frame_video_rect_t ;
bool telnet_ws_projection_sessn_init()  ;
void telnet_ws_projection_sess_release() ;
void telnet_ws_response_projection(struct mg_connection *conn, struct mg_ws_message * wm) ;
void post_jpg_rect(uint16_t x, uint16_t y, void * jpgraw, uint32_t jpgraw_len) ;

void telnet_ws_set_repl_client(struct mg_connection * conn) ;
void telnet_ws_output(uint8_t cmd, int pkgid, const char * data, size_t datalen) ;

// void ws_disp_flush(lv_disp_drv_t * disp, const lv_area_t * area, lv_color_t * color_p) ;
// void ws_driver_input_read(lv_indev_drv_t *drv, lv_indev_data_t *data) ;

void be_telnet_ws_init() ;
void be_telnet_ws_require(JSContext * ctx, JSValue telnet) ;


#endif