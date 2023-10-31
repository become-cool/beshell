/*
 * txiki.js
 *
 * Copyright (c) 2022-present Saúl Ibarra Corretgé <s@saghul.net>
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
 * THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

#include "sjs.h"

#include <curl/curl.h>
#include <string.h>
#include <unistd.h>


static JSValue SJSExit(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    int status;
    if (JS_ToInt32(ctx, &status, argv[0]))
        status = -1;
    // Reset TTY state  (if it had changed) before exiting.
    uv_tty_reset_mode();
    exit(status);
    return JS_UNDEFINED;
}

static JSValue SJSHrtimeMS(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    return JS_NewFloat64(ctx, uv_hrtime() / 1e6);
}

static JSValue SJSUname(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    JSValue obj;
    int r;
    uv_utsname_t utsname;

    r = uv_os_uname(&utsname);
    if (r != 0)
        return SJSThrowErrno(ctx, r);

    obj = JS_NewObjectProto(ctx, JS_NULL);
    JS_DefinePropertyValueStr(ctx, obj, "sysname", JS_NewString(ctx, utsname.sysname), JS_PROP_C_W_E);
    JS_DefinePropertyValueStr(ctx, obj, "release", JS_NewString(ctx, utsname.release), JS_PROP_C_W_E);
    JS_DefinePropertyValueStr(ctx, obj, "version", JS_NewString(ctx, utsname.version), JS_PROP_C_W_E);
    JS_DefinePropertyValueStr(ctx, obj, "machine", JS_NewString(ctx, utsname.machine), JS_PROP_C_W_E);

    return obj;
}

static JSValue SJSGuessHandle(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    int fd;
    if (JS_ToInt32(ctx, &fd, argv[0]))
        return JS_EXCEPTION;

    switch (uv_guess_handle(fd)) {
        case UV_TTY:
            return JS_NewString(ctx, "tty");
        case UV_NAMED_PIPE:
            return JS_NewString(ctx, "pipe");
        case UV_FILE:
            return JS_NewString(ctx, "file");
        case UV_TCP:
            return JS_NewString(ctx, "tcp");
        case UV_UDP:
            return JS_NewString(ctx, "udp");
        default:
            return JS_NewString(ctx, "unknown");
    }
}

static JSValue SJSEnviron(JSContext *ctx, JSValueConst this_val) {
    uv_env_item_t *env;
    int envcount, r, i;

    r = uv_os_environ(&env, &envcount);
    if (r != 0)
        return SJSThrowErrno(ctx, r);

    JSValue obj = JS_NewObjectProto(ctx, JS_NULL);

    for (i = 0; i < envcount; i++) {
        JS_DefinePropertyValueStr(ctx, obj, env[i].name, JS_NewString(ctx, env[i].value), JS_PROP_C_W_E);
    }

    uv_os_free_environ(env, envcount);

    return obj;
}

static JSValue SJSGetEnv(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    const char *name, *str;
    name = JS_ToCString(ctx, argv[0]);
    if (!name)
        return JS_EXCEPTION;
    str = getenv(name);
    JS_FreeCString(ctx, name);
    if (!str)
        return JS_UNDEFINED;
    else
        return JS_NewString(ctx, str);
}

static JSValue SJSSetEnv(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    if (!JS_IsString(argv[0]))
        return JS_ThrowTypeError(ctx, "expected a string");
    if (JS_IsUndefined(argv[1]))
        return JS_ThrowTypeError(ctx, "expected a value");

    const char *name = JS_ToCString(ctx, argv[0]);
    if (!name)
        return JS_EXCEPTION;

    const char *value = JS_ToCString(ctx, argv[1]);
    if (!value)
        return JS_EXCEPTION;

    int r = uv_os_setenv(name, value);
    JS_FreeCString(ctx, name);
    JS_FreeCString(ctx, value);
    if (r != 0)
        return SJSThrowErrno(ctx, r);

    return JS_UNDEFINED;
}

static JSValue SJSUnsetEnv(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    if (!JS_IsString(argv[0]))
        return JS_ThrowTypeError(ctx, "expected a string");

    const char *name = JS_ToCString(ctx, argv[0]);
    if (!name)
        return JS_EXCEPTION;

    int r = uv_os_unsetenv(name);
    JS_FreeCString(ctx, name);
    if (r != 0)
        return SJSThrowErrno(ctx, r);

    return JS_UNDEFINED;
}

static JSValue SJSCwd(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    char buf[1024];
    size_t size = sizeof(buf);
    char *dbuf = buf;
    int r;

    r = uv_cwd(dbuf, &size);
    if (r != 0) {
        if (r != UV_ENOBUFS)
            return SJSThrowErrno(ctx, r);
        dbuf = js_malloc(ctx, size);
        if (!dbuf)
            return JS_EXCEPTION;
        r = uv_cwd(dbuf, &size);
        if (r != 0) {
            js_free(ctx, dbuf);
            return SJSThrowErrno(ctx, r);
        }
    }

    JSValue ret = JS_NewStringLen(ctx, dbuf, size);

    if (dbuf != buf)
        js_free(ctx, dbuf);

    return ret;
}

static JSValue SJSHomeDir(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    char buf[1024];
    size_t size = sizeof(buf);
    char *dbuf = buf;
    int r;

    r = uv_os_homedir(dbuf, &size);
    if (r != 0) {
        if (r != UV_ENOBUFS)
            return SJSThrowErrno(ctx, r);
        dbuf = js_malloc(ctx, size);
        if (!dbuf)
            return JS_EXCEPTION;
        r = uv_os_homedir(dbuf, &size);
        if (r != 0) {
            js_free(ctx, dbuf);
            return SJSThrowErrno(ctx, r);
        }
    }

    JSValue ret = JS_NewStringLen(ctx, dbuf, size);

    if (dbuf != buf)
        js_free(ctx, dbuf);

    return ret;
}

static JSValue SJSTmpDir(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    char buf[1024];
    size_t size = sizeof(buf);
    char *dbuf = buf;
    int r;

    r = uv_os_tmpdir(dbuf, &size);
    if (r != 0) {
        if (r != UV_ENOBUFS)
            return SJSThrowErrno(ctx, r);
        dbuf = js_malloc(ctx, size);
        if (!dbuf)
            return JS_EXCEPTION;
        r = uv_os_tmpdir(dbuf, &size);
        if (r != 0) {
            js_free(ctx, dbuf);
            return SJSThrowErrno(ctx, r);
        }
    }

    JSValue ret = JS_NewStringLen(ctx, dbuf, size);

    if (dbuf != buf)
        js_free(ctx, dbuf);

    return ret;
}

static JSValue SJSRandom(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    size_t size;
    uint8_t *buf = JS_GetArrayBuffer(ctx, &size, argv[0]);
    if (!buf)
        return JS_EXCEPTION;

    uint64_t off = 0;
    if (!JS_IsUndefined(argv[1]) && JS_ToIndex(ctx, &off, argv[1]))
        return JS_EXCEPTION;

    uint64_t len = size;
    if (!JS_IsUndefined(argv[2]) && JS_ToIndex(ctx, &len, argv[2]))
        return JS_EXCEPTION;

    if (off + len > size)
        return JS_ThrowRangeError(ctx, "array buffer overflow");

    int r = uv_random(NULL, NULL, buf + off, len, 0, NULL);
    if (r != 0)
        return SJSThrowErrno(ctx, r);

    return JS_UNDEFINED;
}

static JSValue SJSCpuInfo(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    uv_cpu_info_t *infos;
    int count, i;
    int r = uv_cpu_info(&infos, &count);
    if (r != 0)
        return SJSThrowErrno(ctx, r);

    JSValue val = JS_NewArray(ctx);

    for (i = 0; i < count; i++) {
        uv_cpu_info_t info = infos[i];

        JSValue v = JS_NewObjectProto(ctx, JS_NULL);

        JS_DefinePropertyValueStr(ctx, v, "model", JS_NewString(ctx, info.model), JS_PROP_C_W_E);
        JS_DefinePropertyValueStr(ctx, v, "speed", JS_NewInt64(ctx, info.speed), JS_PROP_C_W_E);

        JSValue t = JS_NewObjectProto(ctx, JS_NULL);
        JS_DefinePropertyValueStr(ctx, t, "user", JS_NewFloat64(ctx, info.cpu_times.user), JS_PROP_C_W_E);
        JS_DefinePropertyValueStr(ctx, t, "nice", JS_NewFloat64(ctx, info.cpu_times.nice), JS_PROP_C_W_E);
        JS_DefinePropertyValueStr(ctx, t, "sys", JS_NewFloat64(ctx, info.cpu_times.sys), JS_PROP_C_W_E);
        JS_DefinePropertyValueStr(ctx, t, "idle", JS_NewFloat64(ctx, info.cpu_times.idle), JS_PROP_C_W_E);
        JS_DefinePropertyValueStr(ctx, t, "irq", JS_NewFloat64(ctx, info.cpu_times.irq), JS_PROP_C_W_E);
        JS_DefinePropertyValueStr(ctx, v, "times", t, JS_PROP_C_W_E);

        JS_SetPropertyUint32(ctx, val, i, v);
    }

    uv_free_cpu_info(infos, count);

    return val;
}

static JSValue SJSLoadavg(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    double avg[3] = { -1, -1, -1 };

    uv_loadavg(avg);

    JSValue val = JS_NewArray(ctx);

    JS_SetPropertyUint32(ctx, val, 0, JS_NewFloat64(ctx, avg[0]));
    JS_SetPropertyUint32(ctx, val, 1, JS_NewFloat64(ctx, avg[1]));
    JS_SetPropertyUint32(ctx, val, 2, JS_NewFloat64(ctx, avg[2]));

    return val;
}

static JSValue SJSNetworkInterfaces(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    uv_interface_address_t *interfaces;
    int count, i;
    int r = uv_interface_addresses(&interfaces, &count);
    if (r != 0)
        return SJSThrowErrno(ctx, r);

    JSValue val = JS_NewArray(ctx);

    for (i = 0; i < count; i++) {
        uv_interface_address_t iface = interfaces[i];
        char mac[18];
        char buf[INET6_ADDRSTRLEN + 1];

        JSValue addr = JS_NewObjectProto(ctx, JS_NULL);

        JS_DefinePropertyValueStr(ctx, addr, "name", JS_NewString(ctx, iface.name), JS_PROP_C_W_E);

        snprintf(mac,
                 sizeof(mac),
                 "%02x:%02x:%02x:%02x:%02x:%02x",
                 (unsigned char)iface.phys_addr[0],
                 (unsigned char)iface.phys_addr[1],
                 (unsigned char)iface.phys_addr[2],
                 (unsigned char)iface.phys_addr[3],
                 (unsigned char)iface.phys_addr[4],
                 (unsigned char)iface.phys_addr[5]);
        JS_DefinePropertyValueStr(ctx, addr, "mac", JS_NewString(ctx, mac), JS_PROP_C_W_E);

        if (iface.address.address4.sin_family == AF_INET) {
            uv_ip4_name(&iface.address.address4, buf, sizeof(buf));
        } else if (iface.address.address4.sin_family == AF_INET6) {
            uv_ip6_name(&iface.address.address6, buf, sizeof(buf));
            JS_DefinePropertyValueStr(ctx, addr, "scopeId", JS_NewUint32(ctx, iface.address.address6.sin6_scope_id), JS_PROP_C_W_E);
        }
        JS_DefinePropertyValueStr(ctx, addr, "address", JS_NewString(ctx, buf), JS_PROP_C_W_E);

        if (iface.netmask.netmask4.sin_family == AF_INET) {
            uv_ip4_name(&iface.netmask.netmask4, buf, sizeof(buf));
        } else if (iface.netmask.netmask4.sin_family == AF_INET6) {
            uv_ip6_name(&iface.netmask.netmask6, buf, sizeof(buf));
        }
        JS_DefinePropertyValueStr(ctx, addr, "netmask", JS_NewString(ctx, buf), JS_PROP_C_W_E);

        JS_DefinePropertyValueStr(ctx, addr, "internal", JS_NewBool(ctx, iface.is_internal), JS_PROP_C_W_E);

        JS_SetPropertyUint32(ctx, val, i, addr);
    }

    uv_free_interface_addresses(interfaces, count);

    return val;
}

static JSValue SJSGethostName(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    char buf[UV_MAXHOSTNAMESIZE];
    size_t size = sizeof(buf);

    int r = uv_os_gethostname(buf, &size);
    if (r != 0)
        return SJSThrowErrno(ctx, r);

    return JS_NewStringLen(ctx, buf, size);
}

static JSValue SJSGetPid(JSContext *ctx, JSValueConst this_val) {
    return JS_NewInt32(ctx, uv_os_getpid());
}

static JSValue SJSGetPpid(JSContext *ctx, JSValueConst this_val) {
    return JS_NewInt32(ctx, uv_os_getppid());
}

static JSValue SJSUserInfo(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    uv_passwd_t p;

    int r = uv_os_get_passwd(&p);
    if (r != 0)
        return SJSThrowErrno(ctx, r);

    JSValue obj = JS_NewObjectProto(ctx, JS_NULL);
    JS_DefinePropertyValueStr(ctx, obj, "username", JS_NewString(ctx, p.username), JS_PROP_C_W_E);
    JS_DefinePropertyValueStr(ctx, obj, "uid", JS_NewInt32(ctx, p.uid), JS_PROP_C_W_E);
    JS_DefinePropertyValueStr(ctx, obj, "gid", JS_NewInt32(ctx, p.gid), JS_PROP_C_W_E);
    JS_DefinePropertyValueStr(ctx, obj, "shell", p.shell ? JS_NewString(ctx, p.shell) : JS_NULL, JS_PROP_C_W_E);
    JS_DefinePropertyValueStr(ctx, obj, "homedir", p.homedir ? JS_NewString(ctx, p.homedir) : JS_NULL, JS_PROP_C_W_E);

    uv_os_free_passwd(&p);

    return obj;
}

static JSValue SJSAvailableParallelism(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    return JS_NewUint32(ctx, uv_available_parallelism());
}

static const JSCFunctionListEntry SJSOSFuncs[] = {
    SJS_CONST(AF_INET),
    SJS_CONST(AF_INET6),
    SJS_CONST(AF_UNSPEC),
    SJS_CONST(STDIN_FILENO),
    SJS_CONST(STDOUT_FILENO),
    SJS_CONST(STDERR_FILENO),
    SJS_CFUNC_DEF("exit", 1, SJSExit),
    SJS_CFUNC_DEF("hrtimeMs", 0, SJSHrtimeMS),
    SJS_CFUNC_DEF("uname", 0, SJSUname),
    SJS_CFUNC_DEF("guessHandle", 1, SJSGuessHandle),
    SJS_CFUNC_DEF("getenv", 0, SJSGetEnv),
    SJS_CFUNC_DEF("setenv", 2, SJSSetEnv),
    SJS_CFUNC_DEF("unsetenv", 1, SJSUnsetEnv),
    SJS_CFUNC_DEF("cwd", 0, SJSCwd),
    SJS_CFUNC_DEF("homedir", 0, SJSHomeDir),
    SJS_CFUNC_DEF("tmpdir", 0, SJSTmpDir),
    SJS_CFUNC_DEF("random", 3, SJSRandom),
    SJS_CFUNC_DEF("cpuInfo", 0, SJSCpuInfo),
    SJS_CFUNC_DEF("loadavg", 0, SJSLoadavg),
    SJS_CFUNC_DEF("networkInterfaces", 0, SJSNetworkInterfaces),
    SJS_CFUNC_DEF("gethostname", 0, SJSGethostName),
    SJS_CGETSET_DEF("environ", SJSEnviron, NULL),
    SJS_CGETSET_DEF("pid", SJSGetPid, NULL),
    SJS_CGETSET_DEF("ppid", SJSGetPpid, NULL),
    SJS_CFUNC_DEF("userInfo", 0, SJSUserInfo),
    SJS_CFUNC_DEF("availableParallelism", 0, SJSAvailableParallelism),
};

static const JSCFunctionListEntry os[] = {
    SJS_OBJECT_DEF("os", SJSOSFuncs, countof(SJSOSFuncs))
};

void SJSInitOS(SJSRuntime* qrt, JSValue ns) {
    SJSRegistJSApi(&os);
}
