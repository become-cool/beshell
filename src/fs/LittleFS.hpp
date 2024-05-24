#pragma once

#include "FSPartition.hpp"

namespace be {
    class LittleFS: public FSPartition {
    private:
        const char * label = nullptr ;
        bool autoFormat=false ;
        uint16_t blockSize = 4096 ;
    public:
        LittleFS(const char * label, bool _autoFormat=false) ;

        bool mount(const char * mountPoint) ;
        void unmount() ;
        bool usage(size_t & total, size_t & used) ;
    } ;
}