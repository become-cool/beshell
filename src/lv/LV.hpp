#pragma once

#include "lvgl.h"
#include "esp_timer.h"

namespace be {
namespace lv {

    class LV {
    private:
        esp_timer_handle_t tickTimer;
        void initTick() ;
    public:
        LV() ;
        void setup() ;
        void loop() ;
    } ;

}}