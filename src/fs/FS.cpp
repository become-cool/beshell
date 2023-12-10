#include "FS.hpp"
#include <cstring>
#include <sys/stat.h>
#include "debug.h"

#ifdef PLATFORM_ESP32
#include "RawFS.hpp"
#endif


#ifdef PLATFORM_ESP32
extern const uint8_t fs_root_img_start[] asm("_binary_fs_root_img_start");
extern const uint8_t fs_root_img_end[] asm("_binary_fs_root_img_end");
#endif


namespace be {
    FS::FS() {
#ifdef PLATFORM_ESP32
        setPrefix("/fs") ;
#else
#endif
    }
    FS::~FS() {
#ifdef PLATFORM_ESP32
        for(auto it=partitions.begin(); it!=partitions.end(); ++it) {
            it->second->unmount() ;
            delete it->second ;
        }
        partitions.clear() ;
#endif
    }

    void FS::mount(const char * mountPoint, FSPartition * partition) {
#ifdef PLATFORM_ESP32
        string point = toVFSPath(mountPoint) ;
        cout << point << endl ;
        partitions[point] = partition ;
        partition->mount(mountPoint) ;
#endif
    }
    

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

    bool FS::exist(const char * path) {
        string _path = toVFSPath(path) ;
        struct stat statbuf;
        return stat(_path.c_str(),&statbuf)>=0 ;
    }
    bool FS::isDir(const char * path) {
        string _path = toVFSPath(path) ;
        struct stat statbuf;
        if(stat(_path.c_str(),&statbuf)>=0) {
            return S_ISDIR(statbuf.st_mode)? true: false ;
        }
        return false ;
    }
    bool FS::isFile(const char * path) {
        string _path = toVFSPath(path) ;
        struct stat statbuf;
        if(stat(_path.c_str(),&statbuf)>=0) {
            return S_ISREG(statbuf.st_mode)? true: false ;
        }
        return false ;
    }


}

