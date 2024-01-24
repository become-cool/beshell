#include "PathModule.hpp"



namespace be {
    NativeModule* PathModule::factory(JSContext * ctx, const char * name) {
        return new PathModule(ctx, name, 0) ;
    }
    PathModule::PathModule(JSContext * ctx, const char * name,uint8_t flagGlobal)
        : NativeModule(ctx, name, flagGlobal)
    {
    }
}