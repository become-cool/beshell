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
#include <cstdint>
#include <map>
#include <vector>
#include "esp_adc/adc_oneshot.h"
#include "quickjs/quickjs.h"
#include "soc/gpio_num.h"
#include "driver/ledc.h"
#include "../js/gpio.c"

using namespace std ;


    
namespace be {
    
    static adc_oneshot_unit_handle_t adc_handles[2] = {NULL} ;

    static JSValue jsHandler = JS_NULL ;
    static vector<uint8_t> pending_level_changes ;

    // static map< uint8_t, pair< vector<JSValue>, vector<JSValue> > > watching_callbacks ;

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
        exportFunction("resetPin",resetPin,0) ;
        exportName("blink") ;

        // adc
        exportFunction("adcConfigWidth",adcConfigWidth,1) ;
        exportFunction("adcConfigAtten",adcConfigAtten,1) ;
        exportFunction("adcRead",adcRead,1) ;
        exportFunction("readAnalog",adcRead,1) ;

        // for watch
        exportFunction("apiSetHandler",apiSetHandler,0) ;
        exportFunction("apiAddISR",apiAddISR,0) ;
        exportFunction("apiRemoveISR",apiRemoveISR,0) ;
        exportName("watch") ;
        exportName("unwatch") ;
        
        // for PWM
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

        
        JSEngine::fromJSContext(ctx)->addLoopFunction(GPIO::loop, nullptr, true, 0) ;
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

    // GPIO to ADC channel mapping for ESP32/ESP32S3/ESP32S2/ESP32C3

    #define GPIO2ADCCHANNEL(vpin, vchannel, vadc) do { \
        gpio_num_t _pin; \
        vchannel = ADC_CHANNEL_0; \
        vadc = 0; \
        int _found = 0; \
        for (int _c = 0; _c < ADC1_CHANNEL_MAX; ++_c) { \
            if (adc1_pad_get_io_num((adc1_channel_t)_c, &_pin) == ESP_OK && _pin == vpin) { \
                vchannel = (adc_channel_t)_c; \
                vadc = 1; \
                _found = 1; \
                break; \
            } \
        } \
        if (!_found) { \
            GPIO2ADCCHANNEL_ADC2(vpin, vchannel, vadc, _found); \
        } \
        if (!_found) { \
            JSTHROW("pin is not a valid adc pin for this chip"); \
        } \
    } while(0) ;

    #if (SOC_ADC_PERIPH_NUM >= 2)
    #define GPIO2ADCCHANNEL_ADC2(vpin, vchannel, vadc, _found) do { \
        gpio_num_t _pin2; \
        for (int _c2 = 0; _c2 < ADC2_CHANNEL_MAX; ++_c2) { \
            if (adc2_pad_get_io_num((adc2_channel_t)_c2, &_pin2) == ESP_OK && _pin2 == vpin) { \
                vchannel = (adc_channel_t)_c2; \
                vadc = 2; \
                _found = 1; \
                break; \
            } \
        } \
    } while(0) ;
    #else
    #define GPIO2ADCCHANNEL_ADC2(vpin, vchannel, vadc, _found) do {} while(0)
    #endif
    
    /**
     * 配置 ADC 位宽（仅支持 ADC1）
     * @function adcConfigWidth
     * @param adc:number ADC编号（仅支持1）
     * @param bits:number 位宽（9-12）
     * @return undefined 设置失败抛出异常
     */
    JSValue GPIO::adcConfigWidth(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        CHECK_ARGC(2)
        ARGV_TO_UINT8(0, adc)
        ARGV_TO_UINT8(1, bits)
        if(adc!=1) {
            JSTHROW("adcConfigWidth() only supports adc1")
        }
        if(bits<9 || bits>12) {
            JSTHROW("adcConfigWidth() arg bits must be 9-12")
        }
        esp_err_t ret = adc1_config_width((adc_bits_width_t)(bits-9));
        if(ret!=ESP_OK) {
            JSTHROW("adc1_config_width() failed with err: %d", ret )
        }
        return JS_UNDEFINED ;
    }

    /**
     * 配置 ADC 通道衰减
     * @function adcConfigAtten
     * @param pin:number ADC引脚编号
     * @param atten:number 衰减等级（0~3）
     * @return undefined 设置失败抛出异常
     */
    JSValue GPIO::adcConfigAtten(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        CHECK_ARGC(2)
        ARGV_TO_UINT8(0, pin)
        ARGV_TO_UINT8(1, atten)
        if(atten<0 || atten>3) {
            JSTHROW("adcConfigAtten() arg atten must be 0-3")
        }
        adc_channel_t channel;
        uint8_t adc = 1;
        GPIO2ADCCHANNEL(pin, channel, adc)
        esp_err_t ret = ESP_FAIL;
        if(adc==1) {
            ret = adc1_config_channel_atten((adc1_channel_t)channel, (adc_atten_t)atten);
        }
#if (SOC_ADC_PERIPH_NUM >= 2)
        else if(adc==2) {
            ret = adc2_config_channel_atten((adc2_channel_t)channel, (adc_atten_t)atten);
        }
#endif
        if(ret!=ESP_OK) {
            JSTHROW("adc1_config_width() failed with err: %d", ret )
        }
        return JS_UNDEFINED ;
    }

    /**
     * 读取 ADC 原始值
     * @function adcRead
     * @param pin:number ADC引脚编号
     * @return number 读取到的 ADC 原始值
     */
    JSValue GPIO::adcRead(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        CHECK_ARGC(1)
        ARGV_TO_UINT8(0, pin)
        adc_channel_t channel;
        uint8_t adc = 1;
        GPIO2ADCCHANNEL(pin, channel, adc)
        int value = 0;
        esp_err_t ret = ESP_FAIL;
        if(adc==1) {
            value = adc1_get_raw((adc1_channel_t)channel);
            ret = (value >= 0) ? ESP_OK : ESP_FAIL;
        }
#if (SOC_ADC_PERIPH_NUM >= 2)
        else if(adc==2) {
            ret = adc2_get_raw((adc2_channel_t)channel, ADC_WIDTH_BIT_12, &value);
        }
#endif
        if(ret != ESP_OK) {
            JSTHROW("adcRead() failed with err: %d", ret )
        }
        return JS_NewInt32(ctx, value);
    }

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
        uint8_t event = pin | (val<<7) ;
        pending_level_changes.push_back(event) ;
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
    
    JSValue GPIO::apiSetHandler(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        CHECK_ARGC(1)
        if(!JS_IsFunction(ctx, argv[0])) {
            JSTHROW("apiSetHandler() arg callback must be a function")
        }
        JS_FreeValue(ctx, jsHandler) ;
        jsHandler = JS_DupValue(ctx, argv[0]) ;
        return JS_UNDEFINED ;
    }
    JSValue GPIO::apiAddISR(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        CHECK_ARGC(1)
        ARGV_TO_GPIO(0, pin)

        installISR(0) ;
        
        esp_err_t err = gpio_isr_handler_add((gpio_num_t)pin, gpio_isr_handler, (void *)pin) ;
        if(err!=ESP_OK) {
            JSTHROW("gpio_isr_handler_add() failed, err:%d", err)
        }

        err = gpio_set_intr_type((gpio_num_t)pin, GPIO_INTR_ANYEDGE);
        if(err!=ESP_OK) {
            JSTHROW("gpio_set_intr_type() failed, err:%d", err)
        }
        err = gpio_intr_enable((gpio_num_t)pin) ;
        if(err!=ESP_OK) {
            JSTHROW("gpio_intr_enable() failed, err:%d", err)
        }
        return JS_UNDEFINED ;
    }
    JSValue GPIO::apiRemoveISR(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        CHECK_ARGC(1)
        ARGV_TO_GPIO(0, pin)
        esp_err_t err = gpio_isr_handler_remove((gpio_num_t)pin);
        if(err!=ESP_OK) {
            JSTHROW("gpio_isr_handler_remove() failed, err:%d", err)
        }
        return JS_UNDEFINED ;
    }

    void GPIO::loop(JSContext * ctx, void * arg) {

        if( !pending_level_changes.size() ){
            return ;
        }
        if( !JS_IsFunction(ctx, jsHandler) ) {
            return ;
        }

        JSValueConst argv[2] ;

        for(auto event: pending_level_changes) {
            argv[0] = JS_NewInt32(ctx, event & 0x7F) ;      // pin number
            argv[1] = JS_NewInt32(ctx, (event>>7) & 0x01) ; // level
            JSValue ret = JS_Call(ctx, jsHandler, JS_UNDEFINED, 2, argv) ;
            if(JS_IsException(ret)) {
                JSEngine::fromJSContext(ctx)->dumpError() ;
            }
            JS_FreeValue(ctx, ret) ;
        }
        
        pending_level_changes.clear() ;
    }
    
    JSValue GPIO::test(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        return JS_UNDEFINED ;
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
