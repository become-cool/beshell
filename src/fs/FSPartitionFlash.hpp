#pragma once

#include "FSPartition.hpp"

namespace beshell {
    class FSPartitionFlash: public FSPartition {
    private:
    public:
        FSPartitionFlash(uint8_t partion_type, uint8_t partion_id) ;
        void mount(char * mountPoint) ;
    } ;
}