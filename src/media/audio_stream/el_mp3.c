#include "audio_stream.h"

#define BUFF_SRC_SIZE   MAINBUF_SIZE/2
#define BUFF_SRC_MEMTYPE   MALLOC_CAP_DMA


static void mp3dec_output(audio_el_mp3_t * el, uint8_t * data, size_t size) {

    if( el->decoder->samprate!=el->samprate || el->decoder->nChans!=el->channels ) {
        // dn2(el->decoder->samprate, el->decoder->nChans)

        // 等这一帧以前的数据播放完成
        // xEventGroupWaitBits(el->base.stats, STAT_DRAIN, false, true, 100);

        printf("i2s bus:%d, samprate:%d, bps:%d, chans:%d \n",
            el->i2s
            , el->decoder->samprate
            , 32
            , el->decoder->nChans) ;
        i2s_set_clk(
            el->i2s
            , el->decoder->samprate
            , 32
            , el->decoder->nChans
        );


        el->samprate = el->decoder->samprate ;
        el->channels = el->decoder->nChans ;
    }

#ifdef PLATFORM_ESP32
    // if(pdTRUE != xRingbufferSend(el->base.ring, buff_pcm, el->info.outputSamps * 2, portMAX_DELAY)) {
    if(pdTRUE != xRingbufferSend(el->base.ring, data, size, portMAX_DELAY)) {
        printf("task mp3 decode xRingbufferSend() wrong ?????\n") ;
    }
    else {
        xEventGroupClearBits(el->base.stats, STAT_DRAIN) ;
    }

#else
    // fwrite(buff_pcm,1,el->info.outputSamps * el->info.nChans,el->fout) ;
    fwrite(data,1,size,el->fout) ;
#endif
}

// 解码任务线程
static void task_mp3_decoder(audio_el_mp3_t * el) {

    // printf("task_mp3_decoder()\n") ;

    EventBits_t bits ;
    int res ;

    size_t data_size = 0 ;
    // unsigned char el->undecode_buff[BUFF_SRC_SIZE] ;

    unsigned char * psrc = el->undecode_buff ;
    int decode_left = 0 ;
    int offset = 0 ;
    int errs = 0 ;

    mp3dec_set_output_func(mp3dec_output) ;

    for(int i=0;i<10;) {
        vTaskDelay(0) ;

        // 等待开始状态
        bits = xEventGroupWaitBits(el->base.stats, STAT_RUNNING|STAT_STOPPING, false, false, portMAX_DELAY);
        if( bits&STAT_STOPPING ) {
            audio_el_stop_when_req(el) ;

            psrc = el->undecode_buff ;
            decode_left = 0 ;

            vTaskDelay(1) ;
            continue ;
        }

        if(decode_left && psrc) {
            memmove(el->undecode_buff, psrc, decode_left);
        }

#ifdef PLATFORM_ESP32
        data_size = 0 ;
        psrc = xRingbufferReceiveUpTo(el->base.upstream->ring, &data_size, 20, BUFF_SRC_SIZE-decode_left);
        if(psrc) {
            memcpy(el->undecode_buff+decode_left, psrc, data_size) ;
            vRingbufferReturnItem(el->base.upstream->ring, psrc) ;   
        }
        if ( data_size==0 || !psrc ) {

            printf("mp3 upstream drain\n") ;

            // 确定前级已流干（ring buffer 里的可读数据可能分在头尾两端，需要两次才能读空）
            if(audio_el_is_drain(el->base.upstream)) {
                // printf("decode's input drain\n") ;
                xEventGroupSetBits(el->base.upstream->stats, STAT_DRAIN) ;

                decode_left = 0 ;
                psrc = NULL ;
                
                vTaskDelay(1) ;
            }
            continue ;
        }

#else
        data_size = fread(el->undecode_buff+decode_left, 1, BUFF_SRC_SIZE-decode_left, el->fin) ;
        if(!data_size) {
            break ;
        }
#endif

        decode_left = decode_left + data_size;
        psrc = el->undecode_buff;
        

        offset = MP3FindSyncWord(psrc, decode_left);
        
        // dn3(data_size, decode_left,offset)
        if (offset < 0) {
            decode_left = 0;
            vTaskDelay(0) ;
            continue;

        } else {
            psrc += offset;                         //data start point
            decode_left -= offset;                 //in buffer

            necho_time("MP3Decode", {
            errs = MP3Decode(el->decoder, &psrc, &decode_left, el, 0);
            })

            if (errs < -1) {
                printf("MP3Decode failed ,code is %d, receive: %d, left data: %d, sync: %d \n",errs, data_size, decode_left, offset);

                // 遇到错误，停止解码
                // audio_pipe_emit_js(el->base.pipe, "error") ;

                ((audio_pipe_t *)el->base.pipe)->error = errs ;
                ((audio_pipe_t *)el->base.pipe)->finished = false ;

                audio_el_set_stat( ((audio_pipe_t *)el->base.pipe)->first, STAT_STOPPING ) ;
                
                // xEventGroupClearBits( el->base.stats, STAT_RUNNING ) ;
                

                vTaskDelay(0) ;
                continue ;
            }

            vTaskDelay(0) ;
        }
    }

    while(1) {}
}


audio_el_t * audio_el_mp3_create(audio_pipe_t * pipe, uint8_t core) {    
    audio_el_mp3_t * el ;
    echo_alloc("audio_el_mp3_t",{
        ELEMENT_CREATE(pipe, audio_el_mp3_t, el, task_mp3_decoder, 1024*3, 5, core, 1024*2)
    })

    echo_alloc("hexli", {
        el->decoder = MP3InitDecoder();
    })

    el->samprate = 0 ;
    el->channels = 0 ;

    el->undecode_buff = heap_caps_malloc(BUFF_SRC_SIZE, BUFF_SRC_MEMTYPE) ;

    return el ;
}


void audio_el_mp3_delete(audio_el_mp3_t * el) {
    MP3FreeDecoder(el->decoder);
    free(el->undecode_buff) ;
    el->undecode_buff = NULL ;
    audio_el_delete(el) ;
}

void audio_el_mp3_reset(audio_el_mp3_t * el) {
    MP3ResetDecoder(el->decoder) ;
}