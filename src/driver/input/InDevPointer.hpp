#pragma once

#include <EventEmitter.hpp>
#include "freertos/FreeRTOS.h"
#include "freertos/semphr.h"
#include "freertos/ringbuf.h"

#define INDEVPOINTER_MAX_NUM 10

namespace be::driver::input {

    typedef struct {
        uint8_t pcount = 0 ;
        int16_t x[INDEVPOINTER_MAX_NUM] = {0} ;
        int16_t y[INDEVPOINTER_MAX_NUM] = {0} ;
    } PointsData ;

    class InDevPointer: public be::EventEmitter {
        DECLARE_NCLASS_META
    private:
        static std::vector<JSCFunctionListEntry> methods ;

        PointsData lastData ;

        enum Event {
            PRESSED=1,
            RELEASED=2,
            MOVED=4,
        };
        int enabledEvents = 0 ;

        RingbufHandle_t ringbuf = nullptr ;
        TaskHandle_t taskDaemonHandle = nullptr ;
        static void task_daemon(InDevPointer * indev) ;
        size_t interval_ticks = 50 ;

        inline void _read(PointsData & data) {
            data.pcount = readPointCount() ;
            if(data.pcount > pointsSize) {
                data.pcount = pointsSize ;
            }
            for( int i=0;i<data.pcount;i++ ) {
                readPos(i, data.x[i], data.y[i]);
            }
        }

    protected: 
        uint8_t pointsSize = 0 ;

        void eventAdded(const char * eventName) ;
        void eventRemoved(const char * eventName) ;

    public:
        InDevPointer(JSContext * ctx, JSValue jsobj, uint8_t pointsSize=1) ;

        virtual bool dataReady() = 0 ;
        virtual bool readPos(uint8_t i, int16_t &x, int16_t &y) = 0 ;
        virtual uint8_t readPointCount() = 0 ;

        void read() ;
        inline uint8_t lastPointCount() const {
            return lastData.pcount ;
        } ;
        inline int16_t lastX(uint8_t i=0) const {
            if(i>=lastData.pcount) {
                return 0 ;
            }
            return lastData.x[i] ; 
        } ;
        inline int16_t lastY(uint8_t i=0) const {
            if(i>=lastData.pcount) {
                return 0 ;
            }
            return lastData.y[i] ;
        } ;

        bool _usedForLVGL = false ;
        
        virtual void startDaemon(size_t interval_ms=8, uint8_t core=1) ;
        virtual void stopDaemon() ;
        
        static JSValue setup(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue read(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue readPos(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue readPointCount(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue lastPointCount(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue lastX(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue lastY(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue startDaemon(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;

    } ;
}