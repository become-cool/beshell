
#pragma once

#include <stddef.h>
#include <stdint.h>

namespace beshell {
    class FSPartition {
    private:
    protected:
        const char * mountPoint ;
    public:
        virtual void mount(const char * mountPoint) = 0 ;
    } ;
}