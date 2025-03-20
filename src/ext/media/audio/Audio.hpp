#pragma once

#include "BeShell.hpp"
#include "NativeModule.hpp"

namespace be::media{
    class Audio: public be::NativeModule {
    public:
        static char const * const name ;
        Audio(JSContext * ctx, const char * name) ;
    } ;
}
