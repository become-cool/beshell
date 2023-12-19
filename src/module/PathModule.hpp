#pragma once
#include "NativeModule.hpp"

namespace be {
    class PathModule: public NativeModule {
    private:
    protected:
        void defineExports() ;
    public:
        PathModule() ;
    } ;
}