/**
 * > GPIO 模块的例子请参考：[用 JavaScript 控制 GPIO](../guide/gpio.md)
 * 
 * @module gpio
 */

#include "GPIO.hpp"
#include "driver/adc.h"
#include "driver/gpio.h"
#include "hal/adc_types.h"
#include <cstddef>
#include <map>
#include <vector>
#include "esp_adc/adc_oneshot.h"
#include "soc/gpio_num.h"
#include "driver/ledc.h"
#include "../js/gpio.c"

using namespace std ;


    
namespace be {
    
    static adc_oneshot_unit_handle_t adc_handles[2] = {NULL} ;

    /**
     * bit1:   未触发的 isr
     * bit2:   前次电平值
     * bit3:   监听下降沿
     * bit4:   监听上升沿
     */
    static uint8_t gpio_state[56] = {
        0,0,0,0,0,0,0,0,
        0,0,0,0,0,0,0,0,
        0,0,0,0,0,0,0,0,
        0,0,0,0,0,0,0,0,
        0,0,0,0,0,0,0,0,
        0,0,0,0,0,0,0,0,
        0,0,0,0,0,0,0,0,
    } ;
    static bool pending_event = false ;

    static map< uint8_t, pair< vector<JSValue>, vector<JSValue> > > watching_callbacks ;

    static map<gpio_num_t, adc_channel_t> adc_channels ;
    static map<gpio_num_t, adc_unit_t> adc_units ;
    static map<gpio_num_t, bool> adc_channel_configured ;

    bool GPIO::isr_installed=false ;

    GPIO::GPIO(JSContext * ctx, const char * name)
        : NativeModule(ctx, name, 0)
    {
        exportFunction("setMode",setMode,2) ;
        exportFunction("pull",pull,2) ;
        exportFunction("write",write,2) ;
        exportFunction("read",read,1) ;
        exportFunction("watch",watch,0) ;
        exportFunction("unwatch",unwatch,0) ;
        exportFunction("adcUnitInit",adcUnitInit,1) ;
        exportFunction("adcChannelInit",adcChannelInit,1) ;
        exportFunction("adcRead",adcRead,1) ;
        exportFunction("readAnalog",adcRead,1) ;
        exportFunction("adcInfo",adcInfo,0) ;
        exportFunction("resetPin",resetPin,0) ;
        exportName("blink") ;
        
        exportFunction("apiConfigPWM",apiConfigPWM,0) ;
        exportFunction("apiWritePWM",apiWritePWM,0) ;
        exportFunction("apiUpdatePWM",apiUpdatePWM,0) ;
        exportFunction("apiStopPWM",apiStopPWM,0) ;
        exportFunction("pwmMaxSpeedMode",pwmMaxSpeedMode,0) ;
        exportName("configPWM") ;
        exportName("writePWM") ;
        exportName("updatePWM") ;
        exportName("stopPWM") ;

        EXPORT_FUNCTION(test) ;


        // 反射 gpio -> adc unit/通道
        gpio_num_t pin ;
        for(int c=0;c<ADC1_CHANNEL_MAX;c++) {
            adc_channel_t channel = (adc_channel_t)c ;
            adc1_pad_get_io_num((adc1_channel_t)channel, &pin) ;
            adc_channels[pin] = channel ;
            adc_units[pin] = ADC_UNIT_1 ;
        }
        for(int c=0;c<ADC2_CHANNEL_MAX;c++) {
            adc_channel_t channel = (adc_channel_t)c ;
            adc2_pad_get_io_num((adc2_channel_t)channel, &pin) ;
            adc_channels[pin] = channel ;
            adc_units[pin] = ADC_UNIT_2 ;
        }
    }

    void GPIO::exports(JSContext *ctx) {
        JSEngineEvalEmbeded(ctx, gpio)
    }

    /**
     * 设置 GPIO 的工作模式
     * 
     * @function setMode
     * @param pin:number 引脚序号
     * @param mode:string 模式, 可用值: "input"|"output"|"output-od"|"input-output"|"input-output-od"
     * 
     * @return bool
     */
    JSValue GPIO::setMode(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        ASSERT_ARGC(2)
        ARGV_TO_UINT8(0, pin)
        const char * mode = JS_ToCString(ctx, argv[1]) ;

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
            JSTHROW("unknow pin mode(input, output, output-od, input-output, input-output-od)")
        }
        
        if(gpio_config(&conf) != ESP_OK)
        {
            JSTHROW("set pin mode failed, arg invalid?")
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
     * @function pull
     * @param pin:number mcu可用的gpio编号
     * @param mode:string
     * @return undefined
     */
    JSValue GPIO::pull(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        ASSERT_ARGC(2)
        ARGV_TO_UINT8(0, pin)
        ARGV_TO_CSTRING(1, mode)

        if(strcmp(mode,"up")==0) {
            gpio_set_pull_mode((gpio_num_t)pin, GPIO_PULLUP_ONLY) ;
            gpio_pullup_en((gpio_num_t)pin) ;
        }
        else if(strcmp(mode,"down")==0) {
            gpio_set_pull_mode((gpio_num_t)pin, GPIO_PULLDOWN_ONLY) ;
            gpio_pulldown_en((gpio_num_t)pin) ;
        }
        else if(strcmp(mode,"updown")==0) {
            gpio_set_pull_mode((gpio_num_t)pin, GPIO_PULLUP_PULLDOWN) ;
            gpio_pulldown_en((gpio_num_t)pin) ;
            gpio_pullup_en((gpio_num_t)pin) ;
        }
        else if(strcmp(mode,"floating")==0) {
            gpio_set_pull_mode((gpio_num_t)pin, GPIO_FLOATING) ;
            gpio_pullup_dis((gpio_num_t)pin) ;
            gpio_pulldown_dis((gpio_num_t)pin) ;
        }
        else {
            JSTHROW("unknow pin pull mode(up|down|updown|floating)")
        }
        JS_FreeCString(ctx, mode) ;
        return JS_UNDEFINED ;
    }

    /**
     * gpio 电平输出
     * 
     * @function write
     * @param pin:number mcu可用的gpio编号
     * @param value:number 输出电平 0|1 
     * @return undefined
     */
    JSValue GPIO::write(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        ASSERT_ARGC(2)
        ARGV_TO_UINT8(0, pin)
        ARGV_TO_UINT8(1, value)
        return gpio_set_level((gpio_num_t)pin, value)==ESP_OK? JS_TRUE: JS_FALSE ;
    }
    /**
     * gpio 电平输入
     * 
     * @function read
     * @param pin:number mcu可用的gpio编号
     * @return 0|1
     */
    JSValue GPIO::read(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        ASSERT_ARGC(1)
        ARGV_TO_UINT8(0, pin)
        gpio_get_level((gpio_num_t)pin);
        return JS_NewUint32(ctx, gpio_get_level((gpio_num_t)pin)) ;
    }


    // JSValue GPIO::readAnalog(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    //     return JS_UNDEFINED ;
    // }
    // JSValue GPIO::writeAnalog(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    //     return JS_UNDEFINED ;
    // }
    
    
    // /**
    //  * 设置 adc 的位宽
    //  * 
    //  * @function adcSetBits
    //  * @param adc:number adc (目前版本只能为 1)
    //  * @param bits:number 位宽 (9-12)
    //  * @return bool
    //  */
    // JSValue GPIO::adcSetBits(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    //     ASSERT_ARGC(2)
    //     ARGV_TO_UINT8(0, adc)
    //     ARGV_TO_UINT8(1, bits)
    //     if(adc!=1) {
    //         JSTHROW("adcConfigBits() only set adc1")
    //     }
    //     if( bits<9 || bits>12 ) {
    //         JSTHROW("adcConfigBits() arg bits must be 9-12")
    //     }
    //     return (adc1_config_width((adc_bits_width_t)(bits-9)) == ESP_OK)? JS_TRUE: JS_FALSE ;
    // }

    
    #define MAPCHANNEL(gpionum, channelnum, adcnum, vpin, vchannel, vadc)   \
        if(vpin==gpionum) {                                                 \
            vchannel = ADC_CHANNEL_##channelnum ;                           \
            vadc = adcnum ;                                                 \
        }

    #define GPIO2ADCCHANNEL(vpin, vchannel, vadc)               \
        adc_channel_t vchannel = ADC_CHANNEL_0 ;                \
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
            JSTHROW("pin is not a valid adc pin, must be 0, 2, 4, 12-15, 25-27, 32-39.")   \
        }
    
    // /**
    //  * 设置 gpio 使用哪个 adc 通道
    //  * 
    //  * @function adcSetChannelAtten
    //  * param pin:number mcu可用的gpio编号
    //  * param atten:number adc通道 (1|2)
    //  * @return bool
    //  */
    // JSValue GPIO::adcSetChannelAtten(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {

    //     ASSERT_ARGC(2)
    //     ARGV_TO_UINT8(0, pin)
    //     ARGV_TO_UINT8(1, atten)

    //     if( atten<0 || atten>3 ) {
    //         JSTHROW("setChannelAtten() arg atten must be 0-3")
    //     }

    //     GPIO2ADCCHANNEL(pin, channel, adc)

    //     esp_err_t ret = ESP_FAIL ;
    //     if(adc==1) {
    //         ret = adc1_config_channel_atten((adc1_channel_t)channel, (adc_atten_t)atten) ;
    //     }
        
    //     #if (SOC_ADC_PERIPH_NUM >= 2)
    //     else if(adc==2) {
    //         ret = adc2_config_channel_atten((adc2_channel_t)channel, (adc_atten_t)atten) ;
    //     }
    //     #endif
        
    //     return (ret==ESP_OK)? JS_TRUE: JS_FALSE ;
    // }

    /**
     * 设置 GPIO PWM 输出
     * 
     * 参数 `options` 是一个对象，包含以下可选属性:
     * ```
     * {
     *   mode:number = 0 ,          // 速度模式，支持高速：0=高速模式，1=低速模式；不支持高速：0=低速模式
     *   duty:number = 0 ,          // 占空比，0-max, max 取决于 resolution
     *   freq:number = 1000 ,       // PWM频率，1-40000Hz
     *   channel:number = 0 ,       // PWM通道，0-7
     *   resolution:number = 10 ,   // 占空比分辨率，1-20位
     *   timer:number = 0 ,         // 定时器编号，0-3
     *   clk:number = 0 ,           // 时钟配置，0=自动时钟，1=APB时钟
     *   intr:number = 0 ,          // 中断类型，0=禁用中断，1=启用中断
     * }
     * ```
     * 
     * @function configPWM
     * @param pin:number GPIO引脚编号
     * @param options:object=null 配置选项
     * @return undefined
     */
    JSValue GPIO::apiConfigPWM(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        CHECK_ARGC(1)
        ARGV_TO_UINT8(0, pin)
        
        // Default values
        uint8_t speed_mode = LEDC_LOW_SPEED_MODE;
        uint32_t duty = 0;
        uint32_t freq = 1000;
        uint8_t channel = 0;
        ledc_timer_bit_t duty_resolution = LEDC_TIMER_10_BIT;
        ledc_timer_t timer_num = LEDC_TIMER_0;
        ledc_clk_cfg_t clk_cfg = LEDC_AUTO_CLK;
        ledc_intr_type_t intr_type = LEDC_INTR_DISABLE;
        
        // Process options object if provided
        if (argc > 1 && !JS_IsUndefined(argv[1])) {
            if (!JS_IsObject(argv[1])) {
                JSTHROW("Second parameter must be an options object")
            }
            
            // Extract speed_mode
            JSValue js_speed_mode = JS_GetPropertyStr(ctx, argv[1], "mode");
            if (!JS_IsUndefined(js_speed_mode)) {
                uint32_t sm;
                if (JS_ToUint32(ctx, &sm, js_speed_mode) != 0) {
                    JS_FreeValue(ctx, js_speed_mode);
                    JSTHROW("Invalid mode value")
                }
                if (sm > LEDC_SPEED_MODE_MAX) {
                    JS_FreeValue(ctx, js_speed_mode);
                    JSTHROW("Speed mode must be 0 - %d", LEDC_SPEED_MODE_MAX)
                }
                speed_mode = sm;
            }
            JS_FreeValue(ctx, js_speed_mode);
            
            // Extract duty
            JSValue js_duty = JS_GetPropertyStr(ctx, argv[1], "duty");
            if (!JS_IsUndefined(js_duty)) {
                if (JS_ToUint32(ctx, &duty, js_duty) != 0) {
                    JS_FreeValue(ctx, js_duty);
                    JSTHROW("Invalid duty value")
                }
                // if (duty > 1023) {
                //     duty = 1023;  // Limit to 10-bit resolution
                // }
            }
            JS_FreeValue(ctx, js_duty);
            
            // Extract freq
            JSValue js_freq = JS_GetPropertyStr(ctx, argv[1], "freq");
            if (!JS_IsUndefined(js_freq)) {
                if (JS_ToUint32(ctx, &freq, js_freq) != 0) {
                    JS_FreeValue(ctx, js_freq);
                    JSTHROW("Invalid frequency value")
                }
                if (freq < 1) freq = 1;
                if (freq > 40000) freq = 40000; // Limit frequency to reasonable range
            }
            JS_FreeValue(ctx, js_freq);
            
            // Extract channel
            JSValue js_channel = JS_GetPropertyStr(ctx, argv[1], "channel");
            if (!JS_IsUndefined(js_channel)) {
                uint32_t ch;
                if (JS_ToUint32(ctx, &ch, js_channel) != 0) {
                    JS_FreeValue(ctx, js_channel);
                    JSTHROW("Invalid channel value")
                }
                if (ch > 7) {
                    JS_FreeValue(ctx, js_channel);
                    JSTHROW("Channel must be between 0-7")
                }
                channel = ch;
            }
            JS_FreeValue(ctx, js_channel);
            
            // Extract duty_resolution
            JSValue js_duty_resolution = JS_GetPropertyStr(ctx, argv[1], "resolution");
            if (!JS_IsUndefined(js_duty_resolution)) {
                uint32_t res;
                if (JS_ToUint32(ctx, &res, js_duty_resolution) != 0) {
                    JS_FreeValue(ctx, js_duty_resolution);
                    JSTHROW("Invalid resolution value")
                }
                if (res < 1 || res > 20) {
                    JS_FreeValue(ctx, js_duty_resolution);
                    JSTHROW("Duty resolution must be between 1-20")
                }
                duty_resolution = (ledc_timer_bit_t)res;
            }
            JS_FreeValue(ctx, js_duty_resolution);
            
            // Extract timer_num
            JSValue js_timer_num = JS_GetPropertyStr(ctx, argv[1], "timer");
            if (!JS_IsUndefined(js_timer_num)) {
                uint32_t tm;
                if (JS_ToUint32(ctx, &tm, js_timer_num) != 0) {
                    JS_FreeValue(ctx, js_timer_num);
                    JSTHROW("Invalid timer value")
                }
                if (tm > 3) {
                    JS_FreeValue(ctx, js_timer_num);
                    JSTHROW("Timer number must be between 0-3")
                }
                timer_num = (ledc_timer_t)tm;
            }
            JS_FreeValue(ctx, js_timer_num);
            
            // Extract clk_cfg
            JSValue js_clk_cfg = JS_GetPropertyStr(ctx, argv[1], "clk");
            if (!JS_IsUndefined(js_clk_cfg)) {
                uint32_t clk;
                if (JS_ToUint32(ctx, &clk, js_clk_cfg) != 0) {
                    JS_FreeValue(ctx, js_clk_cfg);
                    JSTHROW("Invalid clk value")
                }
                clk_cfg = (ledc_clk_cfg_t)clk;
            }
            JS_FreeValue(ctx, js_clk_cfg);
            
            // Extract intr_type
            JSValue js_intr_type = JS_GetPropertyStr(ctx, argv[1], "intr");
            if (!JS_IsUndefined(js_intr_type)) {
                uint32_t intr;
                if (JS_ToUint32(ctx, &intr, js_intr_type) != 0) {
                    JS_FreeValue(ctx, js_intr_type);
                    JSTHROW("Invalid intr value")
                }
                intr_type = (ledc_intr_type_t)intr;
            }
            JS_FreeValue(ctx, js_intr_type);
        }
        
        // Configure LEDC timer
        ledc_timer_config_t timer_conf = {
            .speed_mode = (ledc_mode_t)speed_mode,
            .duty_resolution = duty_resolution,
            .timer_num = timer_num,
            .freq_hz = freq,
            .clk_cfg = clk_cfg
        };
        
        esp_err_t err = ledc_timer_config(&timer_conf);
        if (err != ESP_OK) {
            JSTHROW("Config LEDC timer failed, err: %d", err)
        }
        
        // Configure LEDC channel
        ledc_channel_config_t channel_conf = {
            .gpio_num = pin,
            .speed_mode = (ledc_mode_t)speed_mode,
            .channel = (ledc_channel_t)channel,
            .intr_type = intr_type,
            .timer_sel = timer_num,
            .duty = duty,
            .hpoint = 0
        };
        
        err = ledc_channel_config(&channel_conf);
        if (err != ESP_OK) {
            JSTHROW("Config LEDC channel failed, err: %d", err)
        }
        
        return JS_UNDEFINED;
    }
    /**
     * 设置 PWM 通道的占空比
     * 
     * @function writePWM
     * @param mode:number 速度模式
     * @param channel:number 通道
     * @param duty:number 占空比值 (0-max), max 取决于 configPWM() 传入的 options.duty_resolution
     * @param update:boolean=true 是否立即更新
     * @return undefined
     */
    JSValue GPIO::apiWritePWM(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        CHECK_ARGC(3)
        ARGV_TO_UINT8(0, speed_mode)
        ARGV_TO_UINT8(1, channel)
        ARGV_TO_UINT32(2, duty)
        
        // Validate speed_mode
        if (speed_mode > LEDC_SPEED_MODE_MAX) {
            JSTHROW("Speed mode must be 0 - %d", LEDC_SPEED_MODE_MAX)
        }
        
        // Validate channel
        if (channel > 7) {
            JSTHROW("Channel must be between 0-7")
        }
        
        bool update = true;  // Default is true
        if (argc > 3 && !JS_IsUndefined(argv[3])) {
            update = JS_ToBool(ctx, argv[3]);
        }
        
        // Set the duty cycle
        esp_err_t err = ledc_set_duty((ledc_mode_t)speed_mode, (ledc_channel_t)channel, duty);
        if (err != ESP_OK) {
            JSTHROW("Set PWM duty failed, err: %d", err)
        }
        
        // Update the duty if requested
        if (update) {
            err = ledc_update_duty((ledc_mode_t)speed_mode, (ledc_channel_t)channel);
            if (err != ESP_OK) {
                JSTHROW("Update PWM duty failed, err: %d", err)
            }
        }
        
        return JS_UNDEFINED;
    }

    
    /**
     * 更新 PWM 通道的占空比
     * 
     * @function updatePWM
     * @param mode:number 速度模式
     * @param channel:number 通道
     * @return undefined
     */
    JSValue GPIO::apiUpdatePWM(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        CHECK_ARGC(2)
        ARGV_TO_UINT8(0, speed_mode)
        ARGV_TO_UINT8(1, channel)

        // Validate speed_mode
        if (speed_mode > LEDC_SPEED_MODE_MAX) {
            JSTHROW("Speed mode must be 0 - %d", LEDC_SPEED_MODE_MAX)
        }
        
        // Validate channel
        if (channel > 7) {
            JSTHROW("Channel must be between 0-7")
        }

        // Update the duty cycle
        esp_err_t err = ledc_update_duty((ledc_mode_t)speed_mode, (ledc_channel_t)channel);
        if (err != ESP_OK) {
            JSTHROW("Update PWM duty failed, err: %d", err)
        }
        
        return JS_UNDEFINED;
    }

    /**
     * 停止 PWM 输出
     * 
     * @function stopPWM
     * @param mode:number 速度模式
     * @param channel:number 通道
     * @return undefined
     */
    JSValue GPIO::apiStopPWM(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        CHECK_ARGC(2)
        ARGV_TO_UINT8(0, speed_mode)
        ARGV_TO_UINT8(1, channel)
        
        // Validate speed_mode
        if (speed_mode > LEDC_SPEED_MODE_MAX) {
            JSTHROW("Speed mode must be 0 - %d", LEDC_SPEED_MODE_MAX)
        }
        
        // Validate channel
        if (channel > 7) {
            JSTHROW("Channel must be between 0-7")
        }
        
        // Stop PWM output
        esp_err_t err = ledc_stop((ledc_mode_t)speed_mode, (ledc_channel_t)channel, 0);
        if (err != ESP_OK) {
            JSTHROW("Stop PWM failed, err: %d", err)
        }
        
        return JS_UNDEFINED;
    }

    /**
     * 返回 PWM 的最大速度模式 (LEDC_SPEED_MODE_MAX)， 取决于 LEDC_SPEED_MODE_MAX 的定义
     * 
     * @function pwmMaxSpeedMode
     * @return number
     */
    JSValue GPIO::pwmMaxSpeedMode(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        return JS_NewUint32(ctx, LEDC_SPEED_MODE_MAX) ;
    }
    
    static void /*IRAM_ATTR*/ gpio_isr_handler(void* arg) {

        gpio_num_t pin = (gpio_num_t) (uint32_t) arg ;
        uint8_t val = gpio_get_level( pin ) ;

        if( ((gpio_state[pin] >> 1) & 1) == val ) {
            return ;
        }

        if(
            (val && (gpio_state[pin]&8) )       // 上升沿
            || (!val && (gpio_state[pin]&4) )   // 下降沿
        ) {
            gpio_state[pin] |= 1 ;
        }

        // 更新前值
        if(val) {
            gpio_state[pin] |= 2 ;
        }
        else {
            gpio_state[pin] &= (~2) ;
        }

        pending_event = true ;
    }

    
    bool GPIO::installISR(int flag) {
        if(isr_installed) {
            return true ;
        }
        esp_err_t res = gpio_install_isr_service(flag) ;
        if(res!=ESP_OK) {
            printf("gpio_install_isr_service() failed:%d\n", res) ;
            return false ;
        }
        isr_installed = true ;
        return true ;
    }

    void GPIO::uninstallISR() {
        if(!isr_installed) {
            return ;
        }
        gpio_uninstall_isr_service() ;
        isr_installed = false ;
    }


    /**
     * 监听 gpio 外部电平变化
     * 
     * @function watch
     * @param pin:number mcu可用的gpio编号
     * @param mode:string 监听模式， 可选值为: "rising"|"falling"|"both"
     * @param callback:function 回调函数，callback 的原型为 `function(pin, value)`
     * @return bool
     */
    JSValue GPIO::watch(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        CHECK_ARGC(3)
        ARGV_TO_UINT8(0, pin)
        string ARGV_TO_STRING(1, edge)
        if(!JS_IsFunction(ctx, argv[2])) {
            JSTHROW("watch() arg callback must be a function")
        }

        int mode = 0 ;
        if(edge=="rising") {
            mode = 1 ;
        } else if(edge=="falling") {
            mode = 2 ;
        } else if(edge=="both") {
            mode = 3 ;
        } else {
            JSTHROW("watch() arg edge must be rising|falling|both")
        }

        if(!installISR(0)){
            JSTHROW("install gpio isr failed")
        }

        gpio_isr_handler_remove((gpio_num_t)pin);

        //gpio_state[pin] &= (~12) ;
        gpio_state[pin] |= mode << 2 ;
        mode = gpio_state[pin] >> 2 & 3 ;
        // dn(mode)

        if(gpio_get_level((gpio_num_t)pin)) {
            gpio_state[pin] |= 2 ;
        }
        else {
            gpio_state[pin] &= (~2) ;
        }

        //gpio_set_intr_type((gpio_num_t)pin, (gpio_int_type_t)mode);
        if(mode>0){
            gpio_isr_handler_add((gpio_num_t)pin, gpio_isr_handler, (void *)pin);
        }
        gpio_set_intr_type((gpio_num_t)pin, GPIO_INTR_ANYEDGE);
        gpio_intr_enable((gpio_num_t)pin) ;

        if(mode&1) {
            watching_callbacks[pin].first.push_back( JS_DupValue(ctx,argv[2]) ) ;
        }
        if(mode&2) {
            watching_callbacks[pin].second.push_back( JS_DupValue(ctx,argv[2]) ) ;
        }

        JSEngine::fromJSContext(ctx)->addLoopFunction(GPIO::loop, nullptr, true, 0) ;

        return JS_UNDEFINED ;
    }
    JSValue GPIO::unwatch(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        return JS_UNDEFINED ;
    }

    void GPIO::loop(JSContext * ctx, void * arg) {

        if(!pending_event) {
            return ;
        }

        pending_event = false ;

        JSValueConst argv[2] ;

        for(uint8_t p=0; p<sizeof(gpio_state); p++) {
            if( gpio_state[p] & 1 ) {
                gpio_state[p] &= ~1;
                
                uint8_t level = (gpio_state[p]>>1) & 0x01 ;

                argv[0] = JS_NewInt32(ctx, p) ;
                argv[1] = JS_NewInt32(ctx, gpio_state[p]) ;


                for(auto callback: level? watching_callbacks[p].first : watching_callbacks[p].second) {
                    JS_Call(ctx, callback, JS_UNDEFINED, 2, argv) ;
                }

                JS_FreeValue(ctx, argv[0]) ;
                JS_FreeValue(ctx, argv[1]) ;
            }
        }
    }
    
    JSValue GPIO::test(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        return JS_NewInt32(ctx, gpio_state[7]) ;
    }

    static int _initADCUnit(adc_unit_t uint_num) {
        if(adc_handles[uint_num]!=NULL) {
            return -1 ;
        }

        // adc_oneshot_unit_init_cfg_t init_config = {
        //     .unit_id = uint_num,
        //     .clk_src = ADC_RTC_CLK_SRC_DEFAULT,
        //     .ulp_mode = ADC_ULP_MODE_DISABLE,
        // };
        
        // return adc_oneshot_new_unit(&init_config, &adc_handles[uint_num]) ;
        return 0 ;
    }
    
    static int _initADCChannel(adc_unit_t uint_num, adc_channel_t channel, gpio_num_t pin) {

        esp_err_t err ;
        if(!adc_handles[uint_num]) {
            err = _initADCUnit(uint_num) ;
            if(err!=ESP_OK) {
                return err ;
            }
        }

        // 通道配置
        // adc_oneshot_chan_cfg_t config = {
        //     .atten = ADC_ATTEN_DB_12,  // Use ADC_ATTEN_DB_0 as ADC_ATTEN_DB_11 is deprecated
        //     .bitwidth = ADC_BITWIDTH_12,
        // };
        
        // err = adc_oneshot_config_channel(adc_handles[uint_num], channel, &config) ;
        // if(err!=ESP_OK) {
        //     return err ;
        // }

        adc_channel_configured[pin] = true ;

        return ESP_OK ;
    }

    JSValue GPIO::adcUnitInit(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        CHECK_ARGC(1)
        uint32_t uint_num = 1 ;
        if( JS_ToUint32(ctx, &uint_num, argv[0])!=0 ) {
            JSTHROW("Invalid param type")
        }
        if(uint_num<1 || uint_num>2) {
            JSTHROW("Invalid param value")
        }
        int err = _initADCUnit((adc_unit_t)uint_num) ;
        if(err==-1) {
            JSTHROW("unit already inited")
        }
        if(err!=ESP_OK) {
            JSTHROW("init adc unit failed, err:%d", err)
        }
        return JS_UNDEFINED ;
    }

    JSValue GPIO::adcChannelInit(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        CHECK_ARGC(1)
        ARGV_TO_GPIO(0, pin)

        if(!adc_units.count(pin) || !adc_channels.count(pin)) {
            JSTHROW("pin is not a valid adc pin.")
        }
        adc_unit_t uint_num = adc_units[pin] ;
        adc_channel_t channel = adc_channels[pin] ;
        
        if(!adc_handles[uint_num]) {
            esp_err_t err = _initADCUnit(uint_num) ;
            if(err!=ESP_OK) {
                JSTHROW("init adc unit failed, err:%d", err)
            }
        }
        
        esp_err_t err = _initADCChannel(uint_num, (adc_channel_t)channel, pin) ;
        if(err!=ESP_OK) {
            JSTHROW("init adc channel failed, err:%d", err)
        }

        return JS_UNDEFINED ;
    }

    JSValue GPIO::adcRead(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        CHECK_ARGC(1)
        ARGV_TO_GPIO(0, pin)

        if(!adc_units.count(pin) || !adc_channels.count(pin)) {
            JSTHROW("pin is not a valid adc pin.")
        }
        adc_unit_t uint_num = adc_units[pin] ;
        adc_channel_t channel = adc_channels[pin] ;

        if(!adc_channel_configured.count(pin) || !adc_channel_configured[pin]) {
            esp_err_t err = _initADCChannel(uint_num, (adc_channel_t)channel, pin) ;
            if(err!=ESP_OK) {
                JSTHROW("init adc channel failed, err:%d", err)
            }
        }

        int value = 0 ;

        // esp_err_t err = adc_oneshot_read(adc_handles[uint_num], (adc_channel_t)channel, &value) ;
        // if(err!=ESP_OK) {
        //     JSTHROW("read adc failed, err:%d", err)
        // }

        return JS_NewInt32(ctx, value) ;
    }

    JSValue GPIO::adcInfo(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        JSValue obj = JS_NewObject(ctx) ;
        int i = 0 ;
        for(auto &kv: adc_channels) {

            JSValue item = JS_NewObject(ctx) ;
            JS_SetPropertyStr(ctx, item, "channel", JS_NewInt32(ctx, kv.second)) ;
            JS_SetPropertyStr(ctx, item, "unit", JS_NewInt32(ctx, adc_units[kv.first])) ;
            JS_SetPropertyUint32(ctx, obj, JS_NewInt32(ctx, kv.first), item) ;
        }
        return obj ;
    }

    /**
     * 重置 GPIO 引脚到默认状态
     * 
     * @function resetPin
     * @param pin:number GPIO引脚编号
     * @return undefined
     */
    JSValue GPIO::resetPin(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        CHECK_ARGC(1)
        ARGV_TO_UINT8(0, pin)
        
        // Validate pin number
        if (pin >= GPIO_NUM_MAX) {
            JSTHROW("Invalid GPIO pin number")
        }
        
        // Reset the pin to default state
        esp_err_t err = gpio_reset_pin((gpio_num_t)pin);
        if (err != ESP_OK) {
            JSTHROW("Reset GPIO pin failed, err: %d", err)
        }
        
        return JS_UNDEFINED;
    }
}


/**
 * GPIO 闪烁，执行该函数后，指定的引脚会持续高低电平切换。
 * 
 * @function blink
 * @param pin:number 引脚序号
 * @param time:number 间隔时间，单位毫秒，闪烁的半周期
 * 
 * @return number 定时器id，可使用 `clearTimeout()` 停止闪烁。
 */
