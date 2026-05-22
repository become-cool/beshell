#include "InDevPointer.hpp"
#include "qjs_utils.h"

using namespace std ;

    namespace be::driver::input {

    DEFINE_NCLASS_META(InDevPointer, EventEmitter)
    
    std::vector<JSCFunctionListEntry> InDevPointer::methods = {
        JS_CFUNC_DEF("setup", 0, InDevPointer::setup),
        JS_CFUNC_DEF("read", 0, InDevPointer::read),
        JS_CFUNC_DEF("readPos", 0, InDevPointer::readPos),
        JS_CFUNC_DEF("readPointCount", 0, InDevPointer::readPointCount),
        JS_CFUNC_DEF("lastPointCount", 0, InDevPointer::lastPointCount),
        JS_CFUNC_DEF("lastX", 0, InDevPointer::lastX),
        JS_CFUNC_DEF("lastY", 0, InDevPointer::lastY),
        JS_CFUNC_DEF("startDaemon", 0, InDevPointer::startDaemon),
    } ;

    InDevPointer::InDevPointer(JSContext * ctx, JSValue jsobj, uint8_t psize)
        : EventEmitter(ctx,jsobj)
    {
        if(psize>INDEVPOINTER_MAX_NUM) {
            psize = INDEVPOINTER_MAX_NUM ;
        }
        pointsSize = psize ;
    }

    JSValue InDevPointer::setup(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        THIS_NCLASS(InDevPointer, thisobj)
        ASSERT_ARGC(1)

        JSValue daemon = JS_GetPropertyStr(ctx, argv[0], "daemon") ;
        if( JS_IsObject(daemon) ) {

            uint32_t GET_UINT32_PROP_OPT(daemon, "dur", dur, 8)
            uint32_t GET_UINT32_PROP_OPT(daemon, "core", core, 1)

            if(dur>1000){
                JS_FreeValue(ctx,daemon) ;
                JSTHROW("Input device daemon duration should be less than 1000ms.") ;
            }
            if(core>portNUM_PROCESSORS){
                JS_FreeValue(ctx,daemon) ;
                JSTHROW("Input device daemon core should be less than the number of processors.") ;
            }

            thisobj->startDaemon(dur,core) ;
        }
        else {
            JSTHROW("daemon property should be an object.")
        }
        JS_FreeValue(ctx,daemon) ;

        return JS_UNDEFINED ;
    }
        
    JSValue InDevPointer::readPos(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        THIS_NCLASS(InDevPointer, that)
        ARGV_TO_UINT8_OPT(0, idx, 0)
        int16_t x=0, y=0 ;
        if( !that->readPos(idx,x,y) ) {
            return JS_NULL ;
        }
        JSValue arr = JS_NewArray(ctx) ;
        JS_SetPropertyUint32(ctx,arr,0,JS_NewUint32(ctx,x)) ;
        JS_SetPropertyUint32(ctx,arr,1,JS_NewUint32(ctx,y)) ;
        return arr ;
    }

    JSValue InDevPointer::read(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        THIS_NCLASS(InDevPointer, that)
        that->read();
        return JS_NewUint32(ctx,that->lastData.pcount) ;
    }
    JSValue InDevPointer::readPointCount(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        THIS_NCLASS(InDevPointer, that)
        uint8_t points = that->readPointCount();
        return JS_NewUint32(ctx,points) ;
    }
    JSValue InDevPointer::lastPointCount(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        THIS_NCLASS(InDevPointer, that)
        return JS_NewInt32(ctx,that->lastData.pcount) ;
    }
    JSValue InDevPointer::lastX(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        THIS_NCLASS(InDevPointer, that)
        return JS_NewInt32(ctx,that->lastData.x[0]) ;
    }
    JSValue InDevPointer::lastY(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        THIS_NCLASS(InDevPointer, that)
        return JS_NewInt32(ctx,that->lastData.y[0]) ;
    }


    void InDevPointer::read() {

        PointsData data ;

        if(taskDaemonHandle) {
            size_t item_size ;
            PointsData * _data = (PointsData *)xRingbufferReceive(ringbuf, &item_size, 0) ;
            if(!_data || item_size!=sizeof(PointsData)) {
                return ;
            }

            // 释放接收到的缓冲区
            vRingbufferReturnItem(ringbuf, (void *)_data);

            // printf("< %d, %d, %d\n",data.pcount,data.x[0],data.y[0]) ;
            data = *_data ;
        } else {
            _read(data) ;
        }

        if(enabledEvents>0 && 0!=memcmp(&data,&lastData,sizeof(PointsData))) {
            if( enabledEvents&PRESSED && data.pcount>lastData.pcount ) {
                JSValue jsname = JS_NewString(ctx,"pressed") ;
                emitSync(jsname,{}) ;
                JS_FreeValue(ctx,jsname) ;
            }
            else if( enabledEvents&RELEASED && data.pcount<lastData.pcount ) {
                JSValue jsname = JS_NewString(ctx,"released") ;
                emitSync(jsname,{}) ;
                JS_FreeValue(ctx,jsname) ;
            }
        }

        lastData = data ;
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

    void InDevPointer::startDaemon(size_t _interval_ms, uint8_t core) {
        if(taskDaemonHandle) {
            return ;
        }
        ringbuf = xRingbufferCreate(5*sizeof(PointsData),RINGBUF_TYPE_NOSPLIT) ;
        xTaskCreatePinnedToCore((TaskFunction_t)task_daemon, "task_indev", 1024*2, (void *)this, 10, &taskDaemonHandle, core) ;
        interval_ticks = _interval_ms/portTICK_PERIOD_MS ;
    }
    void InDevPointer::stopDaemon() {
        if(!taskDaemonHandle) {
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
    }
    void InDevPointer::task_daemon(InDevPointer * indev) {

        PointsData data ;
        PointsData lastData ;

        while(1) {

            indev->_read(data) ;
            if( memcmp(&data,&lastData, sizeof(PointsData))!=0 ) {

                // printf("> %d, %d, %d\n",data.pcount,data.x[0],data.y[0]) ;

                if( xRingbufferSend(indev->ringbuf, (void *)&data, sizeof(PointsData), 0) == pdFALSE ){
                    // printf("input device ringbuffer full.\n") ;

                    // 如果缓冲区满，删除最早的数据项
                    size_t item_size;
                    PointsData * discarded_data = (PointsData *)xRingbufferReceive(indev->ringbuf, &item_size, 0);
                    if (discarded_data != NULL) {
                        vRingbufferReturnItem(indev->ringbuf, (void *)discarded_data);
                    }

                    // 再次尝试发送
                    xRingbufferSend(indev->ringbuf, (void *)&data, sizeof(PointsData), 0) ;
                }

                memcpy(&lastData,&data, sizeof(PointsData)) ;
            }

            vTaskDelay(10/portTICK_PERIOD_MS) ;
        }
    }
}