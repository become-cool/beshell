#ifndef ONCE_H_DEBUG
#define ONCE_H_DEBUG

#include <stdint.h>
#include "qjs_utils.h"

#define pf(...) printf(__VA_ARGS__) ;printf("\n") ;
#define dd printf("%d@%s()\n", __LINE__, __FUNCTION__) ;
#define df(msg) printf("@%d %s\n", __LINE__, msg) ;
#define dm(msg) printf("%s: %dKB\n", msg, esp_get_free_heap_size()/1024);
#define dp(p)   printf(#p"@%p (%d)\n", p, __LINE__) ;
#define ds(s)   printf(#s"=%s @%d\n", s, __LINE__) ;
#define dstr(s)   printf(#s"=%s @%d\n", s.c_str(), __LINE__) ;
#define dn(v)   printf(#v"=%d @%d\n", (int)v, __LINE__) ;
#define dn64(v)   printf(#v"=%lld\n", v) ;
#define dn2(v1,v2)              printf(#v1"=%d, "#v2"=%d @%d\n", (int)v1, (int)v2, __LINE__) ;
#define dn3(v1,v2,v3)           printf(#v1"=%d, "#v2"=%d, "#v3"=%d @%d\n", (int)v1, (int)v2, (int)v3, __LINE__) ;
#define dn4(v1,v2,v3,v4)        printf(#v1"=%d, "#v2"=%d, "#v3"=%d, "#v4"=%d @%d\n", (int)v1, (int)v2, (int)v3, (int)v4, __LINE__) ;
#define dn5(v1,v2,v3,v4,v5)     printf(#v1"=%d, "#v2"=%d, "#v3"=%d, "#v4"=%d, "#v5"=%d @%d\n", (int)v1, (int)v2, (int)v3, (int)v4, (int)v5, __LINE__) ;

#define dvar(jsvar) {                               \
    const char * cstr = JS_ToCString(ctx, jsvar) ;  \
    printf("%s=%s %d@%s()\n",#jsvar, cstr, __LINE__, __FUNCTION__) ;                \
    JS_FreeCString(ctx, cstr) ;                     \
}

#define dfunc   printf("%s()@%d\n", __FUNCTION__, __LINE__) ;

#define YES_OR_NO(exp) printf(#exp"? %s\n", (exp)? "yes": "no") ;
#define IS_NULL(exp) YES_OR_NO((exp)==NULL)

#define dref(var) printf(#var" ref count:%d @%d\n", ((JSRefCountHeader *)JS_VALUE_GET_PTR(var))->ref_count, __LINE__) ;

#define record_time(var, codes)                                 \
    int64_t var = 0 ;                                           \
    {                                                           \
        var = gettime() ;                                       \
        codes                                                   \
        var = gettime()-var ;                                   \
    }

#define echo_time(msg, codes)                                   \
    {                                                           \
        int64_t __tt = gettime() ;                              \
        codes                                                   \
        printf(msg": %lldms\n", gettime()-__tt) ;               \
    }
#define echof_time(msg, codes, ...)                             \
    {                                                           \
        int64_t __tt = gettime() ;                              \
        codes                                                   \
        printf(msg": %lldms\n", __VA_ARGS__, gettime()-__tt) ;  \
    }
#define necho_time(msg, codes)  codes
#define nechof_time(msg, codes, ...)  codes


#define echo_alloc(msg, codes)                                  \
    {                                                           \
        size_t __m = heap_caps_get_free_size(MALLOC_CAP_DMA) ;  \
        codes                                                   \
        printf(msg" alloc: %d\n", __m - heap_caps_get_free_size(MALLOC_CAP_DMA)) ; \
    }
#define necho_alloc(msg, codes, ...)  codes

#ifdef __cplusplus
extern "C" {
#endif

void print_block(uint8_t * data, int length, int columns) ;

#ifdef __cplusplus
}
#endif

#endif
