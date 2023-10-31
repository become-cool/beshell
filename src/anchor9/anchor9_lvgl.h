#ifndef __H_ANCHOR9_LVGL__
#define __H_ANCHOR9_LVGL__

#include "quickjs-libc.h"
#include "anchor9.h"



JSValue js_lv_obj_link_to(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;

JSValue js_lv_obj_unlink(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;

JSValue js_lv_obj_each_link(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;


#endif