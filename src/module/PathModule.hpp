#pragma once
#include "NativeModule.hpp"

namespace be {
    class PathModule: public NativeModule {
    public:
        PathModule(JSContext * ctx, const char * name,uint8_t flagGlobal) ;
        static NativeModule* factory(JSContext * ctx, const char * name) ;
    } ;
}