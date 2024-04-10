#include "TimerStepper.hpp"
#include <cmath>

#define MAX_FREQ 3500                   // 最大频率
#define DEFUALT_ACCEL 2000             // 默认电机加速度
#define ACCEL_TIMER_US 1000

using namespace std ;

namespace be::driver::motion {
    DEFINE_NCLASS_META(TimerStepper, EventEmitter)
    std::vector<JSCFunctionListEntry> TimerStepper::methods = {
        JS_CFUNC_DEF("setup", 0, TimerStepper::setup),
        JS_CFUNC_DEF("run", 0, TimerStepper::run),
        JS_CFUNC_DEF("runSteps", 0, TimerStepper::runSteps),
        JS_CFUNC_DEF("runTo", 0, TimerStepper::runTo),
        JS_CFUNC_DEF("stop", 0, TimerStepper::stop),
        JS_CFUNC_DEF("setFreq", 0, TimerStepper::setFreq),
        JS_CFUNC_DEF("freq", 0, TimerStepper::freq),
        JS_CFUNC_DEF("setAccel", 0, TimerStepper::setAccel),
        JS_CFUNC_DEF("accel", 0, TimerStepper::accel),
        JS_CFUNC_DEF("isRunning", 0, TimerStepper::isRunning),
        JS_CFUNC_DEF("isStopping", 0, TimerStepper::isStopping),
        JS_CFUNC_DEF("pos", 0, TimerStepper::pos),
        JS_CFUNC_DEF("setPos", 0, TimerStepper::setPos),
        JS_CFUNC_DEF("stepPin", 0, TimerStepper::stepPin),
        JS_CFUNC_DEF("dirPin", 0, TimerStepper::dirPin),
        JS_CFUNC_DEF("limitPin", 0, TimerStepper::limitPin),
        JS_CFUNC_DEF("passing", 0, TimerStepper::passing),
        JS_CFUNC_DEF("setPassing", 0, TimerStepper::setPassing),
        JS_CFUNC_DEF("clearPassing", 0, TimerStepper::clearPassing),
        JS_CFUNC_DEF("calculateTravelTime", 0, TimerStepper::calculateTravelTime),
        JS_CFUNC_DEF("startTime", 0, TimerStepper::startTime),
        JS_CFUNC_DEF("stopTime", 0, TimerStepper::stopTime),
        JS_CFUNC_DEF("setDelay", 0, TimerStepper::setDelay),
        JS_CFUNC_DEF("getDelay", 0, TimerStepper::getDelay),
    } ;

    TimerStepper::TimerStepper(JSContext * ctx, JSValue jsobj)
        : EventEmitter(ctx,build(ctx,jsobj))
    {
    }

    TimerStepper::~TimerStepper() {
        if(is_running) {
            is_running = false ;
            emit_stop_event(true) ;
        }

        if(timer) {
            esp_timer_stop(timer) ;
            esp_timer_delete(timer) ;
        }
    }

    // 解一元二次方程
    static bool solve_unary_quadratic(float a,float b,float c, float * x1, float * x2) {
        float delat = b*b-4*a*c ;
        // printf("a=%f,b=%f,c=%f; d=%f\n",a,b,c,delat) ;
        if(delat>0) {

            *x1 = ( -b+sqrt(delat) )/(2*a) ;
            if(x2) {
                *x2 = ( -b-sqrt(delat) )/(2*a) ;
            }
            

            // printf("sqrt(delat)=%f, 2*a=%f\n", sqrt(delat), 2*a) ;
            // printf("x1=%f\n", *x1) ;

            return true ;
        }
        else if(delat==0) {
            *x1 = ( -b+sqrt(delat) )/(2*a) ;
            if(x2) {
                *x2 = *x1 ;
            }
            return true ;
        }
        else {
            return false ;
        }
    }

    /**
     * 计算下一步所需的时间
     * 
     * @param v0 
     * @param a 
     */
    bool TimerStepper::calculate_step_time_freq(uint16_t v0, uint16_t a, float * time) {
        float t1, t2 ;
        if(solve_unary_quadratic( (float)a/2, (float)v0, (float)-1, &t1, &t2 )) {
            if(t1>0){
                *time = t1 ;
            } 
            else if(t2>0) {
                *time = t2 ;
            }
            else {
                return false ;
            }
            return true ;
        }
        else {
            return false ;
        }
    }

    void TimerStepper::stepper_timer_callback(TimerStepper * stepper) {
        if(!stepper || !stepper->is_running) {
            return ;
        }

        gpio_set_level(stepper->io_step, stepper->level) ;

        if(stepper->level) {

            stepper->_pos+= stepper->run_dir? 1: -1 ;

            // passing 事件
            if(stepper->use_passing) {
                if(stepper->_pos == stepper->_passing) {
                    stepper->emit_passing_event() ;
                }
            }

            // 处理加减速
            if( stepper->_accel > 0 && stepper->use_accel && (stepper->_freq!=stepper->_freq_target || stepper->is_stopping) ) {
                float next_steps_sec = 0 ;
                if( !stepper->calculate_step_time_freq(stepper->_freq, stepper->_accel, &next_steps_sec) ) {
                    // @todo 错误处理
                    printf("can not solve step timesolve") ;
                }
                else {
                    int16_t new_freq = 0 ;
                    // 减速
                    if(stepper->_freq > stepper->_freq_target || stepper->is_stopping) {
                        new_freq = (int16_t)stepper->_freq - (int16_t)(next_steps_sec * stepper->_accel + 0.5) ;
                        if( new_freq < 0 ) {
                            new_freq = 0 ;
                        }
                    }
                    // 加速
                    else if(stepper->_freq < stepper->_freq_target) {
                        new_freq = (int16_t)stepper->_freq + (int16_t)(next_steps_sec * stepper->_accel + 0.5) ;
                        if( new_freq > stepper->_freq_target ) {
                            new_freq = stepper->_freq_target ;
                        }
                    }
                    stepper->_freq = new_freq ;
                    esp_timer_stop(stepper->timer) ;

                    // 已经减速到 0
                    if(stepper->_freq==0) {
                        stepper->is_running = false ;
                        stepper->is_stopping = false ;
                        stepper->emit_stop_event(false) ;
                        
                        return ;
                    }
                    else {
                        esp_timer_start_periodic(stepper->timer, (next_steps_sec*500000+0.5)) ;
                    }
                }
            }

            // 目的地模式
            if(stepper->use_dest) {

                // 正向运动
                if(stepper->run_dir) {
                    stepper->is_running = stepper->_pos < stepper->_dest ;

                    // 到达减速点 (需要满足accel>0 且 use_accel==true)
                    if(stepper->_accel > 0 && stepper->use_accel) {
                        stepper->is_stopping = stepper->_pos >= stepper->_pos_stopping ;
                    }
                }
                // 反向运动
                else {
                    stepper->is_running = stepper->_pos > stepper->_dest ;
                    
                    // 到达减速点 (需要满足accel>0)
                    if(stepper->_accel > 0 && stepper->use_accel) {
                        stepper->is_stopping = stepper->_pos <= stepper->_pos_stopping ;
                    }
                }

                // 到达目的地
                if(!stepper->is_running) {
                    esp_timer_stop(stepper->timer) ;
                    stepper->emit_stop_event(false) ;
                }
            }
        }

        stepper->level = !stepper->level ;
    }

    

    #define SET_FREQ(freq)                              \
        if(freq<=0 || freq>MAX_FREQ) {                  \
            JSTHROW("arg freq must >0 && <%d", MAX_FREQ)\
        }                                               \
        stepper->_freq_target = freq ;

    #define SET_DEST(dest)                              \
        stepper->use_dest = true ;                      \
        stepper->_dest = dest ;                         \
        stepper->run_dir = dest > stepper->_pos? 1: 0 ;

    #define SET_ACCEL(arg_idx)                          \
        stepper->use_accel = true ;                     \
        if(argc>arg_idx) {                              \
            if(JS_IsFalse(ctx,argv[arg_idx])) {         \
                stepper->use_accel = false ;            \
            }                                           \
            else if( JS_IsNumber(argv[arg_idx]) ) {     \
                int16_t accel = 0 ;                     \
                if( JS_ToInt32(ctx, (int32_t*)&accel, argv[arg_idx])!=0 ) {     \
                    JSTHROW("invalid arg accel, musb be uint or false")         \
                }                                       \
                if(accel<0) {                           \
                    JSTHROW("arg accel musb >= 0")      \
                }                                       \
                stepper->_accel = accel ;                \
            }                                           \
            else {                                      \
                JSTHROW("invalid arg accel, musb be uint or false") \
            }                                           \
        }
        
    void TimerStepper::emit_stop_event(bool sync) {
        // stop_time = gettime_us() ;
        // JSValue eventName = JS_NewString(ctx, "stop") ;
        // MAKE_ARGV3( argv, eventName, JS_NewInt64(ctx, (int64_t)pos), JS_NewUint32(ctx, run_dir) ) ;
        // JSValue emit = JS_GetPropertyStr(ctx, jsobj, "emit") ;
        // if(sync) {
        //     JS_Call(ctx, emit, jsobj, 3, argv) ;
        //     JS_FreeValue(ctx, eventName) ;
        //     free(argv) ;
        // }
        // else {
        //     // eventloop_push_with_argv(ctx, emit, JS_DupValue(ctx, jsobj), 3, argv) ;
        // }

        // JS_FreeValue(ctx, emit) ;
    }

    void TimerStepper::emit_passing_event() {

        // JSValue eventName = JS_NewString(ctx, "passing") ;
        // MAKE_ARGV2( argv, eventName, JS_NewInt64(ctx, (int64_t)_passing) ) ;

        // JSValue emit = JS_GetPropertyStr(ctx, jsobj, "emit") ;
        // eventloop_push_with_argv(ctx, emit, JS_DupValue(ctx, jsobj), 2, argv) ;

        // JS_FreeValue(ctx, argv) ;
    }

    JSValue TimerStepper::constructor(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        auto obj = new TimerStepper(ctx) ;
        obj->shared() ;
        return obj->jsobj ;
    }

    
    /**
     * {
     *   pin: {
     *     step
     *     dir
     *     en
     *     limit_forward
     *     limit_backward
     *   }
     * } 
     */
    JSValue TimerStepper::setup(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        THIS_NCLASS(TimerStepper, stepper)
        CHECK_ARGC(1)

        JSValue optPin = JS_GetPropertyStr(ctx, argv[0], "pin") ;
        if( !JS_IsObject(optPin) ) {
            JSTHROW("property 'pin' must be an object") ;
        }

        GET_INT32_PROP(optPin, "step", stepper->io_step, )
        GET_INT32_PROP_OPT(optPin, "dir", stepper->io_dir, GPIO_NUM_NC)
        GET_INT32_PROP_OPT(optPin, "en", stepper->io_en, GPIO_NUM_NC)
        GET_INT32_PROP_OPT(optPin, "limit_forward", stepper->io_limit_forward, GPIO_NUM_NC)
        GET_INT32_PROP_OPT(optPin, "limit_backward", stepper->io_limit_backward, GPIO_NUM_NC)

        // stepup gpio
        gpio_config_t cfg = {
            .pin_bit_mask = (1ULL<<stepper->io_step),
            .mode = GPIO_MODE_OUTPUT,
            .pull_up_en = GPIO_PULLUP_DISABLE,
            .pull_down_en = GPIO_PULLDOWN_DISABLE,
            .intr_type = GPIO_INTR_DISABLE,
        };
        CALL_IDF_API( gpio_config(&cfg) )

        if(stepper->io_dir>-1) {
            cfg.pin_bit_mask = (1ULL<<stepper->io_dir) ;
            cfg.mode = GPIO_MODE_OUTPUT ;
            cfg.pull_up_en = GPIO_PULLUP_DISABLE ;
            cfg.pull_down_en = GPIO_PULLDOWN_DISABLE ;
            CALL_IDF_API( gpio_config(&cfg) ) ;
            cfg.intr_type = GPIO_INTR_DISABLE ;
        }

        const esp_timer_create_args_t timer_args = {
            .callback = (esp_timer_cb_t) &stepper_timer_callback,
            .arg = stepper ,
            .name = "timer-stepper"
        };
        CALL_IDF_API( esp_timer_create(&timer_args, &(stepper->timer)) )

        return JS_UNDEFINED ;
    }


    // 计算加速阶段，返回从0加速到目标速度所需要的时间和步数
    int64_t TimerStepper::calculate_accel(int64_t dest, float * out_time) {

        float accel_time = (float)_freq_target / _accel ;
        uint16_t accel_steps = (uint16_t)(_accel * accel_time*accel_time/2 + 0.5) ;

        // 根据行程和加速度，无法到达目标速度
        if(accel_steps > _dest/2) {
            accel_steps = _dest/2 ;

            // 需要重新计算时间
            if(out_time) {
                // 根据距离，算时间
                solve_unary_quadratic( (float)_accel/2, 0, (float)-accel_steps, out_time, NULL ) ;
                return accel_steps ;
            }
        } 
        
        // printf("accel = %d hz/s\n", accel) ;
        // printf("accel time: %d ms, steps: %d\n", (int)(accel_time*1000+0.5), accel_steps) ;
        // dn(pos_stopping)

        if(out_time) {
            *out_time = accel_time ;
        }

        return accel_steps ;
    }
    
    JSValue TimerStepper::run() {

        if(io_dir>-1) {
            CALL_IDF_API( gpio_set_level(io_dir, run_dir) )
        }

        _freq = 0 ;
        uint32_t period = run_delay_us ;

        // 使用加速度
        if(_accel>0 && use_accel) {

            if(use_dest) {
                int64_t accel_steps = calculate_accel(abs(_dest-_pos), NULL) ;
                
                // 设置减速位置
                if(run_dir) {
                    _pos_stopping = _dest - accel_steps ;
                }
                else {
                    _pos_stopping = _dest + accel_steps ;
                }
            }
            else {
                _pos_stopping = 0 ;
            }

            if(!period) {

                float first_steps_time = 0 ;

                if( !calculate_step_time_freq(0, _accel, &first_steps_time)) {
                    JSTHROW("can not solve step time")
                }

                _freq = (uint16_t)(first_steps_time * _accel + 0.5) ;

                // printf("first step at %d ms, freq=%d\n", (int)(first_steps_time*1000+0.5), _freq) ;

                period = 500000*first_steps_time+0.5 ;

            }
        }

        // 不使用加速度
        else {

            if(use_dest) {
                _pos_stopping = _dest ;
            }

            if(!period) {

                _freq = _freq_target ;

                period = 500000/_freq ;
            }
        }

        // dn(use_accel)

        level = 0 ;
        CALL_IDF_API( esp_timer_start_periodic(timer, period) )

        is_running = true ;

        start_time = gettime_us() ;

        return JS_UNDEFINED ;
    }
    JSValue TimerStepper::run(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        THIS_NCLASS(TimerStepper, stepper)
        if(stepper->is_running) {
            JSTHROW("stepper is running already")
        }
        stepper->run_dir = 1 ;
        if(argc>0) {
            stepper->run_dir = JS_ToBool(ctx, argv[0]) ;
        }
        if(argc>1) {
            ARGV_TO_INT32(1, freq)
            SET_FREQ(freq)
        }
        stepper->use_dest = false ;

        SET_ACCEL(2)

        return stepper->run() ;

        
        return JS_UNDEFINED ;
    }
    JSValue TimerStepper::runTo(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        THIS_NCLASS(TimerStepper, stepper)
        CHECK_ARGC(1)
        if(stepper->is_running) {
            JSTHROW("stepper is running already")
        }
        ARGV_TO_INT64(0, dest) ;
        if(dest==stepper->_pos) {
            return JS_UNDEFINED ;
        }

        if(argc>1) {
            ARGV_TO_INT32(1, freq)
            SET_FREQ(freq)
        }

        SET_DEST(dest)
        SET_ACCEL(2)

        return stepper->run() ;

    }
    JSValue TimerStepper::runSteps(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        THIS_NCLASS(TimerStepper, stepper)
        CHECK_ARGC(1)
        if(stepper->is_running) {
            JSTHROW("stepper is running already")
        }
        ARGV_TO_INT64(0, steps) ;

        if(argc>1) {
            ARGV_TO_INT32(1, freq)
            SET_FREQ(freq)
        }
        SET_DEST(stepper->_pos + steps)
        SET_ACCEL(2)
        return stepper->run() ;
    }
    JSValue TimerStepper::stop(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        THIS_NCLASS(TimerStepper, stepper)

        bool force = false ;
        if(argc>0) {
            force = JS_ToBool(ctx, argv[0]) ;
        }
        if(stepper->is_running) {
            // force==true 或 无加速度，直接停止
            if( force || !stepper->use_accel || stepper->_accel == 0 ) {
                dd
                stepper->is_running = false;
                stepper->is_stopping = false;
                stepper->_freq = 0;

                esp_timer_stop(stepper->timer) ;
                stepper->emit_stop_event(true) ;
            }

            else {
                // 等待计时器减速停止
                stepper->is_stopping = true ;
            }
        }
        
        return JS_UNDEFINED ;
    }

    JSValue TimerStepper::setFreq(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        CHECK_ARGC(1)
        THIS_NCLASS(TimerStepper, stepper)
        ARGV_TO_INT32(0, freq) ;
        SET_FREQ(freq) ;
        return JS_UNDEFINED ;
    }
    JSValue TimerStepper::freq(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        THIS_NCLASS(TimerStepper, stepper)
        return JS_NewUint32(ctx, stepper->_freq) ;
    }
    JSValue TimerStepper::setAccel(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        THIS_NCLASS(TimerStepper, stepper)
        CHECK_ARGC(1)
    
        ARGV_TO_INT32(0, accel) ;
        if(accel<0) {
            JSTHROW("freq accel must >0")
        }
        stepper->_accel = accel ;
        return JS_UNDEFINED ;
    }
    JSValue TimerStepper::accel(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        THIS_NCLASS(TimerStepper, stepper)
        return JS_NewUint32(ctx, stepper->_accel) ;
    }
    JSValue TimerStepper::isRunning(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        THIS_NCLASS(TimerStepper, stepper)
        return JS_NewBool(ctx, stepper->is_running) ;
    }
    JSValue TimerStepper::isStopping(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        THIS_NCLASS(TimerStepper, stepper)
        return JS_NewBool(ctx, stepper->is_stopping) ;
    }
    JSValue TimerStepper::pos(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        THIS_NCLASS(TimerStepper, stepper)
        return JS_NewInt64(ctx, stepper->_pos) ;
    }
    JSValue TimerStepper::setPos(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        THIS_NCLASS(TimerStepper, stepper)
        CHECK_ARGC(1)
        ARGV_TO_INT64(0, pos) ;
        stepper->_pos = pos ;
        return JS_UNDEFINED ;
    }
    JSValue TimerStepper::stepPin(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        THIS_NCLASS(TimerStepper, stepper)
        return JS_NewInt32(ctx, stepper->io_step) ;
    }
    JSValue TimerStepper::dirPin(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        THIS_NCLASS(TimerStepper, stepper)
        return JS_NewInt32(ctx, stepper->io_dir) ;
    }
    JSValue TimerStepper::limitPin(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        THIS_NCLASS(TimerStepper, stepper)
        return JS_NewInt32(ctx, stepper->io_limit_forward) ;
    }
    JSValue TimerStepper::passing(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        THIS_NCLASS(TimerStepper, stepper)
        return JS_NewInt64(ctx, stepper->_passing) ;
    }
    JSValue TimerStepper::setPassing(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        THIS_NCLASS(TimerStepper, stepper)
        CHECK_ARGC(1)
        ARGV_TO_INT64(0, passing) ;
        stepper->_passing = passing ;
        stepper->use_passing = true ;
        return JS_UNDEFINED ;
    }
    JSValue TimerStepper::clearPassing(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        THIS_NCLASS(TimerStepper, stepper)
        stepper->use_passing = false ;
        return JS_UNDEFINED ;
    }
    JSValue TimerStepper::calculateTravelTime(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        THIS_NCLASS(TimerStepper, stepper)
        CHECK_ARGC(1)
        ARGV_TO_INT64(0, steps)
        
        float accel_time = 0 ;
        int64_t accel_steps = stepper->calculate_accel(steps, &accel_time) ;

        float time = accel_time*2 + ((steps-2*accel_steps) * (1/(float)stepper->_freq_target)) ;

        return JS_NewFloat64(ctx, time) ;
    }
    JSValue TimerStepper::startTime(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        THIS_NCLASS(TimerStepper, stepper)
        return JS_NewInt64(ctx, stepper->start_time) ;
    }
    JSValue TimerStepper::stopTime(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        THIS_NCLASS(TimerStepper, stepper)
        return JS_NewInt64(ctx, stepper->stop_time) ;
    }
    JSValue TimerStepper::setDelay(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        THIS_NCLASS(TimerStepper, stepper)
        ARGV_TO_UINT32(0, delay) ;
        stepper->run_delay_us = delay ;
        return JS_UNDEFINED ;
    }
    JSValue TimerStepper::getDelay(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        THIS_NCLASS(TimerStepper, stepper)
        return JS_NewInt32(ctx, stepper->run_delay_us) ;
    }
}