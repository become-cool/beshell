#include "utils.h"
#include "mallocf.h"
#include <time.h>
#include <stdarg.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>

#ifdef PLATFORM_ESP32
#include "esp_heap_caps.h"
#endif

uint64_t gettime() {
	struct timespec tm = {0, 0};
	clock_gettime(CLOCK_REALTIME, &tm);
	return tm.tv_nsec/1000/1000 + tm.tv_sec*1000 ;
}

uint64_t gettime_us() {
	struct timespec tm = {0, 0};
	clock_gettime(CLOCK_REALTIME, &tm);
	return tm.tv_nsec/1000 + tm.tv_sec*1000*1000 ;
}
uint64_t gettime_ns() {
	struct timespec tm = {0, 0};
	clock_gettime(CLOCK_REALTIME, &tm);
	return tm.tv_nsec + tm.tv_sec*1000*1000*1000 ;
}

inline void * mallocDMA(size_t size) {
#ifdef PLATFORM_ESP32
    void * data = heap_caps_malloc(size, MALLOC_CAP_DMA) ;
	// if(!data) {
	// 	return heap_caps_malloc(size, MALLOC_CAP_SPIRAM) ;
	// }
	return data ;
#else
    return malloc(size) ;
#endif
}

// 在 gc delete ArrayBuffer 时 free() 内存
void freeArrayBuffer(JSRuntime *rt, void *opaque, void *ptr) {
	if(ptr){
    	free(ptr) ;
	}
}
// 在 gc delete ArrayBuffer 时不 free() 内存
void nofreeArrayBuffer(JSRuntime *rt, void *opaque, void *ptr) {}


JSValue js_get_prop(JSContext *ctx, JSValue obj, int depth, ...) {
    va_list argptr;
    va_start(argptr, depth);

	char * name ;
	for(int i=0;i<depth-1;i++) {
		name = va_arg(argptr, char *) ;
		obj = JS_GetPropertyStr(ctx,obj,name) ;
		if( !JS_IsObject(obj) ) {
			va_end(argptr);
			return obj ;
		}
		JS_FreeValue(ctx,obj) ;
	}
	name = va_arg(argptr, char *) ;
	va_end(argptr);
	return JS_GetPropertyStr(ctx,obj,name) ;
}

JSValue js_get_glob_prop(JSContext *ctx, int depth, ...) {
	JSValue obj = JS_GetGlobalObject(ctx) ;
	JS_FreeValue(ctx, obj) ;

    va_list argptr;
    va_start(argptr, depth);

	char * name ;
	for(int i=0;i<depth-1;i++) {
		name = va_arg(argptr, char *) ;
		obj = JS_GetPropertyStr(ctx,obj,name) ;
		if( !JS_IsObject(obj) ) {
			va_end(argptr);
			return obj ;
		}
		JS_FreeValue(ctx,obj) ;
	}
	
	name = va_arg(argptr, char *) ;

	va_end(argptr);

	return JS_GetPropertyStr(ctx,obj,name) ;
}


bool qjs_instanceof(JSContext *ctx, JSValue obj, JSClassID clz_id) {
    JSValue proto = JS_GetClassProto(ctx, clz_id);
    JSValue cotr = JS_GetPropertyStr(ctx, proto, "constructor") ;
    bool res = JS_IsInstanceOf(ctx, obj, cotr) ;
    JS_FreeValue(ctx,proto) ;
    JS_FreeValue(ctx,cotr) ;
	return res ;
}