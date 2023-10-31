#include "driver_camera.h"
#include "module_metadata.h"
#include "module_wifi.h"
#include "module_mg.h"
#include "esp_camera.h"
#include <string.h>
#include "img_converters.h"

#define PART_BOUNDARY "123456789000000000000987654321"
static const char *_STREAM_CONTENT_TYPE = "multipart/x-mixed-replace;boundary=" PART_BOUNDARY;
static const char *_STREAM_BOUNDARY = "\r\n--" PART_BOUNDARY "\r\n";
static const char *_STREAM_PART = "Content-Type: image/jpeg\r\nContent-Length: %u\r\nX-Timestamp: %d.%06d\r\n\r\n";


bool inited = false ;

bool driver_camera_has_inited() {
    return inited ;
}

static JSValue js_camera_has_setup(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    return inited? JS_TRUE: JS_FALSE ;
}

/*
@brief 
@param pins
@param options

format:
yuv422
grayscale
rgb565
jpeg

size:
QVGA    320 x 240
CIF     352 x 288
VGA     640 x 480
SVGA    800 x 600
XGA     1024 x 768
SXGA    1280 x 1024
UXGA    1600 x 1200
*/
static JSValue js_camera_setup(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {

    if(inited) {
        THROW_EXCEPTION("camera device has setuped.")
    }

    CHECK_ARGC(1)
    if( !JS_IsObject(argv[0]) ) {
        THROW_EXCEPTION("setup param must be a object")
    }
    
    camera_config_t config ;
    memset(&config, 0, sizeof(camera_config_t));

    ASSIGN_INT_PROP(argv[0], "d0", config.pin_d0)
    ASSIGN_INT_PROP(argv[0], "d1", config.pin_d1)
    ASSIGN_INT_PROP(argv[0], "d2", config.pin_d2)
    ASSIGN_INT_PROP(argv[0], "d3", config.pin_d3)
    ASSIGN_INT_PROP(argv[0], "d4", config.pin_d4)
    ASSIGN_INT_PROP(argv[0], "d5", config.pin_d5)
    ASSIGN_INT_PROP(argv[0], "d6", config.pin_d6)
    ASSIGN_INT_PROP(argv[0], "d7", config.pin_d7)
    ASSIGN_INT_PROP(argv[0], "xclk", config.pin_xclk)
    ASSIGN_INT_PROP(argv[0], "pclk", config.pin_pclk)
    ASSIGN_INT_PROP(argv[0], "vsync", config.pin_vsync)
    ASSIGN_INT_PROP(argv[0], "href", config.pin_href)
    ASSIGN_INT_PROP(argv[0], "sda", config.pin_sscb_sda)
    ASSIGN_INT_PROP(argv[0], "scl", config.pin_sscb_scl)
    ASSIGN_INT_PROP_DEFAULT(argv[0], "pwdn", config.pin_pwdn, -1)
    ASSIGN_INT_PROP_DEFAULT(argv[0], "reset", config.pin_reset, -1)

    ASSIGN_INT_PROP_DEFAULT(argv[0], "ledc_channel", config.ledc_channel, 1)
    ASSIGN_INT_PROP_DEFAULT(argv[0], "ledc_timer", config.ledc_timer, 1)
    ASSIGN_INT_PROP_DEFAULT(argv[0], "jpeg_quality", config.jpeg_quality, 10)
    ASSIGN_UINT_PROP_DEFAULT(argv[0], "fb_count", config.fb_count, 2)
    ASSIGN_INT_PROP_DEFAULT(argv[0], "xclk_freq", config.xclk_freq_hz, 20000000)
    
    JSValue format = JS_GetPropertyStr(ctx, argv[0], "format") ;
    if( format==JS_UNDEFINED ) {
        config.pixel_format = PIXFORMAT_JPEG ;
    }
    else {
        char * cformat = (char *)JS_ToCString(ctx, format) ;
        JS_FreeValue(ctx,format) ;

        if(strcmp(cformat, "jpeg")==0) {
            config.pixel_format = PIXFORMAT_JPEG ;
        }
        else if(strcmp(cformat, "rgb565")==0) {
            config.pixel_format = PIXFORMAT_RGB565 ;
        }
        else if(strcmp(cformat, "yuv422")==0) {
            config.pixel_format = PIXFORMAT_YUV422 ;
        }
        else if(strcmp(cformat, "grayscale")==0) {
            config.pixel_format = PIXFORMAT_GRAYSCALE ;
        }
        else {
            THROW_EXCEPTION("unknow options.format value")
        }
        JS_FreeCString(ctx, cformat) ;
    }


    JSValue size = JS_GetPropertyStr(ctx, argv[0], "size") ;
    if( format==JS_UNDEFINED ) {
        config.frame_size = PIXFORMAT_JPEG ;
    }
    else {
        char * csize = (char *)JS_ToCString(ctx, size) ;
        JS_FreeValue(ctx,size) ;

        if(strcmp(csize, "QVGA")==0) {
            config.frame_size = FRAMESIZE_QVGA ;
        }
        else if(strcmp(csize, "CIF")==0) {
            config.frame_size = FRAMESIZE_CIF ;
        }
        else if(strcmp(csize, "VGA")==0) {
            config.frame_size = FRAMESIZE_VGA ;
        }
        else if(strcmp(csize, "SVGA")==0) {
            config.frame_size = FRAMESIZE_SVGA ;
        }
        else if(strcmp(csize, "XGA")==0) {
            config.frame_size = FRAMESIZE_XGA ;
        }
        else if(strcmp(csize, "SXGA")==0) {
            config.frame_size = FRAMESIZE_SXGA ;
        }
        else if(strcmp(csize, "UXGA")==0) {
            config.frame_size = FRAMESIZE_UXGA ;
        }
        else {
            THROW_EXCEPTION("unknow options.size value")
        }
        JS_FreeCString(ctx, csize) ;
    }

    config.fb_location = CAMERA_FB_IN_PSRAM ;
    config.grab_mode = 0 ;

    esp_err_t err = esp_camera_init(&config);
    if (err != ESP_OK) {
        THROW_EXCEPTION("Camera Init Failed, %s (%d)", esp_err_to_name(err), err)
    }

    inited = true ;

    // camera_fb_t * fb = esp_camera_fb_get();
    // dn2(fb->width, fb->height)
    // dn2(fb->format, fb->len)    
    // esp_camera_fb_return(fb);

    return JS_TRUE ;
}


esp_err_t ws_rtc_camera_stream(httpd_req_t *req) {

    httpd_resp_set_hdr(req, "Access-Control-Allow-Origin", "*");
    httpd_resp_set_hdr(req, "X-Framerate", "60");

    if(!inited) {
        httpd_resp_set_status(req, "404") ;
        return httpd_resp_send(req, "Camera device not setuped", 5);
    }

    esp_err_t res = httpd_resp_set_type(req, _STREAM_CONTENT_TYPE);

    char *part_buf[128];
    camera_fb_t * fb = NULL ;
    size_t hlen = 0 ;

    while (true) {

        fb = esp_camera_fb_get();
        if (!fb) {
            httpd_resp_send(req, "Camera device not setuped", 5);
            break ;
        }

        res = httpd_resp_send_chunk(req, _STREAM_BOUNDARY, strlen(_STREAM_BOUNDARY));
        if(res!=ESP_OK) {
            break ;
        }
        
        hlen = snprintf((char *)part_buf, 128, _STREAM_PART, fb->len, fb->timestamp.tv_sec, fb->timestamp.tv_usec);
        res = httpd_resp_send_chunk(req, (const char *)part_buf, hlen);
        if(res!=ESP_OK) {
            break ;
        }

        res = httpd_resp_send_chunk(req, (const char *)fb->buf, fb->len);
        if(res!=ESP_OK) {
            break ;
        }
        
        esp_camera_fb_return(fb);
        fb = NULL ;
    }

    if(fb) {
        esp_camera_fb_return(fb);
    }

    return ESP_OK;
}

static httpd_handle_t stream_httpd = NULL;

static JSValue js_camera_start_http_stream(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    CHECK_ARGC(1)
    ARGV_TO_INT32(0, port)
    
    if(!wifi_has_inited() || stream_httpd) {
        return JS_FALSE ;
    }
    
    httpd_config_t config = HTTPD_DEFAULT_CONFIG();
    config.max_uri_handlers = 8;
    config.server_port = port ;
    config.ctrl_port += port ;
    config.core_id = 1 ;

    httpd_uri_t stream_uri = {
        .uri = "/",
        .method = HTTP_GET,
        .handler = ws_rtc_camera_stream,
        .user_ctx = NULL};

    if (httpd_start(&stream_httpd, &config) != ESP_OK) {
        printf("start camera stream server faild\n") ;
        return JS_FALSE ;
    }
    
    httpd_register_uri_handler(stream_httpd, &stream_uri);

    return JS_TRUE ;
}

TaskHandle_t task_cam_tcp_handle = NULL ;

static void task_camera_tcp_stream(void * data) {

    camera_fb_t * fb = NULL ;
    uint8_t pkgid = 0 ;
    
    DEF_PKG(pkg)
    pkg.header[PKG0519_POS_CMD] = CMD_DATA ;
    
    uint8_t * lenbuff = pkg.header + PKG0519_HEADERLEN_WITHOUT_DATALEN ;

    int listen_sock =  socket(AF_INET, SOCK_STREAM, IPPROTO_IP);
    if(listen_sock < 0)  {
        printf("Unable to create socket: errno %d\n", errno);
    }

    struct sockaddr_in dest_addr;
    dest_addr.sin_family = AF_INET;
    dest_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    dest_addr.sin_port = htons(8018);

    int err = bind(listen_sock, (struct sockaddr *)&dest_addr, sizeof(dest_addr));
    if(err != 0)  {
        printf("Socket unable to bind: errno %d\n", errno);
        close(listen_sock);
    }
    printf("Socket bound, port 8019\n");

    err = listen(listen_sock, 1);    // 这里为啥是1，网上大多数是5
    if(err != 0)  {
        printf("Error occurred during listen: errno %d\n", errno);
        close(listen_sock);
    }

    while(1) {
        struct sockaddr_in6 source_addr; // Large enough for both IPv4 or IPv6
        uint addr_len = sizeof(source_addr);
        int sock = accept(listen_sock, (struct sockaddr *)&source_addr, &addr_len);
        if(sock < 0)  {
            printf("Unable to accept connection: errno %d\n", errno);
            close(listen_sock);
        }
        
        printf("camera tcp client come in\n") ;
        uint8_t lenbytes = 1 ;
        int sent = 0 ;

        while(1) {
            fb = esp_camera_fb_get();
            if(!fb) {
                vTaskDelay(10) ;
                continue;
            }

            pkg.header[PKG0519_POS_ID] = pkgid ++ ;
            pkg.header[PKG0519_POS_VERIFY] = telnet_prot0519_checksum(fb->buf, fb->len) ;

            lenbytes = telnet_prot0519_pack_data_len(fb->len, lenbuff) ;

            if( send(sock, pkg.header, PKG0519_HEADERLEN_WITHOUT_DATALEN + lenbytes, 0)<0 ){
                break ;
            }

            necho_time("send", {
                sent = send(sock, fb->buf, fb->len, 0) ;
            })
            if( sent<0 ){
                break ;
            }

            esp_camera_fb_return(fb);
            fb = NULL ;
            // dd
            vTaskDelay(1) ;
        }

        printf("camera tcp client close\n") ;

        if(fb) {
            esp_camera_fb_return(fb);
            fb = NULL ;
        }

        shutdown(sock, 0) ;
        close(sock) ;

    }
}

static JSValue js_camera_start_tcp_stream(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    CHECK_ARGC(1)
    ARGV_TO_STRING_E(0, url, "invalid url")
    
    if(task_cam_tcp_handle) {
        THROW_EXCEPTION("camera tcp stream has started.\n")
    }

    // mg_listen(be_module_mg_mgr(), url, camera_tcp_stream, NULL);
    xTaskCreatePinnedToCore(&task_camera_tcp_stream, "task_camera_tcp_stream", 1024*2, NULL, 5, &task_cam_tcp_handle, 1) ;

    JS_FreeCString(ctx, url) ;

    return JS_UNDEFINED ;
}


static JSValue js_camera_unsetup(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    if(!inited) {
        THROW_EXCEPTION("camera device not setup yet.")
    }
    if(esp_camera_deinit()==ESP_OK) {
        inited = false ;
        return JS_TRUE ;
    }
    else {
        return JS_FALSE ;
    }
}

static JSValue js_camera_capture(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    CHECK_ARGC(0)
    
    if(!inited) {
        THROW_EXCEPTION("camera device not setup yet.")
    }

    camera_fb_t * fb = esp_camera_fb_get();
    if(fb==NULL) {
        return JS_NULL ;
    }

    uint8_t * buff = malloc(fb->len) ;
    if(!buff) {
        esp_camera_fb_return(fb);
        THROW_EXCEPTION("out of memory") ;
    }
    memcpy(buff, fb->buf, fb->len) ;
    JSValue arrbuff = JS_NewArrayBuffer(ctx, buff, fb->len, freeArrayBuffer, NULL, false) ;

    JS_SetPropertyStr(ctx, arrbuff, "width", JS_NewInt32(ctx, fb->width) ) ;
    JS_SetPropertyStr(ctx, arrbuff, "height", JS_NewInt32(ctx, fb->height) ) ;
    JS_SetPropertyStr(ctx, arrbuff, "format", JS_NewInt32(ctx, fb->format ) ) ;
    
    esp_camera_fb_return(fb);
    return arrbuff ;
}

static JSValue js_camera_jpeg2rgb888(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    CHECK_ARGC(2)

    // JSValue propWidth = JS_GetPropertyStr(ctx, argv[0], "width") ;
    // JSValue propHeight = JS_GetPropertyStr(ctx, argv[0], "height") ;

    // int width = 0;
    // int height = 0;
    // if( JS_ToInt32(ctx, &width, propWidth)!=0 || JS_ToInt32(ctx, &height, propHeight)!=0 ){
    //     THROW_EXCEPTION("arg frame is not a valid frame object, missing width or height property.")
    // }
    // dn2(width, height)

    ARGV_TO_ARRAYBUFFER(0, jpgbuf, jpglen) 
    ARGV_TO_ARRAYBUFFER(1, rawbuf, rawlen) 

    return fmt2rgb888(jpgbuf, jpglen, PIXFORMAT_JPEG, rawbuf)? JS_TRUE : JS_FALSE;
}

struct mg_connection * stream_client = NULL ;
int64_t last_stream_tick = 0 ;

bool be_module_driver_camera_response(struct mg_connection *c, int ev, void *ev_data){

    if(ev==MG_EV_CLOSE) {
        // 推流接收客户端已断开
        if(c==stream_client) {
            printf("camera stream client closed.\n") ;
            stream_client = NULL ;
            return false ; // <- 可继续处理
        }
    }

    else if (ev == MG_EV_HTTP_MSG) {

        struct mg_http_message *hm = (struct mg_http_message *)ev_data;
        if(!hm) {
            mg_http_reply(c, 500, "", "%s", "Invalid mg_http_message");
            return true ;
        }

        if (mg_http_match_uri(hm, "/camera/capture")) {
            if(!inited) {
                mg_http_reply(c, 404, "", "%s", "Camera device not setuped");
                return true ;
            }
            camera_fb_t * fb = esp_camera_fb_get();
            if (!fb) {
                mg_http_reply(c, 500, "", "%s", "Camera capture failed");
                return true ;
            }

            mg_printf(c, "HTTP/1.1 200 OK\r\n") ;
            mg_printf(c, "Content-Type: image/jpeg\r\n") ;
            mg_printf(c, "Content-Disposition: inline; filename=picture.jpg\r\n") ;
            mg_printf(c, "Content-Length: %d\r\n\r\n", fb->len);

            mg_send(c, fb->buf, fb->len);

            esp_camera_fb_return(fb);

            return true ;
        }
    }

    return false ;
}

void be_module_driver_camera_init() {
    inited = false ;
    // web_camera_init() ;
}

void be_module_driver_camera_require(JSContext *ctx, JSValue driver) {

    JSValue camera = JS_NewObject(ctx);
    JS_SetPropertyStr(ctx, driver, "camera", camera);

    JS_SetPropertyStr(ctx, camera, "setup", JS_NewCFunction(ctx, js_camera_setup, "setup", 1));
    JS_SetPropertyStr(ctx, camera, "unsetup", JS_NewCFunction(ctx, js_camera_unsetup, "unsetup", 1));
    JS_SetPropertyStr(ctx, camera, "hasSetup", JS_NewCFunction(ctx, js_camera_has_setup, "hasSetup", 1));
    JS_SetPropertyStr(ctx, camera, "capture", JS_NewCFunction(ctx, js_camera_capture, "capture", 1));
    JS_SetPropertyStr(ctx, camera, "startHTTPStream", JS_NewCFunction(ctx, js_camera_start_http_stream, "startHTTPStream", 1));
    JS_SetPropertyStr(ctx, camera, "startTCPStream", JS_NewCFunction(ctx, js_camera_start_tcp_stream, "startTCPStream", 1));
}


void be_module_driver_camera_reset(JSContext *ctx) {
    if(inited) {
        esp_camera_deinit() ;
        inited = false ;
    }
}

