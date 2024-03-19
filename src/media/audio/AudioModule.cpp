#include "AudioModule.hpp"
#include "MusicPlayer.hpp"


namespace be::media{
    AudioModule::AudioModule(JSContext * ctx, const char * name)
        : NativeModule(ctx, name, 0)
    {
        exportClass<MusicPlayer>();
    }
}