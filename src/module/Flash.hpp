#pragma once

#include "BeShell.hpp"
#include "NativeModule.hpp"
#include "esp_partition.h"


namespace be{
    
    class Partition: public be::NativeClass {
        DECLARE_NCLASS_META
    private:
        static std::vector<JSCFunctionListEntry> methods ;
    public:

        const esp_partition_t * partition ; 
        Partition(const esp_partition_t * partition, JSContext * ctx, JSValue _jsobj=JS_NULL) ;

        static JSValue erase(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue read(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue write(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue label(JSContext *ctx, JSValueConst this_val) ;
        static JSValue type(JSContext *ctx, JSValueConst this_val) ;
        static JSValue subtype(JSContext *ctx, JSValueConst this_val) ;
        static JSValue address(JSContext *ctx, JSValueConst this_val) ;
        static JSValue size(JSContext *ctx, JSValueConst this_val) ;
        static JSValue readonly(JSContext *ctx, JSValueConst this_val) ;
        static JSValue encrypted(JSContext *ctx, JSValueConst this_val) ;
        static JSValue invalidSetter(JSContext *ctx, JSValueConst this_val, JSValueConst value) ;

    } ;

    class Flash: public be::NativeModule {
    public:
        static char const * const name ;
        Flash(JSContext * ctx, const char * name) ;
        static JSValue allPartitions(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;

        static JSValue partition(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
    } ;
}
