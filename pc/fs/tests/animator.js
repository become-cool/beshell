// animator 模块 PC 单元测试
// 运行: build/beshell-pc.exe /tests/animator.js （输出末行打印 [TEST] SUMMARY）
import { Timeline, tween, morph } from "animator"

let pass = 0, fail = 0
function assert(cond, name) {
    if(cond) { pass++; console.log("[TEST] ok -", name) }
    else { fail++; console.log("[TEST] FAIL -", name) }
}
function assertEq(a, b, name) {
    assert(a===b, name + " (expect " + b + ", got " + a + ")")
}
function assertApprox(a, b, name, eps=1e-6) {
    assert(Math.abs(a-b)<=eps, name + " (expect " + b + ", got " + a + ")")
}
function assertThrow(fn, name) {
    try { fn(); assert(false, name + " (no exception)") }
    catch(e) { pass++; console.log("[TEST] ok -", name) }
}
const sleep = (ms)=> new Promise(r=> setTimeout(r, ms))

// ---------- tween ----------
console.log("[TEST] --- tween ---")
assertEq(tween.linear(0.5), 0.5, "tween.linear(0.5)")
assertEq(tween.inQuad(0.5), 0.25, "tween.inQuad(0.5)")
assertEq(tween.ease("inQuad", 0.5), 0.25, "tween.ease(name)")
assertEq(tween.ease(4, 0.5), 0.25, "tween.ease(id=4 is inQuad)")
assertApprox(tween.inOutSine(0), 0, "inOutSine(0)=0")
assertApprox(tween.inOutSine(1), 1, "inOutSine(1)=1")
assertApprox(tween.outQuad(250, 0, 100, 1000), 43.75, "outQuad 全参数形式")
assertThrow(()=> tween.ease("noSuchEase", 0.5), "unknown ease name throws")

// ---------- Timeline 静态求值（不播放） ----------
console.log("[TEST] --- timeline evalAt/cursor ---")
{
    const obj = { x: 0, y: 0 }
    const tl = new Timeline(30, 90)
    tl.set(0, obj, "x", 0)
    tl.set(90, obj, "x", 100)
    tl.set(90, obj, "y", 100, "inQuad")

    // evalAt(frame)：各轨道在指定帧的插补值
    let v = tl.evalAt(0)
    assertEq(v.length, 2, "evalAt 返回全部轨道")
    assertEq(v[0].prop, "x", "evalAt 条目含属性名")
    assertEq(v[0].obj, obj, "evalAt 条目含目标对象")
    assertApprox(v[0].value, 0, "evalAt(0) x=0")
    assertApprox(tl.evalAt(45)[0].value, 50, "evalAt(45) x=50 (linear)")
    assertApprox(tl.evalAt(90)[0].value, 100, "evalAt(90) x=100")
    assertApprox(tl.evalAt(120)[0].value, 100, "evalAt 超出末帧保持末帧值")
    assertApprox(tl.evalAt(45)[1].value, 25, "evalAt(45) y=25 (inQuad)")

    // 游标：evalAt() 缺省用 cursor
    assertEq(tl.cursor, 0, "cursor 默认 0")
    tl.cursor = 45
    assertEq(tl.cursor, 45, "cursor 读写")
    assertApprox(tl.evalAt()[0].value, 50, "evalAt() 缺省求值游标帧")

    // add 合并语义：同帧值累加
    tl.add(90, obj, "x", 10)
    assertApprox(tl.evalAt(90)[0].value, 110, "add 同帧累加")

    // unset(frame, obj, prop)：移除该帧该属性
    tl.unset(90, obj, "x")
    assertApprox(tl.evalAt(90)[0].value, 0, "unset 后只剩 0 帧关键帧，全程保持 0")

    // 属性表形式 + 表级 ease
    const o2 = { a: 0, b: 0 }
    const tl2 = new Timeline(30, 100)
    tl2.set(100, o2, { a: 100, b: [100, "inQuad"], ease: "linear" })
    const v2 = tl2.evalAt(50)
    assertApprox(v2[0].value, 50, "属性表表级 ease (a linear 50%)")
    assertApprox(v2[1].value, 25, "数组形式单独覆盖 ease (b inQuad 50%)")

    // ensureOrigin：首个关键帧 >0 时自动捕获 0 帧当前值
    const o3 = { z: 5 }
    const tl3 = new Timeline(30, 30)
    tl3.set(30, o3, "z", 50)
    assertApprox(tl3.evalAt(0)[0].value, 5, "ensureOrigin 捕获 0 帧当前值")
    assertApprox(tl3.evalAt(15)[0].value, 27.5, "ensureOrigin 后可插补")

    // 未知缓动报错
    assertThrow(()=> tl.set(10, obj, "x", 1, "badEase"), "未知缓动名抛异常")

    // playAt 游标预览：写入插补值但不破坏关键帧与原点
    const o4 = { x: 10, y: 0 }
    const tl4 = new Timeline(30, 90)
    tl4.set(90, o4, "x", 100)               // 轨道创建时捕获原点 10
    tl4.playAt(45)
    assertApprox(o4.x, 55, "playAt(45) 写入插补值 (10→100 中点)")
    assertEq(tl4.cursor, 45, "playAt 同步游标")
    assertApprox(tl4.evalAt(0)[0].value, 10, "playAt 后 0 帧原点仍为 10")
    assertApprox(tl4.evalAt(90)[0].value, 100, "playAt 后 90 帧关键帧不变")
    // 预览后再为未跟踪属性建轨道：捕获的是未被预览污染的当前值
    tl4.playAt(90)                          // o4.x=100（预览值）
    tl4.set(30, o4, "y", 60)                // y 无轨道，playAt 从未写过它
    assertApprox(tl4.evalAt(0)[1].value, 0, "新轨道原点未被 playAt 污染")
    // playAt 的 change 事件：有变更才触发
    let pch = 0
    tl4.on("change", ()=> pch++)
    tl4.playAt(60)
    assertEq(pch, 1, "playAt 有变更时触发一次 change")
    tl4.playAt(60)
    assertEq(pch, 1, "playAt 无变更时不触发 change")
}

// ---------- 播放控制与事件 ----------
async function testPlayback() {
    console.log("[TEST] --- timeline playback ---")

    // 基本播放 + nextFrame + change/stop 事件
    {
        const o = { v: 0 }
        const tl = new Timeline(1000, 20)
        tl.set(20, o, "v", 20)
        let changes = 0, frames = 0, stopped = false
        tl.on("change", ()=> changes++)
        tl.on("frame", ()=> frames++)
        tl.on("stop", ()=> stopped = true)
        tl.play()
        while(!stopped) {
            try { await tl.nextFrame() }
            catch(e) { break }      // stop 时挂起的 nextFrame 以 reject 结束
        }
        assert(stopped, "播完自动触发 stop 事件")
        assertApprox(o.v, 20, "播完后目标值到达末帧值")
        assertEq(frames, 20, "frame 事件每帧触发一次")
        assert(changes>0 && changes<=20, "change 事件仅在有实际变更的帧触发")
    }

    // 暂停/继续
    {
        const o = { v: 0 }
        const tl = new Timeline(1000, 100)
        tl.set(100, o, "v", 100)
        let paused = false
        tl.on("pause", ()=> paused = true)
        tl.play()
        await tl.nextFrame()
        await tl.nextFrame()
        tl.pause()
        assert(paused, "pause 事件触发")
        const vAtPause = o.v
        await sleep(50)
        assertEq(o.v, vAtPause, "暂停期间目标值不变")
        tl.resume()
        let stopped = false
        tl.on("stop", ()=> stopped = true)
        while(!stopped) {
            try { await tl.nextFrame() } catch(e) { break }
        }
        assertApprox(o.v, 100, "resume 后播完")
    }

    // 区间倒放
    {
        const o = { v: 0 }
        const tl = new Timeline(1000, 10)
        tl.set(10, o, "v", 10)
        let stopped = false
        tl.on("stop", ()=> stopped = true)
        tl.play(10, 0)          // 从 10 倒放到 0
        while(!stopped) {
            try { await tl.nextFrame() } catch(e) { break }
        }
        assertApprox(o.v, 0, "倒放结束回到 0 帧值")
    }

    // arrive 事件与 jumpTo 帧动作
    {
        const o = { v: 0 }
        const tl = new Timeline(1000, 10)
        tl.set(10, o, { v: 10 }, { jumpTo: 5, jumpCount: 1 })   // 到 10 帧时跳回 5，仅 1 次
        let arrive5 = 0, stopped = false
        tl.on("arrive:5", ()=> arrive5++)
        tl.on("stop", ()=> stopped = true)
        tl.play()
        while(!stopped) {
            try { await tl.nextFrame() } catch(e) { break }
        }
        assert(stopped, "jumpTo 次数用完后播完停止")
        assert(arrive5>=2, "jumpTo 后再次途经第 5 帧触发 arrive:5 (got "+arrive5+")")
    }

    // morph 一次性动画（fire-and-forget，顶层不持有也应播完）
    {
        let last = 0, done = false
        morph(0, 100, 30, (v)=> last = v, "linear", 1.70158, 1000)
            .on("stop", ()=> done = true)
        while(!done) { await sleep(5) }
        assertApprox(last, 100, "morph 播完到达目标值")
    }
}

testPlayback()
    .then(()=> {
        console.log("[TEST] SUMMARY pass=" + pass + " fail=" + fail)
    })
    .catch((e)=> {
        fail++
        console.log("[TEST] SUMMARY pass=" + pass + " fail=" + fail + " (async error: " + e + ")")
    })
