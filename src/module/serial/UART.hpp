#pragma once

#include <NativeClass.hpp>
#include "driver/uart.h"

namespace be{
    class UART: public be::NativeClass {
        DECLARE_NCLASS_META
    private:
        static std::vector<JSCFunctionListEntry> methods ;
        // static std::vector<JSCFunctionListEntry> staticMethods ;

        uart_port_t m_uartNum ;
        
        static UART * uart0 ;
        static UART * uart1 ;
        static UART * uart2 ;

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