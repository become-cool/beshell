#include "stack.h"


stack_base_t* stack_shift(stack_base_t** ppstack) {
    if(*ppstack) {
        stack_base_t * var = *ppstack ;
        *ppstack = var->_next ;
        return var ;
    }
    else {
        return NULL ;
    }
}

void stack_unshift(stack_base_t** ppstack, stack_base_t * pitem) {
    if(pitem) {
        pitem->_next = *ppstack ;
        *ppstack = pitem ;
    }
}

stack_base_t* stack_prev(stack_base_t* pstack, stack_base_t * pitem) {
    if(!pstack || !pitem) {
        return NULL ;
    }
    for(stack_base_t* pit=pstack; pit!=NULL; pit=pit->_next) {
        if(pit->_next == pitem) {
            return pit ;
        }
    }
    return NULL ;
}

void stack_remove(stack_base_t** ppstack, stack_base_t * pitem) {

    if( !(*ppstack) || !pitem ) {
        return ;
    }
    
    else if( (*ppstack)==pitem ) {
        *ppstack = pitem->_next;
        pitem->_next = NULL ;
    }
    else {
        stack_base_t * prev = stack_prev(*ppstack, pitem) ;
        if(prev) {
            prev->_next = pitem ;
            pitem->_next = NULL ;
        }
    }
}
bool stack_exists(stack_base_t* pstack, stack_base_t * pitem) {
    if(pstack && pitem) {
        for(stack_base_t* pit=pstack; pit!=NULL; pit=pit->_next) {
            if(pit == pitem) {
                return true ;
            }
        }
    }
    return false ;
}
size_t stack_count(stack_base_t * pstack) {
    size_t size = 0 ;
    for(stack_base_t* pit=pstack; pit!=NULL; pit=pit->_next) {
        size ++ ;
    }
    return size ;
}