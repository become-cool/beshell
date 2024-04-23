#pragma once

#include "driver/I2CDevice.hpp"

namespace be::driver::sys {
    class DS1307: public I2CDevice {
        DECLARE_NCLASS_META
    private:
        static std::vector<JSCFunctionListEntry> methods ;
    public:

        DS1307(JSContext * ctx, JSValue _jsobj=JS_NULL) ;
        static JSValue constructor(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;

        bool read(int & sec, int & min, int & hour, int & day, int & date, int & mth, int & year) ;
        bool set(int sec, int min=-1, int hour=-1, int day=-1, int date=-1, int mth=-1, int year=-1) ;

        static JSValue read(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue set(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
    } ;
}