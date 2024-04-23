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

        bool readDayOfWeek(int & dow) ;
        bool setDayOfWeek(int dow) ;
        bool readTime(int & year, int & month, int & day, int & hour, int & minute, int & second) ;
        bool setTime(int year, int month, int day, int hour, int minute, int second) ;

        static JSValue read(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue set(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
    } ;
}