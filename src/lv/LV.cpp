#include "LV.hpp"
#include "fs/FS.hpp"
#include <dirent.h>
#include "BeShell.hpp"
#include "debug.h"
using namespace std;


namespace be::lv {

    std::map<const char *, const lv_image_dsc_t  *> LV::preloadedImageDsc ;

    LV::LV() {}

    static void lv_tick_inc_cb(void *data) {
        uint32_t tick_inc_period_ms = *((uint32_t *) data);
        lv_tick_inc(tick_inc_period_ms);
    }
    
    void LV::initTick() {
        static const uint32_t tick_inc_period_ms = 20;
        const esp_timer_create_args_t periodic_timer_args = {
                .callback = lv_tick_inc_cb,
                .arg = (void*)&tick_inc_period_ms,
                .dispatch_method = ESP_TIMER_TASK,            
                .skip_unhandled_events = true,
        };
        ESP_ERROR_CHECK(esp_timer_create(&periodic_timer_args, &tickTimer));
        ESP_ERROR_CHECK(esp_timer_start_periodic(tickTimer, tick_inc_period_ms * 1000));
    }

    static void * fs_open(lv_fs_drv_t * drv, const char * path, lv_fs_mode_t mode) {
        FILE * f = NULL;
        if(mode == LV_FS_MODE_WR) {
            f = fopen(path, "wb");
        }
        else if(mode == LV_FS_MODE_RD) {
            f = fopen(path, "rb");
        }
        else if(mode == (LV_FS_MODE_WR | LV_FS_MODE_RD)) {
            f = fopen(path, "rb+");
        }
        return f;
    }
    static lv_fs_res_t fs_close(lv_fs_drv_t * drv, void * file_p) {
        lv_fs_res_t res = LV_FS_RES_OK;
        if (EOF == fclose((FILE *) file_p)) {
            res = LV_FS_RES_NOT_EX;
        }
        return res;
    }
    
    static lv_fs_res_t fs_read(lv_fs_drv_t * drv, void * file_p, void * buf, uint32_t btr, uint32_t * br) {
        (void) drv;
        *br = fread(buf, 1, btr, (FILE *) file_p);
        return LV_FS_RES_OK;
    }

    static lv_fs_res_t fs_write(lv_fs_drv_t * drv, void * file_p, const void * buf, uint32_t btw, uint32_t * bw) {
        (void) drv;
        *bw = fwrite(buf, 1, btw, (FILE *) file_p);
        return LV_FS_RES_OK;
    }

    static lv_fs_res_t fs_seek(lv_fs_drv_t * drv, void * file_p, uint32_t pos, lv_fs_whence_t whence) {
        (void) drv;
        int ret_val = fseek((FILE *) file_p, pos, whence);
        if (0 != ret_val) {
            return LV_FS_RES_UNKNOWN;
        }
        return LV_FS_RES_OK;
    }

    static lv_fs_res_t fs_tell(lv_fs_drv_t * drv, void * file_p, uint32_t * pos_p) {
        (void) drv;
        *pos_p = ftell((FILE *) file_p);
        return LV_FS_RES_OK;
    }

    static lv_fs_res_t fs_size (lv_fs_drv_t * drv, void * file_p, uint32_t * size_p) {
        (void) drv;		/*Unused*/
        FILE * fp = (FILE*)file_p;        /*Just avoid the confusing casings*/

        uint32_t cur = ftell(fp);

        fseek(fp, 0L, SEEK_END);
        *size_p = ftell(fp);

        /*Restore file pointer*/
        fseek(fp, cur, SEEK_SET);

        return LV_FS_RES_OK;
    }

    static void * fs_dir_open(lv_fs_drv_t * drv, const char *path) {
        (void) drv;
        DIR * dir = NULL;

        dir = opendir(path);
        if (NULL == dir) {
            return nullptr;
        }

        return dir;
    }
    
    static lv_fs_res_t fs_dir_read(lv_fs_drv_t * drv, void * rddir_p, char *fn) {
        readdir((DIR *) rddir_p);
        return LV_FS_RES_OK;
    }

    static lv_fs_res_t fs_dir_close(lv_fs_drv_t * drv, void * rddir_p) {
        (void) drv;
        int ret_val = closedir((DIR*)rddir_p);
        if (0 != ret_val) {
            return LV_FS_RES_UNKNOWN;
        }
        return LV_FS_RES_OK;
    }

    void LV::initFS(const BeShell & beshell) {
        // lv_fs_drv_init(&fs_drv);

        // fs_drv.letter = 'C';
        // fs_drv.open_cb = fs_open;
        // fs_drv.close_cb = fs_close;
        // fs_drv.read_cb = fs_read;
        // fs_drv.write_cb = fs_write;
        // fs_drv.seek_cb = fs_seek;
        // fs_drv.tell_cb = fs_tell;
        // // fs_drv.size_cb = fs_size;

        // fs_drv.dir_close_cb = fs_dir_close;
        // fs_drv.dir_open_cb = fs_dir_open;
        // fs_drv.dir_read_cb = fs_dir_read;

        // lv_fs_drv_register(&fs_drv);
    }

    void LV::setup(const BeShell & beshell) {
        initTick() ;
        initFS(beshell) ;
        lv_init() ;
    }
    void LV::loop() {
        lv_task_handler() ;
    }

    void preloadImage(const char * name, const lv_image_dsc_t  * imgDsc) {
        LV::addImageDsc[name] = imgDsc ;
    }

}