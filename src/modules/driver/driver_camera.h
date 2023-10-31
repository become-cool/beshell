#ifndef __H_MODULE_CAMERA__
#define __H_MODULE_CAMERA__

#include "quickjs-libc.h"
#include <stdbool.h>
#include "mongoose.h"
#include "esp_http_server.h"
#include "utils.h"
#include "telnet_protocal_0519.h"

bool driver_camera_has_inited() ;

esp_err_t ws_rtc_camera_stream(httpd_req_t *req) ;

bool be_module_driver_camera_response(struct mg_connection *c, int ev, void *ev_data) ;

void be_module_driver_camera_init() ;
void be_module_driver_camera_require(JSContext *ctx, JSValue driver) ;
void be_module_driver_camera_loop(JSContext *ctx) ;
void be_module_driver_camera_reset(JSContext *ctx) ;

typedef void (*RemoteVideoCallback)(bepkg_t * pkg);

typedef struct {
    bepkg_t pkg ;
    TaskHandle_t task ;

    RemoteVideoCallback callback ;

} be_rv_strem_t ;


#endif