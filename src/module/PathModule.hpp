#pragma once
#include "NativeModule.hpp"

namespace be {
    class PathModule: public NativeModule {
    private:
    protected:
        void defineExports() ;
    public:
        using NativeModule::NativeModule;
        static NativeModule* factory(JSContext * ctx, const char * name) ;
    } ;
}