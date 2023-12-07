#pragma once

#include <stddef.h>
#include <stdint.h>
#include <iostream>
#include <string>
#include <memory>
#include <map>
#include "FSPartitionRaw.hpp"
#include "FSPartitionFlash.hpp"

using namespace std ;



namespace beshell {
    class FS {
    private:

        map<const char *, std::unique_ptr<FSPartition>> partitions;
        string prefix ;

    public:
        FS() ;
        ~FS() ;
#ifdef PLATFORM_ESP32
        void mountRootTar() ;        
#endif
        
        void setPrefix(const char * path) ;
        std::string toVFSPath(const char * path) ;

        void mountRaw(const char * path, void * ptr, size_t size) ;
        void mountPartition(const char * path, uint8_t partion_type, uint8_t partion_id) ;
    } ;
}