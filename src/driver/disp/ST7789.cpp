#include "./ST7789.hpp"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_lcd_panel_io.h"
#include "esp_lcd_panel_vendor.h"
#include "esp_lcd_panel_ops.h"
#include "driver/spi_master.h"
#include "driver/gpio.h"

using namespace std ;

namespace be::driver::disp {

    DEFINE_NCLASS_META(ST7789, Display)

    std::vector<JSCFunctionListEntry> ST7789::methods = {
        JS_CFUNC_DEF("setup", 0, ST7789::setup),
        JS_CFUNC_DEF("test", 0, ST7789::test),
    } ;

    ST7789::ST7789(JSContext * ctx, JSValue _jsobj,uint16_t width=0, uint16_t height=0)
        : Display(ctx,build(ctx,_jsobj),width,height)
    {
    }

    JSValue ST7789::constructor(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        auto obj = new ST7789(ctx,this_val) ;
        obj->shared() ;
        return obj->jsobj ;
    }

    JSValue ST7789::setup(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {

        THIS_NCLASS(ST7789,that)
        ASSERT_ARGC(1)

        spi_host_device_t GET_INT_PROP(argv[0], "spi", spi, spi_host_device_t, )
        GET_UINT16_PROP(argv[0], "width", that->_width, )
        GET_UINT16_PROP(argv[0], "height", that->_height, )
        unsigned int GET_UINT32_PROP_OPT(argv[0], "pclk_hz", pclk_hz, 10*1000*1000)
        int GET_INT32_PROP_OPT(argv[0], "spi_mode", spi_mode, 0)
        
        gpio_num_t GET_GPIO_PROP(argv[0], "dc", dc, ) ;
        gpio_num_t GET_GPIO_PROP(argv[0], "cs", cs, ) ;
        gpio_num_t GET_GPIO_PROP_OPT(argv[0], "res", res, GPIO_NUM_NC) ;
        
        // 配置LCD面板的SPI接口
        esp_lcd_panel_io_spi_config_t io_config = {
            .cs_gpio_num = cs,
            .dc_gpio_num = dc,
            .spi_mode = spi_mode,
            .pclk_hz = pclk_hz,
            .trans_queue_depth = 10,
            .lcd_cmd_bits = 8,
            .lcd_param_bits = 8,
        };

        esp_lcd_panel_io_handle_t io_handle = NULL;
        esp_err_t err ;
        CALL_IDF_API(esp_lcd_new_panel_io_spi(spi, &io_config, &io_handle), "create panel io failed")

        // 创建ST7789面板
        
        const esp_lcd_panel_dev_config_t panel_config = {
            .reset_gpio_num = res,
            .color_space = ESP_LCD_COLOR_SPACE_BGR,
            .bits_per_pixel = 16,
        };
        
        CALL_IDF_API(esp_lcd_new_panel_st7789(io_handle, &panel_config, &that->handle), "create panel failed")

        // 初始化LCD面板
        CALL_IDF_API(esp_lcd_panel_reset(that->handle), "reset panel failed")
        CALL_IDF_API(esp_lcd_panel_init(that->handle),"init panel failed")
        CALL_IDF_API(esp_lcd_panel_invert_color(that->handle, true), "invert color failed")
        CALL_IDF_API(esp_lcd_panel_disp_on_off(that->handle, true), "display on failed")

        return JS_UNDEFINED ;
    }

    JSValue ST7789::test(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        THIS_NCLASS(ST7789,that)
        // esp_lcd_panel_draw_bitmap(that->handle, 0, 0, that->_width, that->_height, (uint16_t[]) {0xF800});

        uint16_t chunk_buffer[170 * 100];

        for (int i = 0; i < that->_width * that->_height; i++) {
            chunk_buffer[i] = 0xF800;
        }
    
        // 分块绘制全屏
        for (int y_start = 0; y_start < that->_height; y_start += 100) {
            int y_end = y_start + 100;
            if (y_end > that->_height) y_end = that->_height;
            ESP_ERROR_CHECK(esp_lcd_panel_draw_bitmap(that->handle, 0, y_start, that->_width, y_end, chunk_buffer));
        }

        return JS_UNDEFINED ;
    }

}