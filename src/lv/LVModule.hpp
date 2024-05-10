#pragma once

#include "BeShell.hpp"
#include "NativeModule.hpp"
#include "driver/display/DisplayModule.hpp"
#include "deps/lvgl/lvgl.h"

namespace be {
namespace lv {

    class LVModule: public be::NativeModule {
    public:

        static const char * const name ;
        
        LVModule(JSContext * ctx, const char * name) ;

        // void import() ;

        static void use(be::BeShell * beshell) ;

        static JSValue screen(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue load(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue pct(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue registerDisplay(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue registerInputDevice(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue loadFont(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue disableAllInDev(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue enableAllInDev(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;

    } ;

}}
