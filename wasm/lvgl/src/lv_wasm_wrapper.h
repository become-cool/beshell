#ifndef ONCE_H_LV_WASM_WRAPPER
#define ONCE_H_LV_WASM_WRAPPER

#include "lvgl.h"

EMSCRIPTEN_KEEPALIVE lv_obj_t * lv_obj_create(lv_obj_t * parent);

#endif