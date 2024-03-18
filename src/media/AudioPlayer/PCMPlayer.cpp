#include "PCMPlayer.hpp"

using namespace std ;

namespace be::media {
    DEFINE_NCLASS_META(PCMPlayer, NativeClass)
    std::vector<JSCFunctionListEntry> PCMPlayer::methods = {
        JS_CFUNC_DEF("method", 0, PCMPlayer::jsMethod),
    } ;

    PCMPlayer::PCMPlayer(JSContext * ctx, JSValue _jsobj)
        : NativeClass(ctx,build(ctx,_jsobj))
    {
    }

    JSValue PCMPlayer::constructor(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        auto obj = new PCMPlayer(ctx) ;
        return obj->jsobj ;
    }

    JSValue PCMPlayer::jsMethod(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        return JS_UNDEFINED ;
    }
}