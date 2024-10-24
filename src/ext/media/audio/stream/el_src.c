#include "audio_stream.h"
#include "driver/i2s.h"
#include <stdbool.h>
#include <string.h>


audio_el_src_t *  audio_el_src_create(audio_pipe_t * pipe, uint8_t core) {
    audio_el_src_t * el ;
    ELEMENT_CREATE(pipe, audio_el_src_t, el, task_src, 1024*5, 10, core, 1024*2)
    el->base.name = "src" ;
    return el ;
}

bool audio_el_src_strip_mp3(audio_el_src_t * el) {

    if(el->file){
        fclose(el->file) ;
        el->file = NULL ;
    }
    el->file = fopen(el->src_path,"rb") ;
    if(!el->file) {
        printf("can not open file: %s", el->src_path) ;
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


struct WavHeader {
    char riffHeader[4];    // "RIFF"
    uint32_t fileSize;     // 文件大小
    char waveHeader[4];    // "WAVE"
    char fmtHeader[4];     // "fmt "
    uint32_t fmtChunkSize; // 格式块大小（通常为 16）
    uint16_t audioFormat;  // 音频格式（1 = PCM）
    uint16_t numChannels;  // 通道数
    uint32_t sampleRate;   // 采样率
    uint32_t byteRate;     // 每秒字节数
    uint16_t blockAlign;   // 块对齐
    uint16_t bitsPerSample;// 每个样本的位数
};

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

    struct WavHeader header ;

    // 读取 WAV 文件头
    if (fread(&header, sizeof(struct WavHeader), 1, el->file) != 1) {
        printf("read wav header faild.") ;
        fclose(el->file);
        return false;
    }

    // 检查 RIFF 和 WAVE 标识是否合法
    if (strncmp(header.riffHeader, "RIFF", 4) != 0 || strncmp(header.waveHeader, "WAVE", 4) != 0) {
        printf("invalid wav file.") ;
        fclose(el->file);
        el->file = NULL ;
        return false ;
    }

    // dn3(
    //     header.sampleRate,
    //     header.bitsPerSample,
    //     header.numChannels
    // )

    uint8_t bps = header.bitsPerSample ;
    i2s_channel_t ch = (i2s_channel_t)header.numChannels ;

    if(bps==16 && ch==1) {
        ch = 2 ;
        // bps = 32 ;
        ((audio_pipe_t *)el->base.pipe)->need_expand = true ;
    }
    else {
        ((audio_pipe_t *)el->base.pipe)->need_expand = false ;
    }

    i2s_set_clk(((audio_pipe_t *)el->base.pipe)->i2s,
        header.sampleRate,
        bps ,
        ch
    );
    
    i2s_stop(((audio_pipe_t *)el->base.pipe)->i2s) ;
    i2s_zero_dma_buffer(((audio_pipe_t *)el->base.pipe)->i2s) ;
    vTaskDelay(100 / portTICK_PERIOD_MS); 
    i2s_start(((audio_pipe_t *)el->base.pipe)->i2s) ;

    // 查找 'data' 块
    char chunkId[4];
    uint32_t chunkSize;
    int offset = sizeof(struct WavHeader);  // 初始偏移量为 WavHeader 的大小

    // 循环读取每个块，直到找到 'data' 块
    while (fread(chunkId, 4, 1, el->file) == 1) {
        if (fread(&chunkSize, sizeof(chunkSize), 1, el->file) != 1) {
            printf("read chunkSize faild.") ;
            fclose(el->file);
            el->file = NULL ;
            return false ;
        }

        if (strncmp(chunkId, "data", 4) == 0) {
            // 'data' 标记(4 字节) + 块大小(4 字节) = 8 字节
            fseek(el->file, offset + 8, SEEK_SET);
            // dn(offset + 8)
            return true ;
        } else {
            // 跳过非 'data' 块
            // dn(chunkSize)
            fseek(el->file, chunkSize, SEEK_CUR);
            offset += 8 + chunkSize;  // 8 字节为 chunkId 和 chunkSize 的大小
        }
    }

    return false ;
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
            printf("task_src() open file: %s\n", el->src_path) ;
            el->file = fopen(el->src_path,"rb") ;
            if(!el->file) {
                printf("can not open file: %s", el->src_path) ;
                goto finish ;
            }
                printf("file opened: %s", el->src_path) ;
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