#include "be_list.h"
#include <stdio.h>

void be_list_init(be_list_t * lst) {
    lst->head = NULL ;
    lst->tail = NULL ;
    lst->count = 0 ;
}

void be_list_append(be_list_t * lst, be_list_item_t * item) {

    if(be_list_includes(lst,item)) {
        printf("item has exists in list already\n"); ;
        return ;
    }
    
    item->prev = lst->tail;
    item->next = NULL;

    if(lst->tail) {
        lst->tail->next = item ;
    }
    lst->tail = item ;

    if(!lst->head) {
        lst->head = item ;
    }

    lst->count ++ ;
}


void be_list_remove(be_list_t * lst, be_list_item_t * item) {

    if(item->prev) {
        item->prev->next = item->next ;
    }
    if(item->next) {
        item->next->prev = item->prev ;
    }

    if(lst->head==item) {
        lst->head = item->next ;
    }
    if(lst->tail==item) {
        lst->tail = item->prev ;
    }

    item->next = NULL ;
    item->prev = NULL ;

    lst->count -- ;
}

bool be_list_check(be_list_t * lst){
    if(lst->head==NULL) {
        return true ;
    }
    be_list_item_t * a = lst->head ;
    be_list_item_t * b = lst->head ;
    while(1) {
        a = a->next ;
        b = b->next ;
        if(b==NULL) {
            return true ;
        }
        b = b->next ;
        if(b==NULL) {
            return true ;
        }

        if(a==b) {
            return false ;
        }
    }
    
}

be_list_item_t * be_list_shift(be_list_t * lst) {

    if(!lst || !lst->head) {
        return NULL ;
    }

    be_list_item_t * item = lst->head ;
    lst->head = item->next ;

    if(lst->head) {
        lst->head->prev = NULL ;
    }

    if(lst->tail==item) {
        lst->tail = NULL ;
    }


    item->next = NULL ;
    item->prev = NULL ;

    lst->count -- ;

    return item ;
}


be_list_item_t * be_list_pop(be_list_t * lst) {

    if(!lst || !lst->tail) {
        return NULL ;
    }

    be_list_item_t * item = lst->tail ;
    lst->tail = item->prev ;

    if(lst->tail) {
        lst->tail->next = NULL ;
    }
    else {
        lst->head = NULL ;
    }

    item->prev = NULL ;
    item->next = NULL ;

    lst->count -- ;

    return item ;
}

bool be_list_includes(be_list_t * lst, be_list_item_t * item) {
    FOREACH_LIST(lst, _item) {
        if(item==_item) {
            return true ;
        }
    }
    return false ;
}

bool be_list_is_empty(be_list_t * lst) {
    if( lst->head!=NULL && lst->tail!=NULL ){
        return 0 ;
    }
    if(lst->head!=NULL || lst->tail!=NULL) {
        printf("lst->head==NULL? %d; lst->tail==NULL? %d\n", lst->head==NULL?1:0, lst->tail==NULL?1:0 ) ;
    }
    return 1 ;
}

void be_list_print(be_list_t * lst) {
    if(!lst->head) {
        printf("list is empty\n") ;
        return ;
    }
    printf("list count:%d, head:@%p, tail:@%p :\n", lst->count, lst->head, lst->tail);
    be_list_item_t * a = lst->head ;
    be_list_item_t * b = lst->head ;
    while(1) {
        printf("-@%p, <-prev:@%p, ->next:@%p\n", a, a->prev, a->next) ;
        a = a->next ;
        if(a==NULL) {
            return ;
        }
        if(b) {
            b = b->next ;
            if(b) {
                b = b->next ;
                if(a==b) {
                    printf("list is backloop\n") ;
                    return ;
                }
            }
        }

    }
}


unsigned int be_list_recount(be_list_t * lst) {
    unsigned int cnt = 0 ;
    FOREACH_LIST(lst, item) {
        cnt ++ ;
    }
    lst->count = cnt ;
    return cnt ;
}
