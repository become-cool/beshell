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

using namespace std ;

namespace be::driver {
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

    /**
     * {
     *   spi 
     *   cs 
     *   mount 
     *   khz=20000
     * } 
     */
    JSValue SDCard::setup(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        CHECK_ARGC(1)

        spi_host_device_t GET_INT32_PROP(argv[0], "spi", spi, )
        gpio_num_t GET_INT32_PROP(argv[0], "cs", cs, )
        int GET_INT32_PROP_OPT(argv[0], "khz", khz, 20000)
        string GET_STR_PROP(argv[0], "mount", mount, )
        mount = FS::toVFSPath(ctx,mount.c_str()) ;

        esp_err_t ret;
        esp_vfs_fat_sdmmc_mount_config_t mount_config = {
            .format_if_mount_failed = false,
            .max_files = 5,
            .allocation_unit_size = 16 * 1024
        };

        // This initializes the slot without card detect (CD) and write protect (WP) signals.
        // Modify slot_config.gpio_cd and slot_config.gpio_wp if your board has these signals.
        sdspi_device_config_t slot_config = SDSPI_DEVICE_CONFIG_DEFAULT();
        slot_config.gpio_cs = (gpio_num_t)cs;
        slot_config.host_id = (spi_host_device_t)spi;

        sdmmc_card_t* card;
        sdmmc_host_t host = SDSPI_HOST_DEFAULT();
        host.slot = spi ;
        host.max_freq_khz = khz ;

        ret = esp_vfs_fat_sdspi_mount(mount.c_str(), &host, &slot_config, &mount_config, &card);

        if (ret == ESP_OK) {
            // dn(card->max_freq_khz)
            sdmmc_card_print_info(stdout, card);
        }
        else {
            if (ret == ESP_FAIL) {
                JSTHROW("Failed to mount the card .\n")
            } else {
                JSTHROW("Failed to initialize the card (%d:%s).\n",ret, esp_err_to_name(ret))
            }
        }

    // Card has been initialized, print its properties
    //sdmmc_card_print_info(stdout, card);


        return JS_TRUE ;
    }


}