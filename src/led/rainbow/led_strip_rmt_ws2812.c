#include <stdlib.h>
#include <string.h>
#include <sys/cdefs.h>
#include "esp_log.h"
#include "esp_attr.h"
#include "led_strip.h"
#include "module_utils.h"
#include "utils.h"

#define RMT_TX_CHANNEL RMT_CHANNEL_0

static const char *TAG = "ws2812";
#define STRIP_CHECK(a, str, goto_tag, ret_value, ...)                             \
    do                                                                            \
    {                                                                             \
        if (!(a))                                                                 \
        {                                                                         \
            ESP_LOGE(TAG, "%s(%d): " str, __FUNCTION__, __LINE__, ##__VA_ARGS__); \
            ret = ret_value;                                                      \
            goto goto_tag;                                                        \
        }                                                                         \
    } while (0)

#define WS2812_T0H_NS (350)
#define WS2812_T0L_NS (1000)
#define WS2812_T1H_NS (1000)
#define WS2812_T1L_NS (350)
#define WS2812_RESET_US (280)

static uint32_t ws2812_t0h_ticks = 0;
static uint32_t ws2812_t1h_ticks = 0;
static uint32_t ws2812_t0l_ticks = 0;
static uint32_t ws2812_t1l_ticks = 0;


/**
 * @brief Conver RGB data to RMT format.
 *
 * @note For WS2812, R,G,B each contains 256 different choices (i.e. uint8_t)
 *
 * @param[in] src: source data, to converted to RMT format
 * @param[in] dest: place where to store the convert result
 * @param[in] src_size: size of source data
 * @param[in] wanted_num: number of RMT items that want to get
 * @param[out] translated_size: number of source data that got converted
 * @param[out] item_num: number of RMT items which are converted from source data
 */
static void IRAM_ATTR ws2812_rmt_adapter(const void *src, rmt_item32_t *dest, size_t src_size,
        size_t wanted_num, size_t *translated_size, size_t *item_num)
{
    if (src == NULL || dest == NULL) {
        *translated_size = 0;
        *item_num = 0;
        return;
    }
    const rmt_item32_t bit0 = {{{ ws2812_t0h_ticks, 1, ws2812_t0l_ticks, 0 }}}; //Logical 0
    const rmt_item32_t bit1 = {{{ ws2812_t1h_ticks, 1, ws2812_t1l_ticks, 0 }}}; //Logical 1
    size_t size = 0;
    size_t num = 0;
    uint8_t *psrc = (uint8_t *)src;
    rmt_item32_t *pdest = dest;
    while (size < src_size && num < wanted_num) {
        for (int i = 0; i < 8; i++) {
            // MSB first
            if (*psrc & (1 << (7 - i))) {
                pdest->val =  bit1.val;
            } else {
                pdest->val =  bit0.val;
            }
            num++;
            pdest++;
        }
        size++;
        psrc++;
    }
    *translated_size = size;
    *item_num = num;
}


static esp_err_t ws2812_fill(led_strip_t *strip, int index, int length, int h, int s, int v, int dh, int ds, int dv){

    if(length==0) {
        return ESP_OK ;
    }

    ws2812_t *ws2812 = __containerof(strip, ws2812_t, parent);

    int end = index + length;
    if(end > strip->strip_len){
        end = strip->strip_len ;
    }
    if(end<0) {
        end = 0 ;
    }

    if(index<0) {
        index = 0 ;
    }

    // dn3(index, length, end)
    // dn3(h,s,v)
    // dn3(dh,ds,dv)

    if(h<0) h = 0 ;
    if(s<0) s = 0 ;
    if(v<0) v = 0 ;
    
    if(h>360) h = 360 ;
    if(s>100) s = 100 ;
    if(v>100) v = 100 ;

    end*= 3 ;
    uint32_t r, g, b ;
    for(uint32_t i=index*3;i<end;i+=3) {

        hsv2rgb(h,s,v,&r,&g,&b) ;

        // printf("hsv(%d,%d,%d)->rgb(%d,%d,%d)\n", h,s,v,r,g,b) ;

        strip->buffer[i] = gamma8_correct(g) ;
        strip->buffer[i+1] = gamma8_correct(r) ;
        strip->buffer[i+2] = gamma8_correct(b) ;

        h+= dh ;
        s+= ds ;
        v+= dv ;

        if(h<0) h = 360 + h ;
        if(s<0) s = 0 ;
        if(v<0) v = 0 ;
        
        if(h>360) h = h - 360 ;
        if(s>100) s = 100 ;
        if(v>100) v = 100 ;
    }

    return ESP_OK ;
}

static esp_err_t ws2812_set_pixel(led_strip_t *strip, uint32_t index, uint32_t red, uint32_t green, uint32_t blue)
{
    esp_err_t ret = ESP_OK;
    ws2812_t *ws2812 = __containerof(strip, ws2812_t, parent);
    STRIP_CHECK(index < strip->strip_len, "index out of the maximum number of leds", err, ESP_ERR_INVALID_ARG);
    uint32_t start = index * 3;
    // In thr order of GRB
    strip->buffer[start + 0] = green & 0xFF;
    strip->buffer[start + 1] = red & 0xFF;
    strip->buffer[start + 2] = blue & 0xFF;
    return ESP_OK;
err:
    return ret;
}

static esp_err_t ws2812_refresh(led_strip_t *strip, uint32_t timeout_ms)
{
    esp_err_t ret = ESP_OK;
    ws2812_t *ws2812 = __containerof(strip, ws2812_t, parent);
    ret = rmt_write_sample(ws2812->rmt_channel, strip->buffer, strip->strip_len * 3, true) ;
    STRIP_CHECK( ret== ESP_OK, "transmit RMT samples failed", err, ret);
    return rmt_wait_tx_done(ws2812->rmt_channel, pdMS_TO_TICKS(timeout_ms));
err:
    return ret;
}

static esp_err_t ws2812_clear(led_strip_t *strip, bool refresh, uint32_t timeout_ms)
{
    ws2812_t *ws2812 = __containerof(strip, ws2812_t, parent);
    // Write zero to turn off all leds
    memset(strip->buffer, 0, strip->strip_len * 3);
    if(refresh) {
        return ws2812_refresh(strip, timeout_ms);
    }
    else {
        return ESP_OK ;
    }
}

static esp_err_t ws2812_del(led_strip_t *strip)
{
    ws2812_t *ws2812 = __containerof(strip, ws2812_t, parent);
    free(ws2812);
    return ESP_OK;
}

led_strip_t *led_strip_new_rmt_ws2812(const led_strip_config_t *config)
{
    led_strip_t *ret = NULL;
    STRIP_CHECK(config, "configuration can't be null", err, NULL);

    // 24 bits per led
    ws2812_t *ws2812 = calloc(1, sizeof(ws2812_t));
    STRIP_CHECK(ws2812, "request memory for ws2812 failed", err, NULL);

    HMALLOC(ws2812->parent.buffer, config->max_leds * 3)
    STRIP_CHECK(ws2812->parent.buffer, "request memory for ws2812 failed", err, NULL);

    uint32_t counter_clk_hz = 0;
    STRIP_CHECK(rmt_get_counter_clock((rmt_channel_t)config->dev, &counter_clk_hz) == ESP_OK,
                "get rmt counter clock failed", err, NULL);
    // ns -> ticks
    float ratio = (float)counter_clk_hz / 1e9;
    ws2812_t0h_ticks = (uint32_t)(ratio * WS2812_T0H_NS);
    ws2812_t0l_ticks = (uint32_t)(ratio * WS2812_T0L_NS);
    ws2812_t1h_ticks = (uint32_t)(ratio * WS2812_T1H_NS);
    ws2812_t1l_ticks = (uint32_t)(ratio * WS2812_T1L_NS);

    // set ws2812 to rmt adapter
    rmt_translator_init((rmt_channel_t)config->dev, ws2812_rmt_adapter);

    ws2812->rmt_channel = (rmt_channel_t)config->dev;
    ws2812->parent.strip_len = config->max_leds;

    ws2812->parent.fill = ws2812_fill;
    ws2812->parent.set_pixel = ws2812_set_pixel;
    ws2812->parent.refresh = ws2812_refresh;
    ws2812->parent.clear = ws2812_clear;
    ws2812->parent.del = ws2812_del;

    return &ws2812->parent;
err:
    return ret;
}

led_strip_t * led_strip_init(uint8_t channel, uint8_t gpio, uint16_t led_num)
{
    static led_strip_t *pStrip;

    rmt_config_t config = RMT_DEFAULT_CONFIG_TX(gpio, channel);
    // set counter clock to 40MHz
    config.clk_div = 2;

    ESP_ERROR_CHECK(rmt_config(&config));
    ESP_ERROR_CHECK(rmt_driver_install(config.channel, 0, 0));

    // install ws2812 driver
    led_strip_config_t strip_config = LED_STRIP_DEFAULT_CONFIG(led_num, (led_strip_dev_t)config.channel);

    pStrip = led_strip_new_rmt_ws2812(&strip_config);

    if ( !pStrip ) {
        ESP_LOGE(TAG, "install WS2812 driver failed");
        return NULL;
    }

    // Clear LED strip (turn off all LEDs)
    ESP_ERROR_CHECK(pStrip->clear(pStrip, true, 100));

    return pStrip;
}

esp_err_t led_strip_denit(led_strip_t *strip)
{
    if(strip->buffer) {
        free(strip->buffer) ;
        strip->buffer = NULL ;
    }

    ws2812_t *ws2812 = __containerof(strip, ws2812_t, parent);
    ESP_ERROR_CHECK(rmt_driver_uninstall(ws2812->rmt_channel));
    return strip->del(strip);
}
