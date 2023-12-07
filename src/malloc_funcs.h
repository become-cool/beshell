#ifndef ONCE_H_MALLOC_FUNCS
#define ONCE_H_MALLOC_FUNCS

#include "deps/quickjs/quickjs-libc.h"

#ifdef __cplusplus
extern "C" {
#endif

void * js_def_malloc(JSMallocState *s, size_t size) ;
void js_def_free(JSMallocState *s, void *ptr) ;
void * js_def_realloc(JSMallocState *s, void *ptr, size_t size) ;

#ifdef __cplusplus
}
#endif

#endif