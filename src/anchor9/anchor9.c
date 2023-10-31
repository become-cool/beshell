#include "anchor9.h"




void a9_init() {

}

void a9_loop() {

}


a9_link_t * a9_get_link(a9_rect_t * from, uint8_t from_anchor, a9_rect_t * to, uint8_t to_anchor) {
    STACK_FOREACH(from->links, plink, a9_link_t) {
        
    }
}

a9_link_t * a9_link(a9_rect_t * from, uint8_t from_anchor, a9_rect_t * to, uint8_t to_anchor) {
    // 
}

void a9_unlink(a9_link_t * link) {

}

a9_rect_t * a9_another(a9_link_t * link, a9_rect_t * thisone) {
    if( link->from_rect==thisone ) {
        return link->to_rect ;
    }
    else if( link->to_rect==thisone ) {
        return link->from_rect ;
    }
    else {
        return NULL ;
    }
}
