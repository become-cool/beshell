#pragma once

#include "debug.h"
#include "deps/quickjs/quickjs-libc.h"
#include "deps/quickjs/cutils.h"
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif


#define JS_IsFalse(ctx, var) (JS_IsBool(var) && !JS_ToBool(ctx, var))
#define JS_IsTrue(ctx, var) (JS_IsBool(var) && JS_ToBool(ctx, var))

#define VAR_REFCNT(var) ((JSRefCountHeader *)JS_VALUE_GET_PTR(var))->ref_count
#define P_VAR_REFCNT(var) printf(#var" ref count:%d @%d\n", VAR_REFCNT(var), __LINE__) ;

#define THROW_EXCEPTION(...)                                \
    JS_ThrowReferenceError(ctx, __VA_ARGS__);               \
    return JS_EXCEPTION ;


#define THROW_GOTO(label, ...)                              \
    JS_ThrowReferenceError(ctx, __VA_ARGS__);               \
    goto label ;

#define THROW_EXCEPTION_FREE(exec, ...)                     \
    JS_ThrowReferenceError(ctx, __VA_ARGS__);               \
    exec                                                    \
    return JS_EXCEPTION ;

#define CHECK_ARGC(num)                                     \
    if(argc<num) {                                          \
        THROW_EXCEPTION("Missing param")                    \
    }

#define ARGV_TO_INT_VAR(i, var, api)                        \
	if( api(ctx, &var, argv[i])!=0 ) {                      \
        THROW_EXCEPTION("Invalid param type")               \
	}
#define  ARGV_TO_UINT8_VAR(i,var)   ARGV_TO_INT_VAR(i, var, JS_ToUint32)
#define   ARGV_TO_INT8_VAR(i,var)   ARGV_TO_INT_VAR(i, var, JS_ToInt32)
#define ARGV_TO_UINT16_VAR(i,var)   ARGV_TO_INT_VAR(i, var, JS_ToUint32)
#define ARGV_TO_INT16_VAR(i,var)    ARGV_TO_INT_VAR(i, var, JS_ToInt32)
#define ARGV_TO_UINT32_VAR(i,var)   ARGV_TO_INT_VAR(i, var, JS_ToUint32)
#define ARGV_TO_INT32_VAR(i,var)    ARGV_TO_INT_VAR(i, var, JS_ToInt32)
#define ARGV_TO_INT64_VAR(i,var)    ARGV_TO_INT_VAR(i, var, JS_ToInt64)
#define ARGV_TO_DOUBLE_VAR(i,var)   ARGV_TO_INT_VAR(i, var, JS_ToFloat64)


#define ARGV_TO_INT_VAR_OPT(i, var, api)            \
    if( i<argc ) {                                  \
        if( api(ctx, &var, argv[i])!=0 ) {          \
            THROW_EXCEPTION("Invalid param type")   \
        }                                           \
    }
#define  ARGV_TO_UINT8_VAR_OPT(i,var)   ARGV_TO_INT_VAR_OPT(i, var, JS_ToUint32)
#define   ARGV_TO_INT8_VAR_OPT(i,var)   ARGV_TO_INT_VAR_OPT(i, var, JS_ToInt32)
#define ARGV_TO_UINT16_VAR_OPT(i,var)   ARGV_TO_INT_VAR_OPT(i, var, JS_ToUint32)
#define  ARGV_TO_INT16_VAR_OPT(i,var)   ARGV_TO_INT_VAR_OPT(i, var, JS_ToInt32)
#define ARGV_TO_UINT32_VAR_OPT(i,var)   ARGV_TO_INT_VAR_OPT(i, var, JS_ToUint32)
#define  ARGV_TO_INT32_VAR_OPT(i,var)   ARGV_TO_INT_VAR_OPT(i, var, JS_ToInt32)
#define  ARGV_TO_INT64_VAR_OPT(i,var)   ARGV_TO_INT_VAR_OPT(i, var, JS_ToInt64)
#define ARGV_TO_DOUBLE_VAR_OPT(i,var)   ARGV_TO_INT_VAR_OPT(i, var, JS_ToFloat64)


#define ARGV_TO_INT_VAR_DEF(i, var, api, def)       \
    if( i>=argc ) {                                 \
        var = def ;                                 \
    }                                               \
    else {                                          \
        if( api(ctx, &var, argv[i])!=0 ) {          \
            THROW_EXCEPTION("Invalid param type")   \
        }                                           \
    }
	      
#define  ARGV_TO_UINT8_VAR_DEF(i,var, def)   ARGV_TO_INT_VAR_DEF(i, var, JS_ToUint32, def)
#define   ARGV_TO_INT8_VAR_DEF(i,var, def)   ARGV_TO_INT_VAR_DEF(i, var, JS_ToInt32, def)
#define ARGV_TO_UINT16_VAR_DEF(i,var, def)   ARGV_TO_INT_VAR_DEF(i, var, JS_ToUint32, def)
#define ARGV_TO_INT16_VAR_DEF(i,var, def)    ARGV_TO_INT_VAR_DEF(i, var, JS_ToInt32, def)
#define ARGV_TO_UINT32_VAR_DEF(i,var, def)   ARGV_TO_INT_VAR_DEF(i, var, JS_ToUint32, def)
#define ARGV_TO_INT32_VAR_DEF(i,var, def)    ARGV_TO_INT_VAR_DEF(i, var, JS_ToInt32, def)
#define ARGV_TO_INT64_VAR_DEF(i,var, def)    ARGV_TO_INT_VAR_DEF(i, var, JS_ToInt64, def)
#define ARGV_TO_DOUBLE_VAR_DEF(i,var, def)   ARGV_TO_INT_VAR_DEF(i, var, JS_ToFloat64, def)


#define ARGV_TO_INT(i, var, ctype, tmp_type, api)           \
	ctype var = 0 ;                                         \
    {                                                       \
        tmp_type tmp ;                                      \
        if( api(ctx, &tmp, argv[i])!=0 ) {                  \
            THROW_EXCEPTION("Invalid param type")           \
        }                                                   \
        var = (ctype)tmp ;                                  \
    }
    
#define  ARGV_TO_UINT8(i,var)   ARGV_TO_INT(i, var, uint8_t,  uint32_t,  JS_ToUint32)
#define   ARGV_TO_INT8(i,var)   ARGV_TO_INT(i, var, int8_t,   int32_t,   JS_ToInt32)
#define ARGV_TO_UINT16(i,var)   ARGV_TO_INT(i, var, uint16_t, uint32_t,  JS_ToUint32)
#define ARGV_TO_INT16(i,var)    ARGV_TO_INT(i, var, int16_t,  int32_t,   JS_ToInt32)
#define ARGV_TO_UINT32(i,var)   ARGV_TO_INT(i, var, uint32_t, uint32_t,  JS_ToUint32)
#define ARGV_TO_INT32(i,var)    ARGV_TO_INT(i, var, int32_t,  int32_t,   JS_ToInt32)
#define ARGV_TO_INT64(i,var)    ARGV_TO_INT(i, var, int64_t,  int64_t,   JS_ToInt64)
#define ARGV_TO_DOUBLE(i,var)   ARGV_TO_INT(i, var, double,    double,   JS_ToFloat64)


#define ARGV_TO_INT_OPT(i, var, ctype, api, def)    \
	ctype var = def ;                               \
    if( i<argc ) {                                  \
        if( api(ctx, &var, argv[i])!=0 ) {          \
            THROW_EXCEPTION("Invalid param type")   \
        }                                           \
    }

#define  ARGV_TO_UINT8_OPT(i,var,def)   ARGV_TO_INT_OPT(i, var, uint32_t,  JS_ToUint32, def)
#define   ARGV_TO_INT8_OPT(i,var,def)   ARGV_TO_INT_OPT(i, var, int32_t,   JS_ToInt32, def)
#define ARGV_TO_UINT16_OPT(i,var,def)   ARGV_TO_INT_OPT(i, var, uint32_t, JS_ToUint32, def)
#define ARGV_TO_INT16_OPT(i,var,def)    ARGV_TO_INT_OPT(i, var, int32_t,  JS_ToInt32, def)
#define ARGV_TO_UINT32_OPT(i,var,def)   ARGV_TO_INT_OPT(i, var, uint32_t, JS_ToUint32, def)
#define ARGV_TO_INT32_OPT(i,var,def)    ARGV_TO_INT_OPT(i, var, int32_t,  JS_ToInt32, def)
#define ARGV_TO_INT64_OPT(i,var,def)    ARGV_TO_INT_OPT(i, var, int64_t,  JS_ToInt64, def)
#define ARGV_TO_DOUBLE_OPT(i,var,def)   ARGV_TO_INT_OPT(i, var, double, JS_ToFloat64, def)


#define ARGV_TO_STRING_LEN(i, var, len)                     \
    size_t len = 0 ;                                        \
    const char * var = JS_ToCStringLen(ctx, &len, argv[i]) ;
#define ARGV_TO_STRING_LEN_E(i, var, len, msg)              \
    if(!JS_IsString(argv[i])) {                             \
        THROW_EXCEPTION(msg)                                \
    }                                                       \
    ARGV_TO_STRING_LEN(i, var, len)


#define ARGV_TO_STRING(i, var)                              \
    std::string var ;                                       \
    {                                                       \
        const char * cstr = JS_ToCString(ctx, argv[i]) ;    \
        var = cstr ;                                        \
        JS_FreeCString(ctx, cstr) ;                         \
    }


#define ARGV_TO_CSTRING(i, var)                              \
    const char * var = JS_ToCString(ctx, argv[i]) ;

#define ARGV_AS_CSTRING_E(i, var, msg)                       \
    var = NULL ;                                            \
    if(!JS_IsString(argv[i])) {                             \
        THROW_EXCEPTION(msg)                                \
    }                                                       \
    var = JS_ToCString(ctx, argv[i]) ;
#define ARGV_TO_CSTRING_E(i, var, msg)  const char * ARGV_AS_STRING_E(i, var, msg)

#define ARGV_AS_CSTRING_C(i, var, err_code)                  \
    var = NULL ;                                            \
    if(!JS_IsString(argv[i])) {                             \
        err_code                                            \
    }                                                       \
    var = JS_ToCString(ctx, argv[i]) ;
#define ARGV_TO_CSTRING_C(i, var, err_code)  char * ARGV_AS_CSTRING_C(i, var, err_code)
    
#define ARGV_TO_ARRAYBUFFER(i, var, varlen)                                         \
    size_t varlen = 0;                                                              \
    uint8_t * var = (uint8_t *)JS_GetArrayBuffer(ctx, &varlen, argv[i]) ;           \
    if(!var) {                                                                      \
        THROW_EXCEPTION("argv is not a ArrayBuffer")                                \
    }


#define CALL_IDF_API(func_invoke)           \
    {                                       \
        esp_err_t res = func_invoke ;       \
        if( res != ESP_OK ) {               \
            THROW_EXCEPTION(#func_invoke " failed, err: %d", res)   \
        }                                   \
    }

#define CALLBACK(func, thisobj, argc, argv)                                         \
    {                                                                               \
    JSValue ret = JS_Call(ctx, func, thisobj, argc, argv) ;                         \
    if( JS_IsException(ret) ) {                                                     \
        js_std_dump_error(ctx) ;                                                    \
    }                                                                               \
    JS_FreeValue(ctx, ret) ;                                                        \
    }
#define MALLOC_ARGV1(argv, arg1)                                                    \
    argv = malloc(sizeof(JSValue)) ;                                                \
    argv[0] = arg1 ;
#define MAKE_ARGV1(argv, arg1)                                                      \
    JSValueConst * MALLOC_ARGV1(argv, arg1)

#define MALLOC_ARGV2(argv, arg1, arg2)                                              \
    argv = malloc(sizeof(JSValue)*2) ;                                              \
    argv[0] = arg1 ;                                                                \
    argv[1] = arg2 ;
#define MAKE_ARGV2(argv, arg1, arg2)                                                \
    JSValueConst * MALLOC_ARGV2(argv, arg1, arg2)

#define MALLOC_ARGV3(argv, arg1, arg2, arg3)                                        \
    argv = malloc(sizeof(JSValue)*3) ;                                              \
    argv[0] = arg1 ;                                                                \
    argv[1] = arg2 ;                                                                \
    argv[2] = arg3 ;
#define MAKE_ARGV3(argv, arg1, arg2, arg3)                                          \
    JSValueConst * MALLOC_ARGV3(argv, arg1, arg2, arg3)

#define MALLOC_ARGV4(argv, arg1, arg2, arg3, arg4)                                  \
    argv = malloc(sizeof(JSValue)*4) ;                                              \
    argv[0] = arg1 ;                                                                \
    argv[1] = arg2 ;                                                                \
    argv[2] = arg3 ;                                                                \
    argv[3] = arg4 ;
#define MAKE_ARGV4(argv, arg1, arg2, arg3, arg4)                                    \
    JSValueConst * MALLOC_ARGV4(argv, arg1, arg2, arg3, arg4)


#define JS_CALL_ARG1(ctx, func, arg1)                           \
    {                                                           \
        MAKE_ARGV1(argv, arg1)                                  \
        JS_Call(ctx, func, JS_UNDEFINED, 1, argv) ;             \
        free(argv) ;                                            \
    }

#define JS_CALL_ARG2(ctx, func, arg1, arg2)                     \
    {                                                           \
        MAKE_ARGV2(argv, arg1, arg2)                            \
        JS_Call(ctx, func, JS_UNDEFINED, 2, argv) ;             \
        free(argv) ;                                            \
    }
#define JS_CALL_ARG3(ctx, func, arg1, arg2, arg3)               \
    {                                                           \
        MAKE_ARGV3(argv, arg1, arg2, arg3)                      \
        JS_Call(ctx, func, JS_UNDEFINED, 3, argv) ;             \
        free(argv) ;                                            \
    }
#define JS_CALL_ARG4(ctx, func, arg1, arg2, arg3, arg4)         \
    {                                                           \
        MAKE_ARGV4(argv, arg1, arg2, arg3, arg4)                \
        JS_Call(ctx, func, JS_UNDEFINED, 4, argv) ;             \
        free(argv) ;                                            \
    }


#define STR1(R)  #R
#define STR2(R)  STR1(R)


uint64_t gettime() ;
uint64_t gettime_us() ;
uint64_t gettime_ns() ;

void * mallocDMA(size_t size) ;

void freeArrayBuffer(JSRuntime *rt, void *opaque, void *ptr) ;
void nofreeArrayBuffer(JSRuntime *rt, void *opaque, void *ptr) ;

// SRAM优先于PSRAM
#ifdef PLATFORM_ESP32
#define HMALLOC(var, size) var = heap_caps_malloc(size, MALLOC_CAP_DMA); \
    if(!var) { var = heap_caps_malloc(size, MALLOC_CAP_SPIRAM); }
#else
#define HMALLOC(var, size) var = malloc(size);
#endif

#define GET_INT_PROP(obj, propName, cvar, excp)                                     \
    int32_t cvar = 0 ;                                                              \
    {                                                                               \
        JSValue jsvar = JS_GetPropertyStr(ctx, obj, propName) ;                     \
        if( JS_IsUndefined(jsvar) ) {                                               \
            JS_ThrowReferenceError(ctx, "property %s not exists", propName) ;       \
            excp ;                                                                  \
        }                                                                           \
        if( !JS_IsNumber(jsvar) ) {                                                 \
            JS_FreeValue(ctx, jsvar) ;                                              \
            JS_ThrowReferenceError(ctx, "property %s is not a number", propName) ;  \
            excp ;                                                                  \
        }                                                                           \
        JS_ToInt32(ctx, &cvar, jsvar) ;                                             \
    }

#define GET_INT_PROP_DEFAULT(obj, propName, cvar, default)                              \
    int32_t cvar = 0 ;                                                                  \
    {                                                                                   \
        JSValue jsvar = JS_GetPropertyStr(ctx, obj, propName) ;                         \
        if( JS_IsUndefined(jsvar) ) {                                                     \
            cvar = default ;                                                            \
        }                                                                               \
        else {                                                                          \
            if( !JS_IsNumber(jsvar) ) {                                                 \
                JS_FreeValue(ctx, jsvar) ;                                              \
                THROW_EXCEPTION(ctx, "property %s is not a number", propName) ;         \
            }                                                                           \
            JS_ToInt32(ctx, &cvar, jsvar) ;                                             \
        }                                                                               \
    }

#define GET_INT_PROP_NODEF(obj, propName, cvar)                                         \
    {                                                                                   \
        JSValue jsvar = JS_GetPropertyStr(ctx, obj, propName) ;                         \
        if( !JS_IsUndefined(jsvar) ) {                                                     \
            if( !JS_IsNumber(jsvar) ) {                                                 \
                JS_FreeValue(ctx, jsvar) ;                                              \
                THROW_EXCEPTION(ctx, "property %s is not a number", propName) ;         \
            }                                                                           \
            JS_ToInt32(ctx, &cvar, jsvar) ;                                             \
        }                                                                               \
    }

#define _ASSIGN_INT_PROP_DEFAULT(obj, propName, cvar, getter, default, tmptype)         \
    if(obj==JS_UNDEFINED||obj==JS_NULL) {                                               \
        cvar = default ;                                                                \
    } else {                                                                            \
        JSValue jsvar = JS_GetPropertyStr(ctx, obj, propName) ;                         \
        if( jsvar==JS_UNDEFINED ) {                                                     \
            cvar = default ;                                                            \
        }                                                                               \
        else {                                                                          \
            if( !JS_IsNumber(jsvar) ) {                                                 \
                JS_FreeValue(ctx, jsvar) ;                                              \
                THROW_EXCEPTION(ctx, "property %s is not a number", propName) ;         \
            }                                                                           \
            tmptype tmp = 0 ;                                                           \
            getter(ctx, &tmp, jsvar) ;                                                  \
            cvar = tmp ;                                                                \
        }                                                                               \
    }
#define ASSIGN_INT_PROP_DEFAULT(obj, propName, cvar, default) \
        _ASSIGN_INT_PROP_DEFAULT(obj, propName, cvar, JS_ToInt32, default, int32_t)

#define ASSIGN_UINT_PROP_DEFAULT(obj, propName, cvar, default) \
        _ASSIGN_INT_PROP_DEFAULT(obj, propName, cvar, JS_ToUint32, default, uint32_t)

#define _ASSIGN_INT_PROP(obj, propName, ctype, cvar, getter)                            \
    {                                                                                   \
        JSValue jsvar = JS_GetPropertyStr(ctx, obj, propName) ;                         \
        if( !JS_IsNumber(jsvar) ) {                                                     \
            JS_FreeValue(ctx, jsvar) ;                                                  \
            THROW_EXCEPTION(ctx, "property %s is not a number", propName) ;             \
        }                                                                               \
        getter(ctx, (ctype *) &cvar, jsvar) ;                                           \
    }

#define ASSIGN_INT_PROP(obj, propName, cvar) \
        _ASSIGN_INT_PROP(obj, propName, int, cvar, JS_ToInt32)

#define ASSIGN_UINT_PROP(obj, propName, cvar) \
        _ASSIGN_INT_PROP(obj, propName, uint, cvar, JS_ToUint32)


#define ASSIGN_PROP(obj, propName, jsvar)                                               \
    jsvar = JS_UNDEFINED ;                                                              \
    if(!JS_IsUndefined(obj)&&!JS_IsNull(obj)) {                                               \
        jsvar = JS_GetPropertyStr(ctx, obj, propName) ;                         \
    }                                                                                   \

#define ASSIGN_PROP_P(obj, propName, jsvar, not_exist_code)                             \
    ASSIGN_PROP(obj, propName, jsvar)                                                   \
    if(jsvar==JS_UNDEFINED||jsvar==JS_NULL) {                                           \
        not_exist_code                                                                  \
    }

// 对象属性值 -> cvar
// 需要要手动 JS_FreeCString(ctx, cvar)
#define ASSIGN_STR_PROP(obj, propName, cvar)                                            \
    cvar = NULL ;                                                                       \
    if(!JS_IsUndefined(obj)&&!JS_IsNull(obj)) {                                               \
        JSValue jsvar = JS_GetPropertyStr(ctx, obj, propName) ;                         \
        if( !JS_IsUndefined(jsvar) && !JS_IsNull(jsvar)) {                                    \
            cvar = JS_ToCString(ctx, jsvar) ;                                           \
        }                                                                               \
    }
#define ASSIGN_STR_PROP_C(obj, propName, cvar, err_code)                                \
    cvar = NULL ;                                                                       \
    if((!JS_IsUndefined(obj))&&(!JS_IsNull(obj))) {                                           \
        JSValue jsvar = JS_GetPropertyStr(ctx, obj, propName) ;                         \
        if( !JS_IsUndefined(jsvar) && !JS_IsNull(jsvar)) {                                    \
            cvar = JS_ToCString(ctx, jsvar) ;                                           \
        }                                                                               \
    }                                                                                   \
    if(!cvar) {                                                                         \
        err_code                                                                        \
    }

#define ASSIGN_STR_PROP_E(obj, propName, cvar, err_msg)                                 \
    ASSIGN_STR_PROP_C( obj, propName, cvar, THROW_EXCEPTION(err_msg))


#define ASSIGN_BOOL_PROP(obj, propName, cvar)                                           \
    cvar = false ;                                                                      \
    if(!JS_IsUndefined(obj)&&!JS_IsNull(obj)) {                                               \
        JSValue jsvar = JS_GetPropertyStr(ctx, obj, propName) ;                         \
        cvar = JS_ToBool(ctx, jsvar) ;                                                  \
    }


JSValue js_get_prop(JSContext *ctx, JSValue obj, int depth, ...)  ;
JSValue js_get_glob_prop(JSContext *ctx, int depth, ...)  ;

bool qjs_instanceof(JSContext *ctx, JSValue obj, JSClassID clz_id) ;

#define QJS_DEF_CLASS(typeName, clzName, fullClzName, parentProto, pkgVar)                          \
    qjs_def_class(ctx, clzName, js_##typeName##_class_id, &js_##typeName##_class     \
                , fullClzName, js_##typeName##_constructor, js_##typeName##_proto_funcs, countof(js_##typeName##_proto_funcs), parentProto, pkgVar) ;



#define JS_ForeachArray(jsobj,item,code)    \
    {                                       \
        if( JS_IsArray(ctx,jsobj) ) {       \
            JSValue jslen = JS_GetPropertyStr(ctx, jsobj, "length") ; \
            uint32_t len = 0 ;              \
            JS_ToUint32(ctx, &len, jslen) ; \
            for(int i=0;i<len;i++){         \
                JSValue item = JS_GetPropertyUint32(ctx, jsobj, i) ; \
                code \
                JS_FreeValue(ctx,item) ; \
            } \
            JS_FreeValue(ctx,jslen) ; \
        }\
    }


#define DEF_JS_FUNC(jsvar, code, filename, fail)    \
    jsvar = JS_Eval(ctx, "(" code ")", strlen("(" code ")"), filename, JS_EVAL_TYPE_GLOBAL) ;   \
    if(JS_IsException(jsvar)) {                     \
        fail                                        \
    }


#define JS_IsNone(jsval) (JS_IsUndefined(jsval)||JS_IsNull(jsval))
    
#ifdef __cplusplus
}
#endif