#pragma once

#include <memory>
#include <JSEngine.hpp>
#include <NativeClass.hpp>
#include <misc/Ease.hpp>
#include "widget/Obj.hpp"

namespace be::lv {

    typedef void (*lv_anim_prop_setter)(void *, int32_t);

    class Animation: public be::EventEmitter, public be::ILoopable {
        DECLARE_NCLASS_META
    private:
        static std::vector<JSCFunctionListEntry> methods ;

        Obj * target = nullptr ;
        lv_obj_t * target_lvobj = nullptr ;

        int64_t startTime = 0;

        float duration = 1.0f;
        float startValue = 0.0f;
        float change = 100.0f;
        float ease_s = 0.0f;

        bool running = false ;

        be::misc::EaseFunc easeFunc = nullptr ;
        lv_anim_prop_setter propSetter = nullptr ;

        std::shared_ptr<NativeClass> shared_loopable = nullptr ;

    friend class Obj ;
    public:
        Animation(JSContext * ctx, Obj * target) ;
        // ~Animation() ;

        void start(JSContext * ctx) ;
        void stop(JSContext * ctx) ;
        void loop(JSContext * ctx) ;

        static JSValue start(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue stop(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;

    } ;
}