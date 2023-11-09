#include "driver_stepper_timer.h"
#include "utils.h"
#include "driver/gpio.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
#include "esp_timer.h"
#include <string.h>
#include <math.h>
#include "eventloop.h"


#define MAX_FREQ 3500                   // 最大频率
#define DEFUALT_ACCEL 2000             // 默认电机加速度
#define ACCEL_TIMER_US 1000


typedef struct {

    JSContext * ctx ;
    void * jsobj ;

    int8_t io_step ;
    int8_t io_dir ;
    int8_t io_limit ;
    int8_t io_en ;

    esp_timer_handle_t timer ;
    esp_timer_handle_t timer_accel ;  // 用于实现加速度的计时器

    bool is_running:1 ;
    bool is_stopping:1 ;
    bool level:1 ;          // io step 的脉冲电平
    bool run_dir:1 ;
    bool use_dest:1 ;
    bool use_accel:1 ;      // 使用加速度
    bool use_passing:1 ;    // "passing" 事件
    

    uint16_t freq ;             // 当前频率 (电机当前速度)
    uint16_t freq_target ;      // 目标频率 (电机目标速度)
    // uint16_t freq_hzms ;        // 频率的增减速度 (电机加速度), 每毫秒增加的赫兹数，例如 3hz/ms 表示从 0hz 到 3000hz 需要1秒
    uint16_t accel ;            // 电机加速度(单位hz/s), 是 freq 相对时间的变化化率，例如 1000hz/s 表示从 0hz 到 1000hz 需要1秒
    
    int64_t pos ;           // 当前位置
    int64_t dest ;          // 目标位置 
    int64_t passing ;    // 触发 "passing" 事件的位置
    int64_t pos_stopping ;  // 从该位置开始减速

    uint64_t start_time ;
    uint64_t stop_time ;

    uint32_t run_delay_us ;  // 延迟开始(微秒)

} driver_stepper_timer_t ;


// 接一元二次方程
bool solve_unary_quadratic(float a,float b,float c, float * x1, float * x2) {

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
bool calculate_step_time_freq(uint16_t v0, uint16_t a, float * time) {
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


#define THIS_STEPPER(var)                                                   \
    driver_stepper_timer_t * var = JS_GetOpaque(this_val, js_stepper_timer_class_id);   \
    if(!var) {                                                              \
        THROW_EXCEPTION("must be called as a driver.Stepper method")        \
    }


static void emit_stop_event(driver_stepper_timer_t * stepper, bool sync) {

    stepper->stop_time = gettime_us() ;

    JSValue eventName = JS_NewString(stepper->ctx, "stop") ;
    MAKE_ARGV3( argv, eventName, JS_NewInt64(stepper->ctx, stepper->pos), JS_NewUint32(stepper->ctx, stepper->run_dir) ) ;

    JSValue jsobj = JS_MKPTR(JS_TAG_OBJECT,stepper->jsobj) ;
    JSValue emit = JS_GetPropertyStr(stepper->ctx, jsobj, "emit") ;

    if(sync) {
        JS_Call(stepper->ctx, emit, jsobj, 3, argv) ;
        JS_FreeValue(stepper->ctx, eventName) ;
        free(argv) ;
    }
    else {
        eventloop_push_with_argv(stepper->ctx, emit, JS_DupValue(stepper->ctx, jsobj), 3, argv) ;
    }

    JS_FreeValue(stepper->ctx, emit) ;
}

static void emit_passing_event(driver_stepper_timer_t * stepper) {

    JSValue eventName = JS_NewString(stepper->ctx, "passing") ;
    MAKE_ARGV2( argv, eventName, JS_NewInt64(stepper->ctx, stepper->passing) ) ;

    JSValue jsobj = JS_MKPTR(JS_TAG_OBJECT,stepper->jsobj) ;
    JSValue emit = JS_GetPropertyStr(stepper->ctx, jsobj, "emit") ;

    eventloop_push_with_argv(stepper->ctx, emit, JS_DupValue(stepper->ctx, jsobj), 2, argv) ;

    JS_FreeValue(stepper->ctx, emit) ;
}

static void stepper_timer(void *arg) {
    driver_stepper_timer_t * stepper = (driver_stepper_timer_t*)arg ;
    if(!stepper || !stepper->is_running) {
        return ;
    }

    gpio_set_level(stepper->io_step, stepper->level) ;

    if(stepper->level) {

        stepper->pos+= stepper->run_dir? 1: -1 ;

        // passing 事件
        if(stepper->use_passing) {
            if(stepper->pos == stepper->passing) {
                emit_passing_event(stepper) ;
            }
        }

        // 处理加减速
        if( stepper->accel > 0 && stepper->use_accel && (stepper->freq!=stepper->freq_target || stepper->is_stopping) ) {


            float next_steps_sec = 0 ;
            if( !calculate_step_time_freq(stepper->freq, stepper->accel, &next_steps_sec) ) {
                // @todo 错误处理
                printf("can not solve step timesolve") ;
            }

            else {

                int16_t new_freq = 0 ;

                // 减速
                if(stepper->freq > stepper->freq_target || stepper->is_stopping) {
                    new_freq = (int16_t)stepper->freq - (int16_t)(next_steps_sec * stepper->accel + 0.5) ;
                    if( new_freq < 0 ) {
                        new_freq = 0 ;
                    }
                }
                // 加速
                else if(stepper->freq < stepper->freq_target) {
                    new_freq = (int16_t)stepper->freq + (int16_t)(next_steps_sec * stepper->accel + 0.5) ;
                    if( new_freq > stepper->freq_target ) {
                        new_freq = stepper->freq_target ;
                    }
                }

                stepper->freq = new_freq ;

                esp_timer_stop(stepper->timer) ;

                // 已经减速到 0
                if(stepper->freq==0) {
                    stepper->is_running = false ;
                    stepper->is_stopping = false ;
                    emit_stop_event(stepper, false) ;
                    
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
                stepper->is_running = stepper->pos < stepper->dest ;

                // 到达减速点 (需要满足accel>0 且 use_accel==true)
                if(stepper->accel > 0 && stepper->use_accel) {
                    stepper->is_stopping = stepper->pos >= stepper->pos_stopping ;
                }
            }
            // 反向运动
            else {
                stepper->is_running = stepper->pos > stepper->dest ;
                
                // 到达减速点 (需要满足accel>0)
                if(stepper->accel > 0 && stepper->use_accel) {
                    stepper->is_stopping = stepper->pos <= stepper->pos_stopping ;
                }
            }

            // 到达目的地
            if(!stepper->is_running) {
                esp_timer_stop(stepper->timer) ;
                emit_stop_event(stepper, false) ;
            }
        }
    }

    stepper->level = !stepper->level ;
}



////////////////////////////////////////////////

static JSClassID js_stepper_timer_class_id ;
static JSValue js_stepper_timer_constructor(JSContext *ctx, JSValueConst new_target, int argc, JSValueConst *argv){
    JSValue jsobj = JS_NewObjectClass(ctx, js_stepper_timer_class_id) ;

    driver_stepper_timer_t * HMALLOC(stepper, sizeof(driver_stepper_timer_t))
    if(!stepper) {
        THROW_EXCEPTION("out of memory?")
    }
    memset(stepper, 0, sizeof(driver_stepper_timer_t)) ;
    stepper->freq = 1000 ;
    
    CHECK_ARGC(1)

    // gpio
    ARGV_TO_INT8(0, io_step)
    ARGV_TO_INT8_OPT(1, io_dir, -1)
    ARGV_TO_INT8_OPT(2, io_limit, -1)
    ARGV_TO_INT8_OPT(3, io_en, -1)
    
    stepper->io_step = io_step ;
    stepper->io_dir = io_dir ;
    stepper->io_limit = io_limit ;
    stepper->io_en = io_en ;

    // stepup gpio
    gpio_config_t cfg = {
        .intr_type = GPIO_INTR_DISABLE,
        .mode = GPIO_MODE_OUTPUT,
        .pin_bit_mask = (1ULL<<io_step),
        .pull_down_en = false,
        .pull_up_en = false
    };
    CALL_IDF_API( gpio_config(&cfg) )

    if(io_dir>-1) {
        cfg.intr_type = GPIO_INTR_DISABLE ;
        cfg.mode = GPIO_MODE_OUTPUT ;
        cfg.pin_bit_mask = (1ULL<<io_dir) ;
        cfg.pull_down_en = false ;
        cfg.pull_up_en = false ;
        CALL_IDF_API( gpio_config(&cfg) )
    }

    const esp_timer_create_args_t timer_args = {
        .callback = &stepper_timer,
        .arg = stepper ,
        .name = "soft-stepper"
    };
    CALL_IDF_API( esp_timer_create(&timer_args, &(stepper->timer)) )

    stepper->accel = DEFUALT_ACCEL ;

    JS_SetOpaque(jsobj, stepper) ;
    JS_SetPropertyStr(ctx, jsobj, "_handlers", JS_NewObject(ctx));

    stepper->ctx = ctx ;
    stepper->jsobj = JS_VALUE_GET_PTR(jsobj) ;

    return jsobj ;
}
static void js_stepper_timer_finalizer(JSRuntime *rt, JSValue this_val){
    driver_stepper_timer_t * stepper = JS_GetOpaque(this_val, js_stepper_timer_class_id);
    if(!stepper) {
        printf("must be called as a driver.SoftStepper method\n") ;
    }

    if(stepper->is_running) {
        stepper->is_running = false ;
        emit_stop_event(stepper, true) ;
    }

    esp_timer_stop(stepper->timer) ;
    esp_timer_delete(stepper->timer) ;

    if(stepper) {
        free(stepper) ;
        stepper = NULL ;
        JS_SetOpaque(this_val, NULL) ;
    }

}
static JSClassDef js_stepper_timer_class = {
    "driver.Stepper",
    .finalizer = js_stepper_timer_finalizer,
} ;


static inline JSValue set_freq(JSContext *ctx, driver_stepper_timer_t * stepper, int freq) {
    if(freq<=0 || freq>MAX_FREQ) {
        THROW_EXCEPTION("arg freq must >0 && <%d", MAX_FREQ)
    }
    
    stepper->freq_target = freq ;

    return JS_UNDEFINED ;
}

// 计算加速阶段，返回从0加速到目标速度所需要的时间和步数
static inline int64_t calculate_accel(driver_stepper_timer_t * stepper, int64_t dest, float * out_time) {

    float accel_time = (float)stepper->freq_target / stepper->accel ;
    uint16_t accel_steps = (uint16_t)(stepper->accel * accel_time*accel_time/2 + 0.5) ;

    // 根据行程和加速度，无法到达目标速度
    if(accel_steps > dest/2) {
        accel_steps = dest/2 ;

        // 需要重新计算时间
        if(out_time) {
            // 根据距离，算时间
            solve_unary_quadratic( (float)stepper->accel/2, 0, (float)-accel_steps, out_time, NULL ) ;
            return accel_steps ;
        }
    } 
    
    // printf("accel = %d hz/s\n", stepper->accel) ;
    // printf("accel time: %d ms, steps: %d\n", (int)(accel_time*1000+0.5), accel_steps) ;
    // dn(stepper->pos_stopping)

    if(out_time) {
        *out_time = accel_time ;
    }

    return accel_steps ;
}

static JSValue js_stepper_timer_calculate_travel_time(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    CHECK_ARGC(1)
    THIS_STEPPER(stepper)
    ARGV_TO_INT64(0, steps)
    
    float accel_time = 0 ;
    int64_t accel_steps = calculate_accel(stepper, steps, &accel_time) ;

    float time = accel_time*2 + ((steps-2*accel_steps) * (1/(float)stepper->freq_target)) ;

    return JS_NewFloat64(ctx, time) ;
}

static JSValue run(JSContext *ctx, driver_stepper_timer_t * stepper) {

    if(stepper->io_dir>-1) {
        CALL_IDF_API( gpio_set_level(stepper->io_dir, stepper->run_dir) )
    }

    stepper->freq = 0 ;
    uint32_t period = stepper->run_delay_us ;

    // 使用加速度
    if(stepper->accel>0 && stepper->use_accel) {

        if(stepper->use_dest) {
            int64_t accel_steps = calculate_accel(stepper, abs(stepper->dest-stepper->pos), NULL) ;
            
            // 设置减速位置
            if(stepper->run_dir) {
                stepper->pos_stopping = stepper->dest - accel_steps ;
            }
            else {
                stepper->pos_stopping = stepper->dest + accel_steps ;
            }
        }
        else {
            stepper->pos_stopping = 0 ;
        }

        if(!period) {

            float first_steps_time = 0 ;

            if( !calculate_step_time_freq(0, stepper->accel, &first_steps_time)) {
                THROW_EXCEPTION("can not solve step time")
            }

            stepper->freq = (uint16_t)(first_steps_time * stepper->accel + 0.5) ;

            // printf("first step at %d ms, stepper->freq=%d\n", (int)(first_steps_time*1000+0.5), stepper->freq) ;

            period = 500000*first_steps_time+0.5 ;

        }
    }

    // 不使用加速度
    else {

        if(stepper->use_dest) {
            stepper->pos_stopping = stepper->dest ;
        }

        if(!period) {

            stepper->freq = stepper->freq_target ;

            period = 500000/stepper->freq ;
        }
    }

    // dn(stepper->use_accel)

    stepper->level = 0 ;
    CALL_IDF_API( esp_timer_start_periodic(stepper->timer, period) )

    stepper->is_running = true ;

    stepper->start_time = gettime_us() ;

    return JS_UNDEFINED ;
}

static inline void set_dest(driver_stepper_timer_t * stepper, int64_t dest) {

    stepper->use_dest = true ;
    stepper->dest = dest ;
    stepper->run_dir = dest > stepper->pos? 1: 0 ;    

}

static inline JSValue set_accel(JSContext *ctx, int argc, JSValueConst *argv, driver_stepper_timer_t * stepper, int arg_idx) {
    
    stepper->use_accel = true ;
    if(argc>arg_idx) {
        if(JS_IsFalse(ctx,argv[arg_idx])) {
            stepper->use_accel = false ;
        }
        else if( JS_IsNumber(argv[arg_idx]) ) {
            int16_t accel = 0 ;
            if( JS_ToInt32(ctx, &accel, argv[arg_idx])!=0 ) {
                THROW_EXCEPTION("invalid arg accel, musb be uint or false")
            }
            if(accel<0) {
                THROW_EXCEPTION("arg accel musb >= 0")
            }
            stepper->accel = accel ;
        }
        else {
            THROW_EXCEPTION("invalid arg accel, musb be uint or false")
        }
    }

    return JS_UNDEFINED ;
}

/**
 * @param forward = true
 * @param freq ?
 * @param accel ?  number|false 如果为 false，表示临时禁用 accel
 */
static JSValue js_stepper_timer_run(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {

    THIS_STEPPER(stepper)
    if(stepper->is_running) {
        THROW_EXCEPTION("stepper is running already")
    }
    stepper->run_dir = 1 ;
    if(argc>0) {
        stepper->run_dir = JS_ToBool(ctx, argv[0]) ;
    }
    if(argc>1) {
        ARGV_TO_INT32(1, freq)
        if( JS_IsException(set_freq(ctx, stepper, freq)) ) {
            return JS_EXCEPTION ;
        }
    }
    stepper->use_dest = false ;

    if( JS_IsException( set_accel(ctx, argc, argv, stepper, 2) ) ) {
        return JS_EXCEPTION ;
    }

    return run(ctx, stepper) ;
}


/**
 * @param dest
 * @param freq ?
 * @param accel ?  number|false 如果为 false，表示临时禁用 accel
 */
static JSValue js_stepper_timer_run_to(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {

    CHECK_ARGC(1)
    THIS_STEPPER(stepper)
    if(stepper->is_running) {
        THROW_EXCEPTION("stepper is running already")
    }
    ARGV_TO_INT64(0, dest) ;

    if(argc>1) {
        ARGV_TO_INT32(1, freq)
        if( JS_IsException(set_freq(ctx, stepper, freq)) ) {
            return JS_EXCEPTION ;
        }
    }
    
    set_dest(stepper, dest) ;

    if( JS_IsException( set_accel(ctx, argc, argv, stepper, 2) ) ) {
        return JS_EXCEPTION ;
    }

    return run(ctx, stepper) ;
}



/**
 * @param steps
 * @param freq ?
 * @param accel ?  number|false 如果为 false，表示临时禁用 accel
 */
static JSValue js_stepper_timer_run_steps(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {

    CHECK_ARGC(1)
    THIS_STEPPER(stepper)
        if(stepper->is_running) {
        THROW_EXCEPTION("stepper is running already")
    }
    ARGV_TO_INT64(0, steps) ;

    if(argc>1) {
        ARGV_TO_INT32(1, freq)
        if( JS_IsException(set_freq(ctx, stepper, freq)) ) {
            return JS_EXCEPTION ;
        }
    }

    set_dest(stepper, stepper->pos + steps) ;

    if( JS_IsException( set_accel(ctx, argc, argv, stepper, 2) ) ) {
        return JS_EXCEPTION ;
    }

    return run(ctx, stepper) ;
}

static JSValue js_stepper_timer_stop(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    THIS_STEPPER(stepper)

    bool force = false ;
    if(argc>0) {
        force = JS_ToBool(ctx, argv[0]) ;
    }

    if(stepper->is_running) {

        // force==true 或 无加速度，直接停止
        if( force || !stepper->use_accel || stepper->accel == 0 ) {
            stepper->is_running = false;
            stepper->is_stopping = false;
            stepper->freq = 0;

            esp_timer_stop(stepper->timer) ;
            emit_stop_event(stepper, true) ;
        }

        else {
            // 等待计时器减速停止
            stepper->is_stopping = true ;
        }
    }

    return JS_UNDEFINED ;
}


static JSValue js_stepper_timer_set_freq(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    CHECK_ARGC(1)
    THIS_STEPPER(stepper)
    
    ARGV_TO_INT32(0, freq) ;
    return set_freq(ctx, stepper, freq) ;
}


/**
 * @param runningFreq = false
 */
static JSValue js_stepper_timer_get_freq(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    THIS_STEPPER(stepper)

    if(argc>0) {
        if(JS_ToBool(ctx, argv[0])) {
            return JS_NewUint32(ctx, stepper->freq) ;
        }
    }

    return JS_NewUint32(ctx, stepper->freq_target) ;
}


static JSValue js_stepper_timer_get_accel(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    THIS_STEPPER(stepper)
    return JS_NewUint32(ctx, stepper->accel) ;
}
static JSValue js_stepper_timer_set_accel(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    CHECK_ARGC(1)
    THIS_STEPPER(stepper)
    
    ARGV_TO_INT32(0, accel) ;
    if(accel<0) {
        THROW_EXCEPTION("freq accel must >0")
    }

    stepper->accel = accel ;

    return JS_UNDEFINED ;
}

static JSValue js_stepper_timer_is_running(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    THIS_STEPPER(stepper)
    return JS_NewBool(ctx, stepper->is_running) ;
}
static JSValue js_stepper_timer_is_stopping(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    THIS_STEPPER(stepper)
    return JS_NewBool(ctx, stepper->is_stopping) ;
}

static JSValue js_stepper_timer_set_pos(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    CHECK_ARGC(1)
    THIS_STEPPER(stepper)
    ARGV_TO_INT64(0, pos) ;
    stepper->pos = pos ;
    return JS_UNDEFINED ;
}
static JSValue js_stepper_timer_pos(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    THIS_STEPPER(stepper)
    return JS_NewInt64(ctx, stepper->pos) ;
}

static JSValue js_stepper_timer_get_step_pin(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    THIS_STEPPER(stepper)
    return JS_NewInt32(ctx, stepper->io_step) ;
}
static JSValue js_stepper_timer_get_dir_pin(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    THIS_STEPPER(stepper)
    return JS_NewInt32(ctx, stepper->io_dir) ;
}
static JSValue js_stepper_timer_get_limit_pin(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    THIS_STEPPER(stepper)
    return JS_NewInt32(ctx, stepper->io_limit) ;
}

static JSValue js_stepper_timer_set_delay_us(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    CHECK_ARGC(1)
    THIS_STEPPER(stepper)
    ARGV_TO_UINT32(0, delay) ;
    stepper->run_delay_us = delay ;
    return JS_UNDEFINED ;
}

static JSValue js_stepper_timer_get_delay_us(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    THIS_STEPPER(stepper)
    return JS_NewInt32(ctx, stepper->run_delay_us) ;
}


static JSValue js_stepper_timer_get_passing(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    THIS_STEPPER(stepper)
    return JS_NewInt64(ctx, stepper->passing) ;
}

static JSValue js_stepper_timer_set_passing(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    CHECK_ARGC(1)
    THIS_STEPPER(stepper)
    ARGV_TO_INT64(0, passing) ;
    stepper->passing = passing ;
    stepper->use_passing = true ;
    return JS_UNDEFINED ;
}


static JSValue js_stepper_timer_clear_passing(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    CHECK_ARGC(0)
    THIS_STEPPER(stepper)
    stepper->use_passing = false ;
    return JS_UNDEFINED ;
}

static JSValue js_stepper_timer_get_start_time(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    THIS_STEPPER(stepper)
    return JS_NewInt64(ctx, stepper->start_time) ;
}
static JSValue js_stepper_timer_get_stop_time(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    THIS_STEPPER(stepper)
    return JS_NewInt64(ctx, stepper->stop_time) ;
}

static const JSCFunctionListEntry js_stepper_timer_proto_funcs[] = {
    JS_CFUNC_DEF("run", 0, js_stepper_timer_run),
    JS_CFUNC_DEF("runSteps", 0, js_stepper_timer_run_steps),
    JS_CFUNC_DEF("runTo", 0, js_stepper_timer_run_to),
    JS_CFUNC_DEF("stop", 0, js_stepper_timer_stop),
    JS_CFUNC_DEF("setFreq", 0, js_stepper_timer_set_freq),
    JS_CFUNC_DEF("freq", 0, js_stepper_timer_get_freq),
    JS_CFUNC_DEF("setAccel", 0, js_stepper_timer_set_accel),
    JS_CFUNC_DEF("accel", 0, js_stepper_timer_get_accel), 
    JS_CFUNC_DEF("isRunning", 0, js_stepper_timer_is_running),
    JS_CFUNC_DEF("isStopping", 0, js_stepper_timer_is_stopping),
    JS_CFUNC_DEF("pos", 0, js_stepper_timer_pos),
    JS_CFUNC_DEF("setPos", 0, js_stepper_timer_set_pos),
    JS_CFUNC_DEF("stepPin", 0, js_stepper_timer_get_step_pin),
    JS_CFUNC_DEF("dirPin", 0, js_stepper_timer_get_dir_pin),
    JS_CFUNC_DEF("limitPin", 0, js_stepper_timer_get_limit_pin),
    JS_CFUNC_DEF("passing", 0, js_stepper_timer_get_passing),
    JS_CFUNC_DEF("setPassing", 0, js_stepper_timer_set_passing),
    JS_CFUNC_DEF("clearPassing", 0, js_stepper_timer_clear_passing),
    JS_CFUNC_DEF("calculateTravelTime", 0, js_stepper_timer_calculate_travel_time),
    JS_CFUNC_DEF("startTime", 0, js_stepper_timer_get_start_time),
    JS_CFUNC_DEF("stopTime", 0, js_stepper_timer_get_stop_time),
    JS_CFUNC_DEF("setDelay", 0, js_stepper_timer_set_delay_us),
    JS_CFUNC_DEF("getDelay", 0, js_stepper_timer_get_delay_us),
} ;


void be_module_driver_stepper_timer_init() {
    JS_NewClassID(&js_stepper_timer_class_id);
}

void be_module_driver_stepper_timer_require(JSContext *ctx,JSValue driver) {

    JSValue EventEmitterProto = js_get_glob_prop(ctx, 3, "beapi", "EventEmitter", "prototype") ;
    QJS_DEF_CLASS(stepper_timer, "SoftStepper", "driver.SoftStepper", EventEmitterProto, driver)
    JS_FreeValue(ctx, EventEmitterProto);

    // JS_SetPropertyStr(ctx, driver_stepper_timer, "xxxOoo", JS_NewCFunction(ctx, js_xxx_ooo, "xxxOoo", 1));


}

void be_module_driver_stepper_timer_loop(JSContext *ctx) {
}

void be_module_driver_stepper_timer_reset(JSContext *ctx) {
}

