#pragma once
#include "BeShell.hpp"
#include "NativeModule.hpp"

namespace be {
namespace driver {
namespace display {
    
    class DisplayModule: public NativeModule {
    private:
    public:
        using NativeModule::NativeModule ;
        static NativeModule* factory(JSContext * ctx, const char * name) ;
        
        static void use(const be::BeShell & beshell) ;
    } ;
}}}