#include "RGB565.hpp"
#include "driver/gpio.h"
#include "freertos/FreeRTOS.h"
#include "freertos/semphr.h"
#include "freertos/task.h"
// #include "qjs_utils.h"

#ifdef CONFIG_IDF_TARGET_ESP32S3
#if ESP_IDF_VERSION_MAJOR >= 5

using namespace std ;

namespace be::driver::disp {

    // https://docs.espressif.com/projects/espressif-esp-faq/zh_CN/latest/software-framework/peripherals/lcd.html#id2


// typedef struct {
//     lcd_clock_source_t clk_src;   /*!< Clock source for the RGB LCD peripheral */
//     esp_lcd_rgb_timing_t timings; /*!< RGB timing parameters, including the screen resolution */
//     size_t data_width;            /*!< Number of data lines */
//     size_t bits_per_pixel;        /*!< Frame buffer color depth, in bpp, specially, if set to zero, it will default to `data_width`.
//                                        When using a Serial RGB interface, this value could be different from `data_width` */
//     size_t num_fbs;               /*!< Number of screen-sized frame buffers that allocated by the driver. By default (set to either 0 or 1) only one frame buffer will be used. Maximum number of buffers are 3 */
//     size_t bounce_buffer_size_px; /*!< If it's non-zero, the driver allocates two DRAM bounce buffers for DMA use.
//                                        DMA fetching from DRAM bounce buffer is much faster than PSRAM frame buffer. */
//     size_t sram_trans_align;      /*!< Alignment of buffers (frame buffer or bounce buffer) that allocated in SRAM */
//     size_t psram_trans_align;     /*!< Alignment of buffers (frame buffer) that allocated in PSRAM */
//     int hsync_gpio_num;           /*!< GPIO used for HSYNC signal */
//     int vsync_gpio_num;           /*!< GPIO used for VSYNC signal */
//     int de_gpio_num;              /*!< GPIO used for DE signal, set to -1 if it's not used */
//     int pclk_gpio_num;            /*!< GPIO used for PCLK signal */
//     int disp_gpio_num;            /*!< GPIO used for display control signal, set to -1 if it's not used */
//     int data_gpio_nums[SOC_LCD_RGB_DATA_WIDTH]; /*!< GPIOs used for data lines */
//     struct {
//         uint32_t disp_active_low: 1;     /*!< If this flag is enabled, a low level of display control signal can turn the screen on; vice versa */
//         uint32_t refresh_on_demand: 1;   /*!< If this flag is enabled, the host only refresh the frame buffer when `esp_lcd_panel_draw_bitmap` is called.
//                                               This is useful when the LCD screen has a GRAM and can refresh the LCD by itself. */
//         uint32_t fb_in_psram: 1;         /*!< If this flag is enabled, the frame buffer will be allocated from PSRAM, preferentially */
//         uint32_t double_fb: 1;           /*!< If this flag is enabled, the driver will allocate two screen sized frame buffer, same as num_fbs=2 */
//         uint32_t no_fb: 1;               /*!< If this flag is enabled, the driver won't allocate frame buffer.
//                                               Instead, user should fill in the bounce buffer manually in the `on_bounce_empty` callback */
//         uint32_t bb_invalidate_cache: 1; /*!< If this flag is enabled, in bounce back mode we'll do a cache invalidate on the read data, freeing the cache.
//                                               Can be dangerous if data is written from other core(s). */
//     } flags;                             /*!< LCD RGB panel configuration flags */
// } esp_lcd_rgb_panel_config_t;

    
    SemaphoreHandle_t sem_vsync_end;
    SemaphoreHandle_t sem_gui_ready;

    // static bool rgb_on_vsync_event(esp_lcd_panel_handle_t panel, const esp_lcd_rgb_panel_event_data_t *event_data, void *user_data)
    // {
    //     BaseType_t high_task_awoken = pdFALSE;
    // #if CONFIG_EXAMPLE_AVOID_TEAR_EFFECT_WITH_SEM
    //     if (xSemaphoreTakeFromISR(sem_gui_ready, &high_task_awoken) == pdTRUE) {
    //         xSemaphoreGiveFromISR(sem_vsync_end, &high_task_awoken);
    //     }
    // #endif
    //     return high_task_awoken == pdTRUE;
    // }


    DEFINE_NCLASS_META(RGB565, Display)
    std::vector<JSCFunctionListEntry> RGB565::methods = {
        JS_CFUNC_DEF("setup", 0, RGB565::setup),
    } ;

    RGB565::RGB565(JSContext * ctx, JSValue _jsobj, uint16_t width, uint16_t height)
        : Display(ctx, build(ctx, _jsobj), width, height)
    {}

    JSValue RGB565::setup(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {

        THIS_NCLASS(RGB565,that)
        ASSERT_ARGC(1)

        GET_UINT16_PROP(argv[0], "width", that->_width, )
        GET_UINT16_PROP(argv[0], "height", that->_height, )
        int GET_INT32_PROP_OPT(argv[0], "pclk_hz", pclk_hz, 10*1000*1000)

        uint32_t GET_UINT32_PROP_OPT(argv[0], "bounce_buffer_size_px", bounce_buffer_size_px, 0)
        bool GET_BOOL_PROP_OPT(argv[0], "double_fb", double_fb, false)
        bool GET_BOOL_PROP_OPT(argv[0], "fb_in_psram", fb_in_psram, false)

        JSValue pin = JS_GetPropertyStr(ctx, argv[0], "pin");
        if(!JS_IsObject(pin)){
            JSTHROW("missing pin property")
        }

        gpio_num_t GET_GPIO_PROP(pin, "de", pin_de, ) ;
        gpio_num_t GET_GPIO_PROP(pin, "vsync", pin_vsync, ) ;
        gpio_num_t GET_GPIO_PROP(pin, "hsync", pin_hsync, ) ;
        gpio_num_t GET_GPIO_PROP(pin, "pclk", pin_pclk,) ;

        gpio_num_t GET_GPIO_PROP(pin, "r0", pin_r0, ) ;
        gpio_num_t GET_GPIO_PROP(pin, "r1", pin_r1, ) ;
        gpio_num_t GET_GPIO_PROP(pin, "r2", pin_r2, ) ;
        gpio_num_t GET_GPIO_PROP(pin, "r3", pin_r3, ) ;
        gpio_num_t GET_GPIO_PROP(pin, "r4", pin_r4, ) ;

        gpio_num_t GET_GPIO_PROP(pin, "g0", pin_g0, ) ;
        gpio_num_t GET_GPIO_PROP(pin, "g1", pin_g1, ) ;
        gpio_num_t GET_GPIO_PROP(pin, "g2", pin_g2, ) ;
        gpio_num_t GET_GPIO_PROP(pin, "g3", pin_g3, ) ;
        gpio_num_t GET_GPIO_PROP(pin, "g4", pin_g4, ) ;
        gpio_num_t GET_GPIO_PROP(pin, "g5", pin_g5, ) ;
        
        gpio_num_t GET_GPIO_PROP(pin, "b0", pin_b0, ) ;
        gpio_num_t GET_GPIO_PROP(pin, "b1", pin_b1, ) ;
        gpio_num_t GET_GPIO_PROP(pin, "b2", pin_b2, ) ;
        gpio_num_t GET_GPIO_PROP(pin, "b3", pin_b3, ) ;
        gpio_num_t GET_GPIO_PROP(pin, "b4", pin_b4, ) ;

        esp_lcd_rgb_panel_config_t panel_config ;
        memset(&panel_config, 0, sizeof(esp_lcd_rgb_panel_config_t)) ;

        panel_config.data_width = 16 ; // RGB565 in parallel mode, thus 16bit in width
        panel_config.psram_trans_align = 64 ;

        // bounce_buffer_size_px 的性能说明：
        // https://gitee.com/aleeshadow/esp32-s3-lcd-ev-baord-docs/blob/master/Development%20guide/zh_CN/esp_lcd_rgb.md
        panel_config.bounce_buffer_size_px = bounce_buffer_size_px ; 

        panel_config.clk_src = LCD_CLK_SRC_DEFAULT;
        panel_config.disp_gpio_num = GPIO_NUM_NC;
        panel_config.pclk_gpio_num = pin_pclk;
        panel_config.vsync_gpio_num = pin_vsync;
        panel_config.hsync_gpio_num = pin_hsync;
        panel_config.de_gpio_num = pin_de;

        panel_config.data_gpio_nums[0] = pin_b0 ;
        panel_config.data_gpio_nums[1] = pin_b1 ;
        panel_config.data_gpio_nums[2] = pin_b2 ;
        panel_config.data_gpio_nums[3] = pin_b3 ;
        panel_config.data_gpio_nums[4] = pin_b4 ;

        panel_config.data_gpio_nums[5] = pin_g0 ;
        panel_config.data_gpio_nums[6] = pin_g1 ;
        panel_config.data_gpio_nums[7] = pin_g2 ;
        panel_config.data_gpio_nums[8] = pin_g3 ;
        panel_config.data_gpio_nums[9] = pin_g4 ;
        panel_config.data_gpio_nums[10] = pin_g5 ;

        panel_config.data_gpio_nums[11] = pin_r0 ;
        panel_config.data_gpio_nums[12] = pin_r1 ;
        panel_config.data_gpio_nums[13] = pin_r2 ;
        panel_config.data_gpio_nums[14] = pin_r3 ;
        panel_config.data_gpio_nums[15] = pin_r4 ;

        panel_config.timings.pclk_hz = pclk_hz;
        panel_config.timings.h_res = that->_width;
        panel_config.timings.v_res = that->_height;
        
        // The following parameters should refer to LCD spec
        panel_config.timings.hsync_back_porch = 50;
        panel_config.timings.hsync_front_porch = 10;
        panel_config.timings.hsync_pulse_width = 8;
        panel_config.timings.vsync_back_porch = 20;
        panel_config.timings.vsync_front_porch = 10;
        panel_config.timings.vsync_pulse_width = 8;
        panel_config.timings.flags.pclk_active_neg = false; // RGB data is clocked out on falling edge
        // .flags.hsync_idle_low = true,
        panel_config.flags.fb_in_psram = fb_in_psram; // allocate frame buffer in PSRAM
        panel_config.flags.double_fb = double_fb;   // allocate double frame buffer
        
        esp_err_t res = esp_lcd_new_rgb_panel(&panel_config, &that->handle);
        if( res!= ESP_OK ){
            JSTHROW("create panel failed, err=%d", res)
        }
        
        /* Initialize RGB LCD panel */
        res = esp_lcd_panel_reset(that->handle);
        if( res!= ESP_OK ) {
            JSTHROW("reset panel failed, err=%d", res)
        }
        res = esp_lcd_panel_init(that->handle) ;
        if( res!= ESP_OK ){
            JSTHROW("init panel failed, err=%d", res)
        }
        
        if( JS_IsException(Display::setup(ctx, this_val, argc, argv)) ) {
            return JS_EXCEPTION ;
        }

        return JS_UNDEFINED ;
    }

    bool RGB565::createBuff() {
        _buffSize = _width*_height*sizeof(color_t) ;
        ESP_ERROR_CHECK(esp_lcd_rgb_panel_get_frame_buffer(handle, 2, &_buff1, &_buff2));
        return true ;
    }
    
}

#endif
#endif