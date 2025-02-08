#include "PN532.hpp"

namespace be::driver::comm {
  
DEFINE_NCLASS_META(PN532, I2CDevice)
std::vector<JSCFunctionListEntry> PN532::methods = {
  JS_CFUNC_DEF("readPassiveTargetID", 0, PN532::readPassiveTargetID),
};


PN532::PN532(JSContext *ctx, JSValue _jsobj)
  : I2CDevice(ctx, build(ctx, _jsobj), 1, 1, PN532_I2C_ADDRESS)
{
}

JSValue PN532::constructor(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    auto obj = new PN532(ctx);
    return obj->jsobj;
}


JSValue PN532::readPassiveTargetID(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    THIS_NCLASS(PN532, thisobj)

    uint8_t uid[] = { 0, 0, 0, 0, 0, 0, 0 };
    uint8_t uidLength;

    if(!thisobj->readPassiveTargetID(PN532_MIFARE_ISO14443A, uid, &uidLength, 100)){
        return JS_NULL ;
    }

    return JS_NewArrayBufferCopy(ctx, (uint8_t*)uid, uidLength);
}

} // be::driver::comm
