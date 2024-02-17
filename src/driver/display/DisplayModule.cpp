#include "DisplayModule.hpp"
#include "RGB565.hpp"
#include "ST7701.hpp"

using namespace std ;

namespace be::driver::display {

    DisplayModule::DisplayModule(JSContext * ctx, const char * name)
        : NativeModule(ctx, name, 0)
    {
        exportClass<RGB565>() ;
        exportClass<ST7701>() ;
        exportFunction("rgb", jsFromRGB) ;
        exportFunction("rgb565", jsFromRGB565) ;
        exportFunction("toRGB", jsToRGB) ;
        exportFunction("toRGB565", jsToRGB565) ;
    }

    uint16_t fromRGB(uint8_t r,uint8_t g,uint8_t b) {
        r = r*((float)31/255) + 0.5 ;
        g = g*((float)63/255) + 0.5 ;
        b = b*((float)31/255) + 0.5 ;
        return fromRGB565(r,g,b) ;
    }
    uint16_t fromRGB565(uint8_t r,uint8_t g,uint8_t b) {
        return ((r&31)<<11) | ((g&63)<<5) | (b&31) ;
    }

    void toRGB(uint16_t value, uint8_t * r,uint8_t * g,uint8_t * b) {
        toRGB565(value,r,g,b) ;
        *r = (uint8_t) (((float)*r/31) * 255 + 0.5) ;
        *g = (uint8_t) (((float)*g/63) * 255 + 0.5) ;
        *b = (uint8_t) (((float)*b/31) * 255 + 0.5) ;
    }
    void toRGB565(uint16_t value, uint8_t * r,uint8_t * g,uint8_t * b) {
        *r = (value>>11) & 31 ;
        *g = (value>>5) & 63 ;
        *b = value & 31 ;
    }

    JSValue DisplayModule::jsFromRGB(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        CHECK_ARGC(3)
        ARGV_TO_UINT8(0,r)
        ARGV_TO_UINT8(1,g)
        ARGV_TO_UINT8(2,b)
        return JS_NewUint32(ctx, fromRGB(r,g,b)) ;
    }
    JSValue DisplayModule::jsFromRGB565(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        CHECK_ARGC(3)
        ARGV_TO_UINT8(0,r)
        ARGV_TO_UINT8(1,g)
        ARGV_TO_UINT8(2,b)
        return JS_NewUint32(ctx, fromRGB565(r,g,b)) ;
    }

    JSValue DisplayModule::jsToRGB(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        return JS_UNDEFINED ;
    }
    JSValue DisplayModule::jsToRGB565(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        return JS_UNDEFINED ;
    }
}