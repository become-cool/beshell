#include "Animation.hpp"
#include <cassert>

using namespace std ;

namespace be::lv {
    DEFINE_NCLASS_META(Animation, NativeClass)
    std::vector<JSCFunctionListEntry> Animation::methods = {
        JS_CFUNC_DEF("start", 0, Animation::start),
        JS_CFUNC_DEF("stop", 0, Animation::stop),
    } ;

    Animation::Animation(JSContext * ctx, Obj * target)
        : NativeClass(ctx,build(ctx))
        , target(target)
    {
        assert(target) ;
        target_lvobj = target->lvobj() ;
    }

    void Animation::loop(JSContext * ctx) {
        if(!running){
            return ;
        }
        float t = (float) (gettime()-startTime) ;
        int value = (int) (easeFunc(t, startValue, change, duration, ease_s) + 0.5f);
        propSetter(target_lvobj, value) ;
        if(t >= duration) {
            stop(ctx) ;
        }
    }
    
    void Animation::start(JSContext * ctx) {
        JSEngine::fromJSContext(ctx)->addLoopObject(this, true) ;
        shared_loopable = shared() ;
        startTime = gettime() ;
        running = true ;
    }

    void Animation::stop(JSContext * ctx) {
        JSEngine::fromJSContext(ctx)->removeLoopObject(this) ;
        shared_loopable = nullptr ;
        running = false ;
    }

    JSValue Animation::start(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        THIS_NCLASS(Animation, that)
        that->start(ctx) ;
        return JS_UNDEFINED ;
    }

    JSValue Animation::stop(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        THIS_NCLASS(Animation, that)
        that->stop(ctx) ;
        return JS_UNDEFINED ;
    }
}