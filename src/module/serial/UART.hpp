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
        #if SOC_UART_HP_NUM>1
        static UART * uart1 ;
        #endif
        #if SOC_UART_HP_NUM>2
        static UART * uart2 ;
        #endif
        #if SOC_UART_HP_NUM>3
        static UART * uart3 ;
        #endif
        #if SOC_UART_HP_NUM>4
        static UART * uart4 ;
        #endif
        #if SOC_UART_LP_NUM>0
        static UART * uartlp0 ;
        #endif

        TaskHandle_t taskListenerHandle = nullptr ;
        JSValue listener = JS_NULL ;
        QueueHandle_t data_queue = nullptr ;
        static void task_listen(void * arg) ;
        static void loop(JSContext * ctx, void * opaque) ;

    public:
        UART(JSContext * ctx, uart_port_t port) ;

        static UART * flyweight(JSContext *, uart_port_t) ;

        uart_port_t uartNum() const ;

        static JSValue setup(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue read(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue write(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue listen(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue unsetup(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
    
    } ;
}