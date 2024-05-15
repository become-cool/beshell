#pragma once

#include "BeShell.hpp"
#include "NativeModule.hpp"
#include "deps/lvgl/lvgl.h"

namespace be {
namespace lv {

    class LV: public be::NativeModule {
    private:
        static esp_timer_handle_t tickTimer;
        static void initTick() ;
        static bool used ;
    public:
        static std::map<std::string, const lv_image_dsc_t  *> embededImages ;

    public:

        static const char * const name ;
        
        LV(JSContext * ctx, const char * name) ;

        // void import() ;
        static void loop(const BeShell &, void *) ;
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

    void addImageDsc(const char * name, const lv_image_dsc_t  *) ;
}}
