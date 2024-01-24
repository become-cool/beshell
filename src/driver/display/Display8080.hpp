#pragma once

#include "BeShell.hpp"
#include "Display.hpp"

namespace be {
namespace driver {
namespace display {

    class Display8080: public Display {
    private:
    public:
        Display8080(JSContext * ctx) ;
        static void defineClass(NativeClass *) ;
        static JSClassID classID ;

        static void use(const be::BeShell & beshell) ;
        
    private:
        static JSValue jsConstructor(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static void jsFinalizer(JSRuntime *rt, JSValue val) ;
    } ;
}}}