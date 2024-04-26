#include "GPIOModule.hpp"
#include "driver/adc.h"
#include <map>
#include <vector>

using namespace std ;

namespace be {
    
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

    GPIOModule::GPIOModule(JSContext * ctx, const char * name)
        : NativeModule(ctx, name, 0)
    {
        exportFunction("setMode",setMode,2) ;
        exportFunction("pull",pull,2) ;
        exportFunction("write",write,2) ;
        exportFunction("read",read,1) ;
        exportFunction("adcSetBits",adcSetBits,2) ;
        exportFunction("adcSetChannelAtten",adcSetChannelAtten,2) ;
        exportFunction("readAnalog",readAnalog,0) ;
        exportFunction("writeAnalog",writeAnalog,0) ;
        exportFunction("writePWM",writePWM,0) ;
        exportFunction("readPWM",readPWM,0) ;
        exportFunction("watch",watch,0) ;
        exportFunction("unwatch",unwatch,0) ;
        exportName("blink") ;
        EXPORT_FUNCTION(test) ;
    }

    void GPIOModule::import(JSContext *ctx) {
        JSValue DEF_JS_FUNC(jsBlink, R"(
function (gpio,time) {
    this.setMode(gpio,"output")
    return setInterval(() => {
        this.write(gpio,this.read(gpio)?0:1)
    }, time||1000)
}
    )", "gpio.js", )
        exportValue("blink", jsBlink) ;
    }

    JSValue GPIOModule::setMode(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
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
     * @beapi gpio.pinPull
     * @param pin:number mcu可用的gpio编号
     * @param pullMode:string
     * @return undefined
     */
    JSValue GPIOModule::pull(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
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
    JSValue GPIOModule::write(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        ASSERT_ARGC(2)
        ARGV_TO_UINT8(0, pin)
        ARGV_TO_UINT8(1, value)
        return gpio_set_level((gpio_num_t)pin, value)==ESP_OK? JS_TRUE: JS_FALSE ;
    }
    JSValue GPIOModule::read(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        ASSERT_ARGC(1)
        ARGV_TO_UINT8(0, pin)
        gpio_get_level((gpio_num_t)pin);
        return JS_NewUint32(ctx, gpio_get_level((gpio_num_t)pin)) ;
    }
    JSValue GPIOModule::readAnalog(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        return JS_UNDEFINED ;
    }
    JSValue GPIOModule::writeAnalog(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        return JS_UNDEFINED ;
    }
    
    JSValue GPIOModule::adcSetBits(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        ASSERT_ARGC(2)
        ARGV_TO_UINT8(0, adc)
        ARGV_TO_UINT8(1, bits)
        if(adc!=1) {
            JSTHROW("adcConfigBits() only set adc1")
        }
        if( bits<9 || bits>12 ) {
            JSTHROW("adcConfigBits() arg bits must be 9-12")
        }
        return (adc1_config_width((adc_bits_width_t)(bits-9)) == ESP_OK)? JS_TRUE: JS_FALSE ;
    }

    
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
    JSValue GPIOModule::adcSetChannelAtten(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {

        ASSERT_ARGC(2)
        ARGV_TO_UINT8(0, pin)
        ARGV_TO_UINT8(1, atten)

        if( atten<0 || atten>3 ) {
            JSTHROW("setChannelAtten() arg atten must be 0-3")
        }

        GPIO2ADCCHANNEL(pin, channel, adc)

        esp_err_t ret = ESP_FAIL ;
        if(adc==1) {
            ret = adc1_config_channel_atten((adc1_channel_t)channel, (adc_atten_t)atten) ;
        }
        else if(adc==2) {
            ret = adc2_config_channel_atten((adc2_channel_t)channel, (adc_atten_t)atten) ;
        }
        
        return (ret==ESP_OK)? JS_TRUE: JS_FALSE ;
    }

    JSValue GPIOModule::writePWM(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        return JS_UNDEFINED ;
    }
    JSValue GPIOModule::readPWM(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        return JS_UNDEFINED ;
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

    JSValue GPIOModule::watch(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        CHECK_ARGC(3)
        ARGV_TO_UINT8(0, pin)
        string ARGV_TO_STRING(1, edge)
        if(!JS_IsFunction(ctx, argv[2])) {
            JSTHROW("watch() arg callback must be a function")
        }

        int mode = 0 ;
        if(edge=="rasing") {
            mode = 1 ;
        } else if(edge=="falling") {
            mode = 2 ;
        } else if(edge=="both") {
            mode = 3 ;
        } else {
            JSTHROW("watch() arg edge must be rasing|falling|both")
        }

        esp_err_t ret = gpio_install_isr_service(0);
        dn(ret)
        dn4(ESP_ERR_NO_MEM,ESP_ERR_INVALID_ARG,ESP_ERR_NOT_SUPPORTED,ESP_ERR_INVALID_STATE)

        gpio_isr_handler_remove((gpio_num_t)pin);

        //gpio_state[pin] &= (~12) ;
        gpio_state[pin] |= mode << 2 ;
        mode = gpio_state[pin] >> 2 & 3 ;
        dn(mode)

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

        JSEngine::fromJSContext(ctx)->addLoopFunction(GPIOModule::loop, nullptr, true) ;

        return JS_UNDEFINED ;
    }
    JSValue GPIOModule::unwatch(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        return JS_UNDEFINED ;
    }

    void GPIOModule::loop(JSContext * ctx, void * arg) {

        if(!pending_event) {
            return ;
        }

        pending_event = false ;

        JSValueConst argv[2] ;

        for(uint8_t p=0; p<sizeof(gpio_state); p++) {
            if( gpio_state[p] & 1 ) {
                gpio_state[p] &= ~1;
                
                uint8_t level = (gpio_state[p]>>1) & 0x01 ;

//dn2(gpio_state[p],level)
                argv[0] = JS_NewInt32(ctx, p) ;
                argv[1] = JS_NewInt32(ctx, gpio_state[p]) ;


                for(auto callback: level? watching_callbacks[p].second : watching_callbacks[p].first) {
                    // dd
                    JS_Call(ctx, callback, JS_UNDEFINED, 2, argv) ;
                }

                JS_FreeValue(ctx, argv[0]) ;
                JS_FreeValue(ctx, argv[1]) ;
            }
        }
    }
    
    JSValue GPIOModule::test(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        return JS_NewInt32(ctx, gpio_state[7]) ;
    }
}

