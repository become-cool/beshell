#pragma once

#include "BeShell.hpp"
#include "NativeModule.hpp"

namespace be::media{
    class AudioPlayerModule: public be::NativeModule {
    public:
        AudioPlayerModule(JSContext * ctx, const char * name) ;

        // void import(JSContext *ctx) ;

        inline static void use(be::BeShell & beshell) {
            beshell.addModule<AudioPlayerModule>("audio") ;
        }
    } ;
}
