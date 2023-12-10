
#pragma once

#include <stddef.h>
#include <stdint.h>

namespace be {
    class FSPartition {
    private:
    protected:
        const char * mountPoint ;
    public:
        virtual bool mount(const char * mountPoint) = 0 ;
        virtual void unmount() = 0 ;
        virtual ~FSPartition() {}

        static void findPartitionByLabel() ;
    } ;
}