#pragma once

#include <stddef.h>
#include <stdint.h>
#include <iostream>
#include <string>
#include <memory>
#include <map>
#include <quickjs/quickjs.h>


#include "FSPartition.hpp"

using namespace std ;

namespace be {
    class FS {
    private:

#ifdef ESP_PLATFORM
        std::map<std::string, FSPartition *> partitions;
        std::string pwd = "/" ;
#endif
        std::string prefix ;

    public:
        FS() ;
        ~FS() ;
        
        void setPrefix(const char * path) ;
        std::string toVFSPath(const char * path) ;
        std::string & toVFSPath(std::string & path) ;
        static std::string toVFSPath(JSContext * ctx, const char * path) ;
        static std::string toVFSPath(JSContext * ctx, JSValue path) ;
        std::string trimVFSPath(const std::string & path) ;

        void mount(const char * mountPoint, FSPartition * partition) ;

        bool setCwd(const std::string &) ;
        std::string cwd() ;
        std::string resolve(const std::string & path) ;

        bool exist(const char * path) ;
        bool isDir(const char * path) ;
        bool isFile(const char * path) ;
        bool rm(const char * path, bool recursive=false) ;
        bool mkdir(const char * path, bool recursive=false) ;
        bool touch(const char * path) ;

        std::unique_ptr<char> readFileSync(const char * path, int * readed=nullptr, int offset=0, int readlen=-1) ;
        bool writeFileSync(const char * path, const char * data, size_t len, bool append=false) ;
    } ;
}