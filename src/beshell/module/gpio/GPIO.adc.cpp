/**
 * ADC-related implementation split out from GPIO.cpp
 */

#include "GPIO.hpp"
#include <esp_adc/adc_oneshot.h>
/**
 * ADC-related implementation split out from GPIO.cpp
 */

#include "GPIO.hpp"
#include <esp_adc/adc_oneshot.h>
#include <esp_adc/adc_continuous.h>
#include <hal/adc_types.h>
#include <soc/gpio_num.h>
#include <soc/soc_caps.h>
#include <soc/adc_periph.h>
#include <map>
#include <set>
#include <vector>
#include <cstddef>
#include <cstring>
#include "quickjs.h"
#include <JSEngine.hpp>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"

typedef struct  {
    adc_unit_t unit ;
    adc_channel_t channel ;
    bool configured ;
    gpio_num_t pin ;
} adc_channel_info_t ;


namespace be {

    static adc_oneshot_unit_handle_t adc_handles[SOC_ADC_PERIPH_NUM] = {nullptr} ;
    static std::map<gpio_num_t, adc_channel_info_t> map_gpio_adc_info ;
    static std::map<uint16_t, gpio_num_t> map_unit_channel_to_pin ;

    typedef struct {
        uint8_t * data ;
        uint32_t size ;
    } adc_cont_frame_t ;

    typedef struct {
        int id ;
        adc_continuous_handle_t handle ;
        TaskHandle_t task ;
        QueueHandle_t queue ;
        JSValue callback ;
        size_t frame_bytes ;
        uint32_t frame_samples ;
        std::set<gpio_num_t> pin_set ;
        std::map<gpio_num_t, adc_channel_info_t> pin_info ;
        bool running ;
    } adc_cont_handle_ctx ;

    static std::map<int, adc_cont_handle_ctx*> g_adc_cont_handles ;
    static int g_adc_cont_next_id = 1 ;

    static inline uint16_t _make_uc_key(adc_unit_t unit, adc_channel_t channel) {
        return (static_cast<uint16_t>(unit) << 8) | static_cast<uint16_t>(channel) ;
    }

    static adc_channel_info_t* _getChannelInfo(adc_unit_t unit, adc_channel_t channel) {
        for(auto &entry : map_gpio_adc_info) {
            if(entry.second.channel == channel && entry.second.unit == unit) {
                return &entry.second ;
            }
        }
        return nullptr ;
    }

    static void _setChannelConfigured(adc_channel_t channel, bool configured) {
        for(auto &entry : map_gpio_adc_info) {
            if(entry.second.channel == channel) {
                entry.second.configured = configured ;
            }
        }
    }

    static size_t _adcUnitIndex(adc_unit_t unit_id) {
        switch(unit_id) {
            case ADC_UNIT_1:
                return 0 ;
        #if (SOC_ADC_PERIPH_NUM >= 2)
            case ADC_UNIT_2:
                return 1 ;
        #endif
            default:
                return SIZE_MAX ;
        }
    }

    static size_t _gpioToIndex(gpio_num_t pin) {
        int pin_int = static_cast<int>(pin) ;
        if(pin_int < 0 || pin_int >= GPIO_NUM_MAX) {
            return SIZE_MAX ;
        }
        return static_cast<size_t>(pin_int) ;
    }

    /**
     * Initialize pin -> adc reflection. This was previously inside the GPIO
     * constructor; moved here so ADC data lives with ADC implementation.
     */
    void GPIO::adcReflectPins() {
        map_gpio_adc_info.clear() ;
        map_unit_channel_to_pin.clear() ;
        // 直接遍历 adc_channel_io_map 表，只包含有效的 ADC 引脚，避免对非 ADC GPIO 调用 API 产生错误日志
        for(int unit = 0; unit < SOC_ADC_PERIPH_NUM; ++unit) {
            for(int ch = 0; ch < SOC_ADC_MAX_CHANNEL_NUM; ++ch) {
                int gpio = adc_channel_io_map[unit][ch] ;
                if(gpio < 0) continue ;  // 跳过不支持的通道

                adc_channel_info_t info ;
                info.unit = (adc_unit_t)unit ;
                info.channel = (adc_channel_t)ch ;
                info.configured = false ;
                info.pin = (gpio_num_t)gpio ;

                map_gpio_adc_info[(gpio_num_t)gpio] = info ;
                map_unit_channel_to_pin[_make_uc_key((adc_unit_t)unit, (adc_channel_t)ch)] = (gpio_num_t)gpio ;
            }
        }
    }

    static esp_err_t _initADCUnit(adc_unit_t unit_id) {
        size_t idx = _adcUnitIndex(unit_id) ;
        if(idx == SIZE_MAX) {
            return ESP_ERR_INVALID_ARG ;
        }

        if(adc_handles[idx] != NULL) {
            return ESP_ERR_INVALID_STATE ;
        }

        adc_oneshot_unit_init_cfg_t init_config = {
            .unit_id = unit_id,
#if CONFIG_IDF_TARGET_ESP32C6 || CONFIG_IDF_TARGET_ESP32H2
            .clk_src = ADC_DIGI_CLK_SRC_DEFAULT,
#else
            .clk_src = ADC_RTC_CLK_SRC_DEFAULT,
#endif
            .ulp_mode = ADC_ULP_MODE_DISABLE,
        } ;

        return adc_oneshot_new_unit(&init_config, &adc_handles[idx]) ;
    }

    static int _initADCChannel(adc_channel_info_t &channel_info, adc_atten_t atten, adc_bitwidth_t bitwidth) {

        esp_err_t err ;
        adc_unit_t uint_num = channel_info.unit ;
        adc_channel_t channel = channel_info.channel ;
        size_t idx = _adcUnitIndex(uint_num) ;
        if(idx == SIZE_MAX) {
            return ESP_ERR_INVALID_ARG ;
        }

        if(adc_handles[idx] == NULL) {
            err = _initADCUnit(uint_num) ;
            if(err!=ESP_OK && err!=ESP_ERR_INVALID_STATE) {
                return err ;
            }
        }

        adc_oneshot_chan_cfg_t config = {
            .atten = atten,
            .bitwidth = bitwidth,
        } ;
        
        err = adc_oneshot_config_channel(adc_handles[idx], channel, &config) ;
        if(err!=ESP_OK && err!=ESP_ERR_INVALID_STATE) {
            return err ;
        }

        _setChannelConfigured(channel, true) ;

        return ESP_OK ;
    }

    static bool _adcChannelFromPin(JSContext *ctx, gpio_num_t pin, adc_channel_t &channel_out, adc_unit_t &unit_out) {
        size_t pin_index = _gpioToIndex(pin) ;
        if(pin_index == SIZE_MAX) {
            JS_ThrowReferenceError(ctx, "pin is not a valid adc pin.") ;
            return false ;
        }

        if(map_gpio_adc_info.count(pin) == 0) {
            JS_ThrowReferenceError(ctx, "pin is not a valid adc pin.") ;
            return false ;
        }

        auto &info = map_gpio_adc_info.at(pin) ;
        channel_out = info.channel ;
        unit_out = info.unit ;
        return true ;
    }

    /**
     * 初始化指定的 ADC 单元。
     *
     * @module gpio
     * @object adc
     * @function adcUnitInit
     *
     * @param unit:number ADC 单元号，支持 1 或 2（取决于芯片能力）
     *
     * @return undefined
     */
    JSValue GPIO::adcUnitInit(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        CHECK_ARGC(1)
        uint32_t unit_num = 1 ;
        if( JS_ToUint32(ctx, &unit_num, argv[0])!=0 ) {
            JSTHROW("Invalid param type")
        }
        if(unit_num<1 || unit_num>2) {
            JSTHROW("Invalid param value")
        }
        adc_unit_t unit = unit_num == 1 ? ADC_UNIT_1 : ADC_UNIT_2 ;
        esp_err_t err = _initADCUnit(unit) ;
        if(err==ESP_ERR_INVALID_STATE) {
            JSTHROW("unit already inited")
        }
        if(err!=ESP_OK) {
            JSTHROW("init adc unit failed, err:%d", err)
        }
        return JS_UNDEFINED ;
    }

    static JSValue adc_channel_init_helper(JSContext *ctx, adc_unit_t unit, adc_channel_t channel, int argc, JSValueConst *argv) {
        adc_channel_info_t *info = _getChannelInfo(unit, channel) ;
        if(info == nullptr) {
            JSTHROW("invalid adc channel configuration")
        }

        size_t idx = _adcUnitIndex(unit) ;
        if(idx == SIZE_MAX) {
            JSTHROW("invalid adc unit")
        }

        if(adc_handles[idx] == NULL) {
            esp_err_t err = _initADCUnit(unit) ;
            if(err!=ESP_OK && err!=ESP_ERR_INVALID_STATE) {
                JSTHROW("init adc unit failed, err:%d", err)
            }
        }

        adc_atten_t atten = ADC_ATTEN_DB_12 ;
        adc_bitwidth_t bitwidth = ADC_BITWIDTH_DEFAULT ;

        if(argc >= 2) {
            uint32_t atten_val = 0 ;
            if(JS_ToUint32(ctx, &atten_val, argv[1])!=0) {
                JSTHROW("Invalid atten value")
            }
            atten = static_cast<adc_atten_t>(atten_val) ;
        }

        if(argc >= 3) {
            uint32_t bitwidth_val = 0 ;
            if(JS_ToUint32(ctx, &bitwidth_val, argv[2])!=0) {
                JSTHROW("Invalid bitwidth value")
            }
            bitwidth = static_cast<adc_bitwidth_t>(bitwidth_val) ;
        }
        
        esp_err_t err = _initADCChannel(*info, atten, bitwidth) ;
        if(err!=ESP_OK) {
            JSTHROW("init adc channel failed, err:%d", err)
        }

        return JS_UNDEFINED ;
    }


    /**
     * 通过MCU引脚号初始化 ADC 通道。
     * 
     * @module gpio
     * @object adc
     * @function adcPinInit
     *
     * @param pin:number 引脚序号
     * @param atten:number=3 ADC 衰减值, 默认值 ADC_ATTEN_DB_12
     * @param bitwidth:number=0 ADC 位宽, 默认值 ADC_BITWIDTH_DEFAULT
     * 
     * @return undefined
     */
    JSValue GPIO::adcPinInit(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        CHECK_ARGC(1)
        ARGV_TO_GPIO(0, pin)

        adc_channel_t channel ;
        adc_unit_t unit ;
        if(!_adcChannelFromPin(ctx, pin, channel, unit)) {
            return JS_EXCEPTION ;
        }
        
        return adc_channel_init_helper(ctx, unit, channel, argc, argv) ;
    }

    /**
     * 初始化指定 ADC 通道。
     * 
     * @module gpio
     * @object adc
     * @function adcChannelInit
     *
     * @param channel:number adc 通道号
     * @param atten:number=3 ADC 衰减值, 默认值 ADC_ATTEN_DB_12
     * @param bitwidth:number=0 ADC 位宽, 默认值 ADC_BITWIDTH_DEFAULT
     * @param unit:number=1 ADC 单元号, 默认单元1 ，部分 esp32 型号支持单元2
     * 
     * @return undefined
     */
    JSValue GPIO::adcChannelInit(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        CHECK_ARGC(1)
        ARGV_TO_INT(0, channel, adc_channel_t, uint32_t, JS_ToUint32)
        uint32_t unit_num = 1 ;
        if(argc >= 4 && !JS_IsUndefined(argv[3])) {
            if(JS_ToUint32(ctx, &unit_num, argv[3])!=0) {
                JSTHROW("Invalid unit value")
            }
        }
        if(unit_num < 1
#if (SOC_ADC_PERIPH_NUM >= 2)
            || unit_num > SOC_ADC_PERIPH_NUM
#endif
        ) {
            JSTHROW("Invalid unit value")
        }
        adc_unit_t unit = ADC_UNIT_1 ;
#if (SOC_ADC_PERIPH_NUM >= 2)
        if(unit_num == 2) {
            unit = ADC_UNIT_2 ;
        }
#endif
        return adc_channel_init_helper(ctx, unit, channel, argc, argv) ;
    }

    static JSValue adc_channel_read_helper(JSContext *ctx, adc_channel_t channel, adc_unit_t uint_num) {
        adc_channel_info_t *info = _getChannelInfo(uint_num, channel) ;
        if(info == nullptr) {
            JSTHROW("invalid adc channel configuration")
        }

        if(!info->configured) {
            esp_err_t err = _initADCChannel(*info, ADC_ATTEN_DB_11, ADC_BITWIDTH_DEFAULT) ;
            if(err!=ESP_OK) {
                JSTHROW("init adc channel failed, err:%d", err)
            }
        }

        int value = 0 ;

        size_t idx = _adcUnitIndex(uint_num) ;
        if(idx == SIZE_MAX) {
            JSTHROW("invalid adc unit")
        }

        esp_err_t err = adc_oneshot_read(adc_handles[idx], (adc_channel_t)channel, &value) ;
        if(err!=ESP_OK) {
            JSTHROW("read adc failed, err:%d", err)
        }

        return JS_NewInt32(ctx, value) ;
    }
    
    /**
     * 读取指定 ADC 通道的值。
     * 
     * @module gpio
     * @object adc
     * @function adcChannelRead
     * @param channel:number 通道号
     * @param unit:number=1 ADC 单元号, 默认单元1 ，部分 esp32 型号支持单元2
     * 
     * @return number ADC 通道的读取值
     */
    JSValue GPIO::adcChannelRead(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        CHECK_ARGC(1)
        ARGV_TO_INT(0, channel, adc_channel_t, uint32_t, JS_ToUint32)
        uint32_t unit_num = 1 ;
        if(argc >= 2 && !JS_IsUndefined(argv[1])) {
            if(JS_ToUint32(ctx, &unit_num, argv[1])!=0) {
                JSTHROW("Invalid unit value")
            }
        }
        if(unit_num < 1
#if (SOC_ADC_PERIPH_NUM >= 2)
            || unit_num > SOC_ADC_PERIPH_NUM
#endif
        ) {
            JSTHROW("Invalid unit value")
        }
        adc_unit_t unit = ADC_UNIT_1 ;
#if (SOC_ADC_PERIPH_NUM >= 2)
        if(unit_num == 2) {
            unit = ADC_UNIT_2 ;
        }
#endif
        return adc_channel_read_helper(ctx, channel, unit) ;
    }

    /**
     * 通过MCU引脚号读取 ADC 通道的值。
     * 
     * @module gpio
     * @object adc
     * @function adcRead
     * @param pin:number 引脚号
     * 
     * @return number ADC 通道的读取值
     */
    JSValue GPIO::adcRead(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        CHECK_ARGC(1)
        ARGV_TO_GPIO(0, pin)

        adc_channel_t channel ;
        adc_unit_t unit ;
        if(!_adcChannelFromPin(ctx, pin, channel, unit)) {
            return JS_EXCEPTION ;
        }
        return adc_channel_read_helper(ctx, channel, unit) ;
    }

    /**
     * 获取当前 MCU 引脚与 ADC 通道的映射信息。
     *
     * @module gpio
     * @object adc
     * @function adcInfo
     *
     * @return object 以引脚号为键的对象，每项包含 channel 与 unit 字段
     */
    JSValue GPIO::adcInfo(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        JSValue obj = JS_NewObject(ctx) ;
        for(const auto &entry : map_gpio_adc_info) {
            gpio_num_t gpio = entry.first ;
            const adc_channel_info_t &info = entry.second ;
            adc_channel_t channel = info.channel ;
            JSValue item = JS_NewObject(ctx) ;
            JS_SetPropertyStr(ctx, item, "channel", JS_NewInt32(ctx, static_cast<int>(channel))) ;
            int unit_display = 0 ;
            switch(info.unit) {
            case ADC_UNIT_1:
                unit_display = 1 ;
                break ;
#if (SOC_ADC_PERIPH_NUM >= 2)
            case ADC_UNIT_2:
                unit_display = 2 ;
                break ;
#endif
            default:
                unit_display = -1 ;
                break ;
            }
            JS_SetPropertyStr(ctx, item, "unit", JS_NewInt32(ctx, unit_display)) ;
            JS_SetPropertyUint32(ctx, obj, static_cast<uint32_t>(gpio), item) ;
        }
        return obj ;
    }

    static void adc_cont_free_frame(adc_cont_frame_t &frame) {
        if(frame.data) {
            free(frame.data) ;
            frame.data = nullptr ;
        }
        frame.size = 0 ;
    }

    static void adc_cont_task(void * arg) {
        adc_cont_handle_ctx * ctx = (adc_cont_handle_ctx *)arg ;
        uint8_t * buffer = (uint8_t *)malloc(ctx->frame_bytes) ;
        if(!buffer) {
            ctx->running = false ;
            vTaskDelete(NULL) ;
            return ;
        }
        while(ctx->running) {
            uint32_t out_len = 0 ;
            esp_err_t err = adc_continuous_read(ctx->handle, buffer, ctx->frame_bytes, &out_len, 100) ;
            if(err == ESP_OK && out_len > 0) {
                adc_cont_frame_t frame ;
                frame.size = out_len ;
                frame.data = (uint8_t *)malloc(out_len) ;
                if(frame.data) {
                    memcpy(frame.data, buffer, out_len) ;
                    if(xQueueSend(ctx->queue, &frame, 0) != pdTRUE) {
                        adc_cont_free_frame(frame) ;
                    }
                }
            } else if(err == ESP_ERR_TIMEOUT) {
                continue ;
            } else {
                vTaskDelay(1) ;
            }
        }
        free(buffer) ;
        vTaskDelete(NULL) ;
    }

    static bool adc_cont_collect_values(adc_cont_handle_ctx * ctx, const uint8_t * data, uint32_t size, gpio_num_t target_pin, bool filter_pin, adc_unit_t target_unit, adc_channel_t target_channel, std::vector<int> &out_values) {
        if(size == 0) {
            return true ;
        }
        uint32_t max_samples = size / SOC_ADC_DIGI_RESULT_BYTES ;
        if(max_samples == 0) {
            return true ;
        }
        std::vector<adc_continuous_data_t> parsed(max_samples) ;
        uint32_t num_samples = 0 ;
        esp_err_t err = adc_continuous_parse_data(ctx->handle, data, size, parsed.data(), &num_samples) ;
        if(err != ESP_OK) {
            return false ;
        }
        for(uint32_t i = 0; i < num_samples; ++i) {
            const adc_continuous_data_t &sample = parsed[i] ;
            if(!sample.valid) {
                continue ;
            }
            if(filter_pin) {
                uint16_t key = _make_uc_key(sample.unit, sample.channel) ;
                auto it = map_unit_channel_to_pin.find(key) ;
                if(it == map_unit_channel_to_pin.end()) {
                    continue ;
                }
                if(it->second != target_pin) {
                    continue ;
                }
            } else {
                if(sample.unit != target_unit || sample.channel != target_channel) {
                    continue ;
                }
            }
            out_values.push_back(static_cast<int>(sample.raw_data)) ;
        }
        return true ;
    }

    static void adc_cont_dispatch_callback(JSContext * ctx, adc_cont_handle_ctx * handle_ctx, const adc_cont_frame_t &frame) {
        if(!JS_IsFunction(ctx, handle_ctx->callback)) {
            return ;
        }
        if(frame.size == 0) {
            return ;
        }
        uint32_t max_samples = frame.size / SOC_ADC_DIGI_RESULT_BYTES ;
        if(max_samples == 0) {
            return ;
        }
        std::vector<adc_continuous_data_t> parsed(max_samples) ;
        uint32_t num_samples = 0 ;
        esp_err_t err = adc_continuous_parse_data(handle_ctx->handle, frame.data, frame.size, parsed.data(), &num_samples) ;
        if(err != ESP_OK) {
            return ;
        }
        std::map<gpio_num_t, std::vector<int>> pin_values ;
        for(uint32_t i = 0; i < num_samples; ++i) {
            const adc_continuous_data_t &sample = parsed[i] ;
            if(!sample.valid) {
                continue ;
            }
            uint16_t key = _make_uc_key(sample.unit, sample.channel) ;
            auto it = map_unit_channel_to_pin.find(key) ;
            if(it == map_unit_channel_to_pin.end()) {
                continue ;
            }
            gpio_num_t pin = it->second ;
            if(handle_ctx->pin_set.count(pin) == 0) {
                continue ;
            }
            pin_values[pin].push_back(static_cast<int>(sample.raw_data)) ;
        }
        for(auto &entry : pin_values) {
            gpio_num_t pin = entry.first ;
            auto &values = entry.second ;
            JSValue js_arr = JS_NewArray(ctx) ;
            for(size_t i = 0; i < values.size(); ++i) {
                JS_SetPropertyUint32(ctx, js_arr, static_cast<uint32_t>(i), JS_NewInt32(ctx, values[i])) ;
            }
            JSValue argv[2] = { JS_NewInt32(ctx, static_cast<int>(pin)), js_arr } ;
            JSValue ret = JS_Call(ctx, handle_ctx->callback, JS_UNDEFINED, 2, argv) ;
            if(JS_IsException(ret)) {
                JSEngine::fromJSContext(ctx)->dumpError() ;
            }
            JS_FreeValue(ctx, ret) ;
            JS_FreeValue(ctx, argv[0]) ;
            JS_FreeValue(ctx, js_arr) ;
        }
    }

    void GPIO::adcContinuousLoop(JSContext * ctx, void * arg) {
        for(auto &entry : g_adc_cont_handles) {
            adc_cont_handle_ctx * handle_ctx = entry.second ;
            if(!handle_ctx || !JS_IsFunction(ctx, handle_ctx->callback)) {
                continue ;
            }
            adc_cont_frame_t frame ;
            if(xQueueReceive(handle_ctx->queue, &frame, 0) == pdTRUE) {
                adc_cont_dispatch_callback(ctx, handle_ctx, frame) ;
                adc_cont_free_frame(frame) ;
            }
        }
    }

    static bool adc_cont_parse_pins(JSContext * ctx, JSValue pins_val, std::vector<gpio_num_t> &pins_out) {
        if(!JS_IsArray(ctx, pins_val)) {
            JS_ThrowReferenceError(ctx, "pins must be an array") ;
            return false ;
        }
        JSValue len_val = JS_GetPropertyStr(ctx, pins_val, "length") ;
        uint32_t len = 0 ;
        if(JS_ToUint32(ctx, &len, len_val) != 0) {
            JS_FreeValue(ctx, len_val) ;
            JS_ThrowReferenceError(ctx, "invalid pins length") ;
            return false ;
        }
        JS_FreeValue(ctx, len_val) ;
        for(uint32_t i = 0; i < len; ++i) {
            JSValue item = JS_GetPropertyUint32(ctx, pins_val, i) ;
            uint32_t pin_num = 0 ;
            if(JS_ToUint32(ctx, &pin_num, item) != 0) {
                JS_FreeValue(ctx, item) ;
                JS_ThrowReferenceError(ctx, "invalid pin value") ;
                return false ;
            }
            JS_FreeValue(ctx, item) ;
            if(pin_num >= GPIO_NUM_MAX) {
                JS_ThrowReferenceError(ctx, "Invalid GPIO pin") ;
                return false ;
            }
            pins_out.push_back(static_cast<gpio_num_t>(pin_num)) ;
        }
        return true ;
    }

    JSValue GPIO::adcContinuousStart(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        CHECK_ARGC(1)
        if(!JS_IsObject(argv[0])) {
            JSTHROW("options must be an object")
        }
        JSValue pins_val = JS_GetPropertyStr(ctx, argv[0], "pins") ;
        if(JS_IsUndefined(pins_val)) {
            JS_FreeValue(ctx, pins_val) ;
            JSTHROW("options.pins is required")
        }

        std::vector<gpio_num_t> pins ;
        if(!adc_cont_parse_pins(ctx, pins_val, pins)) {
            JS_FreeValue(ctx, pins_val) ;
            return JS_EXCEPTION ;
        }
        JS_FreeValue(ctx, pins_val) ;
        if(pins.empty()) {
            JSTHROW("pins is empty")
        }

        uint32_t sample_hz = 0 ;
        JSValue sample_val = JS_GetPropertyStr(ctx, argv[0], "sample_hz") ;
        if(JS_IsUndefined(sample_val) || JS_ToUint32(ctx, &sample_hz, sample_val)!=0 || sample_hz == 0) {
            JS_FreeValue(ctx, sample_val) ;
            JSTHROW("options.sample_hz is required")
        }
        JS_FreeValue(ctx, sample_val) ;

        uint32_t frame_samples = 256 ;
        GET_UINT32_PROP_OPT(argv[0], "frame_samples", frame_samples, 256)

        uint32_t atten_val = ADC_ATTEN_DB_12 ;
        GET_UINT32_PROP_OPT(argv[0], "atten", atten_val, ADC_ATTEN_DB_12)
        adc_atten_t atten = static_cast<adc_atten_t>(atten_val) ;

        uint32_t bitwidth_val = ADC_BITWIDTH_DEFAULT ;
        GET_UINT32_PROP_OPT(argv[0], "bitwidth", bitwidth_val, ADC_BITWIDTH_DEFAULT)
        adc_bitwidth_t bitwidth = static_cast<adc_bitwidth_t>(bitwidth_val) ;

        std::vector<adc_digi_pattern_config_t> pattern ;
        pattern.reserve(pins.size()) ;
        std::map<gpio_num_t, adc_channel_info_t> pin_info ;
        std::set<gpio_num_t> pin_set ;
        bool has_unit1 = false ;
        bool has_unit2 = false ;
        for(auto pin : pins) {
            adc_unit_t unit ;
            adc_channel_t channel ;
            if(adc_oneshot_io_to_channel(pin, &unit, &channel) != ESP_OK) {
                JSTHROW("pin is not a valid adc pin")
            }
            adc_digi_pattern_config_t cfg = {
                .atten = atten,
                .channel = static_cast<uint8_t>(channel),
                .unit = static_cast<uint8_t>(unit),
                .bit_width = bitwidth,
            } ;
            pattern.push_back(cfg) ;

            adc_channel_info_t info ;
            info.unit = unit ;
            info.channel = channel ;
            info.configured = true ;
            info.pin = pin ;
            pin_info[pin] = info ;
            pin_set.insert(pin) ;
            if(unit == ADC_UNIT_1) {
                has_unit1 = true ;
            } else {
                has_unit2 = true ;
            }
        }

        size_t frame_bytes = static_cast<size_t>(frame_samples) * SOC_ADC_DIGI_DATA_BYTES_PER_CONV ;
        adc_continuous_handle_cfg_t handle_cfg = {
            .max_store_buf_size = static_cast<uint32_t>(frame_bytes * 4),
            .conv_frame_size = static_cast<uint32_t>(frame_bytes),
        } ;

        adc_continuous_handle_t handle = nullptr ;
        esp_err_t err = adc_continuous_new_handle(&handle_cfg, &handle) ;
        if(err != ESP_OK) {
            JSTHROW("adc continuous new handle failed, err:%d", err)
        }

        adc_digi_convert_mode_t conv_mode = ADC_CONV_SINGLE_UNIT_1 ;
#if (SOC_ADC_PERIPH_NUM >= 2)
        if(has_unit1 && has_unit2) {
            conv_mode = ADC_CONV_BOTH_UNIT ;
        } else if(has_unit2) {
            conv_mode = ADC_CONV_SINGLE_UNIT_2 ;
        }
#endif
#if SOC_ADC_DIGI_RESULT_BYTES == 4
        adc_digi_output_format_t out_format = ADC_DIGI_OUTPUT_FORMAT_TYPE2 ;
#else
        adc_digi_output_format_t out_format = ADC_DIGI_OUTPUT_FORMAT_TYPE1 ;
#endif

        adc_continuous_config_t cfg = {
            .pattern_num = static_cast<uint32_t>(pattern.size()),
            .adc_pattern = pattern.data(),
            .sample_freq_hz = sample_hz,
            .conv_mode = conv_mode,
            .format = out_format,
        } ;

        err = adc_continuous_config(handle, &cfg) ;
        if(err != ESP_OK) {
            adc_continuous_deinit(handle) ;
            JSTHROW("adc continuous config failed, err:%d", err)
        }

        err = adc_continuous_start(handle) ;
        if(err != ESP_OK) {
            adc_continuous_deinit(handle) ;
            JSTHROW("adc continuous start failed, err:%d", err)
        }

        adc_cont_handle_ctx * handle_ctx = new adc_cont_handle_ctx() ;
        handle_ctx->id = g_adc_cont_next_id++ ;
        handle_ctx->handle = handle ;
        handle_ctx->task = nullptr ;
        handle_ctx->queue = xQueueCreate(4, sizeof(adc_cont_frame_t)) ;
        handle_ctx->callback = JS_NULL ;
        handle_ctx->frame_bytes = frame_bytes ;
        handle_ctx->frame_samples = frame_samples ;
        handle_ctx->pin_set = pin_set ;
        handle_ctx->pin_info = pin_info ;
        handle_ctx->running = true ;

        if(handle_ctx->queue == nullptr) {
            adc_continuous_stop(handle) ;
            adc_continuous_deinit(handle) ;
            delete handle_ctx ;
            JSTHROW("failed to create adc continuous queue")
        }

        if(argc >= 2 && !JS_IsUndefined(argv[1]) && !JS_IsNull(argv[1])) {
            if(!JS_IsFunction(ctx, argv[1])) {
                adc_continuous_stop(handle) ;
                adc_continuous_deinit(handle) ;
                vQueueDelete(handle_ctx->queue) ;
                delete handle_ctx ;
                JSTHROW("callback must be a function")
            }
            handle_ctx->callback = JS_DupValue(ctx, argv[1]) ;
        }

        BaseType_t task_res = xTaskCreatePinnedToCore(adc_cont_task, "adc_cont", 4096, handle_ctx, 5, &handle_ctx->task, 1) ;
        if(task_res != pdPASS) {
            if(!JS_IsNull(handle_ctx->callback)) {
                JS_FreeValue(ctx, handle_ctx->callback) ;
            }
            adc_continuous_stop(handle) ;
            adc_continuous_deinit(handle) ;
            vQueueDelete(handle_ctx->queue) ;
            delete handle_ctx ;
            JSTHROW("failed to create adc continuous task")
        }

        g_adc_cont_handles[handle_ctx->id] = handle_ctx ;

        return JS_NewInt32(ctx, handle_ctx->id) ;
    }

    JSValue GPIO::adcContinuousRead(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        CHECK_ARGC(2)
        ARGV_TO_INT(0, handle_id, int, int32_t, JS_ToInt32)
        if(g_adc_cont_handles.count(handle_id) == 0) {
            JSTHROW("invalid adc continuous handle")
        }
        adc_cont_handle_ctx * handle_ctx = g_adc_cont_handles[handle_id] ;

        bool use_pin = true ;
        gpio_num_t pin = GPIO_NUM_NC ;
        adc_unit_t unit = ADC_UNIT_1 ;
        adc_channel_t channel = ADC_CHANNEL_0 ;

        if(argc >= 3 && !JS_IsUndefined(argv[2])) {
            use_pin = false ;
            ARGV_TO_INT(1, channel_arg, adc_channel_t, uint32_t, JS_ToUint32)
            ARGV_TO_INT(2, unit_arg, uint32_t, uint32_t, JS_ToUint32)
            channel = channel_arg ;
            if(unit_arg == 1) {
                unit = ADC_UNIT_1 ;
            }
#if (SOC_ADC_PERIPH_NUM >= 2)
            else if(unit_arg == 2) {
                unit = ADC_UNIT_2 ;
            }
#endif
            else {
                JSTHROW("Invalid unit value")
            }
        } else {
            ARGV_TO_GPIO(1, pin_arg)
            pin = pin_arg ;
            if(handle_ctx->pin_set.count(pin) == 0) {
                JSTHROW("pin not configured for this handle")
            }
        }

        if(!use_pin) {
            uint16_t key = _make_uc_key(unit, channel) ;
            auto it = map_unit_channel_to_pin.find(key) ;
            if(it == map_unit_channel_to_pin.end() || handle_ctx->pin_set.count(it->second) == 0) {
                JSTHROW("channel not configured for this handle")
            }
        }

        adc_cont_frame_t frame ;
        if(xQueueReceive(handle_ctx->queue, &frame, 0) != pdTRUE) {
            return JS_NewArray(ctx) ;
        }

        std::vector<int> values ;
        bool ok = adc_cont_collect_values(handle_ctx, frame.data, frame.size, pin, use_pin, unit, channel, values) ;
        adc_cont_free_frame(frame) ;
        if(!ok) {
            JSTHROW("parse adc continuous data failed")
        }

        JSValue arr = JS_NewArray(ctx) ;
        for(size_t i = 0; i < values.size(); ++i) {
            JS_SetPropertyUint32(ctx, arr, static_cast<uint32_t>(i), JS_NewInt32(ctx, values[i])) ;
        }
        return arr ;
    }

    JSValue GPIO::adcContinuousSetCallback(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        CHECK_ARGC(2)
        ARGV_TO_INT(0, handle_id, int, int32_t, JS_ToInt32)
        if(g_adc_cont_handles.count(handle_id) == 0) {
            JSTHROW("invalid adc continuous handle")
        }
        adc_cont_handle_ctx * handle_ctx = g_adc_cont_handles[handle_id] ;

        if(JS_IsNull(argv[1]) || JS_IsUndefined(argv[1])) {
            if(!JS_IsNull(handle_ctx->callback)) {
                JS_FreeValue(ctx, handle_ctx->callback) ;
                handle_ctx->callback = JS_NULL ;
            }
            return JS_UNDEFINED ;
        }
        if(!JS_IsFunction(ctx, argv[1])) {
            JSTHROW("callback must be a function or null")
        }
        if(!JS_IsNull(handle_ctx->callback)) {
            JS_FreeValue(ctx, handle_ctx->callback) ;
        }
        handle_ctx->callback = JS_DupValue(ctx, argv[1]) ;
        return JS_UNDEFINED ;
    }

    JSValue GPIO::adcContinuousStop(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        CHECK_ARGC(1)
        ARGV_TO_INT(0, handle_id, int, int32_t, JS_ToInt32)
        if(g_adc_cont_handles.count(handle_id) == 0) {
            JSTHROW("invalid adc continuous handle")
        }
        adc_cont_handle_ctx * handle_ctx = g_adc_cont_handles[handle_id] ;
        g_adc_cont_handles.erase(handle_id) ;

        handle_ctx->running = false ;
        if(handle_ctx->task) {
            vTaskDelete(handle_ctx->task) ;
            handle_ctx->task = nullptr ;
        }

        if(handle_ctx->queue) {
            adc_cont_frame_t frame ;
            while(xQueueReceive(handle_ctx->queue, &frame, 0) == pdTRUE) {
                adc_cont_free_frame(frame) ;
            }
            vQueueDelete(handle_ctx->queue) ;
            handle_ctx->queue = nullptr ;
        }

        if(!JS_IsNull(handle_ctx->callback)) {
            JS_FreeValue(ctx, handle_ctx->callback) ;
            handle_ctx->callback = JS_NULL ;
        }

        if(handle_ctx->handle) {
            adc_continuous_stop(handle_ctx->handle) ;
            adc_continuous_deinit(handle_ctx->handle) ;
            handle_ctx->handle = nullptr ;
        }

        delete handle_ctx ;
        return JS_UNDEFINED ;
    }

}
