#include "audio_stream.h"
#include "module_serial.h"
#include "driver/i2s.h"
#include "utils.h"


// 任务线程：放音
static void task_pcm_playback(audio_el_i2s_t * el) {

    // printf("task_pcm_playback()\n") ;

    EventBits_t bits ;
    int idle_ticks = 10/ portTICK_PERIOD_MS ;
    int64_t t0 = gettime(), t = 0 ;

    char buff[512] ;
    uint32_t data_size ;
    size_t data_wroten ;
    char * pwrite = NULL ;

    for(int i=0;i<10;) {
        
        vTaskDelay(0) ;

        // 等待状态
        bits = xEventGroupWaitBits(el->base.stats, STAT_RUNNING|STAT_STOPPING|STAT_PAUSING, false, false, portMAX_DELAY);

        // 停止信号
        if( bits&STAT_STOPPING ) {
            audio_el_stop_when_req(el) ;
            vTaskDelay(1) ;
            continue ;
        }
        // 暂停信号
        else if( bits&STAT_PAUSING ) {
            i2s_zero_dma_buffer(el->i2s) ;
            audio_el_set_stat(el, STAT_PAUSED) ;
            vTaskDelay(1) ;
            continue ;
        }


        // dp(el->base.upstream->ring)
        // EL_RECV_UPSTREAM(el, pwrite, buff, sizeof(buff), data_size)

        if(!el->base.upstream || !el->base.upstream->ring) {
            vTaskDelay(10) ;
            continue ;
        }
        data_size = 0 ;
        nechof_time("i2s received %d", {
            pwrite = xRingbufferReceiveUpTo(el->base.upstream->ring, &data_size, 10, sizeof(buff));
        },data_size) ;
        if(data_size==0 || !pwrite) {

            printf("el_i2s receive empty\n") ;
            
            // 确定前级已流干（ring buffer 里的可读数据可能分在头尾两端，需要两次才能读空）
            if(audio_el_is_drain(el->base.upstream)) {
                // printf("i2s's input drain\n") ;
                i2s_zero_dma_buffer(el->i2s) ;
                xEventGroupSetBits(el->base.upstream->stats, STAT_DRAIN) ;  
                vTaskDelay(1) ;
            }

            continue ;
        }
        
        memcpy(buff, pwrite, data_size) ;
        vRingbufferReturnItem(el->base.upstream->ring, pwrite) ;
        
        pwrite = buff ;
            
        t = gettime()-t0 ;

        while(data_size) {

            data_wroten = 0 ;
            nechof_time("delay:%lld,%d->%d", {
                // 扩展到 32 sample                
                i2s_write_expand(el->i2s, pwrite, data_size, 16, 32, &data_wroten, portMAX_DELAY );
            }, t, data_size, data_wroten)

            data_size-= data_wroten ;
            pwrite+= data_wroten ;
        }

        t0 = gettime() ;
        // i++ ;

    }

    while(1){}
}

audio_el_i2s_t * audio_el_i2s_create(audio_pipe_t * pipe, uint8_t i2s_num, uint8_t core) {
    audio_el_i2s_t * el = NULL ;
    ELEMENT_CREATE(pipe, audio_el_i2s_t, el, task_pcm_playback, 1024*3, 5, core, 0)
    if(el) {
        el->i2s = i2s_num ;
    }
    return el ;
}



        
void audio_el_i2s_delete(audio_el_i2s_t * el) {
    i2s_zero_dma_buffer(el->i2s);
    audio_el_delete(el) ;
}