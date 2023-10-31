#include "font.hpp"

void CompSetFontSize (lv_obj_t* comp, lv_style_t* style, JSContext* ctx, JSValue obj) {
    int x;
    JS_ToInt32(ctx, &x, obj);

    lv_style_set_text_font(style, &builtin_font_list[x]);
};

void CompSetFontSize1 (lv_obj_t* comp, lv_style_t* style, JSContext* ctx, JSValue obj) {
    int x;
    JS_ToInt32(ctx, &x, obj);

    lv_style_set_text_font(style, &builtin_font_list[x]);
};