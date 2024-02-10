#pragma once

#include "BeShell.hpp"
#include "NativeModule.hpp"

namespace be {
namespace driver {
namespace display {

class DisplayModule: public be::NativeModule {
public:
    DisplayModule(JSContext * ctx, const char * name) ;
} ;

inline static void use(be::BeShell & beshell) {
    beshell.addModule<DisplayModule>("driver/display") ;
}

}}}