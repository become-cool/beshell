#include "audio_stream.h"
#include "module_fs.h"


audio_el_t *  audio_el_src_create(audio_pipe_t * pipe, uint8_t core) {
    audio_el_src_t * el ;
    ELEMENT_CREATE(pipe, audio_el_src_t, el, task_src, 1024*3, 5, core, 1024*2)
    return el ;
}

bool audio_el_src_strip_mp3(audio_el_src_t * el) {
    if(!audio_el_src_strip_pcm(el)){
        return false ;
    }

    char tag[10];
    int tag_len = 0;
    int read_bytes = fread(tag, 1, 10, el->file);

    if(read_bytes != 10) {
        printf("mp3 file length invalid (%d)\n",read_bytes) ; // @TODO: post js event
        return false ;
    }
    
    if (memcmp(tag,"ID3",3) == 0)  {
        tag_len = ((tag[6] & 0x7F)<< 21)|((tag[7] & 0x7F) << 14) | ((tag[8] & 0x7F) << 7) | (tag[9] & 0x7F);
        fseek(el->file, tag_len - 10, SEEK_SET);
    }
    else  {
        fseek(el->file, 0, SEEK_SET);
    }

    return true ;
}


bool audio_el_src_strip_pcm(audio_el_src_t * el) {

    if(el->file){
        fclose(el->file) ;
        el->file = NULL ;
    }
    el->file = fopen(el->src_path,"rb") ;
    if(!el->file) {
        printf("can not open file: %s", el->src_path) ;
        return false ;
    }

    return true ;
}


void audio_el_src_delete(audio_el_src_t * el) {
    if(el->file) {
        fclose(el->file) ;
        el->file = NULL ;
    }
    audio_el_delete(el) ;
}


// 任务线程：文件读入
void task_src(audio_el_src_t * el) {
    // printf("task_src()\n") ;
    // int cmd ;

    char buff[512] ;
    size_t read_bytes ;
    EventBits_t bits ;

    while(1) {

        // 等待开始状态
        bits = xEventGroupWaitBits(el->base.stats, STAT_RUNNING|STAT_STOPPING, false, false, portMAX_DELAY);
        if( bits&STAT_STOPPING ) {
            audio_el_stop_when_req(el) ;

            if(el->file) {
                fclose(el->file) ;
                el->file = NULL ;
            }

            vTaskDelay(1) ;
            continue ;
        }

        if(!el->file) {
            goto finish ;
            continue ;
        }

        nechof_time("fs read, bytes %d", {
            read_bytes = fread(buff, 1, sizeof(buff), el->file) ;
        }, read_bytes)
        if(!read_bytes) {
            goto finish ;
            continue ;
        }

        if(pdTRUE != xRingbufferSend(el->base.ring, buff, read_bytes, portMAX_DELAY)) {
            printf("task src xRingbufferSend() faild ???") ;
        }
        else {
            xEventGroupClearBits(el->base.stats, STAT_DRAIN) ;
        }
        vTaskDelay(0) ;
        continue;
finish:
        ((audio_pipe_t *)el->base.pipe)->finished = true ;
        audio_el_set_stat( el, STAT_STOPPING ) ;
        vTaskDelay(10) ;
        continue;

    }
}