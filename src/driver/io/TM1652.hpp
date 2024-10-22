#pragma once

#include "BeShell.hpp"

namespace be::driver::io {
    class TM1652 {
    public:
        static std::string name ; 
        static void exportDriver(JSContext * ctx) ;
    } ;
}