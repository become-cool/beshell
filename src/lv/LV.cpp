#include "lv.hpp"
using namespace std;


namespace be {
namespace lv {

    LV::LV() {

    }

    static void lv_tick_inc_cb(void *data) {
        uint32_t tick_inc_period_ms = *((uint32_t *) data);
        lv_tick_inc(tick_inc_period_ms);
    }
    
    void LV::initTick() {
        static const uint32_t tick_inc_period_ms = 2;
        const esp_timer_create_args_t periodic_timer_args = {
                .callback = lv_tick_inc_cb,
                .arg = (void*)&tick_inc_period_ms,
                .dispatch_method = ESP_TIMER_TASK,            
                .skip_unhandled_events = true,
        };
        ESP_ERROR_CHECK(esp_timer_create(&periodic_timer_args, &tickTimer));
    }

    void LV::setup() {
        initTick() ;

        lv_init() ;
    }
    void LV::loop() {
        lv_task_handler() ;
    }

}}