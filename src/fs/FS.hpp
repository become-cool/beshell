#pragma once

#include <stddef.h>
#include <stdint.h>
#include <iostream>
#include <string>
#include <memory>
#include <map>


#ifdef PLATFORM_ESP32
#include "FSPartitionRaw.hpp"
#include "FSPartitionFlash.hpp"
#endif

using namespace std ;



namespace beshell {
    class FS {
    private:

#ifdef PLATFORM_ESP32
        map<const char *, std::unique_ptr<FSPartition>> partitions;
#endif
        string prefix ;

    public:
        FS() ;
        ~FS() ;
        
        void setPrefix(const char * path) ;
        std::string toVFSPath(const char * path) ;

#ifdef PLATFORM_ESP32
        void mountRootTar() ;     
        void mountRaw(const char * path, void * ptr, size_t size) ;
        void mountPartition(const char * path, uint8_t partion_type, uint8_t partion_id) ;   
#endif
    } ;
}