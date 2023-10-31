#include "module_gpio.h"
#include "utils.h"
#include "js_main_loop.h"
#include "eventloop.h"
// #include "arduino/esp32-hal-gpio.h"
#include "string.h"
#include "math.h"
#include "driver/gpio.h"
#include "driver/ledc.h"
#include "hal/ledc_types.h"

// #include "driver/dac.h"
#include <esp_adc_cal.h>


#define PIN_CNT 48

/**
 * bit1:   未触发的 isr
 * bit2:   前次电平值
 * bit3:   监听下降沿
 * bit4:   监听上升沿
 */
static uint32_t gpio_state[PIN_CNT] = {
    0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,
} ;


/**
 * 设置 gpio 的工作模式
 *  
 * 参数 `mode` 的可选值为:
 * * input 输入
 * * output 输出
 * * output-od 开漏输出
 * * input-output 输入/输出 (分时复用)
 * * input-output-od 输入/开漏输出 (分时复用)
 * 
 * @beapi beapi.gpio.pinMode
 * @param pin:number mcu可用的gpio编号
 * @param mode:string
 * @return undefined
 */
JSValue js_gpio_pin_mode(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    
    CHECK_ARGC(2)
    ARGV_TO_UINT8(0, pin)
    char * mode = JS_ToCString(ctx, argv[1]) ;

	gpio_config_t conf = {
		    .pin_bit_mask = (1ULL<<pin),			/*!< GPIO pin: set with bit mask, each bit maps to a GPIO */
		    .mode = GPIO_MODE_DISABLE,              /*!< GPIO mode: set input/output mode                     */
		    .pull_up_en = GPIO_PULLUP_DISABLE,      /*!< GPIO pull-up                                         */
		    .pull_down_en = GPIO_PULLDOWN_DISABLE,  /*!< GPIO pull-down                                       */
		    .intr_type = GPIO_INTR_DISABLE      	/*!< GPIO interrupt type                                  */
	};

    // esp_err_t err ;
    if(strcmp(mode,"input")==0) {
        // err = gpio_set_direction(pin, GPIO_MODE_INPUT) ;
        conf.mode = GPIO_MODE_INPUT ;
    }
    else if(strcmp(mode,"output")==0) {
        // err = gpio_set_direction(pin, GPIO_MODE_OUTPUT) ;
        conf.mode = GPIO_MODE_OUTPUT ;
    }
    else if(strcmp(mode,"output-od")==0) {
        // err = gpio_set_direction(pin, GPIO_MODE_OUTPUT_OD) ;
        conf.mode = GPIO_MODE_OUTPUT_OD ;
    }
    else if(strcmp(mode,"input-output")==0) {
        // err = gpio_set_direction(pin, GPIO_MODE_INPUT_OUTPUT) ;
        conf.mode = GPIO_MODE_INPUT_OUTPUT ;
    }
    else if(strcmp(mode,"input-output-od")==0) {
        // err = gpio_set_direction(pin, GPIO_MODE_INPUT_OUTPUT_OD) ;
        conf.mode = GPIO_MODE_INPUT_OUTPUT_OD ;
    }
    else {
        THROW_EXCEPTION("unknow pin mode(input, output, output-od, input-output, input-output-od)")
    }
    
	if(gpio_config(&conf) != ESP_OK) {
        THROW_EXCEPTION("set pin mode failed, arg invalid?")
    }

    JS_FreeCString(ctx, mode) ;
    return JS_UNDEFINED ;
}


/**
 * 设置 gpio pull 模式
 *  
 * 参数 `pullMode` 的可选值为:
 * * up 上拉
 * * down 下拉
 * * updown 同时上下拉
 * * floating 悬空
 * 
 * @beapi beapi.gpio.pinPull
 * @param pin:number mcu可用的gpio编号
 * @param pullMode:string
 * @return undefined
 */
JSValue js_gpio_pin_pull(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    
    CHECK_ARGC(2)
    ARGV_TO_UINT8(0, pin)
    char * mode = JS_ToCString(ctx, argv[1]) ;

    if(strcmp(mode,"up")==0) {
        gpio_set_pull_mode(pin, GPIO_PULLUP_ONLY) ;
        gpio_pullup_en(pin) ;
    }
    else if(strcmp(mode,"down")==0) {
        gpio_set_pull_mode(pin, GPIO_PULLDOWN_ONLY) ;
        gpio_pulldown_en(pin) ;
    }
    else if(strcmp(mode,"updown")==0) {
        gpio_set_pull_mode(pin, GPIO_PULLUP_PULLDOWN) ;
        gpio_pulldown_en(pin) ;
        gpio_pullup_en(pin) ;
    }
    else if(strcmp(mode,"floating")==0) {
        gpio_set_pull_mode(pin, GPIO_FLOATING) ;
        gpio_pullup_dis(pin) ;
        gpio_pulldown_dis(pin) ;
    }
    else {
        THROW_EXCEPTION("unknow pin pull mode(up|down|updown|floating)")
    }

    JS_FreeCString(ctx, mode) ;
    return JS_UNDEFINED ;
}

/**
 * 以数字方式读取 gpio
 * 
 * @beapi beapi.gpio.digitalRead
 * @param pin:number mcu可用的gpio编号
 * @return 0|1
 */
JSValue js_gpio_digital_read(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    CHECK_ARGC(1)
    ARGV_TO_UINT8(0, pin)
    uint8_t value = gpio_get_level(pin) ;
    return JS_NewInt32(ctx, value) ;
}
/**
 * 在 gpio 上以数字方式输出 
 * 
 * @beapi beapi.gpio.digitalWrite
 * @param pin:number mcu可用的gpio编号
 * @param value:0|1
 * @return write
 */
JSValue js_gpio_digital_write(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    CHECK_ARGC(2)
    ARGV_TO_UINT8(0, pin)
    ARGV_TO_UINT8(1, value)
    
    esp_err_t res = gpio_set_level(pin, value==0? 0: 1) ;
    if(res != ESP_OK) {
        THROW_EXCEPTION("gpio_set_level() failed with code: %d", res)
    }
    // digitalWrite(pin, value) ;

    return JS_UNDEFINED ;
}



/**
 * 设置 gpoi 的 adc 采样位宽
 * 
 * @beapi beapi.gpio.adcConfigBits
 * @param pin:number mcu可用于 adc 的gpio编号
 * @param bits:9-12
 * @return bool
 */
// JSValue js_adc_set_bits(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
//     CHECK_ARGC(2)
//     ARGV_TO_UINT8(0, adc)
//     ARGV_TO_UINT8(1, bits)
//     if(adc!=1) {
//         THROW_EXCEPTION("adcConfigBits() only set adc1")
//     }
//     if( bits<9 || bits>12 ) {
//         THROW_EXCEPTION("adcConfigBits() arg bits must be 9-12")
//     }
//     return (adc1_config_width(bits-9) == ESP_OK)? JS_TRUE: JS_FALSE ;
// }

#define MAPCHANNEL(gpionum, channelnum, adcnum, vpin, vchannel, vadc)   \
    if(vpin==gpionum) {                                                 \
        vchannel = channelnum ;                                         \
        vadc = adcnum ;                                                 \
    }

#define GPIO2ADCCHANNEL(vpin, vchannel, vadc)               \
    adc_channel_t vchannel = 0 ;                            \
    uint8_t vadc = 0 ;                                      \
    MAPCHANNEL(36, 0, 1, vpin, vchannel, vadc)              \
    else MAPCHANNEL(37, 1, 1, vpin, vchannel, vadc)         \
    else MAPCHANNEL(38, 2, 1, vpin, vchannel, vadc)         \
    else MAPCHANNEL(39, 3, 1, vpin, vchannel, vadc)         \
    else MAPCHANNEL(32, 4, 1, vpin, vchannel, vadc)         \
    else MAPCHANNEL(33, 5, 1, vpin, vchannel, vadc)         \
    else MAPCHANNEL(34, 6, 1, vpin, vchannel, vadc)         \
    else MAPCHANNEL(35, 7, 1, vpin, vchannel, vadc)         \
    else MAPCHANNEL(4, 0, 2, vpin, vchannel, vadc)          \
    else MAPCHANNEL(0, 1, 2, vpin, vchannel, vadc)          \
    else MAPCHANNEL(2, 2, 2, vpin, vchannel, vadc)          \
    else MAPCHANNEL(15, 3, 2, vpin, vchannel, vadc)         \
    else MAPCHANNEL(13, 4, 2, vpin, vchannel, vadc)         \
    else MAPCHANNEL(12, 5, 2, vpin, vchannel, vadc)         \
    else MAPCHANNEL(14, 6, 2, vpin, vchannel, vadc)         \
    else MAPCHANNEL(27, 7, 2, vpin, vchannel, vadc)         \
    else MAPCHANNEL(25, 8, 2, vpin, vchannel, vadc)         \
    else MAPCHANNEL(26, 9, 2, vpin, vchannel, vadc)         \
    else {                                                  \
        THROW_EXCEPTION("pin is not a valid adc pin, must be 0, 2, 4, 12-15, 25-27, 32-39.")   \
    }

/**
 * 设置 gpoi 所使用的 adc 通道
 * 
 * @beapi beapi.gpio.setChannelAtten
 * @param pin:number mcu可用于 adc 的gpio编号
 * @param atten:0-3
 * @return bool
 */
// JSValue js_adc_set_channel_atten(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
//     CHECK_ARGC(2)
//     ARGV_TO_UINT8(0, pin)
//     ARGV_TO_UINT8(1, atten)

//     if( atten<0 || atten>3 ) {
//         THROW_EXCEPTION("setChannelAtten() arg atten must be 0-3")
//     }

//     GPIO2ADCCHANNEL(pin, channel, adc)

//     esp_err_t ret = ESP_FAIL ;
//     if(adc==1) {
//         ret = adc1_config_channel_atten(channel, atten) ;
//     }
//     else if(adc==2) {
//         ret = adc2_config_channel_atten(channel, atten) ;
//     }
    
//     return (ret==ESP_OK)? JS_TRUE: JS_FALSE ;
// }



/**
 * 读取 adc 采样结果
 * 
 * @beapi beapi.gpio.analogRead
 * @param pin:number mcu可用于 adc 的gpio编号
 * @return number
 */
// JSValue js_gpio_analog_read(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
//     CHECK_ARGC(1)
//     ARGV_TO_UINT8(0, pin)

//     uint8_t bits = 12 ;
//     if(argc>1) {
//         if( JS_ToUint32(ctx, &bits, argv[1]) ) {
//             THROW_EXCEPTION("Invalid param type");
//         }
// 	}
    
//     GPIO2ADCCHANNEL(pin, channel, adc)
    
//     int val = 0 ;
//     if(adc==1) {
//         adc1_config_width(bits-9) ;
//         val = adc1_get_raw(channel) ;
//     }
//     else if(adc==2) {
//         adc2_get_raw(channel, bits-9, &val) ;
//     }
//     return JS_NewInt32(ctx, val) ;
// }

// JSValue js_gpio_analog_write(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
//     CHECK_ARGC(2)
//     ARGV_TO_UINT8(0, pin)
//     ARGV_TO_UINT8(1, value)

//     if(pin<25 || pin>26) {
//         THROW_EXCEPTION("analogWrite() arg pin must be 25 or 26")  
//     }
//     pin-= 25 ; // DAC_CHANNEL_1 or DAC_CHANNEL_2
    
//     dac_output_enable(pin);
//     dac_output_voltage(pin, value);

//     return JS_UNDEFINED ;
// }

JSValue _gpio_isr_js_callback = NULL ;

// uint8_t _gpio_mask = 0  ;
static void IRAM_ATTR _gpio_isr_handler(void* arg) {
    uint32_t pin = (uint32_t) arg ;
    uint8_t val = gpio_get_level(pin) ;
    if( ((gpio_state[pin] >> 1) & 1) == val ) {
        return ;
    }

    if(
        (val && (gpio_state[pin]&8) )       // 上升沿
        || (!val && (gpio_state[pin]&4) )   // 下降沿
    ) {
        gpio_state[pin] |= 1 ;
    }

    // 更新值
    if(val) {
        gpio_state[pin] |= 2 ;
    }
    else {
        gpio_state[pin] &= (~2) ;
    }
}


/**
 * 为 gpio 设置输入中断
 * 
 * 参数模式:
 * * 1 上升沿
 * * 2 下降沿
 * * 3 上升沿/下降沿
 * 
 * @beapi beapi.gpio.setPinISR
 * @param pin:number mcu可用于输入的gpio编号
 * @param mode:1|2|3
 */
JSValue js_gpio_set_pin_isr(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    CHECK_ARGC(2)
    ARGV_TO_UINT8(0, pin)
    ARGV_TO_UINT8(1, mode)

    gpio_set_intr_type(pin, GPIO_INTR_ANYEDGE);
    gpio_isr_handler_remove(pin);

    gpio_state[pin] &= (~12) ;
    gpio_state[pin] |= (mode&3) << 2 ;
    
    if(gpio_get_level(pin)) {
        gpio_state[pin] |= 2 ;
    }
    else {
        gpio_state[pin] &= (~2) ;
    }


    // gpio_set_intr_type(pin, mode);
    if(mode>0){
        gpio_isr_handler_add(pin, _gpio_isr_handler, (void *)pin);
    }
    gpio_intr_enable(pin) ;

    return JS_UNDEFINED;
}

/**
 * 取消 gpio 输入中断
 * 
 * 
 * @beapi beapi.gpio.unsetPinISR
 * @param pin:number mcu可用于输入的gpio编号
 */
JSValue js_gpio_unset_pin_isr(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    CHECK_ARGC(1)
    ARGV_TO_UINT8(0, pin)

    gpio_isr_handler_remove(pin);
    gpio_intr_disable(pin) ;
    
    gpio_state[pin] &= (~12) ;

    return JS_UNDEFINED;
}

/**
 * 设置 gpio 输入中断的回调函数
 * 
 * > callback 只是一个回调函数, 在 js 事件循环中被调用, BeShell JS 代码没有机会在中断中执行
 * 
 * > 只能设置一个回调函数, 如果已经存在, 则会替换掉原来的
 * 
 * @beapi beapi.gpio.setPinISRCallback
 * @param pin:number mcu可用于输入的gpio编号
 */
JSValue js_gpio_set_pin_isr_callback(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    CHECK_ARGC(1)
    if(!JS_IsFunction(ctx, argv[0])) {
        THROW_EXCEPTION("Arg not a function")
    }

    if(_gpio_isr_js_callback)
        JS_FreeValue(ctx, _gpio_isr_js_callback) ;
    _gpio_isr_js_callback = JS_DupValue(ctx, argv[0]) ;

    return JS_UNDEFINED;
}

/**
 * 设置 PWM 定时器
 * 
 * 返回 0 表示 api 调用成功, 返回非 0 表示错误代码
 * 
 * @beapi beapi.gpio.pwmConfigTimer
 * @param timer:0-3 定时器编号
 * @param freq:number 频率(hz)
 * @param resolution:1-20 PWM分辨率
 * @param speedMode:0|1 速度模式, 0: high, 1: low
 * @return number
 */
JSValue js_pwm_config_timer(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {

    CHECK_ARGC(4)
    ARGV_TO_UINT8(0, timer)
    ARGV_TO_UINT32(1, freq)
    ARGV_TO_UINT8(2, resolution)
    ARGV_TO_UINT8(3, speedMode)

    printf("timer=%d, freq=%d, resolution=%d, speedMode=%d \n",timer,(int)freq,resolution,speedMode) ;

    if(timer<0 || timer>4) {
        THROW_EXCEPTION("pwm timer must be 0-4")
    }
    if(resolution<1 || resolution>20) {
        THROW_EXCEPTION("pwm resolution must be 1-20(bit)")
    }
    if(speedMode<0 || speedMode>1) {
        THROW_EXCEPTION("pwm speed mode must be 0(high) or 1(low)")
    }

    ledc_timer_config_t timerConf = {
        .duty_resolution = resolution,      // resolution of PWM duty
        .freq_hz = freq,                    // frequency of PWM signal
        .speed_mode = speedMode,            // timer mode
        .timer_num = timer,                 // timer index
    } ;
    return JS_NewInt32(ctx, ledc_timer_config(&timerConf)) ;
}


/**
 * 设置 PWM 通道
 * 
 * 返回 0 表示 api 调用成功, 返回非 0 表示错误代码
 * 
 * @beapi beapi.gpio.pwmConfigChannel
 * @param pin:number mcu可用于输出的gpio编号
 * @param duty:number 占空比
 * @param channel:0-7
 * @param timer:0-3 定时器编号
 * @param speedMode:0|1 速度模式, 0: high, 1: low
 * @return number
 */
JSValue js_pwm_config_channel(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    
    CHECK_ARGC(5)
    ARGV_TO_UINT8(0, gpio)
    ARGV_TO_UINT32(1, duty)
    ARGV_TO_UINT8(2, channel)
    ARGV_TO_UINT8(3, timer)
    ARGV_TO_UINT8(4, speedMode)
    
    // printf("gpio=%d, duty=%d, channel=%d, timer=%d, speedMode=%d \n",gpio,duty,channel,timer, speedMode) ;

    if(timer<0 || timer>4) {
        THROW_EXCEPTION("pwm timer must be 0-4")
    }
    if(channel<0 || channel>7) {
        THROW_EXCEPTION("pwm channel must be 0-7")
    }
    if(speedMode<0 || speedMode>1) {
        THROW_EXCEPTION("pwm speed mode must be 0(high) or 1(low)")
    }

    ledc_channel_config_t channelConf = {
        .channel    = channel,
        .duty       = duty,
        .gpio_num   = gpio,
#ifdef SOC_LEDC_SUPPORT_HS_MODE
        .speed_mode = LEDC_HIGH_SPEED_MODE,
#else
        .speed_mode = LEDC_LOW_SPEED_MODE,
#endif
        .intr_type  = LEDC_INTR_DISABLE,
        .hpoint     = 0,
        .timer_sel  = timer
    } ;
    return JS_NewInt32(ctx, ledc_channel_config(&channelConf) );
}



typedef struct {
    uint32_t freq ;
    uint8_t resolution ;
} timer_hold_t ;
typedef struct {
    uint8_t gpio ;
    uint8_t timer ;
} channel_hold_t ;

static timer_hold_t timerHolds[2][4] = { {{255,0},{255,0},{255,0},{255,0}}, {{255,0},{255,0},{255,0},{255,0}} };
static channel_hold_t channelHolds[2][8] = {
    {{255,255},{255,255},{255,255},{255,255},{255,255},{255,255},{255,255},{255,255}}, 
    {{255,255},{255,255},{255,255},{255,255},{255,255},{255,255},{255,255},{255,255}}
};


// 释放没有被占用的 timer
void free_timer(uint8_t speedMode, uint8_t timer){
    for(uint8_t c=0; c<LEDC_CHANNEL_MAX; c++) {
        if(channelHolds[speedMode][c].timer == timer) {
            return ;
        }
    }

    // printf("free timer %d\n",timer) ;

    timerHolds[speedMode][timer].freq = 255 ;
    timerHolds[speedMode][timer].resolution = 255 ;
}

JSValue pwm_config(JSContext *ctx, uint8_t gpio, uint32_t freq, uint8_t resolution, uint32_t duty, int8_t * outSpeedMode, int8_t * outChannel){
    
    uint8_t channel = 255 ;
    uint8_t timer = 255 ;
    uint8_t speedMode=0 ;
    
    for(speedMode=0; speedMode<LEDC_SPEED_MODE_MAX; speedMode++) {

        // 分配 channel
        for(uint8_t c=0; c<LEDC_CHANNEL_MAX; c++) {
            // 已经分给给该 gpio 的 channel
            if(channelHolds[speedMode][c].gpio==gpio) {
                channel = c ;
                break ;
            }
            // 遇到第一个未分配的channel作为备选
            if(channel==255 && channelHolds[speedMode][c].gpio==255) {
                channel = c ;
            }
        }

        // 无可用 channel , 切换 speed mode 再找
        if(channel==255) {
            continue ;
        }

        // 如果该 channal 已经被设置，其使用的timer 没有其他 channel 使用
        // 则先释放这个timer
        if(channelHolds[speedMode][channel].timer!=255){
            uint8_t _t = channelHolds[speedMode][channel].timer ;
            channelHolds[speedMode][channel].timer = 255 ;  // 解除此 channel 对 timer 的引用
            free_timer( speedMode, _t ) ;
        }

        // 分配 timer
        for(uint8_t t=0; t<LEDC_TIMER_MAX; t++) {
            // 使用相同的 freq 和 resolution
            if( timerHolds[speedMode][t].freq==freq && timerHolds[speedMode][t].resolution==resolution ) {
                timer = t ;
                break ;
            }
            // 第一个遇到未分配timer作为备选
            if( timer==255 && timerHolds[speedMode][t].freq==255) {
                timer = t ;
            }
        }

        // 无可用 timer , 切换 speed mode 再找
        if(timer==255) {
            continue ;
        }

        // bingo
        break ;
    }

    if(speedMode>=LEDC_SPEED_MODE_MAX) {
        THROW_EXCEPTION("There is no free timer or channel for this gpio") ;
    }

    if(outSpeedMode)
        *outSpeedMode = speedMode ;
    if(outChannel)
        *outChannel = channel ;

    // printf("use speed mode:%d, timer:%d, channel: %d\n", speedMode, timer, channel) ;

    // timer 尚未设置
    if(timerHolds[speedMode][timer].freq==255){
        
        timerHolds[speedMode][timer].freq = freq ;
        timerHolds[speedMode][timer].resolution = resolution ;

        ledc_timer_config_t timerConf = {
            .duty_resolution = resolution,      // resolution of PWM duty
            .freq_hz = freq,                    // frequency of PWM signal
            .speed_mode = speedMode,            // timer mode
            .timer_num = timer,                 // timer index
        } ;
        dn(timerConf.duty_resolution)
        dn(timerConf.freq_hz)
        dn(timerConf.speed_mode)
        dn(timerConf.timer_num)
        esp_err_t ret = ledc_timer_config(&timerConf) ;
        if(ret<0) {
            return JS_NewInt32(ctx, ret) ;
        }
    }
    
    channelHolds[speedMode][channel].gpio = gpio ;
    channelHolds[speedMode][channel].timer = timer ;

    ledc_channel_config_t channelConf = {
        .channel    = channel,
        .duty       = duty,
        .gpio_num   = gpio,
        .speed_mode = speedMode,
        .intr_type  = LEDC_INTR_DISABLE,
        .hpoint     = 0,
        .timer_sel  = timer
    } ;
        dn(channelConf.channel)
        dn(channelConf.duty)
        dn(channelConf.gpio_num)
        dn(channelConf.speed_mode)
        dn(channelConf.intr_type)
        dn(channelConf.hpoint)
        dn(channelConf.timer_sel)
    esp_err_t ret = ledc_channel_config(&channelConf) ;
    if(ret<0) {
        return JS_NewInt32(ctx, ret-1000) ;
    }
    return JS_NewInt32(ctx, 0) ;
}


/**
 * 设置 PWM
 * 
 * 返回 0 表示 api 调用成功, 返回非 0 表示错误代码
 * 
 * @beapi beapi.gpio.pwmConfig
 * @param pin:number mcu可用于输出的gpio编号
 * @param freq:number 频率(hz)
 * @param resolution:1-20 PWM分辨率
 * @param duty:number 占空比
 * @return number
 */
JSValue js_pwm_config(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    CHECK_ARGC(4)
    ARGV_TO_UINT8(0, gpio)
    ARGV_TO_UINT32(1, freq)
    ARGV_TO_UINT8(2, resolution)
    ARGV_TO_UINT32(3, duty)

    if(resolution<1 || resolution>20) {
        THROW_EXCEPTION("pwm resolution must be 1-20(bit)")
    }
    return pwm_config(ctx, gpio, freq, resolution, duty, NULL, NULL) ;
}


/**
 * PWM 输出
 * 
 * 返回 0 表示 api 调用成功, 返回非 0 表示错误代码
 * 
 * @beapi beapi.gpio.pwmWrite
 * @param pin:number mcu可用于输出的gpio编号
 * @param duty:number 占空比
 * @return number
 */
JSValue js_pwm_write(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {

    CHECK_ARGC(2)
    ARGV_TO_UINT8(0, gpio)
    ARGV_TO_DOUBLE(1, value)
    uint32_t duty = 0 ;

    int8_t speedMode = 255 ;
    int8_t channel = 255 ;
    for(speedMode=0; speedMode<LEDC_SPEED_MODE_MAX; speedMode++) {
        for(uint8_t c=0; c<LEDC_CHANNEL_MAX; c++) {
            // 已经分给给该 gpio 的 channel
            if(channelHolds[speedMode][c].gpio==gpio) {
                channel = c ;
                goto endloop ;
            }
        }
    }
    // 设置为默认的 freq/resolution
    if( JS_IsException(pwm_config(ctx, gpio, 50, 16, 0, &speedMode, &channel)) ) {
        return JS_EXCEPTION ;
    }
endloop:

    duty = value * pow(2,timerHolds[speedMode][ channelHolds[speedMode][channel].timer ].resolution) + 0.5 ;
    // printf("speed mode:%d, channel:%d, duty: %d\n", speedMode, channel, duty) ;

    esp_err_t ret = ledc_set_duty(speedMode, channel, duty) ;
    if(ret<0) {
        return JS_NewInt32(ctx, ret) ;
    }
    ret = ledc_update_duty(speedMode, channel) ;
    if(ret<0) {
        return JS_NewInt32(ctx, ret-1000) ;
    }

    return JS_NewInt32(ctx, 0) ;
}

/**
 * 停止 PWM 输出
 * 
 * @beapi beapi.gpio.pwmStop
 * @param pin:number mcu可用于输出的gpio编号
 */
JSValue js_pwm_stop(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    CHECK_ARGC(1)
    ARGV_TO_UINT8(0, gpio)

    uint8_t timer = 255 ;
    uint8_t speedMode = 255 ;

    // 找到对应的 channel
    for(uint8_t sm=0; sm<LEDC_SPEED_MODE_MAX; sm++) {
        // reset channels
        for(uint8_t c=0; c<LEDC_CHANNEL_MAX; c++) {
            if( channelHolds[sm][c].gpio == gpio){

                timer = channelHolds[sm][c].timer ;
                speedMode = sm ;

                // printf("channel=%d, timer=%d, speedMode=%d\n", c, timer, speedMode) ;

                ledc_stop(sm, c, 0) ;
                gpio_set_direction(gpio, GPIO_MODE_DEF_OUTPUT);
                channelHolds[sm][c].gpio = 255 ;
                channelHolds[sm][c].timer = 255 ;

                break ;
            }
        }
    }


    if(timer==255){
        return JS_UNDEFINED ;
    }
    
    // 如果没有其他 channel 使用 timer ，则释放 timer
    free_timer(speedMode, timer) ;

    return JS_UNDEFINED ;
}

void be_module_gpio_require(JSContext *ctx) {
    
    JSValue global = JS_GetGlobalObject(ctx);
    JSValue beapi = JS_GetPropertyStr(ctx, global, "beapi") ;

    JSValue gpio = JS_NewObject(ctx);
    JS_SetPropertyStr(ctx, beapi, "gpio", gpio);
    
    JS_SetPropertyStr(ctx, gpio, "pinMode", JS_NewCFunction(ctx, js_gpio_pin_mode, "pinMode", 1));
    JS_SetPropertyStr(ctx, gpio, "pinPull", JS_NewCFunction(ctx, js_gpio_pin_pull, "pinPull", 1));
    JS_SetPropertyStr(ctx, gpio, "digitalRead", JS_NewCFunction(ctx, js_gpio_digital_read, "digitalRead", 1));
    JS_SetPropertyStr(ctx, gpio, "digitalWrite", JS_NewCFunction(ctx, js_gpio_digital_write, "digitalWrite", 1));
    // JS_SetPropertyStr(ctx, gpio, "adcConfigBits", JS_NewCFunction(ctx, js_adc_set_bits, "adcConfigBits", 1));
    // JS_SetPropertyStr(ctx, gpio, "adcConfigAtten", JS_NewCFunction(ctx, js_adc_set_channel_atten, "adcConfigAtten", 1));
    // JS_SetPropertyStr(ctx, gpio, "analogRead", JS_NewCFunction(ctx, js_gpio_analog_read, "analogRead", 1));
    // JS_SetPropertyStr(ctx, gpio, "analogWrite", JS_NewCFunction(ctx, js_gpio_analog_write, "analogWrite", 1));
    JS_SetPropertyStr(ctx, gpio, "pwmConfigTimer", JS_NewCFunction(ctx, js_pwm_config_timer, "pwmConfigTimer", 1));
    JS_SetPropertyStr(ctx, gpio, "pwmConfigChannel", JS_NewCFunction(ctx, js_pwm_config_channel, "pwmConfigChannel", 1));
    JS_SetPropertyStr(ctx, gpio, "pwmConfig", JS_NewCFunction(ctx, js_pwm_config, "pwmConfig", 1));
    JS_SetPropertyStr(ctx, gpio, "pwmWrite", JS_NewCFunction(ctx, js_pwm_write, "pwmWrite", 1));
    JS_SetPropertyStr(ctx, gpio, "pwmStop", JS_NewCFunction(ctx, js_pwm_stop, "pwmStop", 1));
    JS_SetPropertyStr(ctx, gpio, "setPinISR", JS_NewCFunction(ctx, js_gpio_set_pin_isr, "setPinISR", 1));
    JS_SetPropertyStr(ctx, gpio, "unsetPinISR", JS_NewCFunction(ctx, js_gpio_unset_pin_isr, "unsetPinISR", 1));
    JS_SetPropertyStr(ctx, gpio, "setPinISRCallback", JS_NewCFunction(ctx, js_gpio_set_pin_isr_callback, "setPinISRCallback", 1));

    JS_FreeValue(ctx, global);
    JS_FreeValue(ctx, beapi);
}

void be_module_gpio_init() {
    // gpio_install_isr_service(0);
    
    for(uint8_t p=0; p<PIN_CNT; p++) {
        gpio_state[p] = 0 ;
    }
}

void be_module_gpio_reset(JSContext *ctx) {
    // 清空 js 回调函数
    if(_gpio_isr_js_callback) {
        JS_FreeValue(ctx, _gpio_isr_js_callback) ;
        _gpio_isr_js_callback = NULL ;
    }

    for(uint8_t p=0; p<PIN_CNT; p++) {
        gpio_state[p] = 0 ;
    }


    // 停止所有 pwm
    for(uint8_t speedMode=0; speedMode<LEDC_SPEED_MODE_MAX; speedMode++) {
        // reset channels
        for(uint8_t c=0; c<LEDC_CHANNEL_MAX; c++) {
            // printf("speedMode=%d, channel=%d, timer=%d, gpio=%d\n",speedMode,c,channelHolds[speedMode][c].timer,channelHolds[speedMode][c].gpio) ;
            if(channelHolds[speedMode][c].gpio!=255) {
                ledc_stop(speedMode, c, 0) ;
                gpio_set_direction(channelHolds[speedMode][c].gpio, GPIO_MODE_DEF_OUTPUT);
                channelHolds[speedMode][c].gpio = 255 ;
                channelHolds[speedMode][c].timer = 255 ;
            }
        }
        // reset timers
        for(uint8_t t=0; t<LEDC_TIMER_MAX; t++) {
            timerHolds[speedMode][t].freq = 255 ;
            timerHolds[speedMode][t].resolution = 0 ;
        }
    }
}


inline void be_module_gpio_loop(JSContext *ctx)  {
    if(!_gpio_isr_js_callback)
        return ;
    
    JSValueConst * argv = malloc(sizeof(JSValue)*2) ;

    for(uint8_t p=0; p<PIN_CNT; p++) {
        if( gpio_state[p] & 1 ) {
            gpio_state[p] &= ~1;

            argv[0] = JS_NewInt32(ctx, p) ;
            argv[1] = JS_NewInt32(ctx, gpio_state[p]) ;
            JS_Call(ctx, _gpio_isr_js_callback, JS_UNDEFINED, 2, argv) ;
            JS_FreeValue(ctx, argv[0]) ;
            JS_FreeValue(ctx, argv[1]) ;
        }
    }

    free(argv) ;
}