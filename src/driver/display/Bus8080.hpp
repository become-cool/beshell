#pragma once

#include "BeShell.hpp"
#include "NativeObject.hpp"

namespace be {
namespace driver {
namespace display {

    class Bus8080: public NativeObject {
    private:
        uint8_t busnum = 0 ;
        
        inline static NativeClass* defineClass(JSContext * ctx) ;
    public:
        Bus8080(JSContext * ctx, uint8_t _busnum=0) ;
        static JSClassID classID ;
        static std::map<JSContext*, be::NativeClass*> mapCtxClasses ;

    } ;

}}}