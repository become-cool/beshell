#include "module/serial/I2C.hpp"
using namespace std;

namespace be {

    std::map<JSContext*, NativeClass*> I2C::mapCtxClasses ;

    I2C::I2C(JSContext * ctx, uint8_t _busnum)
        : NativeObject(ctx, classID, "I2C")
        , busnum(_busnum)
    {
        defineClass(JSContext * ctx).create() ;
    }



    NativeClass * I2C::defineClass(JSContext * ctx) {
        if(mapCtxClasses.count(ctx)<1) {
            mapCtxClasses[ctx] = new NativeClass(
                ctx, classID, "I2C"
                , nullptr, 0, nullptr, 0
                , nullptr
                , nullptr
                , nullptr) ;
        }
        return mapCtxClasses[ctx] ;
    }


}