#ifndef __H_ANCHOR9__
#define __H_ANCHOR9__

/*

* 不区分 from/to 方向
* from/to 仅用于 offset_x/y

*/

#include <stdint.h>
#include <stdbool.h>
#include "stack.h"

#define ANCHOR9_H_LFT 1
#define ANCHOR9_H_MID 2
#define ANCHOR9_H_RGT 4
#define ANCHOR9_V_TOP 1 << 3
#define ANCHOR9_V_MID 2 << 3
#define ANCHOR9_V_BTM 4 << 3

#define ANCHOR9_LFTTOP      ANCHOR9_H_LFT | ANCHOR9_V_TOP
#define ANCHOR9_TOP         ANCHOR9_H_MID | ANCHOR9_V_TOP
#define ANCHOR9_RGTTOP      ANCHOR9_H_RGT | ANCHOR9_V_TOP
#define ANCHOR9_LFT         ANCHOR9_H_LFT | ANCHOR9_V_MID
#define ANCHOR9_CENTER      ANCHOR9_H_MID | ANCHOR9_V_MID
#define ANCHOR9_RGT         ANCHOR9_H_RGT | ANCHOR9_V_MID
#define ANCHOR9_LFTBTM      ANCHOR9_H_LFT | ANCHOR9_V_BTM
#define ANCHOR9_BTM         ANCHOR9_H_MID | ANCHOR9_V_BTM
#define ANCHOR9_RGTBTM      ANCHOR9_H_RGT | ANCHOR9_V_BTM


typedef struct _a9_rect_t a9_rect_t ;

typedef struct _a9_link_t {

    stack_base_t stack ;
    
    a9_rect_t * from_rect ;
    uint8_t _from_anchor ;

    a9_rect_t * to_rect ;
    uint8_t _to_anchor ;

    int16_t _offset_x ;
    int16_t _offset_y ;

    bool _fixed ;

} a9_link_t ;


inline uint8_t a9_link_get_from_anchor(a9_link_t * link) {
    return link->_from_anchor ;
}
inline void a9_link_set_from_anchor(a9_link_t * link, uint8_t val) {
    link->_from_anchor = val ;
}

inline uint8_t a9_link_get_to_anchor(a9_link_t * link) {
    return link->_to_anchor ;
}
inline void a9_link_set_to_anchor(a9_link_t * link, uint8_t val) {
    link->_to_anchor = val ;
}

inline uint8_t a9_link_get_offset_x(a9_link_t * link) {
    return link->_offset_x ;
}
inline void a9_link_set_offset_x(a9_link_t * link, uint8_t val) {
    link->_offset_x = val ;
}

inline uint8_t a9_link_get_offset_y(a9_link_t * link) {
    return link->_offset_y ;
}
inline void a9_link_set_offset_y(a9_link_t * link, uint8_t val) {
    link->_offset_y = val ;
}

inline uint8_t a9_link_get_fixed(a9_link_t * link) {
    return link->_fixed ;
}
inline void a9_link_set_fixed(a9_link_t * link, uint8_t val) {
    link->_fixed = val ;
}


typedef struct _a9_rect_t {
    a9_link_t * links ;
    bool updated ;
} a9_rect_t ;


a9_link_t * a9_link(a9_rect_t * from, uint8_t from_anchor, a9_rect_t * to, uint8_t to_anchor) ;

void a9_unlink(a9_link_t * link) ;

a9_rect_t * a9_another(a9_link_t * link, a9_rect_t * thisone) ;


void a9_init() ;

void a9_loop() ;

#endif