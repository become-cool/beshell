#pragma once

#include <EventEmitter.hpp>
#include "freertos/FreeRTOS.h"
#include "freertos/semphr.h"
#include "freertos/ringbuf.h"

#define INDEVPOINTER_MAX_NUM 10

namespace be::driver::input {
    class InDevPointer: public be::EventEmitter {
        DECLARE_NCLASS_META
    private:
        static std::vector<JSCFunctionListEntry> methods ;

        uint8_t _pointCount = 0 ;
        uint16_t lastX0 = 0 ;
        uint16_t lastY0 = 0 ;

        enum Event {
            PRESSED=1,
            RELEASED=2,
            MOVED=4,
        };
        int enabledEvents = 0 ;

        Event daemonLastStatus = RELEASED ;
        RingbufHandle_t ringbuf = nullptr ;
        TaskHandle_t taskDaemonHandle = nullptr ;
        bool daemonRunning = false ;
        static void task_daemon(InDevPointer * indev) ;

    protected: 
        void eventAdded(const char * eventName) ;
        void eventRemoved(const char * eventName) ;

    public:
        InDevPointer(JSContext * ctx, JSValue jsobj) ;

        virtual bool dataReady() = 0 ;
        virtual bool readPos(uint8_t i, uint16_t &x, uint16_t &y) = 0 ;
        virtual uint8_t readPointCount() = 0 ;

        void read() ;
        inline int pointCount() { return _pointCount ; } ;
        inline int lastX() { return lastX0 ; } ;
        inline int lastY() { return lastY0 ; } ;

        bool _usedForLVGL = false ;
        
        void startDaemon() ;
        void stopDaemon() ;
        
        static JSValue setup(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue readPos(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue readPointCount(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue lastX(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue lastY(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue startDaemon(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;

    } ;
}