#include "LittleFS.hpp"
#include <iostream>
#include "debug.h"
#include "esp_vfs.h"
#include "esp_partition.h"
#include "deps/littlefs/src/esp_littlefs.h"

using namespace std ;

namespace be {
    LittleFS::LittleFS(const char * _label, bool _autoFormat) 
        : label(_label)
        , autoFormat(_autoFormat)
    {
    }

    bool LittleFS::mount(const char * mountPoint) {
        dn(blockSize) 

        const esp_vfs_littlefs_conf_t conf = {
            .base_path = mountPoint,
            .partition_label = label,
            .format_if_mount_failed = autoFormat ,
            .block_size = blockSize
        };
        if(esp_vfs_littlefs_register(&conf)!=ESP_OK){
            printf("Failed to mount %s as LittleFS.\n", mountPoint) ;
            return false ;
        }
        this->mountPoint = mountPoint ;
        cout << mountPoint << endl ;
        return true ;
    }
    void LittleFS::unmount() {
        cout << "not implements" <<endl ;
    }
}