#include "driver/display/Bus8080.hpp"
using namespace std;

namespace be {
namespace driver {
namespace display {

    JSClassID Bus8080::classID = 0 ;
    std::map<JSContext*, be::NativeClass*> Bus8080::mapCtxClasses ;

    Bus8080::Bus8080(JSContext * ctx, uint8_t _busnum)
        : NativeObject(ctx, defineClass(ctx))
        , busnum(_busnum)
    {
    }
    be::NativeClass* Bus8080::defineClass(JSContext * ctx) {
        if(mapCtxClasses.count(ctx)<1) {
            mapCtxClasses[ctx] = new be::NativeClass(
                ctx, classID, "Bus8080"
                , nullptr
                , nullptr
                , {}) ;
        }
        return mapCtxClasses[ctx] ;
    }
}}}