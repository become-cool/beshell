#include "I2S.hpp"
#include "esp_system.h"
#include "thread.hpp"

using namespace std ;

namespace be {

    I2S * I2S::i2s0 = nullptr ;
    
    #if SOC_I2S_NUM > 1
    I2S * I2S::i2s1 = nullptr ;
    #endif

    DEFINE_NCLASS_META(I2S, NativeClass)
    std::vector<JSCFunctionListEntry> I2S::methods = {
        JS_CFUNC_DEF("setup", 0, I2S::setup),
    } ;

    I2S::I2S(JSContext * ctx, i2s_port_t busnum)
        : NativeClass(ctx,build(ctx))
        , busnum(busnum)
    {
    }


    // typedef struct {

    //     i2s_mode_t              mode;                       /*!< I2S work mode */
    //     uint32_t                sample_rate;                /*!< I2S sample rate */
    //     i2s_bits_per_sample_t   bits_per_sample;            /*!< I2S sample bits in one channel */
    //     i2s_channel_fmt_t       channel_format;             /*!< I2S channel format.*/
    //     i2s_comm_format_t       communication_format;       /*!< I2S communication format */
    //     int                     intr_alloc_flags;           /*!< Flags used to allocate the interrupt. One or multiple (ORred) ESP_INTR_FLAG_* values. See esp_intr_alloc.h for more info */
    //     union {
    //         int dma_desc_num;                               /*!< The total number of descriptors used by I2S DMA to receive/transmit data */
    //         int dma_buf_count __attribute__((deprecated));  /*!< This is an alias to 'dma_desc_num' for backward compatibility */
    //     };
    //     union {
    //         int dma_frame_num;                              /*!< Frame number for one-time sampling. Frame here means the total data from all the channels in a WS cycle */
    //         int dma_buf_len __attribute__((deprecated));    /*!< This is an alias to 'dma_frame_num' for backward compatibility */
    //     };
    //     bool                    use_apll;                   /*!< I2S using APLL as main I2S clock, enable it to get accurate clock */
    //     bool                    tx_desc_auto_clear;         /*!< I2S auto clear tx descriptor if there is underflow condition (helps in avoiding noise in case of data unavailability) */
    //     int                     fixed_mclk;                 /*!< I2S using fixed MCLK output. If use_apll = true and fixed_mclk > 0, then the clock output for i2s is fixed and equal to the fixed_mclk value. If fixed_mclk set, mclk_multiple won't take effect */
    //     i2s_mclk_multiple_t     mclk_multiple;              /*!< The multiple of I2S master clock(MCLK) to sample rate */
    //     i2s_bits_per_chan_t     bits_per_chan;              /*!< I2S total bits in one channel， only take effect when larger than 'bits_per_sample', default '0' means equal to 'bits_per_sample' */

    // #if SOC_I2S_SUPPORTS_TDM
    //     i2s_channel_t           chan_mask;                  /*!< I2S active channel bit mask, set value in `i2s_channel_t` to enable specific channel, the bit map of active channel can not exceed (0x1<<total_chan). */
    //     uint32_t                total_chan;                 /*!< I2S Total number of channels. If it is smaller than the biggest active channel number, it will be set to this number automatically. */
    //     bool                    left_align;                 /*!< Set to enable left alignment */
    //     bool                    big_edin;                   /*!< Set to enable big endian */
    //     bool                    bit_order_msb;              /*!< Set to enable msb order */
    //     bool                    skip_msk;                   /*!< Set to enable skip mask. If it is enabled, only the data of the enabled channels will be sent, otherwise all data stored in DMA TX buffer will be sent */
    // #endif // SOC_I2S_SUPPORTS_TDM

    // } i2s_driver_config_txx ;

    struct i2s_conf {
        i2s_port_t busnum ;
        i2s_config_t config ;
        i2s_pin_config_t pin ;
    } ;

    static void task_install_i2s(struct i2s_conf  * conf) {
        assert(conf) ;

        esp_err_t res = i2s_driver_install(conf->busnum, &conf->config, 0, NULL) ;
        if(res!=ESP_OK){
            // printf("i2s_driver_install() faild with error: %d\n", res) ;
            free(conf) ;
            return ;
        }

#if SOC_I2S_SUPPORTS_ADC_DAC
        // if ((config->i2s_config.mode & I2S_MODE_DAC_BUILT_IN) != 0) {
        //     // i2s_set_dac_mode(I2S_DAC_CHANNEL_BOTH_EN);
        // } else
#endif
        {
            res = i2s_set_pin(conf->busnum, &conf->pin);
            if(res!=ESP_OK){
                printf("i2s_set_pin() faild with error: %d", res) ;
                free(conf) ;
                return ;
            }
        }

        printf("i2s_driver_install() and i2s_set_pin() success\n") ;
        free(conf) ;

        vTaskDelete(NULL) ;

        while(1){
            vTaskDelay(1000);
        }
    }

    JSValue I2S::setup(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {

        THIS_NCLASS(I2S, that)
        ASSERT_ARGC(1)

        i2s_config_t i2s_config = {
            .mode = (i2s_mode_t)(I2S_MODE_MASTER | I2S_MODE_TX | I2S_MODE_RX),
            .sample_rate = 8000,
            .bits_per_sample = I2S_BITS_PER_SAMPLE_16BIT,
            .channel_format = I2S_CHANNEL_FMT_RIGHT_LEFT,
            .communication_format = I2S_COMM_FORMAT_STAND_I2S,
            .intr_alloc_flags = ESP_INTR_FLAG_LEVEL2 | ESP_INTR_FLAG_IRAM,
            .dma_buf_count = 2,
            .dma_buf_len = 128,
            .use_apll = true,
            .tx_desc_auto_clear = true,
            .fixed_mclk = 0
        };

        i2s_pin_config_t pin_config = {
            .bck_io_num = -1,
            .ws_io_num = -1,
            .data_out_num = -1,
            .data_in_num = -1
        };


        if(!JS_IsObject(argv[0])) {
            JSTHROW("arg %s must be an object", "opts");
        }

        int coreId = 0 ;

        GET_INT32_PROP_OPT(argv[0], "mode", i2s_config.mode, (i2s_mode_t)(I2S_MODE_MASTER | I2S_MODE_TX | I2S_MODE_RX))
        GET_UINT32_PROP_OPT(argv[0], "sample_rate", i2s_config.sample_rate, 44100)
        GET_INT32_PROP_OPT(argv[0], "bits_per_sample", i2s_config.bits_per_sample, I2S_BITS_PER_SAMPLE_16BIT)
        GET_INT32_PROP_OPT(argv[0], "channel_format", i2s_config.channel_format, I2S_CHANNEL_FMT_RIGHT_LEFT)
        GET_INT32_PROP_OPT(argv[0], "communication_format", i2s_config.communication_format, I2S_COMM_FORMAT_STAND_I2S)
        GET_INT32_PROP_OPT(argv[0], "intr_alloc_flags", i2s_config.intr_alloc_flags, ESP_INTR_FLAG_LEVEL2 )
        GET_INT32_PROP_OPT(argv[0], "dma_buf_count", i2s_config.dma_buf_count, 2)
        GET_INT32_PROP_OPT(argv[0], "dma_buf_len", i2s_config.dma_buf_len, 128)
        GET_INT32_PROP_OPT(argv[0], "use_apll", i2s_config.use_apll, 1)
        
        GET_UINT32_PROP(argv[0], "lrclk", pin_config.ws_io_num, )
        GET_UINT32_PROP(argv[0], "sclk", pin_config.bck_io_num, )
        GET_INT32_PROP_OPT(argv[0], "sout", pin_config.data_out_num, -1)
        GET_INT32_PROP_OPT(argv[0], "sin", pin_config.data_in_num, -1)

        GET_UINT32_PROP_OPT(argv[0], "core", coreId, xPortGetCoreID())
        if(coreId>2) {
            JSTHROW("core id (%d) out of range, max is 2", coreId)
        }

        
        esp_err_t res ;
        i2s_port_t bus = that->busnum ;
        run_wait_on_core([&res, bus, &i2s_config](){
            res = i2s_driver_install(bus, &i2s_config, 0, NULL) ;
        }, coreId) ;
        if(res!=ESP_OK){
            JSTHROW("i2s_driver_install() faild with error: %d", res) ;
        }

        res = i2s_set_pin(that->busnum, &pin_config);
        if(res!=ESP_OK){
            JSTHROW("i2s_set_pin() faild with error: %d", res) ;
        }

        return JS_UNDEFINED ;
    }
    

    #define DEFINE_BUS(busconst, var)           \
        if(bus==busconst) {                     \
            if(!var) {                          \
                var = new I2S(ctx, busconst) ;  \
            }                                   \
            return var ;                        \
        }
    I2S * I2S::flyweight(JSContext * ctx, i2s_port_t bus) {
        DEFINE_BUS(I2S_NUM_0, i2s0)
        #if SOC_I2S_NUM>1
        else DEFINE_BUS(I2S_NUM_1, i2s1)
        #endif
        return nullptr ;
    }
}