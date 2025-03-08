#pragma once

#include "../../I2CDevice.hpp"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"


namespace be::driver::sensor {
    
    class QMI8658: public be::driver::I2CDevice {
        DECLARE_NCLASS_META

    public:
        QMI8658(JSContext * ctx, JSValue _jsobj=JS_NULL) ;
        static JSValue constructor(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;

        virtual int setup() {
            return I2CDevice::setup() ;
        }
        static JSValue setup(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue readXYZ(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;

        static JSValue startReceive(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue stopReceive(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
    
        

    private:
        static std::vector<JSCFunctionListEntry> methods ;

        static void task_listen(QMI8658 * arg) ;
        static void task_listen2(QMI8658 * arg) ;
        TaskHandle_t taskListenerHandle = nullptr ;
    } ;
}