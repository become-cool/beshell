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
        static map<std::string, FSPartition *> partitions;
        static std::string pwd ;
#endif
        static std::string prefix ;
        static bool inited ;

    public:
        FS() ;
        ~FS() ;
        
        static void setPrefix(const char * path) ;
        static std::string toVFSPath(const char * path) ;
        static std::string & toVFSPath(std::string & path) ;
        static std::string toVFSPath(JSContext * ctx, JSValue path) ;
        static std::string trimVFSPath(const std::string & path) ;

        static void mount(const char * mountPoint, FSPartition * partition) ;

        static bool setCwd(const std::string &) ;
        static std::string cwd() ;
        static std::string resolve(const std::string & path) ;

        static bool exist(const char * path) ;
        static bool isDir(const char * path) ;
        static bool isFile(const char * path) ;
        static bool rm(const char * path, bool recursive=false) ;
        static bool mkdir(const char * path, bool recursive=false) ;
        static bool touch(const char * path) ;

        static std::unique_ptr<char> readFileSync(const char * path, int * readed=nullptr, int offset=0, int readlen=-1) ;
        static bool writeFileSync(const char * path, const char * data, size_t len, bool append=false) ;
    } ;
}