#ifndef _MODULE_LVGL_JS_UTILS_H
#define _MODULE_LVGL_JS_UTILS_H

#include "quickjs-libc.h"

#define DEF_CLASS_START(name)                                       \
    static JSClassID js_##name##_class_id ;                         \
    static void js_##name##_finalizer(JSRuntime *rt, JSValue val)

#define DEF_CLASS_END(name, jsname)                         \
    static JSClassDef js_##name##_class = {                 \
        jsname,                                             \
        .finalizer = js_##name##_finalizer,                 \
    };


#define REGISTER_JS_CLASS(name)                                                                                 \
    {                                                                                                           \
        JS_NewClass(JS_GetRuntime(ctx), js_##name##_class_id, &js_##name##_class);                              \
        JSValue proto = JS_NewObject(ctx);                                                                      \
        JS_SetPropertyFunctionList(ctx, proto, js_##name##_proto_funcs, countof(js_##name##_proto_funcs));      \
        JS_SetClassProto(ctx, js_##name##_class_id, proto);                                                     \
    }


#define REGISTER_JS_CLASS_CTOR(name, ctorfun, jsname, jsctro)                                                   \
    JSValue jsctro = JS_NewCFunction2(ctx, ctorfun, jsname, 1, JS_CFUNC_constructor, 0) ;                       \
    {                                                                                                           \
        JS_NewClass(JS_GetRuntime(ctx), js_##name##_class_id, &js_##name##_class);                              \
        JSValue proto = JS_NewObject(ctx);                                                                      \
        JS_SetPropertyFunctionList(ctx, proto, js_##name##_proto_funcs, countof(js_##name##_proto_funcs));      \
        JS_SetClassProto(ctx, js_##name##_class_id, proto);                                                     \
        JS_SetConstructor(ctx, jsctro, proto) ;                                                                 \
    }
#endif