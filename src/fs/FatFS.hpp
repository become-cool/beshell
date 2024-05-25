#pragma once

#include "FSPartition.hpp"

namespace be {
    class FatFS: public FSPartition {
    private:
        const char * label = nullptr ;
        bool autoFormat=false ;
        uint16_t blockSize = 4096 ;
        int32_t wl_handle = 0;
        const char * mountPoint = nullptr ;
    public:
        FatFS(const char * label, bool _autoFormat=false) ;

        bool mount(const char * mountPoint) ;
        void unmount() ;
        bool usage(size_t & total, size_t & used) ;
    } ;
}