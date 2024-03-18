#include "MP3Player.hpp"

using namespace std ;

namespace be::media {
    DEFINE_NCLASS_META(MP3Player, NativeClass)
    std::vector<JSCFunctionListEntry> MP3Player::methods = {
        JS_CFUNC_DEF("pause", 0, MP3Player::pause),
        JS_CFUNC_DEF("resume", 0, MP3Player::resume),
        JS_CFUNC_DEF("stop", 0, MP3Player::stop),
        JS_CFUNC_DEF("isRunning", 0, MP3Player::isRunning),
        JS_CFUNC_DEF("isPaused", 0, MP3Player::isPaused),
        JS_CFUNC_DEF("detach", 0, MP3Player::detach),
        JS_CFUNC_DEF("printStats", 0, MP3Player::printStats),
    } ;

    MP3Player::MP3Player(JSContext * ctx, JSValue _jsobj)
        : NativeClass(ctx,build(ctx,_jsobj))
    {
    }

    JSValue MP3Player::constructor(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        auto obj = new MP3Player(ctx) ;
        return obj->jsobj ;
    }
    
    JSValue MP3Player::pause(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        return JS_UNDEFINED ;
    }
    JSValue MP3Player::resume(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        return JS_UNDEFINED ;
    }
    JSValue MP3Player::stop(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        return JS_UNDEFINED ;
    }
    JSValue MP3Player::isRunning(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        return JS_UNDEFINED ;
    }
    JSValue MP3Player::isPaused(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        return JS_UNDEFINED ;
    }
    JSValue MP3Player::detach(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        return JS_UNDEFINED ;
    }
    JSValue MP3Player::printStats(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        return JS_UNDEFINED ;
    }
}