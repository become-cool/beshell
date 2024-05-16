#pragma once

#include "NativeModule.hpp"

namespace be {

    class BT: public be::NativeModule {
    public:
        static const char * const name ;
        BT(JSContext * ctx, const char * name) ;

    } ;

}