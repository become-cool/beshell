#pragma once

#include "BeShell.hpp"

namespace be::driver::io {
    class PCA9557 {
    public:
        static std::string name ; 
        static void exportDriver(JSContext * ctx) ;
    } ;
}