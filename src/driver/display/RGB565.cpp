#include "RGB565.hpp"
#include "driver/gpio.h"


using namespace std ;

namespace be::driver::display {

#define GPIO_LCD_DE     (GPIO_NUM_5)
#define GPIO_LCD_VSYNC  (GPIO_NUM_6)
#define GPIO_LCD_HSYNC  (GPIO_NUM_7)
#define GPIO_LCD_PCLK   (GPIO_NUM_4)

#define GPIO_LCD_R0    (GPIO_NUM_11)
#define GPIO_LCD_R1    (GPIO_NUM_21)
#define GPIO_LCD_R2    (GPIO_NUM_47)
#define GPIO_LCD_R3    (GPIO_NUM_48)
#define GPIO_LCD_R4    (GPIO_NUM_45)

#define GPIO_LCD_G0    (GPIO_NUM_19)
#define GPIO_LCD_G1    (GPIO_NUM_20)
#define GPIO_LCD_G2    (GPIO_NUM_4)
#define GPIO_LCD_G3    (GPIO_NUM_46)
#define GPIO_LCD_G4    (GPIO_NUM_9)
#define GPIO_LCD_G5    (GPIO_NUM_10)

#define GPIO_LCD_B0    (GPIO_NUM_15)
#define GPIO_LCD_B1    (GPIO_NUM_16)
#define GPIO_LCD_B2    (GPIO_NUM_17)
#define GPIO_LCD_B3    (GPIO_NUM_18)
#define GPIO_LCD_B4    (GPIO_NUM_8)

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

    DEFINE_NCLASS_META(RGB565, Display)
    std::vector<JSCFunctionListEntry> RGB565::methods = {
        JS_CFUNC_DEF("test", 0, RGB565::jsTest),
    } ;

    RGB565::RGB565(JSContext * ctx, JSValue _jsobj, uint16_t width, uint16_t height)
        : Display(ctx, build(ctx, _jsobj), width, height)
    {
        _width = 480 ;
        _height = 480 ;

        esp_lcd_rgb_panel_config_t panel_config ;
        memset(&panel_config, 0, sizeof(esp_lcd_rgb_panel_config_t)) ;

        panel_config.data_width = 16 ; // RGB565 in parallel mode, thus 16bit in width
        panel_config.psram_trans_align = 64 ;

    // #if CONFIG_EXAMPLE_USE_BOUNCE_BUFFER
        // panel_config.bounce_buffer_size_px = 10 * LCD_WIDTH,
    // #endif   

        panel_config.clk_src = LCD_CLK_SRC_DEFAULT;
        panel_config.disp_gpio_num = GPIO_NUM_NC;
        panel_config.pclk_gpio_num = GPIO_LCD_PCLK;
        panel_config.vsync_gpio_num = GPIO_LCD_VSYNC;
        panel_config.hsync_gpio_num = GPIO_LCD_HSYNC;
        panel_config.de_gpio_num = GPIO_LCD_DE;

        panel_config.data_gpio_nums[0] = GPIO_LCD_B0 ;
        panel_config.data_gpio_nums[1] = GPIO_LCD_B1 ;
        panel_config.data_gpio_nums[2] = GPIO_LCD_B2 ;
        panel_config.data_gpio_nums[3] = GPIO_LCD_B3 ;
        panel_config.data_gpio_nums[4] = GPIO_LCD_B4 ;

        panel_config.data_gpio_nums[5] = GPIO_LCD_G0 ;
        panel_config.data_gpio_nums[6] = GPIO_LCD_G1 ;
        panel_config.data_gpio_nums[7] = GPIO_LCD_G2 ;
        panel_config.data_gpio_nums[8] = GPIO_LCD_G3 ;
        panel_config.data_gpio_nums[9] = GPIO_LCD_G4 ;
        panel_config.data_gpio_nums[10] = GPIO_LCD_G5 ;

        panel_config.data_gpio_nums[11] = GPIO_LCD_R0 ;
        panel_config.data_gpio_nums[12] = GPIO_LCD_R1 ;
        panel_config.data_gpio_nums[13] = GPIO_LCD_R2 ;
        panel_config.data_gpio_nums[14] = GPIO_LCD_R3 ;
        panel_config.data_gpio_nums[15] = GPIO_LCD_R4 ;

            // .timings = {
        panel_config.timings.pclk_hz = 14 * 1000 * 1000;
        panel_config.timings.h_res = _width;
        panel_config.timings.v_res = _height;
        
        // The following parameters should refer to LCD spec
        panel_config.timings.hsync_back_porch = 50;
        panel_config.timings.hsync_front_porch = 10;
        panel_config.timings.hsync_pulse_width = 8;
        panel_config.timings.vsync_back_porch = 20;
        panel_config.timings.vsync_front_porch = 10;
        panel_config.timings.vsync_pulse_width = 8;
        panel_config.timings.flags.pclk_active_neg = false; // RGB data is clocked out on falling edge
                // .flags.hsync_idle_low = true,
            // },
        panel_config.flags.fb_in_psram = true; // allocate frame buffer in PSRAM
    // #if CONFIG_EXAMPLE_DOUBLE_FB
        panel_config.flags.double_fb = true;   // allocate double frame buffer
    // #endif // CONFIG_EXAMPLE_DOUBLE_FB
        // };
        ESP_ERROR_CHECK(esp_lcd_new_rgb_panel(&panel_config, &handle));

        /* Register event callbacks */
        // esp_lcd_rgb_panel_event_callbacks_t cbs = {
        //     .on_vsync = rgb_on_vsync_event,
        // };
        // ESP_ERROR_CHECK(esp_lcd_rgb_panel_register_event_callbacks(handle, &cbs, &disp_drv));

        /* Initialize RGB LCD panel */
        ESP_ERROR_CHECK(esp_lcd_panel_reset(handle));
        ESP_ERROR_CHECK(esp_lcd_panel_init(handle));
    }

    // void RGB565::setup() {

    // }


    void RGB565::drawRect(coord_t x1,coord_t y1,coord_t x2,coord_t y2,color_t * pixels) {
        assert(handle) ;
        esp_lcd_panel_draw_bitmap(handle, x1, y1, x2, y2, pixels);
    }

    JSValue RGB565::jsTest(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        RGB565 * disp = (RGB565*)fromJS(this_val) ;
        assert(disp) ;

        void * buff = malloc(100 * 100 * 2) ;
        memset((void*)buff, 255, 100*100*2) ;

        esp_lcd_panel_draw_bitmap(disp->handle, 0, 0, 100, 100, (void *)buff);

        return JS_UNDEFINED ;
    }

    JSValue RGB565::constructor(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        auto obj = new RGB565(ctx) ;
        obj->self = std::shared_ptr<RGB565> (obj) ;
        return obj->jsobj ;
    }

    bool RGB565::createBuff() {
        ESP_ERROR_CHECK(esp_lcd_rgb_panel_get_frame_buffer(handle, 2, &buff1, &buff2));
        lv_display_set_buffers(lv_display, buff1, buff2, _width*_height*sizeof(lv_color_t), LV_DISPLAY_RENDER_MODE_FULL);
        return true ;
    }
}