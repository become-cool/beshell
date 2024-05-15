#include "I2S.hpp"

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
            .dma_buf_len = 512,
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

        GET_INT32_PROP_OPT(argv[0], "mode", i2s_config.mode, (i2s_mode_t)(I2S_MODE_MASTER | I2S_MODE_TX | I2S_MODE_RX))
        GET_UINT32_PROP_OPT(argv[0], "sample_rate", i2s_config.sample_rate, 8000)
        GET_INT32_PROP_OPT(argv[0], "bits_per_sample", i2s_config.bits_per_sample, I2S_BITS_PER_SAMPLE_16BIT)
        GET_INT32_PROP_OPT(argv[0], "channel_format", i2s_config.channel_format, I2S_CHANNEL_FMT_RIGHT_LEFT)
        GET_INT32_PROP_OPT(argv[0], "communication_format", i2s_config.communication_format, I2S_COMM_FORMAT_STAND_I2S)
        GET_INT32_PROP_OPT(argv[0], "intr_alloc_flags", i2s_config.intr_alloc_flags, ESP_INTR_FLAG_LEVEL2 | ESP_INTR_FLAG_IRAM)
        GET_INT32_PROP_OPT(argv[0], "dma_buf_count", i2s_config.dma_buf_count, 2)
        GET_INT32_PROP_OPT(argv[0], "dma_buf_len", i2s_config.dma_buf_len, 256)
        GET_INT32_PROP_OPT(argv[0], "use_apll", i2s_config.use_apll, 1)
        
        GET_UINT32_PROP(argv[0], "lrclk", pin_config.ws_io_num, )
        GET_UINT32_PROP(argv[0], "sclk", pin_config.bck_io_num, )
        GET_INT32_PROP_OPT(argv[0], "sout", pin_config.data_out_num, -1)
        GET_INT32_PROP_OPT(argv[0], "sin", pin_config.data_in_num, -1)

        // i2s_config.channel_format = I2S_CHANNEL_FMT_ONLY_LEFT ;
        // i2s_config.channel_format = I2S_CHANNEL_FMT_RIGHT_LEFT ;
        dn(I2S_CHANNEL_FMT_RIGHT_LEFT)
        dn(I2S_CHANNEL_FMT_ONLY_LEFT)

        printf("I2S %d, lrclk:%d, sclk:d%, sout:%d\n", pin_config.ws_io_num, pin_config.bck_io_num, pin_config.data_out_num) ;
        dn(i2s_config.channel_format)
        dn(i2s_config.communication_format)
        dn(i2s_config.sample_rate)
        dn(i2s_config.bits_per_sample)

        esp_err_t res = i2s_driver_install(that->busnum, &i2s_config, 0, NULL) ;
        if(res!=ESP_OK){
            JSTHROW("i2s_driver_install() faild with error: %d", res)
        }
        res = i2s_set_pin(that->busnum, &pin_config);
        if(res!=ESP_OK){
            JSTHROW("i2s_set_pin() faild with error: %d", res)
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