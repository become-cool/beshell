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

        void mount(const char * mountPoint, FSPartition * partition) ;

        bool exist(const char * path) ;
        bool isDir(const char * path) ;
        bool isFile(const char * path) ;
        bool rm(const char * path, bool recursive) ;
        bool mkdir(const char * path, bool recursive) ;
    } ;
}