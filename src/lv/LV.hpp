#pragma once

#include "deps/lvgl/lvgl.h"
#include "esp_timer.h"
#include <map>
#include <string>

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
        
        static std::map<std::string, const lv_image_dsc_t  *> embededImages ;
    } ;

    void addImageDsc(const char * name, const lv_image_dsc_t  *) ;
}}