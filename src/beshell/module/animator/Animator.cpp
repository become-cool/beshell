#include "Animator.hpp"
#include "Timeline.hpp"

namespace be::animator {

    /**
     * 动画模块
     *
     * 提供基于 BeShell 主循环的动画能力，导出三个成员：
     * - `tween` : 缓动函数库（linear / inQuad / outBounce / ... 共 31 种）
     * - `Timeline` : 关键帧动画时间线类（帧节拍、跨关键帧插值、事件）
     * - `morph()` : 一次性值过渡动画（基于 Timeline 实现）
     *
     * 缓动函数曲线效果预览：[缓动函数速查表](https://www.xuanfengge.com/easeing/easeing/)
     *
     * 示例：
     * ```javascript
     * import { tween, Timeline, morph } from "animator"
     *
     * // 1. tween: 直接计算缓动值
     * tween.inQuad(0.5)          // 0.25 (t=0.5 时的 inQuad 缓动值)
     * tween.ease("outBounce", 0.5)  // 通过名称指定缓动类型
     *
     * // 2. morph: 一次性动画, 1 秒内从 0 过渡到 100
     * morph(0, 100, 1000, (value)=> {
     *     console.log(value)     // 每帧回调新值
     * }, "inOutQuad")
     *
     * // 3. Timeline: 关键帧动画
     * const obj = { x: 0 }
     * const tl = new Timeline(30, 90)      // 30fps, 90 帧(3 秒)
     * tl.set(30, obj, "x", 100, "outQuad")   // 第 30 帧到达 100
     * tl.set(60, obj, "x", 50)                // 第 60 帧回到 50 (linear)
     * tl.on("change", (frame)=> console.log(frame, obj.x))
     * tl.play()
     * ```
     *
     * @module animator
     */
    char const * const Animator::name = "animator" ;

    Animator::Animator(JSContext * ctx, const char * name, uint8_t flagGlobal)
        : NativeModule(ctx, name, flagGlobal)
    {
        // 时间线类
        exportClass<Timeline>() ;

        // 以下导出项在 Animator-js.cpp 的 exports() 中导出
        exportName("tween") ;       // 缓动函数集合对象
        exportName("morph") ;       // 一次性动画函数
    }
}
