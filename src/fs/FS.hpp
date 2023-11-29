#pragma once

#include <stddef.h>
#include <stdint.h>
#include <iostream>
#include <memory>
#include <map>
#include "FSPartitionRaw.hpp"
#include "FSPartitionFlash.hpp"

using namespace std ;



namespace beshell {
    class FS {
    private:

        map<const char *, std::unique_ptr<FSPartition>> partitions;

    public:
        FS() ;
        ~FS() ;
#ifdef PLATFORM_ESP32
        void mountRootTar() ;        
#endif
        void mountRaw(const char * path, void * ptr, size_t size) ;
        void mountPartition(const char * path, uint8_t partion_type, uint8_t partion_id) ;
    } ;
}