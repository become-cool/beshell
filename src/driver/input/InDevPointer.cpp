#include "InDevPointer.hpp"

using namespace std ;

    namespace be::driver {

    DEFINE_NCLASS_META(InDevPointer, NativeClass)
    
    std::vector<JSCFunctionListEntry> InDevPointer::methods = {
        JS_CFUNC_DEF("readPos", 0, InDevPointer::readPos),
        JS_CFUNC_DEF("readPointCount", 0, InDevPointer::readPointCount),
    } ;

    InDevPointer::InDevPointer(JSContext * ctx, JSValue jsobj)
        : NativeClass(ctx,jsobj)
    {}

        
    JSValue InDevPointer::readPos(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        THIS_NCLASS(InDevPointer, thisobj)
        ARGV_TO_UINT8_OPT(0, idx, 0)
        uint16_t x=0, y=0 ;
        if( !thisobj->readPos(idx,x,y) ) {
            return JS_NULL ;
        }
        JSValue arr = JS_NewArray(ctx) ;
        JS_SetPropertyUint32(ctx,arr,0,JS_NewUint32(ctx,x)) ;
        JS_SetPropertyUint32(ctx,arr,1,JS_NewUint32(ctx,y)) ;
        return arr ;
    }

    JSValue InDevPointer::readPointCount(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        THIS_NCLASS(InDevPointer, thisobj)
        uint8_t points = thisobj->readPointCount();
        return JS_NewUint32(ctx,points) ;
    }

}