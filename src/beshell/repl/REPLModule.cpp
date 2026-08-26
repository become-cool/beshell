#include "REPLModule.hpp"
#include "REPLChannelNClass.hpp"
#include "../js/repl.c"
#include "qjs_utils.h"
#include "../BeShell.hpp"

namespace be{

    /**
     * REPL 交互式命令行模块
     * 
     * REPL (Read-Eval-Print Loop) 提供交互式 JavaScript 执行环境，
     * 支持通过串口、WebSocket、USB CDC、MQTT 等多种通道进行远程控制和调试。
     * REPL 还支持文件系统操作。
     * 
     * ## 模块启用
     * 
     * 但具体的 REPL 通道需要单独启用：
     * 
     * | 通道 | 启用方式 | 说明 |
     * |------|---------|------|
     * | UART0 | 默认启用 | 默认激活，无需额外配置 |
     * | USB CDC | `beshell.use<REPLCDC>()` | 需要芯片支持 USB OTG 且 sdkconfig 中开启 |
     * | WebSocket | JS 侧启用 | 使用 `repl.ws.connect()` |
     * | MQTT | JS 侧启用 | 使用 `repl.mqtt.start()` |
     * 
     * > `uart0` 通道是 `BeShell` 默认的 REPL 通道，不需要做额外配置就可以工作。
     * 
     * ## 主要功能
     * 
     * - **命令执行**: 远程执行 JavaScript 代码
     * - **文件传输**: 通过 REPL 协议上传/下载文件
     * - **加密通信**: 支持 AES-CTR 加密保护通信内容
     * - **多通道支持**: 串口、WebSocket、USB CDC、MQTT
     * 
     * 示例：
     * ```javascript
     * import * as repl from "repl"
     * 
     * // 创建自定义 REPL 通道
     * const channel = new repl.REPLChannel()
     * 
     * // 启用 WebSocket REPL
     * const ws = repl.ws.connect("ws://192.168.1.100:8080")
     * 
     * // 启用 USB CDC REPL（需要 C++ 侧启用 REPLCDC）
     * repl.cdc.start()
     * 
     * // 启用 MQTT REPL
     * repl.mqtt.start(mqttClient, "beshell/repl/in", "beshell/repl/out")
     * 
     * // 设置访问密码
     * repl.setPassword("mysecret")
     * 
     * // 启用加密通信
     * repl.enableCrypto()
     * repl.setCryptoKey(keyBuffer, viBuffer)
     * ```
     * 
     * @module repl
     */
    char const * const REPLModule::name = "repl" ;
    REPLModule * REPLModule::singleton = nullptr ;

    // C++ relay function: bridges REPL::unknownPkgHook → EventModule::emitSyncFree("pkg.unknown")
    void REPLModule::unknownPkgRelay(REPLChannel * ch, std::unique_ptr<Package> pkg) {
        if (!singleton) {
            return ;
        }

        JSContext *ctx = singleton->ctx ;

        // Build head fields as JS object
        JSValue headObj = JS_NewObject(ctx) ;
        JS_SetPropertyStr(ctx, headObj, "h1", JS_NewInt32(ctx, pkg->head.fields.h1)) ;
        JS_SetPropertyStr(ctx, headObj, "h2", JS_NewInt32(ctx, pkg->head.fields.h2)) ;
        JS_SetPropertyStr(ctx, headObj, "pkgid", JS_NewInt32(ctx, pkg->head.fields.pkgid)) ;
        JS_SetPropertyStr(ctx, headObj, "cmd", JS_NewInt32(ctx, pkg->head.fields.cmd)) ;
        JS_SetPropertyStr(ctx, headObj, "len1", JS_NewInt32(ctx, pkg->head.fields.len1)) ;
        JS_SetPropertyStr(ctx, headObj, "len2", JS_NewInt32(ctx, pkg->head.fields.len2)) ;

        // Build body ArrayBuffer — no copy needed, emitSyncFree is synchronous
        JSValue bodyBuf ;
        if (pkg->body() && pkg->body_len > 0) {
            bodyBuf = JS_NewArrayBuffer(ctx, pkg->body(), pkg->body_len, nofreeArrayBuffer, NULL, false) ;
        } else {
            bodyBuf = JS_NewArrayBuffer(ctx, nullptr, 0, nofreeArrayBuffer, NULL, false) ;
        }

        singleton->emitSyncFree("pkg.unknown", {headObj, bodyBuf}) ;
    }

    std::vector<NativeModuleExportorFunc> REPLModule::exportors ;

    REPLModule::REPLModule(JSContext * ctx, const char * name)
        : EventModule(ctx, name, 0)
    {
        exportClass<REPLChannelNClass>() ;
        exportName("ws") ;
        exportName("cdc") ;
        exportName("mqtt") ;
        exportName("startLog") ;
        exportName("stopLog") ;
        
        EXPORT_FUNCTION(enableCrypto) ;
        EXPORT_FUNCTION(disableCrypto) ;
        EXPORT_FUNCTION(setCryptoKey) ;
        EXPORT_FUNCTION(setPassword) ;
        EXPORT_FUNCTION(enableUnknownPkgEvent) ;
        EXPORT_FUNCTION(disableUnknownPkgEvent) ;

        singleton = this ;
    }

    void REPLModule::registerExportor(NativeModuleExportorFunc func) {
        exportors.push_back(func) ;
    }

    void REPLModule::exports(JSContext *ctx) {

        EventModule::exports(ctx) ;

        for(auto func : exportors) {
            func(ctx, this) ;
        }

        JSEngineEvalEmbeded(ctx, repl)
    }

    /**
     * 启用 REPL 加密通信
     * 
     * 启用 AES-CTR 加密模式，保护 REPL 通信内容。
     * 启用后需要使用 `setCryptoKey()` 设置加密密钥。
     * 
     * 示例：
     * ```javascript
     * import * as repl from "repl"
     * 
     * // 启用加密
     * repl.enableCrypto()
     * 
     * // 设置 16 字节密钥和初始向量
     * const key = new Uint8Array([0x01, 0x02, ...])  // 16 字节
     * const vi = new Uint8Array([0x10, 0x20, ...])   // 16 字节
     * repl.setCryptoKey(key.buffer, vi.buffer)
     * ```
     * 
     * @module repl
     * @function enableCrypto
     * @return undefined
     * @see setCryptoKey
     */
    JSValue REPLModule::enableCrypto(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        JSEngine::fromJSContext(ctx)->beshell->repl->enableCrypto = true ;
        return JS_UNDEFINED ;
    }
    /**
     * 禁用 REPL 加密通信
     * 
     * 关闭 AES-CTR 加密，恢复明文通信模式。
     * 
     * 示例：
     * ```javascript
     * import * as repl from "repl"
     * 
     * // 禁用加密
     * repl.disableCrypto()
     * ```
     * 
     * @module repl
     * @function disableCrypto
     * @return undefined
     */
    JSValue REPLModule::disableCrypto(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        JSEngine::fromJSContext(ctx)->beshell->repl->enableCrypto = false ;
        return JS_UNDEFINED ;
    }
    /**
     * 设置 REPL 加密密钥
     * 
     * 配置 AES-CTR 加密所需的密钥和初始向量（IV）。
     * 密钥和 IV 都必须是 16 字节（128 位）的 ArrayBuffer。
     * 
     * 示例：
     * ```javascript
     * import * as repl from "repl"
     * 
     * // 准备 16 字节密钥和 IV
     * const key = new Uint8Array([
     *     0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08,
     *     0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F, 0x10
     * ])
     * const vi = new Uint8Array([
     *     0x10, 0x20, 0x30, 0x40, 0x50, 0x60, 0x70, 0x80,
     *     0x90, 0xA0, 0xB0, 0xC0, 0xD0, 0xE0, 0xF0, 0x00
     * ])
     * 
     * // 设置密钥并启用加密
     * repl.setCryptoKey(key.buffer, vi.buffer)
     * repl.enableCrypto()
     * ```
     * 
     * @module repl
     * @function setCryptoKey
     * @param key:ArrayBuffer 16 字节 AES 密钥
     * @param vi:ArrayBuffer 16 字节初始向量 (IV)
     * @return undefined
     * @throws 密钥长度不是 16 字节
     * @throws IV 长度不是 16 字节
     * @see enableCrypto
     */
    JSValue REPLModule::setCryptoKey(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        CHECK_ARGC(2)
        if(!JS_IsArrayBuffer(argv[0])){
            JSTHROW("crypto key must be array buffer") ;
        }
        if(!JS_IsArrayBuffer(argv[1])){
            JSTHROW("crypto vi must be array buffer") ;
        }

        size_t key_len, vi_len ;
        uint8_t * key = JS_GetArrayBuffer(ctx,&key_len, argv[0]) ;
        uint8_t * vi = JS_GetArrayBuffer(ctx,&vi_len, argv[1]) ;

        if(key_len!=16) {
            JSTHROW("crypto key length must be 16") ;
        }
        if(vi_len!=16) {
            JSTHROW("crypto vi length must be 16") ;
        }

        auto repl = JSEngine::fromJSContext(ctx)->beshell->repl ;
        memcpy( repl->cryptoKey, key, 16) ;
        memcpy( repl->cryptoVI, vi, 16) ;

        return JS_UNDEFINED ;
    }

    /**
     * 设置 REPL 访问密码
     * 
     * 设置 REPL 远程访问的密码验证。设置后，远程连接需要提供正确的密码才能执行命令。
     * 密码验证在命令执行前进行，用于防止未授权访问。
     * 
     * 示例：
     * ```javascript
     * import * as repl from "repl"
     * 
     * // 设置访问密码
     * repl.setPassword("mySecretPassword")
     * 
     * // 清除密码（空字符串表示无密码）
     * repl.setPassword("")
     * ```
     * 
     * @module repl
     * @function setPassword
     * @param password:string 访问密码，空字符串表示不启用密码验证
     * @return undefined
     */
    JSValue REPLModule::setPassword(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        CHECK_ARGC(1)
        const char * pwd = JS_ToCString(ctx, argv[0]) ;
        JSEngine::fromJSContext(ctx)->beshell->cammonds->setPassword(pwd) ;
        JS_FreeCString(ctx, pwd) ;
        return JS_UNDEFINED ;
    }

    JSValue REPLModule::enableUnknownPkgEvent(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        JSEngine::fromJSContext(ctx)->beshell->repl->unknownPkgHook = unknownPkgRelay ;
        return JS_UNDEFINED ;
    }

    JSValue REPLModule::disableUnknownPkgEvent(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        JSEngine::fromJSContext(ctx)->beshell->repl->unknownPkgHook = nullptr ;
        return JS_UNDEFINED ;
    }
}

// ============================================================================
// 以下对象和函数在 js/repl.js 中实现，通过 exportValue 导出到 repl 模块
// ============================================================================

/**
 * WebSocket REPL 对象
 * 
 * 提供通过 WebSocket 连接进行 REPL 远程控制的功能。
 * 
 * 示例：
 * ```javascript
 * import * as repl from "repl"
 * 
 * // 连接到 WebSocket 服务器
 * const conn = repl.ws.connect("ws://192.168.1.100:8080")
 * 
 * // 连接成功后即可通过 WebSocket 进行 REPL 通信
 * ```
 * 
 * @module repl
 * @object ws
 */

/**
 * 连接到 WebSocket REPL 服务器
 * 
 * 建立 WebSocket 连接并将其作为 REPL 通信通道。
 * 连接成功后，服务器可以通过此连接发送 REPL 命令。
 * 
 * 示例：
 * ```javascript
 * import * as repl from "repl"
 * 
 * const conn = repl.ws.connect("ws://192.168.1.100:8080/repl")
 * 
 * // 检查连接状态
 * if (conn.isConnected()) {
 *     console.log("WebSocket REPL 已连接")
 * }
 * ```
 * 
 * @module repl
 * @object ws
 * @function connect
 * @param url:string WebSocket 服务器地址
 * @return object WebSocket 连接对象
 */

/**
 * USB CDC REPL 对象
 * 
 * 提供通过 USB CDC (USB Serial/JTAG) 进行 REPL 控制的功能。
 * 
 * ## 前置条件
 * 
 * 1. **芯片支持**: 需要 ESP32-S2/S3/C3/C6 等支持 USB Serial/JTAG 的芯片
 * 2. **编译条件**: 代码以 `SOC_USB_SERIAL_JTAG_SUPPORTED` 宏保护，芯片不支持时自动禁用
 * 3. **C++ 启用**: 在 C++ 代码中调用 `beshell.use<REPLCDC>()`
 * 
 * ## C++ 启用示例
 * 
 * ```cpp
 * #include "soc/soc_caps.h"
 * 
 * // 需要检查芯片是否支持 USB Serial/JTAG
 * #if SOC_USB_SERIAL_JTAG_SUPPORTED
 *     beshell.use<REPLCDC>() ;
 * #endif
 * ```
 * 
 * ## JS 使用示例
 * 
 * ```javascript
 * import * as repl from "repl"
 * 
 * // 启动 USB CDC REPL
 * try {
 *     repl.cdc.start()
 *     console.log("USB CDC REPL 已启动")
 * } catch(e) {
 *     console.error("CDC 模块未启用:", e.message)
 * }
 * ```
 * 
 * @module repl
 * @object cdc
 */

/**
 * 启动 USB CDC REPL
 * 
 * 初始化 USB Serial/JTAG 并设置为 REPL 通信通道。
 * 
 * ## 前置要求
 * 
 * 必须在 C++ 侧先启用 REPLCDC：
 * ```cpp
 * #if SOC_USB_SERIAL_JTAG_SUPPORTED
 *     beshell.use<REPLCDC>() ;
 * #endif
 * ```
 * 
 * ## 使用示例
 * ```javascript
 * import * as repl from "repl"
 * 
 * // 启动 CDC REPL
 * repl.cdc.start()
 * 
 * // 现在可以通过 USB 连接进行 REPL 通信
 * ```
 * 
 * @module repl
 * @object cdc
 * @function start
 * @return undefined
 * @throws CDC 模块未启用时抛出错误
 */

/**
 * MQTT REPL 对象
 * 
 * 提供通过 MQTT 协议进行 REPL 远程控制的功能。
 * 适用于物联网场景下的远程设备管理。
 * 
 * 示例：
 * ```javascript
 * import * as repl from "repl"
 * import { MQTT } from "mqtt"
 * 
 * // 创建 MQTT 客户端
 * const mqtt = new MQTT()
 * mqtt.setup({
 *     host: "mqtt.eclipse.org",
 *     port: 1883,
 *     clientId: "beshell-device"
 * })
 * 
 * // 启动 MQTT REPL
 * repl.mqtt.start(mqtt)
 * 
 * // 停止 MQTT REPL
 * repl.mqtt.stop()
 * ```
 * 
 * @module repl
 * @object mqtt
 */

/**
 * 启动 MQTT REPL
 * 
 * 将 MQTT 连接设置为 REPL 通信通道。
 * 默认使用主题格式：`beshell/repl/in/<mac>` 和 `beshell/repl/out/<mac>`
 * 
 * 示例：
 * ```javascript
 * import * as repl from "repl"
 * import { MQTT } from "mqtt"
 * 
 * const mqtt = new MQTT()
 * mqtt.setup({ host: "broker.hivemq.com", port: 1883 })
 * 
 * // 使用默认主题（基于设备 MAC 地址）
 * repl.mqtt.start(mqtt)
 * 
 * // 或使用自定义主题
 * repl.mqtt.start(mqtt, "mydevice/cmd", "mydevice/resp")
 * ```
 * 
 * @module repl
 * @object mqtt
 * @function start
 * @param mqtt:MQTT MQTT 客户端实例
 * @param topicIn:string 输入主题（命令），默认为 `beshell/repl/in/<mac>`
 * @param topicOut:string 输出主题（响应），默认为 `beshell/repl/out/<mac>`
 * @return undefined
 * @throws MQTT REPL 已启动时抛出错误
 */

/**
 * 停止 MQTT REPL
 * 
 * 关闭 MQTT REPL 通道并取消订阅相关主题。
 * 
 * 示例：
 * ```javascript
 * import * as repl from "repl"
 * 
 * // 停止 MQTT REPL
 * repl.mqtt.stop()
 * ```
 * 
 * @module repl
 * @object mqtt
 * @function stop
 * @return undefined
 */
