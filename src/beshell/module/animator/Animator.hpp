#pragma once

#include "../../NativeModule.hpp"

namespace be::animator {

    class Animator: public NativeModule {
    public:
        static char const * const name ;

        Animator(JSContext * ctx, const char * name, uint8_t flagGlobal=0) ;

    protected:
        // 在 Animator-js.cpp 中实现（JS 包装层，导出 tween 对象）
        void exports(JSContext *ctx) override ;

    private:
        static JSValue apiEase(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
    } ;
}
