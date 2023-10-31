#ifndef __H_STACK__
#define __H_STACK__

#include "stdbool.h"
#include <stddef.h>

typedef struct _stack_base {
    struct _stack_base * _next ;
} stack_base_t;

stack_base_t* stack_shift(stack_base_t** ppstack) ;

void stack_unshift(stack_base_t** ppstack, stack_base_t * pitem) ;

stack_base_t* stack_prev(stack_base_t* pstack, stack_base_t * pitem) ;

void stack_remove(stack_base_t** ppstack, stack_base_t * pitem) ;

bool stack_exists(stack_base_t* pstack, stack_base_t * pitem) ;

size_t stack_count(stack_base_t* pstack) ;

#define STACK_FOREACH(pstack, pitem, type)                      \
    for(type* pitem=pstack; pitem!=NULL; pitem=(type*)((stack_base_t*)pitem)->_next)

#define STACK_SEARCH(pstack, member, member_val, var, type)     \
    type * var = NULL ;                                         \
    STACK_FOREACH(pstack, pitem, type) {                        \
        if(((type*)pitem)->##member == member_val) {                     \
            var = pitem ;                                       \
            break ;                                             \
        }                                                       \
    }

#define STACK_IF_EXISTS(pstack, member, member_val, type)       \
    STACK_SEARCH(pstack, member, member_val, _##member_val##_is_exists, type)              \
    if(_##member_val##_is_exists!=NULL)

#define STACK_IF_NOT_EXISTS(pstack, member, member_val, type)   \
    STACK_SEARCH(pstack, member, member_val, _##member_val##_is_exists, type)              \
    if(_##member_val##_is_exists==NULL)

/*
// // 定义
// #define STACK_DEF_START(name)               \
//     typedef struct _##name {                \
//         struct _##name * _next ;

// #define STACK_DEF_END(name)                 \
//     } name;
            
// // 入栈
// #define STACK_UNSHIFT(pstack, item)         \
//     item->_next = pstack ;                  \
//     pstack = item ;

// // 出栈
// #define STACK_SHIFT(pstack, var)            \
//     if(pstack) {                            \
//         var = pstack ;                      \
//         pstack = pstack->_next ;            \
//     }                                       \
//     else {                                  \
//         var = NULL ;                        \
//     }

// #define STACK_FOREACH(pstack, item, type)   \
//     for(type item=pstack; item!=NULL; item=item->_next)

// #define STACK_PREV(pstack, item, var, type) \
//     var = NULL ;                            \
//     STACK_FOREACH(pstack, it, type) {       \
//         if(it->_next == item) {             \
//             var = it ;                      \
//             break ;                         \
//         }                                   \
//     }

// #define STACK_REMOVE(pstack, item, type)            \
//     if(item && pstack) {                            \
//         if(item==pstack) {                          \
//             pstack = item->_next;                   \
//         }                                           \
//         else {                                      \
//             type * prev ;                           \
//             STACK_PREV(pstack, item, prev, type)    \
//             if(prev) {                              \
//                 prev._next = item ;                 \
//             }                                       \
//         }                                           \
//         item->_next = NULL ;                        \
//     }

// #define STACK_COUNT(pstack, cnt)            \
//     cnt = 0 ;                               \
//     STACK_FOREACH(pstack, it, type) {       \
//         cnt ++ ;                            \
//     }

// #define STACK_COUNT_VAR(pstack, cnt)        \
//     size_t cnt ;                            \
//     STACK_COUNT(pstack, cnt)


// #define STACK_EXISTS(pstack, item, exists)  \
//     exists = 0 ;                            \
//     STACK_FOREACH(pstack, it, type) {       \
//         if(item==it) {                      \
//             exists = 1 ;                    \
//             break ;                         \
//         }                                   \
//     }

*/

#endif