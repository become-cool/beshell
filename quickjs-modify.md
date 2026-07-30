# QuickJS 修改记录

升级 QuickJS 版本时需要还原/重新应用以下修改。

---

## 1. `Array.from` 字符串快速路径

**文件**: `src/beshell/deps/quickjs/quickjs.c`
**位置**: `js_array_from` 函数（约 37711 行）
**日期**: 2026-07-29
**原因**: ESP32-S3 PSRAM 环境下，字符串迭代器 `js_string_iterator_next` 使用的 `JSArrayIteratorData` 结构（通过 `js_malloc` 分配在 PSRAM）在内存压力下被破坏，导致 `string_getc` → `LoadProhibited` 崩溃。

### 修改内容

在 `js_array_from` 中，`JS_GetProperty(ctx, items, JS_ATOM_Symbol_iterator)` 之后、`if (!JS_IsUndefined(iter))` 之前，插入字符串快速路径：

```c
    /* Fast path for strings: use indexed access instead of the iterator
       protocol to avoid an ESP32-S3 PSRAM memory corruption bug in the
       string iterator (js_string_iterator_next). The iterator writes to
       JSArrayIteratorData via a js_malloc'd pointer which can become
       stale when PSRAM heap pressure is high. */
    if (JS_IsString(items)) {
        JS_FreeValue(ctx, iter);
        const JSString *str = JS_VALUE_GET_STRING(items);
        len = str->len;
        if (JS_IsConstructor(ctx, this_val))
            r = JS_CallConstructor(ctx, this_val, 0, NULL);
        else
            r = JS_NewArray(ctx);
        if (JS_IsException(r))
            goto exception;
        for (k = 0; k < len; k++) {
            v = JS_GetPropertyUint32(ctx, items, (uint32_t)k);
            if (JS_IsException(v))
                goto exception;
            if (mapping) {
                args[0] = v;
                args[1] = JS_NewInt64(ctx, k);
                v2 = JS_Call(ctx, mapfn, this_arg, 2, args);
                JS_FreeValue(ctx, v);
                v = v2;
                if (JS_IsException(v))
                    goto exception;
            }
            if (JS_DefinePropertyValueInt64(ctx, r, k, v,
                                            JS_PROP_C_W_E | JS_PROP_THROW) < 0)
                goto exception;
        }
    } else if (!JS_IsUndefined(iter)) {
        // ... 原有迭代器路径 ...
```

### 影响

- `Array.from(string)` 对 BMP 字符（含中日常用汉字）行为不变
- 补充平面字符（emoji 等）可能从 code point 变为 surrogate pair（与 `str[i]` 行为一致）
- 非字符串参数走原有迭代器路径，不受影响

### 还原方法

删除 `JS_GetProperty(ctx, items, JS_ATOM_Symbol_iterator)` 之后的 `if (JS_IsString(items)) { ... }` 块，
将 `} else if (!JS_IsUndefined(iter)) {` 恢复为 `if (!JS_IsUndefined(iter)) {`。

---

## 2. （后续修改记录在此追加）
