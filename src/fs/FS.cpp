#include "FS.hpp"
#include "FSPartition.hpp"
#include <cstring>


#ifdef PLATFORM_ESP32
extern const uint8_t fs_root_img_start[] asm("_binary_fs_root_img_start");
extern const uint8_t fs_root_img_end[] asm("_binary_fs_root_img_end");
#endif


namespace be {
    FS::FS() {

    }
    FS::~FS() {
    }

#ifdef PLATFORM_ESP32
    void FS::mountRootTar() {
        this->mountRaw("/fs", (void*)fs_root_img_start, (size_t)(fs_root_img_end-fs_root_img_start) ) ;
    }

    void FS::mountRaw(const char * mountPoint, void * ptr, size_t size) {
        partitions[mountPoint] = std::make_unique<FSPartitionRaw>(ptr,size) ;
        partitions[mountPoint]->mount(mountPoint) ;
    }

    void FS::mountPartition(const char * path, uint8_t partion_type, uint8_t partion_id) {
        
    }

#endif
    

    void FS::setPrefix(const char * path) {
        prefix = path ;
    }
    std::string FS::toVFSPath(const char * path) {
        std::string p(path) ;
        if(path[0]=='/' && strncmp(path,prefix.c_str(),prefix.length())!=0) {
            p = prefix + p ;
        }
        return p ;
    }
}

