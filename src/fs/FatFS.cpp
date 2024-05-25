#include "FatFS.hpp"
#include <iostream>
#include "qjs_utils.h"
#include "debug.h"

#include "esp_flash.h"
#include "esp_flash_spi_init.h"
#include "esp_partition.h"
#include "esp_vfs.h"
#include "esp_vfs_fat.h"
#include "esp_system.h"
#include "soc/spi_pins.h"
#include "wear_levelling.h"

using namespace std ;

namespace be {
    FatFS::FatFS(const char * label, bool autoFormat) 
        : label(label)
        , autoFormat(autoFormat)
    {}

    bool FatFS::mount(const char * mountPoint) {

        const esp_vfs_fat_mount_config_t mount_config = {
                .format_if_mount_failed = autoFormat,
                .max_files = 4,
                .allocation_unit_size = 4096,
        };

        esp_err_t err = esp_vfs_fat_spiflash_mount(mountPoint, label, &mount_config, &wl_handle);
        if (err != ESP_OK) {
            return false;
        }

        this->mountPoint = mountPoint ;
        return true;
    }
    void FatFS::unmount() {
        if(wl_handle==0) {
            return ;
        }
        esp_vfs_fat_spiflash_unmount(mountPoint, wl_handle) ;
        mountPoint = nullptr ;
        wl_handle = 0 ;
    }
    
    bool FatFS::usage(size_t & total, size_t & used) {
        // return esp_FatFS_info(label, &total, &used) == ESP_OK ;
        return false ;
    };
}