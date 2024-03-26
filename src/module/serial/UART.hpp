#pragma once

#include <NativeClass.hpp>
#include "driver/uart.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"

namespace be{
    class UART: public be::NativeClass {
        DECLARE_NCLASS_META
    private:
        static std::vector<JSCFunctionListEntry> methods ;

        uart_port_t m_uartNum ;
        
        static UART * uart0 ;
        static UART * uart1 ;
        static UART * uart2 ;

        TaskHandle_t taskListenerHandle = nullptr ;
        JSValue listener = JS_NULL ;
        static void task_listen(void * arg) ;
        QueueHandle_t data_queue = nullptr ;

    public:
        UART(JSContext * ctx, uart_port_t port) ;

        static UART * flyweight(JSContext *, uart_port_t) ;

        uart_port_t uartNum() const ;

        static JSValue setup(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue read(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue write(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue listen(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue unsetup(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
    
        static void loop(JSContext * ctx, void * opaque) ;
    } ;
}