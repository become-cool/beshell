#pragma once

#include <EventEmitter.hpp>
#include <driver/gpio.h>
#include <esp_system.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>

namespace be::driver::motion {
    class TimerStepper: public be::EventEmitter {
        DECLARE_NCLASS_META
    private:
        static std::vector<JSCFunctionListEntry> methods ;
        // static std::vector<JSCFunctionListEntry> staticMethods ;

        gpio_num_t io_step = GPIO_NUM_NC ;
        gpio_num_t io_dir = GPIO_NUM_NC ;
        gpio_num_t io_limit_forward = GPIO_NUM_NC ;   // 限位开关
        gpio_num_t io_limit_backward = GPIO_NUM_NC ;   // 限位开关
        gpio_num_t io_en = GPIO_NUM_NC ;

        esp_timer_handle_t timer = nullptr;
        esp_timer_handle_t timer_accel = nullptr;  // 用于实现加速度的计时器

        bool is_running:1 ;
        bool is_stopping:1 ;
        bool level:1 ;          // io step 的脉冲电平
        bool run_dir:1 ;
        bool use_dest:1 ;
        bool use_accel:1 ;      // 使用加速度
        bool use_passing:1 ;    // "passing" 事件 (过点触发)
        

        uint16_t _freq ;             // 当前频率 (电机当前速度)
        uint16_t _freq_target ;      // 目标频率 (电机目标速度)
        // uint16_t _freq_hzms ;        // 频率的增减速度 (电机加速度), 每毫秒增加的赫兹数，例如 3hz/ms 表示从 0hz 到 3000hz 需要1秒
        uint16_t _accel ;            // 电机加速度(单位hz/s), 是 freq 相对时间的变化化率，例如 1000hz/s 表示从 0hz 到 1000hz 需要1秒
        
        int64_t _pos ;           // 当前位置
        int64_t _dest ;          // 目标位置 
        int64_t _passing ;    // 触发 "passing" 事件的位置（过点触发）
        int64_t _pos_stopping ;  // 从该位置开始减速

        uint64_t start_time ;
        uint64_t stop_time ;

        uint32_t run_delay_us ;  // 延迟开始(微秒)

        void emit_stop_event(bool sync) ;
        void emit_passing_event() ;
        static void stepper_timer_callback(TimerStepper *arg) ;
        
        bool calculate_step_time_freq(uint16_t v0, uint16_t a, float * time) ;
        int64_t calculate_accel(int64_t dest, float * out_time) ;

        JSValue run() ;

    public:
        TimerStepper(JSContext * ctx, JSValue _jsobj=JS_NULL) ;
        ~TimerStepper() ;
        static JSValue constructor(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        
        static JSValue setup(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;

        static JSValue run(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue runSteps(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue runTo(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue stop(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue setFreq(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue freq(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue setAccel(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue accel(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue isRunning(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue isStopping(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue pos(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue setPos(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue stepPin(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue dirPin(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue limitPin(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue passing(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue setPassing(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue clearPassing(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue calculateTravelTime(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue startTime(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue stopTime(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue setDelay(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue getDelay(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
    
    } ;
}