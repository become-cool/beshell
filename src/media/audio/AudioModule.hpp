#pragma once

#include "BeShell.hpp"
#include "NativeModule.hpp"

namespace be::media{
    class AudioModule: public be::NativeModule {
    public:
        AudioModule(JSContext * ctx, const char * name) ;

        inline static void use(be::BeShell & beshell) {
            beshell.useModule<AudioModule>("audio") ;
        }
    } ;
}
