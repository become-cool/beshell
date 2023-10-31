#include "malloc_funcs.h"
#include <assert.h>
#include <stdlib.h>
#include <esp_heap_caps.h>

#define MALLOC_OVERHEAD 8

/* default memory allocation functions with memory limitation */
static inline size_t js_def_malloc_usable_size(void *ptr) {
    return malloc_usable_size(ptr);
}

void *js_def_malloc(JSMallocState *s, size_t size) {
    void *ptr;

    /* Do not allocate zero bytes: behavior is platform dependent */
    assert(size != 0);

    if (unlikely(s->malloc_size + size > s->malloc_limit))
        return NULL;

    // ptr = malloc(size);
    ptr = heap_caps_malloc( size, MALLOC_CAP_SPIRAM );
    if (!ptr)
        return NULL;

    s->malloc_count++;
    s->malloc_size += js_def_malloc_usable_size(ptr) + MALLOC_OVERHEAD;
    return ptr;
}

void js_def_free(JSMallocState *s, void *ptr) {
    if (!ptr)
        return;

    s->malloc_count--;
    s->malloc_size -= js_def_malloc_usable_size(ptr) + MALLOC_OVERHEAD;
    free(ptr);
}

void *js_def_realloc(JSMallocState *s, void *ptr, size_t size) {
    size_t old_size;

    if (!ptr) {
        if (size == 0)
            return NULL;
        return js_def_malloc(s, size);
    }
    old_size = js_def_malloc_usable_size(ptr);
    if (size == 0) {
        s->malloc_count--;
        s->malloc_size -= old_size + MALLOC_OVERHEAD;
        free(ptr);
        return NULL;
    }
    if (s->malloc_size + size - old_size > s->malloc_limit)
        return NULL;

    ptr = realloc(ptr, size);
    if (!ptr)
        return NULL;

    s->malloc_size += js_def_malloc_usable_size(ptr) - old_size;
    return ptr;
}
