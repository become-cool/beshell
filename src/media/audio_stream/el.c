#include "audio_stream.h"
#include "qjs_utils.h"

inline void audio_el_set_stat(audio_el_t * el, EventBits_t stat) {
    xEventGroupClearBits(el->stats, STAT_ALL_PHASE&(~stat)) ;
    xEventGroupSetBits(el->stats, stat) ;
}


void audio_el_init(
        audio_el_t * el
        , TaskFunction_t pvTaskCode
        , const char *const pcName
        , const uint32_t usStackDepth
        , UBaseType_t uxPriority
        , const BaseType_t xCoreID
        , size_t ringSz
    ) {

    el->ringSz = ringSz ;
    if(ringSz) {
        el->ring = xRingbufferCreate(ringSz, RINGBUF_TYPE_BYTEBUF);
    }
    else {
        el->ring = NULL ;
    }

    el->stats = xEventGroupCreate() ;
    audio_el_set_stat(el, STAT_STOPPED | STAT_DRAIN) ;

    xTaskCreatePinnedToCore(pvTaskCode, pcName, usStackDepth, el, uxPriority, &el->task, xCoreID);
}


void audio_el_delete(audio_el_t * el) {
    vTaskDelete(el->task) ;
    vEventGroupDelete(el->stats) ;
    if(el->ring) {
        vRingbufferDelete(el->ring) ;
    }
    free(el) ;
}

bool audio_el_is_running(audio_el_t * el) {
    return (xEventGroupGetBits(el->stats)&STAT_RUNNING) == STAT_RUNNING ;
}

bool audio_el_is_drain(audio_el_t * el) {
    if(!el->ring) {
        return true ;
    }
    UBaseType_t uxItemsWaiting = 0 ;
    vRingbufferGetInfo(el->ring,NULL,NULL,NULL,NULL,&uxItemsWaiting) ;
    return uxItemsWaiting==0 ;
}

void audio_el_print_stats(audio_el_t * el) {

    printf("%s: %s\n", "running", (xEventGroupGetBits(el->stats)&STAT_RUNNING)? "true": "false") ;
    printf("%s: %s\n", "stopping", (xEventGroupGetBits(el->stats)&STAT_STOPPING)? "true": "false") ;
    printf("%s: %s\n", "stopped", (xEventGroupGetBits(el->stats)&STAT_STOPPED)? "true": "false") ;
    printf("%s: %s\n", "drain", (xEventGroupGetBits(el->stats)&STAT_DRAIN)? "true": "false") ;

    if(el->ring) {
        UBaseType_t free = NULL ;
        UBaseType_t read = NULL ;
        UBaseType_t write = NULL ;
        UBaseType_t acquire = NULL ;
        UBaseType_t waiting = NULL ;
        vRingbufferGetInfo(el->ring, &free, &read, &write, &acquire,&waiting) ;

        #define print_ring_pos(var)  printf("ring %s@%d\n", #var, var) ;
        print_ring_pos(write)
        print_ring_pos(read)
        print_ring_pos(free)
        print_ring_pos(acquire)
        print_ring_pos(waiting)
    }
}



void audio_el_clear_ring(audio_el_t * el) {
    if(el->ring && el->ringSz) {
        vRingbufferDelete(el->ring) ;
        el->ring = xRingbufferCreate(el->ringSz, RINGBUF_TYPE_BYTEBUF);
    }
}


// 设置为 STAT_STOPING ,等待任务线程停止
bool audio_el_stop(audio_el_t * el) {
    EventBits_t stats = xEventGroupGetBits(el->stats) ;

    if( (stats&STAT_STOPPED) == STAT_STOPPED ) {
        return true ;
    }
    if( (stats&STAT_RUNNING) == STAT_RUNNING ) {
        xEventGroupSetBits(el->stats, STAT_STOPPING) ;
    }

    stats = xEventGroupWaitBits(el->stats, STAT_STOPPED, false, false, 100) ;
    return (stats&STAT_STOPPED) == STAT_STOPPED ;

}

// 该函数用于响应 STAT_STOPPING
// 等待数据流干后设置 STAT_STOPPED
#define PLAYER(el) ((audio_pipe_t*)el->pipe)
inline void audio_el_stop_when_req(audio_el_t * el) {
    if( ! (xEventGroupGetBits(el->stats) & STAT_STOPPING) ) {
        printf("not set STAT_STOPPING \n") ;
        return ;
    }

    // 等待数据流干
    if(!audio_el_is_drain(el)) {
        if(el->downstream) {
            audio_el_set_stat(el->downstream, STAT_RUNNING);
        }
        xEventGroupWaitBits(el->stats, STAT_DRAIN, false, true, 100);
    }

    audio_el_set_stat(el, STAT_STOPPED) ;

    // 将 STAT_STOPPING 向后级传递
    if(el->downstream) {
        audio_el_set_stat(el->downstream, STAT_STOPPING) ;
    }

    // 最后一个节点：触发 js finish 事件
    else {
        PLAYER(el)->running = false ;
        PLAYER(el)->paused = false ;
        if( PLAYER(el)->ctx && JS_IsObject(PLAYER(el)->jsobj) ){
            if(PLAYER(el)->error) {
                audio_pipe_emit_js(PLAYER(el), "error", JS_NewInt32(NULL, PLAYER(el)->error)) ;
            }
            audio_pipe_emit_js(PLAYER(el), PLAYER(el)->finished? "finish": "stop", JS_UNDEFINED) ;
        }
    }

    vTaskDelay(0) ;
}