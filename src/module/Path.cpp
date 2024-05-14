#include "Path.hpp"



namespace be {
    NativeModule* Path::factory(JSContext * ctx, const char * name) {
        return new Path(ctx, name, 0) ;
    }
    Path::Path(JSContext * ctx, const char * name,uint8_t flagGlobal)
        : NativeModule(ctx, name, flagGlobal)
    {
    }
}