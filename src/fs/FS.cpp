#include "FS.hpp"
#include <cstring>
#include <sys/stat.h>
#include <unistd.h>
#include <dirent.h>
#include <cstring>
#include "debug.h"

#ifdef PLATFORM_ESP32
#include "RawFS.hpp"
#endif


#ifdef PLATFORM_ESP32
extern const uint8_t fs_root_img_start[] asm("_binary_fs_root_img_start");
extern const uint8_t fs_root_img_end[] asm("_binary_fs_root_img_end");
#endif


namespace be {

    inline static bool isDir(const char * path) {
        struct stat statbuf;
        if(stat(path,&statbuf)>=0) {
            return S_ISDIR(statbuf.st_mode)? true: false ;
        }
        return false ;
    }
    inline static bool isFile(const char * path) {
        struct stat statbuf;
        if(stat(path,&statbuf)>=0) {
            return S_ISREG(statbuf.st_mode)? true: false ;
        }
        return false ;
    }

    // 递归创建目录
    int mkdir_p(const char * file_path) {
        if( !file_path || !(*file_path) )
            return -1 ;
        char* p = strchr((char *)(file_path + 1), '/') ;
        while (p!=NULL) {
            *p = '\0';
#ifdef WIN32
            if (mkdir(file_path) == -1) {
#else
            if (mkdir(file_path, ACCESSPERMS) == -1) {
#endif
                if (errno != EEXIST) {
                    *p = '/';
                    return -1;
                }
            }
            *p = '/';
            p = strchr(p + 1, '/') ;
        }

        if( !isDir(file_path) ) {
#ifdef WIN32
            return mkdir(file_path) ;
#else
            return mkdir(file_path, ACCESSPERMS) ;
#endif
        }
        else 
            return 0 ;
    }


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
        partition->mount(point.c_str()) ;
#endif
    }
    

    void FS::setPrefix(const char * path) {
        prefix = path ;
    }
    std::string FS::toVFSPath(const char * path) {
        std::string p(path) ;
        if(path[0]=='/' && strncmp(path,prefix.c_str(),prefix.length())!=0) {
            if(p.size()>1) {
                p = prefix + p ;
            }
            else {
                p = prefix ;
            }
        }
        return p ;
    }

    bool FS::exist(const char * path) {
        string _path = toVFSPath(path) ;
        struct stat statbuf;
        return stat(_path.c_str(),&statbuf)>=0 ;
    }
    bool FS::isDir(const char * path) {
        return be::isDir(path) ;
    }
    bool FS::isFile(const char * path) {
        return be::isFile(path) ;
    }

    bool FS::rm(const char * _path, bool recursive) {

        string path = toVFSPath(_path) ;
        string childpath ;

        struct stat statbuf;

        // 文件不存在
        if(stat(path.c_str(),&statbuf)<0) {
            return true ;
        }

        // 直接删除文件
        if(S_ISREG(statbuf.st_mode)) {
            return unlink(path.c_str())>-1 ;
        }

        // 目录
        else if(S_ISDIR(statbuf.st_mode)) {
            // 递归删除文件/子目录
            if(recursive) {
                DIR* dir = opendir(path.c_str());
                if(dir) {
                    struct dirent *dirEnt;
                    while((dirEnt = readdir(dir))) {
                        if(strcmp(dirEnt->d_name, ".")==0 || strcmp(dirEnt->d_name, "..")==0) { 
                            continue ;
                        }
                        childpath = path + "/" + dirEnt->d_name ;    
                        rm(childpath.c_str(), recursive) ;
                    }
                }
            }

            // printf("rmdir(%s)\n",path) ;
            return rmdir(path.c_str()) > -1 ;
        }

        // unknow type? soft link ?
        return false ;
    }

    bool FS::mkdir(const char * _path, bool recursive) {
        struct stat statbuf;
        if(stat(_path,&statbuf)>=0) {
            return S_ISDIR(statbuf.st_mode)? true: false ;
        }
        return (recursive? mkdir_p(_path): mkdir(_path, ACCESSPERMS)) == 0 ;
    }
}

