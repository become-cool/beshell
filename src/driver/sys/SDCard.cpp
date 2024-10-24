#include "SDCard.hpp"
#include "fs/FS.hpp"

#include "esp_system.h"
#include "driver/sdspi_host.h"
#include "driver/sdmmc_host.h"
#include "vfs_fat_internal.h"
#include "sdmmc_cmd.h"
#include "diskio_sdmmc.h"
#include "diskio_impl.h"
#include "driver/i2s.h"
#include "driver/gpio.h"
#include "hal/gpio_types.h"

using namespace std ;

namespace be::driver::sys {
    DEFINE_NCLASS_META(SDCard, NativeClass)
    std::vector<JSCFunctionListEntry> SDCard::methods = {
        JS_CFUNC_DEF("setup", 0, SDCard::setup),
    } ;

    SDCard::SDCard(JSContext * ctx, JSValue _jsobj)
        : NativeClass(ctx,build(ctx,_jsobj))
    {
    }

    JSValue SDCard::constructor(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        auto obj = new SDCard(ctx) ;
        return obj->jsobj ;
    }

    struct sdcard_conf {
        char mount_path[256] ;
        sdmmc_host_t host ;
        sdspi_device_config_t slot_config ;
        esp_vfs_fat_sdmmc_mount_config_t mount_config ;
    } ;

    static void sd_card_task(struct sdcard_conf * conf) {
        
        sdmmc_card_t* card;
        esp_err_t ret = esp_vfs_fat_sdspi_mount(conf->mount_path, &(conf->host), &(conf->slot_config), &(conf->mount_config), &card);

        if (ret == ESP_OK) {
            // dn(card->max_freq_khz)
            sdmmc_card_print_info(stdout, card);
        }
        else {
            if (ret == ESP_FAIL) {
                printf("Failed to mount the card .\n") ;
            } else {
                printf("Failed to initialize the card (%d:%s).\n",ret, esp_err_to_name(ret)) ;
            }
        }

        free(conf) ;
        
        // vTaskDelete(NULL) ;

        // while(1){
        //     vTaskDelay(1000);
        // }
    }

    /**
     * {
     *   spi 
     *   cs 
     *   mount 
     *   khz=20000
     * } 
     */
    JSValue SDCard::setup(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        ASSERT_ARGC(1)

        spi_host_device_t GET_INT32_PROP(argv[0], "spi", spi, )
        gpio_num_t GET_INT32_PROP(argv[0], "cs", cs, )
        int GET_INT32_PROP_OPT(argv[0], "khz", khz, 20000)
        string GET_STR_PROP(argv[0], "mount", mount, )
        mount = FS::toVFSPath(mount.c_str()) ;

        struct sdcard_conf * conf = (struct sdcard_conf*)malloc(sizeof(struct sdcard_conf)) ;
        memset(conf, 0, sizeof(struct sdcard_conf)) ;

        conf->mount_config.format_if_mount_failed = false ;
        conf->mount_config.max_files = 5 ;
        conf->mount_config.allocation_unit_size = 16 * 1024 ;

        // esp_vfs_fat_sdmmc_mount_config_t mount_config = {
        //     .format_if_mount_failed = false,
        //     .max_files = 5,
        //     .allocation_unit_size = 16 * 1024
        // };

        // This initializes the slot without card detect (CD) and write protect (WP) signals.
        // Modify slot_config.gpio_cd and slot_config.gpio_wp if your board has these signals.
        // sdspi_device_config_t slot_config = SDSPI_DEVICE_CONFIG_DEFAULT();
        conf->slot_config = SDSPI_DEVICE_CONFIG_DEFAULT();
        conf->slot_config.gpio_cs = (gpio_num_t)cs;
        conf->slot_config.host_id = (spi_host_device_t)spi;

        conf->host = SDSPI_HOST_DEFAULT();
        // sdmmc_host_t host = SDSPI_HOST_DEFAULT();
        conf->host.slot = spi ;
        conf->host.max_freq_khz = khz ;

        strncpy(conf->mount_path, mount.c_str(),sizeof(conf->mount_path)) ;

        gpio_config_t io_conf = {
            .pin_bit_mask = (1ULL << cs),
            .mode = GPIO_MODE_OUTPUT,
            .pull_up_en = GPIO_PULLUP_DISABLE,
            .pull_down_en = GPIO_PULLDOWN_DISABLE,
            .intr_type = GPIO_INTR_DISABLE,
        };
        gpio_config(&io_conf);


        // xTaskCreatePinnedToCore((TaskFunction_t)sd_card_task, "SD Card Task", 4096, (void*)conf, 5, NULL, 1);
        sd_card_task(conf) ;


    // Card has been initialized, print its properties
    //sdmmc_card_print_info(stdout, card);


        return JS_UNDEFINED ;
    }


}