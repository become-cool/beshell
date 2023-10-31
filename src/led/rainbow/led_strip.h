// Copyright 2019 Espressif Systems (Shanghai) PTE LTD
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#include "esp_err.h"
#include "driver/rmt.h"
#include "be_list.h"
#include "ease.h"



typedef struct strip_keyframe_s {
    be_list_item_t item ;

    size_t frm ;

    int16_t x ;
    int16_t width ;

    uint16_t h ;    // hue
    int16_t dh ;    // delta hue

    uint16_t s ;    // saturation
    int16_t ds ;    // delta saturation

    uint16_t v ;    // value/brightness
    int16_t dv ;    // delta value

    easing_func_t ease ;
    double ease_s ;
    
} strip_keyframe_t ;


typedef struct {
    be_list_item_t item ;
    be_list_t keyframes ;
} strip_track_t;

typedef struct {

    size_t frames ;
    size_t current ;
    uint32_t fps ;
    bool repeat: 1 ;
    size_t repeat_from ;

    int64_t start_time ;
    int64_t finish_time ;
    
    bool playing: 1 ;
    // TaskHandle_t timeline_task ;
    esp_timer_handle_t timer ;
    
    be_list_t tracks ;
} strip_timeline_t ;


/**
* @brief LED Strip Type
*
*/
typedef struct led_strip_s led_strip_t;

/**
* @brief LED Strip Device Type
*
*/
typedef void *led_strip_dev_t;

/**
* @brief Declare of LED Strip Type
*
*/
struct led_strip_s {
    esp_err_t (*fill)(led_strip_t *strip, uint32_t index, uint32_t length, uint32_t h, uint32_t s, uint32_t v, uint32_t dh, uint32_t ds, uint32_t dv);
    esp_err_t (*set_pixel)(led_strip_t *strip, uint32_t index, uint32_t red, uint32_t green, uint32_t blue);
    esp_err_t (*refresh)(led_strip_t *strip, uint32_t timeout_ms);
    esp_err_t (*clear)(led_strip_t *strip, bool refresh, uint32_t timeout_ms);
    esp_err_t (*del)(led_strip_t *strip);

    uint32_t strip_len;
    uint8_t * buffer ;

    strip_timeline_t timeline ;
};

typedef struct {
    led_strip_t parent;
    rmt_channel_t rmt_channel;
} ws2812_t;

/**
* @brief LED Strip Configuration Type
*
*/
typedef struct {
    uint32_t max_leds;   /*!< Maximum LEDs in a single strip */
    led_strip_dev_t dev; /*!< LED strip device (e.g. RMT channel, PWM channel, etc) */
} led_strip_config_t;

/**
 * @brief Default configuration for LED strip
 *
 */
#define LED_STRIP_DEFAULT_CONFIG(number, dev_hdl) \
    {                                             \
        .max_leds = number,                       \
        .dev = dev_hdl,                           \
    }

/**
* @brief Install a new ws2812 driver (based on RMT peripheral)
*
* @param config: LED strip configuration
* @return
*      LED strip instance or NULL
*/
led_strip_t *led_strip_new_rmt_ws2812(const led_strip_config_t *config);

/**
 * @brief Init the RMT peripheral and LED strip configuration.
 *
 * @param[in] channel: RMT peripheral channel number.
 * @param[in] gpio: GPIO number for the RMT data output.
 * @param[in] led_num: number of addressable LEDs.
 * @return
 *      LED strip instance or NULL
 */
led_strip_t * led_strip_init(uint8_t channel, uint8_t gpio, uint16_t led_num);

/**
 * @brief Denit the RMT peripheral.
 *
 * @param[in] strip: LED strip
 * @return
 *     - ESP_OK
 *     - ESP_FAIL
 */
esp_err_t led_strip_denit(led_strip_t *strip);



inline bool led_strip_timeline_render_frame(led_strip_t * strip, size_t frm) ;
void led_strip_timeline_init(led_strip_t * strip) ;
void led_strip_timeline_delete(strip_timeline_t * timeline) ;
void led_strip_timeline_print(strip_timeline_t * timeline) ;
bool led_strip_timeline_clear(strip_timeline_t * timeline) ;
bool led_strip_timeline_start(strip_timeline_t * timeline) ;
void led_strip_timeline_stop(strip_timeline_t * timeline) ;
strip_track_t * led_strip_timeline_create_track(strip_timeline_t * timeline)  ;
strip_keyframe_t * led_strip_track_create_keyframe(strip_track_t * track) ;


#ifdef __cplusplus
}
#endif
