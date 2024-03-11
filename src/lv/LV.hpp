#pragma once

#include "lvgl.h"
#include "esp_timer.h"
#include <map>

namespace be {
    class BeShell ;
    class FS ;
namespace lv {


    class LV {
    private:
        esp_timer_handle_t tickTimer;
        void initTick() ;

        // be::FS & fs ;
        lv_fs_drv_t fs_drv;
        void initFS(const be::BeShell & beshell) ;

    public:
        LV() ;
        void setup(const be::BeShell & beshell) ;
        void loop() ;
        
        static std::map<const char *, const lv_image_dsc_t  *> preloadedImageDsc ;
    } ;

    void addImageDsc(const char * name, const lv_image_dsc_t  *) ;
}}