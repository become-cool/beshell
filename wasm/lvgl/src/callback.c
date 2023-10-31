#include <stdlib.h>
#include <emscripten.h>
#include "lv_conf.h"
#include "lvgl.h"
#include "../../main/debug.h"

static void _cb_adapter_disp_drv_flush(lv_disp_drv_t * drv, const lv_area_t * area, lv_color_t * color_p) {
    // printf("lv_wasm_disp_drv_flush_cb()%d,%d,%d,%d, drv: %p\n", area->x1,area->y1, area->x2, area->y2, drv) ;
    EM_ASM_ARGS({
        Module.onDispDrvFlush && Module.onDispDrvFlush($0,$1,$2,$3,$4,$5)
    }, drv, area->x1,area->y1, area->x2, area->y2, color_p);
    lv_disp_flush_ready(drv) ;
}

EMSCRIPTEN_KEEPALIVE void * cb_adapter_disp_drv_flush() {
    return (void *)_cb_adapter_disp_drv_flush;
}


static void _cb_adapter_indev_drv_read(lv_indev_drv_t *drv, lv_indev_data_t *data) {
    if(drv->user_data) {
        lv_indev_data_t * input_data = (lv_indev_data_t *) drv->user_data ;
        data->point.x = input_data->point.x ;
        data->point.y = input_data->point.y ;
        data->state = input_data->state ;
        // printf("cb_adapter_indev_drv_read(), %d, %d, %d\n", data->point.x, data->point.y, data->state) ;
    }
}

EMSCRIPTEN_KEEPALIVE void * cb_adapter_indev_drv_read(){
    return (void *)_cb_adapter_indev_drv_read;
}



static void lv_event_cb(lv_event_t * event) {
    if(event->code==LV_EVENT_DRAW_MAIN && event->param) {
        lv_area_t * area = (lv_area_t *) event->param ;
        EM_ASM_ARGS({
            Module.onObjEventEmit && Module.onObjEventEmit($0,$1,$2,$3,$4,$5)
        }, event->current_target, event->code, area->x1, area->y1, area->x2, area->y2);
    }
    else if(event->code==LV_EVENT_GESTURE && event->param) {
        lv_dir_t dir = lv_indev_get_gesture_dir(event->param) ;
        EM_ASM_ARGS({
            Module.onObjEventEmit && Module.onObjEventEmit($0,$1,$2)
        }, event->current_target, event->code, dir);
    }
    else {
        EM_ASM_ARGS({
            Module.onObjEventEmit && Module.onObjEventEmit($0,$1)
        }, event->current_target, event->code);
    }
}
typedef struct _lv_event_dsc_t {
    lv_event_cb_t cb;
    void * user_data;
    lv_event_code_t filter : 8;
} lv_event_dsc_t;
static lv_event_dsc_t * find_event_dsc(lv_obj_t* obj, lv_event_code_t e) {
    if(!obj->spec_attr)
        return NULL ;

    if(!obj->spec_attr->event_dsc){
        return NULL ;
    }

    for(int i=0; i<obj->spec_attr->event_dsc_cnt; i++) {
        if(obj->spec_attr->event_dsc[i].cb!=lv_event_cb)
            continue ;
        if(obj->spec_attr->event_dsc[i].filter==LV_EVENT_ALL || obj->spec_attr->event_dsc[i].filter==e) {
            return & obj->spec_attr->event_dsc[i] ;
        }
    }
    
    return NULL ;
}
EMSCRIPTEN_KEEPALIVE void lv_obj_enable_event(lv_obj_t * obj, lv_event_code_t event) {
    if(find_event_dsc(obj, event)!=NULL) {
        return ;
    }
    // printf("lv_obj_enable_event: %d\r\n",event) ;
    lv_obj_add_event_cb(obj, lv_event_cb, event, NULL) ;
}

EMSCRIPTEN_KEEPALIVE void lv_obj_disable_event(lv_obj_t * obj, lv_event_code_t event) {
    lv_event_dsc_t * event_dsc = find_event_dsc(obj, event) ;
    if(event_dsc) {
        // printf("lv_obj_disable_event\r\n") ;
        lv_obj_remove_event_dsc(obj, event) ;
    }
}
