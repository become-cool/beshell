
#pragma once

#include <stddef.h>
#include <stdint.h>

namespace be {
    class FSPartition {
    private:
    protected:
        const char * mountPoint ;
    public:
        virtual ~FSPartition() {}
        virtual bool mount(const char * mountPoint) = 0 ;
        virtual void unmount() = 0 ;
        virtual bool usage(size_t & total, size_t & used) {
            total = 0 ;
            used = 0 ;
            return false ;
        };

        static void findPartitionByLabel() ;
    } ;
}