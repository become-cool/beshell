#pragma once
#include "NativeModule.hpp"

namespace be {
    class Path: public NativeModule {
    public:
        Path(JSContext * ctx, const char * name,uint8_t flagGlobal) ;
        static NativeModule* factory(JSContext * ctx, const char * name) ;
    } ;
}