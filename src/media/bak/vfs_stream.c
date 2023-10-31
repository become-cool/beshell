/*
 * ESPRESSIF MIT License
 *
 * Copyright (c) 2018 <ESPRESSIF SYSTEMS (SHANGHAI) PTE LTD>
 *
 * Permission is hereby granted for use on all ESPRESSIF SYSTEMS products, in which case,
 * it is free of charge, to any person obtaining a copy of this software and associated
 * documentation files (the "Software"), to deal in the Software without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the Software is furnished
 * to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all copies or
 * substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
 * FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
 * COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
 * IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 */

#include <sys/unistd.h>
#include <sys/stat.h>
#include <stdio.h>
#include <string.h>
#include "errno.h"

#include "freertos/FreeRTOS.h"
#include "freertos/semphr.h"
#include "freertos/task.h"

#include "vfs_stream.h"
#include "audio_common.h"
#include "audio_mem.h"
#include "audio_element.h"
#include "wav_head.h"
#include "esp_log.h"
#include "utils.h"
#include "module_serial.h"

#define FILE_WAV_SUFFIX_TYPE  "wav"
#define FILE_OPUS_SUFFIX_TYPE "opus"
#define FILE_AMR_SUFFIX_TYPE "amr"
#define FILE_AMRWB_SUFFIX_TYPE "Wamr"

static const char *TAG = "VFS_STREAM";



typedef enum {
    STREAM_TYPE_UNKNOWN,
    STREAM_TYPE_WAV,
    STREAM_TYPE_OPUS,
    STREAM_TYPE_AMR,
    STREAM_TYPE_AMRWB,
} wr_stream_type_t;

typedef struct vfs_stream {
    audio_stream_type_t type;
    int block_size;
    bool is_open;
    FILE *file;
    wr_stream_type_t w_type;
    bool write_header;
} vfs_stream_t;

static wr_stream_type_t get_type(const char *str)
{
    char *relt = strrchr(str, '.');
    if (relt != NULL) {
        relt ++;
        ESP_LOGD(TAG, "result = %s", relt);
        if (strncasecmp(relt, FILE_WAV_SUFFIX_TYPE, 3) == 0) {
            return STREAM_TYPE_WAV;
        } else if (strncasecmp(relt, FILE_OPUS_SUFFIX_TYPE, 4) == 0) {
            return STREAM_TYPE_OPUS;
        } else if (strncasecmp(relt, FILE_AMR_SUFFIX_TYPE, 3) == 0) {
            return STREAM_TYPE_AMR;
        } else if (strncasecmp(relt, FILE_AMRWB_SUFFIX_TYPE, 4) == 0) {
            return STREAM_TYPE_AMRWB;
        } else {
            return STREAM_TYPE_UNKNOWN;
        }
    } else {
        return STREAM_TYPE_UNKNOWN;
    }
}

static esp_err_t _vfs_open(audio_element_handle_t self)
{
    vfs_stream_t *vfs = (vfs_stream_t *)audio_element_getdata(self);

    audio_element_info_t info;
    char *uri = audio_element_get_uri(self);
    ESP_LOGD(TAG, "_vfs_open,   %s", uri);
    char *path = uri ; //strstr(uri, "/vfs");
    audio_element_getinfo(self, &info);
    if (path == NULL) {
        ESP_LOGE(TAG, "Need file path to open");
        return ESP_FAIL;
    }
    if (vfs->is_open) {
        ESP_LOGE(TAG, "Already opened");
        return ESP_FAIL;
    }
    if (vfs->type == AUDIO_STREAM_READER) {
        vfs->file = fopen(path, "r");
        struct stat siz =  { 0 };
        stat(path, &siz);
        info.total_bytes = siz.st_size;
        ESP_LOGI(TAG, "File size is %d byte, pos:%d", (int)siz.st_size, (int)info.byte_pos);
        if (vfs->file && (info.byte_pos > 0)) {
            if (fseek(vfs->file, info.byte_pos, SEEK_SET) != 0) {
                ESP_LOGE(TAG, "Error seek file");
                return ESP_FAIL;
            }
        }
    } else if (vfs->type == AUDIO_STREAM_WRITER) {
        vfs->file = fopen(path, "w+");
        vfs->w_type =  get_type(path);
        if (vfs->file && STREAM_TYPE_WAV == vfs->w_type) {
            wav_header_t info = {0};
            fwrite(&info, 1, sizeof(wav_header_t), vfs->file);
            fsync(fileno(vfs->file));
        } else if (vfs->file && (STREAM_TYPE_AMR == vfs->w_type) && (vfs->write_header == true)) {
            fwrite("#!AMR\n", 1, 6, vfs->file);
            fsync(fileno(vfs->file));
        } else if (vfs->file && (STREAM_TYPE_AMRWB == vfs->w_type) && (vfs->write_header == true)) {
            fwrite("#!AMR-WB\n", 1, 9, vfs->file);
            fsync(fileno(vfs->file));
        }
    } else {
        ESP_LOGE(TAG, "VFS must be Reader or Writer");
        return ESP_FAIL;
    }
    if (vfs->file == NULL) {
        ESP_LOGE(TAG, "Failed to open file %s", path);
        return ESP_FAIL;
    }
    vfs->is_open = true;
    if (info.byte_pos && fseek(vfs->file, info.byte_pos, SEEK_SET) != 0) {
        ESP_LOGE(TAG, "Failed to seek to %d/%d", (int)info.byte_pos, (int)info.total_bytes);
        return ESP_FAIL;
    }
    int ret = audio_element_set_total_bytes(self, info.total_bytes);
    return ret;
}

static int _vfs_read(audio_element_handle_t self, char *buffer, int len, TickType_t ticks_to_wait, void *context)
{
    vfs_stream_t *vfs = (vfs_stream_t *)audio_element_getdata(self);
    audio_element_info_t info;
    audio_element_getinfo(self, &info);

    int64_t t0=gettime() ;
    int rlen = fread(buffer, 1, len, vfs->file);
    printf("%d,%lld\n",rlen, gettime() -t0);

    if (rlen <= 0) {
        ESP_LOGW(TAG, "No more data, ret:%d", rlen);
    } else {
        audio_element_update_byte_pos(self, rlen);
    }
    return rlen;
}

static int _vfs_write(audio_element_handle_t self, char *buffer, int len, TickType_t ticks_to_wait, void *context)
{
    vfs_stream_t *vfs = (vfs_stream_t *)audio_element_getdata(self);
    audio_element_info_t info;
    audio_element_getinfo(self, &info);
    int wlen = fwrite(buffer, 1, len, vfs->file);
    fsync(fileno(vfs->file));
    ESP_LOGD(TAG, "write:%d, errno:%d, pos:%d", wlen, errno, (int)info.byte_pos);
    if (wlen > 0) {
        audio_element_update_byte_pos(self, wlen);
    }
    return wlen;
}

static int _vfs_process(audio_element_handle_t self, char *in_buffer, int in_len)
{
    int r_size = audio_element_input(self, in_buffer, in_len);
    int w_size = 0;
    if (r_size > 0) {
        w_size = audio_element_output(self, in_buffer, r_size);
    } else {
        w_size = r_size;
    }
    return w_size;
}

static esp_err_t _vfs_close(audio_element_handle_t self)
{
    vfs_stream_t *vfs = (vfs_stream_t *)audio_element_getdata(self);

    if (AUDIO_STREAM_WRITER == vfs->type
        && vfs->file
        && STREAM_TYPE_WAV == vfs->w_type) {
        wav_header_t *wav_info = (wav_header_t *) audio_malloc(sizeof(wav_header_t));

        AUDIO_MEM_CHECK(TAG, wav_info, return ESP_ERR_NO_MEM);

        if (fseek(vfs->file, 0, SEEK_SET) != 0) {
            ESP_LOGE(TAG, "Error seek file, line=%d", __LINE__);
        }
        audio_element_info_t info;
        audio_element_getinfo(self, &info);
        wav_head_init(wav_info, info.sample_rates, info.bits, info.channels);
        wav_head_size(wav_info, (uint32_t)info.byte_pos);
        fwrite(wav_info, 1, sizeof(wav_header_t), vfs->file);
        fsync(fileno(vfs->file));
        audio_free(wav_info);
    }

    if (vfs->is_open) {
        fclose(vfs->file);
        vfs->is_open = false;
    }
    if (AEL_STATE_PAUSED != audio_element_get_state(self)) {
        audio_element_report_pos(self);
        audio_element_set_byte_pos(self, 0);
    }
    return ESP_OK;
}

static esp_err_t _vfs_destroy(audio_element_handle_t self)
{
    vfs_stream_t *vfs = (vfs_stream_t *)audio_element_getdata(self);
    audio_free(vfs);
    return ESP_OK;
}

audio_element_handle_t vfs_stream_init(vfs_stream_cfg_t *config)
{
    audio_element_handle_t el;

    vfs_stream_t *vfs = audio_calloc(1, sizeof(vfs_stream_t));

    AUDIO_MEM_CHECK(TAG, vfs, return NULL);

    audio_element_cfg_t cfg = DEFAULT_AUDIO_ELEMENT_CONFIG();
    cfg.open = _vfs_open;
    cfg.close = _vfs_close;
    cfg.process = _vfs_process;
    cfg.destroy = _vfs_destroy;
    cfg.task_stack = config->task_stack;
    cfg.task_prio = config->task_prio;
    cfg.task_core = config->task_core;
    cfg.out_rb_size = config->out_rb_size;
    cfg.buffer_len = config->buf_sz;
    if (cfg.buffer_len == 0) {
        cfg.buffer_len = VFS_STREAM_BUF_SIZE;
    }

    cfg.tag = "vfs";
    vfs->type = config->type;
    vfs->write_header = config->write_header;

    if (config->type == AUDIO_STREAM_WRITER) {
        cfg.write = _vfs_write;
    } else {
        cfg.read = _vfs_read;
    }

    el = audio_element_init(&cfg);
    AUDIO_MEM_CHECK(TAG, el, goto _vfs_init_exit);
    audio_element_setdata(el, vfs);

    return el;
_vfs_init_exit:
    audio_free(vfs);
    return NULL;
}
