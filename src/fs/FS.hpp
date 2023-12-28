#pragma once

#include <stddef.h>
#include <stdint.h>
#include <iostream>
#include <string>
#include <memory>
#include <map>


#include "FSPartition.hpp"


using namespace std ;


namespace be {
    class FS {
    private:

#ifdef PLATFORM_ESP32
        map<string, FSPartition *> partitions;
#endif
        string prefix ;

    public:
        FS() ;
        ~FS() ;
        
        void setPrefix(const char * path) ;
        std::string toVFSPath(const char * path) ;
        std::string trimVFSPath(std::string & path) ;

        void mount(const char * mountPoint, FSPartition * partition) ;

        bool exist(const char * path) ;
        bool isDir(const char * path) ;
        bool isFile(const char * path) ;
        bool rm(const char * path, bool recursive=false) ;
        bool mkdir(const char * path, bool recursive=false) ;
        std::string readFileSync(const char * path, int offset=0, int readlen=-1) ;
        bool writeFileSync(const char * path, const char * data, size_t len, bool append=false) ;
    } ;
}