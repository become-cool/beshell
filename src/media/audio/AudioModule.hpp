#pragma once

#include "BeShell.hpp"
#include "NativeModule.hpp"

namespace be::media{
    class AudioModule: public be::NativeModule {
    public:
        static const char * const name ;
        AudioModule(JSContext * ctx, const char * name) ;
    } ;
}
