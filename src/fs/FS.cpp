#include "FS.hpp"
#include <cstring>
#include <sys/stat.h>
#include <unistd.h>
#include <dirent.h>
#include <cstring>
#include "debug.h"
#include "path.hpp"
#include <unistd.h>

#ifdef ESP_PLATFORM
#include "RawFS.hpp"
extern const uint8_t fs_root_img_start[] asm("_binary_fs_root_img_start");
extern const uint8_t fs_root_img_end[] asm("_binary_fs_root_img_end");
#endif


#ifdef LINUX_PLATFORM
#include <utime.h>
#endif


#ifdef LINUX_PLATFORM
#include <utime.h>
#include <sys/types.h>
#endif

using namespace std ;


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
#ifdef ESP_PLATFORM
        setPrefix("/fs") ;
#else
#endif
    }
    FS::~FS() {
#ifdef ESP_PLATFORM
        for(auto it=partitions.begin(); it!=partitions.end(); ++it) {
            it->second->unmount() ;
            delete it->second ;
        }
        partitions.clear() ;
#endif
    }

    void FS::mount(const char * mountPoint, FSPartition * partition) {
#ifdef ESP_PLATFORM
        string point = toVFSPath(mountPoint) ;
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
    std::string & FS::toVFSPath(std::string & path) {
        if( path[0]=='/' && path.compare(0, prefix.size(), prefix)!=0 ) {
            path = prefix + path ;
        }
        return path ;
    }
    std::string FS::trimVFSPath(const std::string & path) {
        if (path.substr(0, prefix.size())==prefix) {
            if(path.size()==prefix.size() || path[prefix.size()]=='/') {
                return path.substr(prefix.size()) ;
            }
        }
        return path ;
    }

    bool FS::exist(const char * path) {
        string _path = toVFSPath(path) ;
        struct stat statbuf;
        return stat(_path.c_str(),&statbuf)>=0 ;
    }
    bool FS::isDir(const char * path) {
        toVFSPath(path) ;
        return be::isDir(path) ;
    }
    bool FS::isFile(const char * path) {
        toVFSPath(path) ;
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
        string path = toVFSPath(_path) ;
        struct stat statbuf;
        if(stat(path.c_str(),&statbuf)>=0) {
            return S_ISDIR(statbuf.st_mode)? true: false ;
        }
        if(recursive) {
            return mkdir_p(path.c_str())==0 ;
        } else {
            return mkdir(path.c_str(), ACCESSPERMS) ;
        }
    }
    
    std::unique_ptr<char> FS::readFileSync(const char * cpath, int * readed, int offset, int readlen) {

        std::string path = toVFSPath(cpath) ;
        std::unique_ptr<char> ret ;

        if(readed) {
            *readed = 0 ;
        }

        struct stat statbuf;
        if(stat(path.c_str(),&statbuf)<0) {
            // "path invalid" ;
            if(readed) {
                *readed = -1 ;
            }
            return ret ;
        }
        if(S_ISDIR(statbuf.st_mode)) {
            // "path is a dir" ;
            if(readed) {
                *readed = -2 ;
            }
            return ret ;
        }
        if(readlen<0) {
            readlen = statbuf.st_size ;
        }
        if(readlen<1){
            return ret ;
        }
        
        FILE * fd = fopen(path.c_str(), "r");
        if(NULL==fd) {
            // "can not open file" ;
            if(readed) {
                *readed = -3 ;
            }
            return ret ;
        }

        char * buff = new char[readlen] ;

        if(offset>0) {
            fseek(fd, offset, SEEK_SET) ;
        }

        size_t filelen = fread(buff, 1, readlen, fd) ;
        fclose(fd) ;

        if(readed) {
            * readed = filelen ;
        }
        ret.reset(buff) ;
        return ret ;
    }
    bool FS::writeFileSync(const char * cpath, const char * data, size_t len, bool append) {
        if(data) {
            return false ;
        }

        std::string path = toVFSPath(cpath) ;

        FILE * fd = fopen(path.c_str(), append? "a+": "w");
        if(NULL==fd) {
            return false ;
        }

        fwrite(data, 1, len, fd);
        fclose(fd) ;
        
        return true ;
    }

    bool FS::touch(const char * _path) {
        string path = toVFSPath(_path) ;
        FILE *file = fopen(path.c_str(), "a");
        if (file == NULL) {
            return false ;
        }
        fclose(file);

#ifdef LINUX_PLATFORM
        struct utimbuf new_times;
        new_times.actime = time(NULL);
        new_times.modtime = time(NULL);
        if (utime(path.c_str(), &new_times) != 0) {
            return false ;
        }
#endif
        return true ;
    }

    bool FS::setCwd(const std::string & path) {
#ifdef ESP_PLATFORM
        pwd = trimVFSPath(path) ;
        path_normalize(pwd) ;
        return true ;
#else
        return chdir(path.c_str())==0 ;
#endif
    }
    std::string FS::cwd() {
#ifdef ESP_PLATFORM
        return pwd ;
#else
        char path[256] ;
        if( getcwd(path, sizeof(path)) == nullptr ){
            return "" ;
        } else {
            return path ;
        }
#endif
    }
    std::string FS::resolve(const std::string & path) {
        if(path.length()<1) {
            return cwd().c_str() ;
        }
        string _path ;
        if( path[0] != '/' ) {
            _path+= cwd() + "/" + path ;
        } else {
            _path = path ;
        }
        return path_normalize(_path) ;
    }
}

