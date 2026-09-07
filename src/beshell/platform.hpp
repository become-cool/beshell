#pragma once

#include <stdint.h>

namespace be::platform {

    bool init() ;

    bool networkInit() ;

    // 休眠当前线程/任务（ESP: vTaskDelay；PC: std::this_thread::sleep_for）
    void sleepMs(uint32_t ms) ;

    // 系统启动至今的毫秒数（ESP: esp_timer_get_time；PC: steady_clock）
    uint64_t uptimeMs() ;
    
}