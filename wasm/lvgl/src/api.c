#include <stdlib.h>
#include <emscripten.h>
#include "lv_conf.h"
#include "lvgl.h"
#include "../../main/debug.h"
#include "font_msyh.h"

EMSCRIPTEN_KEEPALIVE void lv_free(void * ptr) {
    free(ptr);
}

EMSCRIPTEN_KEEPALIVE lv_coord_t lv_coord_pct(uint16_t pct) {
    return LV_PCT(pct);
}
EMSCRIPTEN_KEEPALIVE bool lv_is_coord_pct(uint16_t pct) {
    return LV_COORD_IS_PCT(pct);
}

EMSCRIPTEN_KEEPALIVE lv_coord_t lv_coord_size_content() {
    return LV_SIZE_CONTENT;
}


EMSCRIPTEN_KEEPALIVE lv_coord_t lv_indev_data_get_point_x(lv_indev_data_t * pdata) {
    return pdata->point.x ;
}
EMSCRIPTEN_KEEPALIVE void lv_indev_data_set_point_x(lv_indev_data_t * pdata, lv_coord_t x) {
    pdata->point.x = x;
}
EMSCRIPTEN_KEEPALIVE lv_coord_t lv_indev_data_get_point_y(lv_indev_data_t * pdata) {
    return pdata->point.y ;
}
EMSCRIPTEN_KEEPALIVE void lv_indev_data_set_point_y(lv_indev_data_t * pdata, lv_coord_t y) {
    pdata->point.y = y;
}



EMSCRIPTEN_KEEPALIVE void lv_obj_set_coord_x(lv_obj_t * obj, lv_coord_t val) {
    lv_coord_t dx = val - obj->coords.x1 ;
    lv_obj_set_x(obj, lv_obj_get_x_aligned(obj) + dx) ;
}
EMSCRIPTEN_KEEPALIVE void lv_obj_set_coord_y(lv_obj_t * obj, lv_coord_t val) {
    lv_coord_t dy = val - obj->coords.y1 ;
    lv_obj_set_y(obj, lv_obj_get_y_aligned(obj) + dy) ;
}

EMSCRIPTEN_KEEPALIVE void lv_obj_set_coords_redefine(lv_obj_t * obj, lv_coord_t x, lv_coord_t y) {
    lv_coord_t dx = x - obj->coords.x1 ;
    lv_obj_set_x(obj, lv_obj_get_x_aligned(obj) + dx) ;

    lv_coord_t dy = y - obj->coords.y1 ;
    lv_obj_set_y(obj, lv_obj_get_y_aligned(obj) + dy) ;
}

EMSCRIPTEN_KEEPALIVE lv_coord_t lv_obj_get_coord_x(lv_obj_t * obj) {
    return obj->coords.x1 ;
}
EMSCRIPTEN_KEEPALIVE lv_coord_t lv_obj_get_coord_y(lv_obj_t * obj) {
    return obj->coords.y1 ;
}


EMSCRIPTEN_KEEPALIVE void lv_indev_drv_tick(lv_indev_drv_t * indev_drv) {
    lv_indev_read_timer_cb(indev_drv->read_timer) ;
}




EMSCRIPTEN_KEEPALIVE int lv_obj_get_font_height(lv_obj_t * obj, lv_style_selector_t selector) {
    lv_font_t * font = lv_obj_get_style_text_font(obj, selector) ;
    return font? font->line_height: 0 ;
}

EMSCRIPTEN_KEEPALIVE bool lv_label_set_font(lv_obj_t * obj, const char * fontname, lv_style_selector_t selector) {

    lv_font_t * font = NULL ;
    if( strcmp("m8", fontname)==0 ) {
        font = &lv_font_montserrat_8 ;
    }
    if( strcmp("m10", fontname)==0 ) {
        font = &lv_font_montserrat_10 ;
    }
    else if( strcmp("m12", fontname)==0 ) {
        font = &lv_font_montserrat_12 ;
    }
    else if( strcmp("m16", fontname)==0 ) {
        font = &lv_font_montserrat_16 ;
    }
    else if( strcmp("m24", fontname)==0 ) {
        font = &lv_font_montserrat_24 ;
    }
    else if( strcmp("m32", fontname)==0 ) {
        font = &lv_font_montserrat_32 ;
    }
    else if( strcmp("m40", fontname)==0 ) {
        font = &lv_font_montserrat_40 ;
    }
    else if( strcmp("m48", fontname)==0 ) {
        font = &lv_font_montserrat_48 ;
    }
    else if( strcmp("source-han-sans", fontname)==0 ) {
        font = font_msyh() ;
    }
    // else if( strcmp("s16", fontname)==0 ) {
    //     font = & be_font_symbol_16 ;
    // }

    if(!font) {
        return false ;
    }

    lv_obj_set_style_text_font(obj, font, LV_PART_MAIN | LV_STATE_DEFAULT ) ;

    return true ;
}

EMSCRIPTEN_KEEPALIVE char * lv_label_get_font(lv_obj_t * obj, lv_style_selector_t selector) {
    lv_font_t * font = lv_obj_get_style_text_font(obj, selector) ;
    if(!font) {
        return "none" ;
    }
    if( &lv_font_montserrat_8 == font ) {
        return "m8" ;
    }
    else if( &lv_font_montserrat_10 == font ) {
        return "m10" ;
    }
    else if( &lv_font_montserrat_12 == font ) {
        return "m12" ;
    }
    else if( &lv_font_montserrat_16 == font ) {
        return "m16" ;
    }
    else if( &lv_font_montserrat_24 == font ) {
        return "m24" ;
    }
    else if( &lv_font_montserrat_32 == font ) {
        return "m32" ;
    }
    else if( &lv_font_montserrat_40 == font ) {
        return "m40" ;
    }
    else if( &lv_font_montserrat_48 == font ) {
        return "m48" ;
    }
    else if( font_msyh() == font ) {
        return "source-han-sans" ;
    }
    else {
        return "unknow" ;
    }
}
