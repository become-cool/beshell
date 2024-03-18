#include "AudioPlayerModule.hpp"
#include "MP3Player.hpp"
#include "PCMPlayer.hpp"


namespace be::media{
    AudioPlayerModule::AudioPlayerModule(JSContext * ctx, const char * name)
        : NativeModule(ctx, name, 0)
    {
        exportClass<MP3Player>();
        exportClass<PCMPlayer>();
    }
}