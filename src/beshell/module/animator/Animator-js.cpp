#include "Animator.hpp"
#include "Tween.hpp"
#include "Timeline.hpp"
#include "../../qjs_utils.h"
#include <string>

using namespace std ;

namespace be::animator {

    namespace tween {
        // JS 命名风格(lowerCamelCase)名字表，顺序与 TweenEaseType 一致
        const char * const jsNames[] = {
            "linear",
            "inSine", "outSine", "inOutSine",
            "inQuad", "outQuad", "inOutQuad",
            "inCubic", "outCubic", "inOutCubic",
            "inQuart", "outQuart", "inOutQuart",
            "inQuint", "outQuint", "inOutQuint",
            "inExpo", "outExpo", "inOutExpo",
            "inCirc", "outCirc", "inOutCirc",
            "inBounce", "outBounce", "inOutBounce",
            "inBack", "outBack", "inOutBack",
            "inElastic", "outElastic", "inOutElastic"
        } ;
    }

    /**
     * apiEase(type, t, b, c, d [, s])
     * type 支持两种形式：
     *   - 枚举常量(数字)，如 4 (TWEEN_IN_QUAD)，switch 直接分发，无字符串转换
     *   - JS 命名风格字符串，如 "inQuad"，仅此时才发生 字符串 -> 枚举常量 的转换
     * 底层原生函数，不直接导出到模块，仅供 JS 包装层构建 tween 对象使用
     */
    JSValue Animator::apiEase(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        CHECK_ARGC(5) ;

        tween::TweenEaseType type ;
        if(JS_IsNumber(argv[0])) {
            ARGV_TO_INT32(0, id)
            type = (tween::TweenEaseType) id ;
        }
        else if(JS_IsString(argv[0])) {
            string ARGV_TO_STRING(0, name)
            type = tween::map_name_to_id(name.c_str()) ;
            if(type==tween::TWEEN_EASE_UNKNOWN) {
                JSTHROW("unknown ease function: %s", name.c_str())
            }
        }
        else {
            JSTHROW("ease type must be a number or string")
        }

        ARGV_TO_DOUBLE(1, t)
        ARGV_TO_DOUBLE(2, b)
        ARGV_TO_DOUBLE(3, c)
        ARGV_TO_DOUBLE(4, d)
        ARGV_TO_DOUBLE_OPT(5, s, 1.70158)

        return JS_NewFloat64(ctx, tween::ease(type, (float)t, (float)b, (float)c, (float)d, (float)s)) ;
    }

    /**
     * 缓动函数库
     *
     * 包含 31 个缓动函数，统一签名 `(t, b=0, c=1, d=1, s=1.70158)` ：
     * - t: 当前时间/进度, b: 起始值, c: 变化量, d: 总时长, s: 回弹系数(仅 Back/Elastic 有效)
     * - 默认参数 b=0, c=1, d=1 时可直接传入归一化进度 (0~1) 获得归一化缓动值
     *
     * 函数列表：
     * `linear`, 以及 SINE/QUAD/CUBIC/QUART/QUINT/EXPO/CIRC/BOUNCE/BACK/ELASTIC
     * 十组各三个变体: `inXxx` / `outXxx` / `inOutXxx` (lowerCamelCase)，如
     * `inSine` `outSine` `inOutSine` `inQuad` ... `inElastic` `outElastic` `inOutElastic`
     *
     * 通用形式 `tween.ease(type, ...)` 的 type 参数支持名称字符串或枚举常量。
     *
     * 示例：
     * ```javascript
     * import { tween } from "animator"
     *
     * tween.inQuad(0.5)                    // 0.25
     * tween.ease("inQuad", 0.5)            // 同上，通过名称指定
     * tween.outBounce(250, 0, 100, 1000)   // t=250ms 时从 0 到 100 的 outBounce 值
     * ```
     *
     * @module animator
     * @property tween:object 缓动函数库
     */
    /**
     * 计算缓动值
     *
     * @module animator
     * @function tween.ease
     * @param type:string|number 缓动类型: 名称字符串(如 "inQuad")或枚举常量
     * @param t:number 当前时间/进度
     * @param b:number=0 起始值
     * @param c:number=1 变化量(目标值-起始值)
     * @param d:number=1 总时长
     * @param s:number=1.70158 回弹系数(仅 Back/Elastic 有效)
     * @return number 缓动后的值
     */
    void Animator::exports(JSContext *ctx) {

        JSValue api = JS_NewCFunction(ctx, Animator::apiEase, "apiEase", 6) ;

        // 缓动函数名表(JS 命名风格)传给 JS 工厂，由 JS 侧统一生成包装函数
        // 数组下标即枚举常量值
        JSValue jsNameArr = JS_NewArray(ctx) ;
        for(int i=0; i<tween::TWEEN_EASE_COUNT; i++) {
            JS_SetPropertyUint32(ctx, jsNameArr, i, JS_NewString(ctx, tween::jsNames[i])) ;
        }

        JSValue DEF_JS_FUNC(jsFactory, R"(function(api, names) {
const def = (id)=> (t, b=0, c=1, d=1, s=1.70158)=> api(id, t, b, c, d, s)
const tween = {
    ease: (type, t, b=0, c=1, d=1, s=1.70158)=> api(type, t, b, c, d, s)
}
names.forEach((name, id)=> tween[name] = def(id))
return tween
})", "animator", {
            js_std_dump_error(ctx) ;
            JS_FreeValue(ctx, api) ;
            JS_FreeValue(ctx, jsNameArr) ;
            return ;
        })

        JSValueConst args[2] = {api, jsNameArr} ;
        JSValue tween = JS_Call(ctx, jsFactory, JS_UNDEFINED, 2, args) ;
        JS_FreeValue(ctx, jsFactory) ;
        JS_FreeValue(ctx, api) ;
        JS_FreeValue(ctx, jsNameArr) ;
        if(JS_IsException(tween)) {
            js_std_dump_error(ctx) ;
            return ;
        }

        exportValue("tween", tween) ;

        /**
         * 一次性动画：从 startValue 按 ease 缓动过渡到 endValue
         *
         * 每帧回调 callback(newValue, timeline)；播完自动停止。
         * 调用方不持有返回对象时播完即被 GC 销毁（fire-and-forget）。
         *
         * 示例：
         * ```javascript
         * import { morph } from "animator"
         *
         * // 1 秒内从 0 过渡到 100
         * morph(0, 100, 1000, (value)=> console.log(value), "inOutQuad")
         *
         * // 返回值是 Timeline 对象，可控制播放或监听事件
         * const tl = morph(0, 100, 1000, (value)=> console.log(value))
         * tl.on("stop", ()=> console.log("done"))
         * ```
         *
         * @module animator
         * @function morph
         * @param startValue:number 起始值
         * @param endValue:number 目标值
         * @param durTime:number 过渡时长(毫秒)
         * @param callback:function 每帧回调 callback(newValue, timeline)
         * @param ease:string="linear" 缓动类型名称(同 tween 函数名)
         * @param easeS:number=1.70158 缓动参数(仅 Back/Elastic 有效)
         * @param fps:number=30 帧率
         * @return Timeline 内部时间线对象
         */
        JSValue jscotr = NativeClass::getClass(ctx, Timeline::classID) ;
        if(!JS_IsFunction(ctx, jscotr)) {
            printf("animator: Timeline class not found, morph not exported\n") ;
            JS_FreeValue(ctx, jscotr) ;
            return ;
        }

        JSValue DEF_JS_FUNC(jsMorphFactory, R"(function(Timeline) {
return function morph(startValue, endValue, durTime, callback, ease="linear", easeS=1.70158, fps=30) {
    if(typeof callback!="function") {
        throw new Error("callback is not a function")
    }
    const frames = Math.max(1, Math.round(durTime/1000*fps))
    const tl = new Timeline(fps, frames)
    const target = {value: startValue}
    tl.set(frames, target, "value", endValue, ease, easeS)
    tl.on("frame", ()=> {
        try{
            callback(target.value, tl)
        }catch(e){
            console.log(e)
        }
    })
    tl.play()
    return tl
}
})", "animator-js", {
            js_std_dump_error(ctx) ;
            JS_FreeValue(ctx, jscotr) ;
            return ;
        })

        JSValueConst morphArgs[1] = { jscotr } ;
        JSValue morph = JS_Call(ctx, jsMorphFactory, JS_UNDEFINED, 1, morphArgs) ;
        JS_FreeValue(ctx, jsMorphFactory) ;
        JS_FreeValue(ctx, jscotr) ;
        if(JS_IsException(morph)) {
            js_std_dump_error(ctx) ;
            return ;
        }
        exportValue("morph", morph) ;
    }
}
