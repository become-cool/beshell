#include "utils.h"
#include "mallocf.h"
#include <time.h>
#include <stdarg.h>
#include <string.h>
#include <stdbool.h>

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

char * mallocf(char * fmt, ...) {
	char *result;
	va_list args;

	va_start( args, fmt );
	result = vstrdupf( fmt, args );
	va_end( args );

	return result;
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
	if(ptr)
    	free(ptr) ;
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

void echo_error(JSContext * ctx) ;
void eval_code_len(JSContext *ctx,const char * str,size_t len,const char * filename, bool es6Module) {
	
    JSValue ret = JS_Eval(ctx, str, len, filename, es6Module? JS_EVAL_TYPE_MODULE: JS_EVAL_TYPE_GLOBAL) ;   // JS_EVAL_FLAG_STRIP
	if(JS_IsException(ret)) {
		echo_error(ctx) ;
	}
	JS_FreeValue(ctx, ret) ;
}

JSValue qjs_def_class2(
        JSContext *ctx,
        const char * className,
        JSClassID js_class_id,
        JSClassDef * js_class_p,
        const char * cotrName,
        JSCFunction cotr,
        const JSCFunctionListEntry* funcLst,
        uint16_t funcs,
        const JSCFunctionListEntry* staticFuncLst,
        uint16_t staticFuncs,
        JSValue parentProto ,
        JSValue pkg
) {    
    JS_NewClass(JS_GetRuntime(ctx), js_class_id, js_class_p);

    JSValue proto = JS_NewObject(ctx);
    JS_SetPropertyFunctionList(ctx, proto, funcLst, funcs);
    JS_SetClassProto(ctx, js_class_id, proto);

	if(!JS_IsUndefined(parentProto)) {
    	JS_SetPropertyStr(ctx, proto, "__proto__", parentProto);
	}

	if(cotr) {
		JSValue jscotr = JS_NewCFunction2(ctx, cotr, cotrName, 1, JS_CFUNC_constructor, 0) ;
		JS_SetConstructor(ctx, jscotr, proto) ;
		
		if(staticFuncs>0) {
    		JS_SetPropertyFunctionList(ctx, jscotr, staticFuncLst, staticFuncs);
		}
		
		if(!JS_IsUndefined(pkg)) {
			JS_SetPropertyStr(ctx, pkg, className, jscotr);
		}
	}

    return proto ;
}

JSValue qjs_def_class(
        JSContext *ctx,
        const char * className,
        JSClassID js_class_id,
        JSClassDef * js_class_p,
        const char * cotrName,
        JSCFunction cotr,
        const JSCFunctionListEntry* funclst,
        uint16_t funcs,
        JSValue parentProto ,
        JSValue pkg
) {
    return qjs_def_class2(ctx,className,js_class_id,js_class_p,cotrName,cotr,funclst,funcs,NULL,0,parentProto,pkg) ;
}

bool qjs_instanceof(JSContext *ctx, JSValue obj, JSClassID clz_id) {
    JSValue proto = JS_GetClassProto(ctx, clz_id);
    JSValue cotr = JS_GetPropertyStr(ctx, proto, "constructor") ;
    bool res = JS_IsInstanceOf(ctx, obj, cotr) ;
    JS_FreeValue(ctx,proto) ;
    JS_FreeValue(ctx,cotr) ;
	return res ;
}