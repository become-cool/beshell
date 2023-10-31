#include "psram.h"


// #if CONFIG_SPIRAM_SUPPORT || CONFIG_SPIRAM

#include "esp_system.h"
#include "soc/efuse_reg.h"
#include "esp_heap_caps.h"


#ifdef ESP_IDF_VERSION_MAJOR // IDF 4+

    #if CONFIG_IDF_TARGET_ESP32 // ESP32/PICO-D4
        #include "esp32/spiram.h"
    #elif CONFIG_IDF_TARGET_ESP32S2
        #include "esp32s2/spiram.h"
        #include "esp32s2/rom/cache.h"
    #elif CONFIG_IDF_TARGET_ESP32S3
        // #include "esp32s3/spiram.h"
        #include "esp32s3/rom/cache.h"
    #else 
        #error Target CONFIG_IDF_TARGET is not supported
    #endif

#else // ESP32 Before IDF 4.0
    // #include "esp_spiram.h"
#endif




void psram_init() {

    printf("init PSRAM\n");

#ifndef CONFIG_SPIRAM_BOOT_INIT
#if CONFIG_IDF_TARGET_ESP32
    uint32_t chip_ver = REG_GET_FIELD(EFUSE_BLK0_RDATA3_REG, EFUSE_RD_CHIP_VER_PKG);
    uint32_t pkg_ver = chip_ver & 0x7;
    if (pkg_ver == EFUSE_RD_CHIP_VER_PKG_ESP32D2WDQ5 || pkg_ver == EFUSE_RD_CHIP_VER_PKG_ESP32PICOD2) {
        printf("PSRAM not supported!\n");
        return ;
    }
#elif CONFIG_IDF_TARGET_ESP32S2
    extern void esp_config_data_cache_mode(void);
    esp_config_data_cache_mode();
    // Cache_Enable_DCache(0);
#endif
    if (esp_spiram_init() != ESP_OK) {
        printf("PSRAM init failed!\n");
#if CONFIG_IDF_TARGET_ESP32
        // pinMatrixOutDetach(16, false, false);
        // pinMatrixOutDetach(17, false, false);
#endif
        return ;
    }
    esp_spiram_init_cache();
    if (!esp_spiram_test()) {
        printf("PSRAM test failed!\n");
        return ;
    }
    if (esp_spiram_add_to_heapalloc() != ESP_OK) {
        printf("PSRAM could not be added to the heap!\n");
        return ;
    }
#if CONFIG_SPIRAM_MALLOC_ALWAYSINTERNAL && !CONFIG_ARDUINO_ISR_IRAM
        heap_caps_malloc_extmem_enable(CONFIG_SPIRAM_MALLOC_ALWAYSINTERNAL);
#endif
#endif
    printf("PSRAM enabled\n");
    return ;
}


void echo_DMA(const char * where) {
#ifdef PLATFORM_ESP32
    printf( "%s [Free DMA: %d]\n", where, heap_caps_get_free_size(MALLOC_CAP_DMA) ) ;
#endif
}


unsigned int getDMATotal(void) {
    multi_heap_info_t info;
    heap_caps_get_info(&info, MALLOC_CAP_DMA);
    return info.total_free_bytes + info.total_allocated_bytes;
}
unsigned int getDMAUsed(void) {
    multi_heap_info_t info;
    heap_caps_get_info(&info, MALLOC_CAP_DMA);
    return info.total_allocated_bytes;
}

unsigned int getDMAFree(void) {
    return heap_caps_get_free_size(MALLOC_CAP_DMA);
}



unsigned int getHeapTotal(void) {
    multi_heap_info_t info;
    heap_caps_get_info(&info, MALLOC_CAP_INTERNAL);
    return info.total_free_bytes + info.total_allocated_bytes;
}

unsigned int getHeapUsed(void) {
    multi_heap_info_t info;
    heap_caps_get_info(&info, MALLOC_CAP_INTERNAL);
    return info.total_allocated_bytes;
}

unsigned int getHeapFree(void) {
    return heap_caps_get_free_size(MALLOC_CAP_INTERNAL);
}


unsigned int getPsramTotal(void) {
    multi_heap_info_t info;
    heap_caps_get_info(&info, MALLOC_CAP_SPIRAM);
    return info.total_free_bytes + info.total_allocated_bytes;
}


unsigned int getPsramUsed(void) {
    multi_heap_info_t info;
    heap_caps_get_info(&info, MALLOC_CAP_SPIRAM);
    return info.total_allocated_bytes;
}

unsigned int getPsramFree(void) {
    return heap_caps_get_free_size(MALLOC_CAP_SPIRAM);
}

// #endif