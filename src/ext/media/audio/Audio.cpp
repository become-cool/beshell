#include "Audio.hpp"
#include "AudioPlayer.hpp"


namespace be::media{

    char const * const Audio::name = "audio" ;

    Audio::Audio(JSContext * ctx, const char * name)
        : NativeModule(ctx, name, 0)
    {
        exportClass<AudioPlayer>();
    }
}