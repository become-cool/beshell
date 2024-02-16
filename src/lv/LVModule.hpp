#pragma once

#include "BeShell.hpp"
#include "NativeModule.hpp"
#include "driver/display/DisplayModule.hpp"
#include "lvgl.h"

namespace be {
namespace lv {

    class LVModule: public be::NativeModule {
    public:
        LVModule(JSContext * ctx, const char * name) ;

        // void import() ;

        static void use(be::BeShell & beshell) ;

        static JSValue screen(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue loadScreen(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue test(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;

    } ;

}}
