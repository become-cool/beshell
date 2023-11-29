#include "FS.hpp"
#include "FSPartition.hpp"


#ifdef PLATFORM_ESP32
extern const uint8_t fs_root_img_start[] asm("_binary_fs_root_img_start");
extern const uint8_t fs_root_img_end[] asm("_binary_fs_root_img_end");
#endif


namespace beshell {
    FS::FS() {

    }
    FS::~FS() {
    }

#ifdef PLATFORM_ESP32
    void FS::mountRootTar() {
        this->mountRaw("/fs", (void*)fs_root_img_start, (size_t)(fs_root_img_end-fs_root_img_start) ) ;
    }
#endif

    void FS::mountRaw(const char * mountPoint, void * ptr, size_t size) {
        partitions[mountPoint] = std::make_unique<FSPartitionRaw>(ptr,size) ;
        partitions[mountPoint]->mount(mountPoint) ;
    }

    void FS::mountPartition(const char * path, uint8_t partion_type, uint8_t partion_id) {
        
    }
}

