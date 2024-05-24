#include "InDevPointer.hpp"
#include "qjs_utils.h"

using namespace std ;

    namespace be::driver {

    DEFINE_NCLASS_META(InDevPointer, EventEmitter)
    
    std::vector<JSCFunctionListEntry> InDevPointer::methods = {
        JS_CFUNC_DEF("readPos", 0, InDevPointer::readPos),
        JS_CFUNC_DEF("readPointCount", 0, InDevPointer::readPointCount),
        JS_CFUNC_DEF("lastX", 0, InDevPointer::lastX),
        JS_CFUNC_DEF("lastY", 0, InDevPointer::lastY),
        JS_CFUNC_DEF("startDaemon", 0, InDevPointer::startDaemon),
    } ;

    InDevPointer::InDevPointer(JSContext * ctx, JSValue jsobj)
        : EventEmitter(ctx,jsobj)
    {}

        
    JSValue InDevPointer::readPos(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        THIS_NCLASS(InDevPointer, that)
        ARGV_TO_UINT8_OPT(0, idx, 0)
        uint16_t x=0, y=0 ;
        if( !that->readPos(idx,x,y) ) {
            return JS_NULL ;
        }
        JSValue arr = JS_NewArray(ctx) ;
        JS_SetPropertyUint32(ctx,arr,0,JS_NewUint32(ctx,x)) ;
        JS_SetPropertyUint32(ctx,arr,1,JS_NewUint32(ctx,y)) ;
        return arr ;
    }

    JSValue InDevPointer::readPointCount(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        THIS_NCLASS(InDevPointer, that)
        uint8_t points = that->readPointCount();
        return JS_NewUint32(ctx,points) ;
    }
    JSValue InDevPointer::lastX(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        THIS_NCLASS(InDevPointer, that)
        return JS_NewInt32(ctx,that->lastX0) ;
    }
    JSValue InDevPointer::lastY(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        THIS_NCLASS(InDevPointer, that)
        return JS_NewInt32(ctx,that->lastY0) ;
    }

    void InDevPointer::read() {

        int pcnt = 0 ;

        if(daemonRunning) {

        } else {
            pcnt = readPointCount() ;
            if( pcnt>0 ) {
                readPos(0, lastX0, lastY0);
            }
        }

        if(enabledEvents>0 && pcnt!=_pointCount) {
            if( pcnt>0 && enabledEvents&PRESSED ) {
                JSValue jsname = JS_NewString(ctx,"pressed") ;
                emitSync(jsname,{}) ;
                JS_FreeValue(ctx,jsname) ;
            }
            else if( pcnt==0 && enabledEvents&RELEASED ) {
                JSValue jsname = JS_NewString(ctx,"released") ;
                emitSync(jsname,{}) ;
                JS_FreeValue(ctx,jsname) ;
            }
        }
        _pointCount = pcnt ;
    }

    
    void InDevPointer::eventAdded(const char * eventName) {
        if(strcmp(eventName,"released")==0){
            enabledEvents|= RELEASED ;
        } else if(strcmp(eventName,"pressed")==0){
            enabledEvents|= PRESSED ;
        } else if(strcmp(eventName,"moved")==0){
            enabledEvents|= MOVED ;
        }
    }

    void InDevPointer::eventRemoved(const char * eventName) {
        if(strcmp(eventName,"released")==0){
            enabledEvents&= ~RELEASED ;
        } else if(strcmp(eventName,"pressed")==0){
            enabledEvents&= ~PRESSED ;
        } else if(strcmp(eventName,"moved")==0){
            enabledEvents&= ~MOVED ;
        }
    }

    // @todo
    // 后台进程，定时读取点坐标，并发送事件 (未完成)
    JSValue InDevPointer::startDaemon(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        THIS_NCLASS(InDevPointer, that)
        that->startDaemon() ;
        return JS_UNDEFINED ;
    }

    void InDevPointer::startDaemon() {
        if(daemonRunning) {
            return ;
        }
        ringbuf = xRingbufferCreate(10,RINGBUF_TYPE_NOSPLIT) ;
        xTaskCreatePinnedToCore((TaskFunction_t)task_daemon, "task_daemon_indev", 1024*2, (void *)this, 5, &taskDaemonHandle, 1) ;
    }
    void InDevPointer::stopDaemon() {
        if(!daemonRunning) {
            return ;
        }
        if(taskDaemonHandle) {
            vTaskDelete(taskDaemonHandle) ;
            taskDaemonHandle = nullptr ;
        }
        if(ringbuf) {
            vRingbufferDelete(ringbuf) ;
            ringbuf = nullptr ;
        }
        daemonRunning = false ;
    }
    void InDevPointer::task_daemon(InDevPointer * indev) {
        while(1) {

            Event status = indev->readPointCount()>0? PRESSED: RELEASED;
            if( indev->daemonLastStatus != status ){
                dn(status)
            }

            vTaskDelay(10/portTICK_PERIOD_MS) ;
        }
    }
}