#include "Timeline.hpp"
#include "../../qjs_utils.h"
#include <algorithm>
#include <cstdio>
#include <cstdlib>

namespace be::animator {

    /**
     * 关键帧动画时间线
     *
     * 由 BeShell 主循环驱动的等间隔帧节拍，在指定帧设置 "对象.属性=值" 关键帧，
     * 播放时跨关键帧自动插值（支持缓动）。
     *
     * 示例：
     * ```javascript
     * import { Timeline } from "animator"
     *
     * const obj = { x: 0, y: 0 }
     * const tl = new Timeline(30, 90)         // 30fps, 90 帧(3 秒)
     *
     * // 设置关键帧: 帧号, 目标对象, 属性, 值 [, 缓动 [, 缓动参数]]
     * tl.set(30, obj, "x", 100, "outQuad")
     * tl.set(60, obj, "x", 50)
     * // 属性表形式批量设置
     * tl.set(90, obj, { x: 0, y: [200, "inOutBounce"] })
     *
     * tl.on("change", (frame)=> console.log(frame, obj.x, obj.y))
     * tl.on("stop", ()=> console.log("done"))
     * tl.play()
     * ```
     *
     * ## 区间播放与倒放
     *
     * `play(fromFrame, toFrame)` 从指定帧播到指定帧，`toFrame < fromFrame` 时倒放：
     * ```javascript
     * tl.play(90, 0)      // 倒放
     * tl.play(30)         // 从第 30 帧播到总帧数
     * ```
     *
     * ## 帧动作 jumpTo / playTo / stop
     *
     * set/add 的 options 参数可为帧设置动作，播放头到达该帧时触发：
     * ```javascript
     * // 到达第 90 帧时跳回第 0 帧（最多触发 3 次）
     * tl.set(90, obj, { x: 0 }, { jumpTo: 0, jumpCount: 3 })
     * // 到达第 60 帧时倒放到第 30 帧，到达后停止（resume: true 则恢复原方向与终点继续播放）
     * tl.set(60, obj, { x: 50 }, { playTo: 30, playCount: -1 })   // 次数 -1 = 不限
     * // 到达第 60 帧时停止播放；label 为帧标记名
     * tl.set(60, obj, { x: 50 }, { stop: true, label: "end" })
     * ```
     * 次数每次触发 -1，0 不再触发，-1 不限（不递减）；每次 play() 恢复为初始值。
     * jumpTo 为当 tick 立即传送（目标帧动作可链式触发），playTo 为改向播放（支持倒放）。
     * stop 优先于 jumpTo/playTo；playTo 途中途经 jumpTo 帧时放弃 playTo 目标、恢复原方向与终点。
     *
     * 继承自 [EventEmitter](../event-emitter.html)，提供 `on()`/`off()` 等事件监听方法。
     *
     * ## 事件
     *
     * | 事件名 | 回调参数 | 说明 |
     * |--------|----------|------|
     * | frame | frame:number 当前帧序号 | 每帧属性更新后触发（无论值是否变化，无监听时不触发） |
     * | change | frame:number 当前帧序号 | 仅在有属性值实际变更的帧触发（先于 frame 事件） |
     * | stop | frame:number 固定为 0 | 停止时触发（手动 stop() 与播完自动停止均触发） |
     * | pause | frame:number 当前帧序号 | 暂停时触发 |
     * | arrive:<帧号> | frame:number 当前帧序号 | 播放头到达/途经指定帧时触发（含 jumpTo 落点；起始帧不触发） |
     *
     * @module animator
     * @class Timeline
     */
    DEFINE_NCLASS_META(Timeline, EventEmitter)

    void Timeline::eventAdded(const char * eventName) {
        if(strcmp(eventName, "frame")==0) {
            frameListening = true ;
        }
        else if(strcmp(eventName, "change")==0) {
            changeListening = true ;
        }
        else if(strncmp(eventName, "arrive:", 7)==0) {
            long f = atol(eventName+7) ;
            if(f>=0) {
                arriveFrames.insert((uint32_t)f) ;
            }
        }
    }

    void Timeline::eventRemoved(const char * eventName) {
        if(strcmp(eventName, "frame")==0) {
            frameListening = false ;
        }
        else if(strcmp(eventName, "change")==0) {
            changeListening = false ;
        }
        else if(strncmp(eventName, "arrive:", 7)==0) {
            long f = atol(eventName+7) ;
            if(f>=0) {
                arriveFrames.erase((uint32_t)f) ;
            }
        }
    }

    // ---------- Track ----------

    Track::~Track() {
        if(!JS_IsUndefined(obj)) {
            JS_FreeValue(ctx, obj) ;    // 释放 目标/守卫对象 引用计数
        }
    }

    void Track::addKeyframe(uint32_t frame, double value, tween::TweenEaseType ease, double easeS, bool merge) {
        auto it = std::lower_bound(keyframes.begin(), keyframes.end(), frame,
            [](const Keyframe & kf, uint32_t f){ return kf.frame < f ; }) ;
        if(it!=keyframes.end() && it->frame==frame) {
            if(merge) {
                // add: 同帧值累加，ease 未提供时保留原设置
                it->value += value ;
                if(ease!=tween::TWEEN_EASE_UNKNOWN) {
                    it->ease = ease ;
                    it->easeS = easeS ;
                }
            } else {
                // set: 同帧替换，ease 未提供时重置为 linear
                it->value = value ;
                it->ease = (ease==tween::TWEEN_EASE_UNKNOWN) ? tween::TWEEN_LINEAR : ease ;
                it->easeS = easeS ;
            }
        } else {
            Keyframe kf{frame, value
                , (ease==tween::TWEEN_EASE_UNKNOWN) ? tween::TWEEN_LINEAR : ease
                , easeS} ;
            keyframes.insert(it, kf) ;
        }
        cursor = 0 ;    // 段缓存失效，eval 时重新二分定位
    }

    bool Track::removeKeyframeAt(uint32_t frame) {
        auto it = std::lower_bound(keyframes.begin(), keyframes.end(), frame,
            [](const Keyframe & kf, uint32_t f){ return kf.frame < f ; }) ;
        if(it!=keyframes.end() && it->frame==frame) {
            keyframes.erase(it) ;
            cursor = 0 ;
            return true ;
        }
        return false ;
    }

    void Track::ensureOrigin() {
        if(keyframes.empty() || keyframes.front().frame>0) {
            // 用创建时捕获的 originValue（而非读当前值——playAt 预览可能已改写当前值）
            addKeyframe(0, originValue, tween::TWEEN_LINEAR, 1.70158, false) ;
        }
    }

    double Track::readCurrentValue() {
        double v = 0 ;
        if(type==JS_PROP) {
            if(JS_IsObject(obj)) {
                JSValue jv = JS_GetPropertyStr(ctx, obj, prop.c_str()) ;
                JS_ToFloat64(ctx, &v, jv) ;
                JS_FreeValue(ctx, jv) ;
            }
        }
        else if(type==C_INT32)  { v = *(int32_t*)cptr ; }
        else if(type==C_INT64)  { v = (double)*(int64_t*)cptr ; }
        else if(type==C_FLOAT)  { v = *(float*)cptr ; }
        else if(type==C_DOUBLE) { v = *(double*)cptr ; }
        return v ;
    }

    double Track::eval(uint32_t frame) {
        size_t n = keyframes.size() ;
        if(n==0) {
            return 0 ;
        }
        // 首帧之前保持首帧值，末帧之后保持末帧值
        if(frame<=keyframes.front().frame) {
            return keyframes.front().value ;
        }
        if(frame>=keyframes.back().frame) {
            return keyframes.back().value ;
        }
        // cursor 失效（回绕/编辑）时二分重定位：最后满足 kf.frame<=frame 的下标
        if(cursor>=n-1 || keyframes[cursor].frame>frame || keyframes[cursor+1].frame<=frame) {
            auto it = std::upper_bound(keyframes.begin(), keyframes.end(), frame,
                [](uint32_t f, const Keyframe & kf){ return f < kf.frame ; }) ;
            cursor = (size_t)(it - keyframes.begin()) - 1 ;
        }
        const Keyframe & a = keyframes[cursor] ;
        const Keyframe & b = keyframes[cursor+1] ;
        // 相邻同值，不必插补
        if(b.value==a.value) {
            return b.value ;
        }
        double t = (double)(frame-a.frame) / (double)(b.frame-a.frame) ;
        // 缓动设在目标(后一个)关键帧上，表示如何到达
        double k = tween::ease(b.ease, (float)t, 0, 1, 1, (float)b.easeS) ;
        return a.value + (b.value-a.value)*k ;
    }

    void Track::apply(JSContext * ctx, double value) {
        if(type==JS_PROP) {
            if(JS_IsObject(obj)) {
                if(JS_SetPropertyStr(ctx, obj, prop.c_str(), JS_NewFloat64(ctx, value))<0) {
                    js_std_dump_error(ctx) ;
                }
            }
        }
        // 整型四舍五入
        else if(type==C_INT32)  { *(int32_t*)cptr = (int32_t)(value>=0 ? value+0.5 : value-0.5) ; }
        else if(type==C_INT64)  { *(int64_t*)cptr = (int64_t)(value>=0 ? value+0.5 : value-0.5) ; }
        else if(type==C_FLOAT)  { *(float*)cptr = (float)value ; }
        else if(type==C_DOUBLE) { *(double*)cptr = value ; }
    }

    bool Track::applyFrame(JSContext * ctx, uint32_t frame) {
        double v = eval(frame) ;
        // 按目标类型的实际写入精度折算，使变更判定与实际写入值严格一致
        if(type==C_INT32 || type==C_INT64) {
            v = (double)(int64_t)(v>=0 ? v+0.5 : v-0.5) ;
        }
        else if(type==C_FLOAT) {
            v = (double)(float)v ;
        }
        if(!hasValue) {
            // 首帧：目标当前值已等于轨道值时不写入、不算变更
            hasValue = true ;
            lastValue = v ;
            if(readCurrentValue()==v) {
                return false ;
            }
            apply(ctx, v) ;
            return true ;
        }
        if(v==lastValue) {
            return false ;
        }
        apply(ctx, v) ;
        lastValue = v ;
        return true ;
    }

    bool Track::match(JSValue _obj, const char * _prop) const {
        return type==JS_PROP
            && JS_VALUE_GET_PTR(obj)==JS_VALUE_GET_PTR(_obj)
            && prop==_prop ;
    }

    bool Track::match(const void * _cptr) const {
        return type!=JS_PROP && cptr==_cptr ;
    }

    // ---------- Timeline ----------

    Timeline::Timeline(JSContext * ctx, JSValue _jsobj, double _fps, uint64_t frames, bool _repeat, int64_t _repeatCount)
        : EventEmitter(ctx, build(ctx, _jsobj))
        , fps(_fps)
        , interval_us((uint64_t)(1000000.0/_fps))
        , totalFrames(frames)
        , repeat(_repeat)
        , repeatCount(_repeatCount)
        , repeatCountInit(_repeatCount)
    {
        if(interval_us<1) {
            interval_us = 1 ;
        }
    }

    Timeline::~Timeline() {
        if(loopRef) {
            leaveLoop() ;
        }
        for(auto & r : pendingResolves) {
            JS_FreeValue(ctx, r) ;
        }
        for(auto & r : pendingRejects) {
            JS_FreeValue(ctx, r) ;
        }
        for(auto track : tracks) {
            delete track ;    // Track 析构时释放 JS 对象引用
        }
    }

    // ---------- 关键帧 ----------

    bool Timeline::applyTracks(uint32_t frame) {
        bool changed = false ;
        for(auto track : tracks) {
            if(track->applyFrame(ctx, frame)) {
                changed = true ;
            }
        }
        return changed ;
    }

    Track * Timeline::findTrack(JSValue obj, const char * prop) {
        for(auto track : tracks) {
            if(track->match(obj, prop)) {
                return track ;
            }
        }
        return nullptr ;
    }

    Track * Timeline::findOrCreatePtrTrack(void * cptr, Track::TargetType type, JSValue guard) {
        for(auto track : tracks) {
            if(track->match(cptr)) {
                return track ;
            }
        }
        Track * track = new Track(ctx) ;
        track->type = type ;
        track->cptr = cptr ;
        if(JS_IsObject(guard)) {
            track->obj = JS_DupValue(ctx, guard) ;    // 守卫对象 引用计数+1
        }
        track->originValue = track->readCurrentValue() ;    // 创建时捕获原值（防 playAt 预览污染）
        tracks.push_back(track) ;
        return track ;
    }

    void Timeline::set(uint32_t frame, JSValue obj, const char * prop, double value, tween::TweenEaseType ease, double easeS) {
        if(frame>totalFrames) {
            totalFrames = frame ;    // 关键帧超出总帧数时扩展总帧数
        }
        Track * track = findTrack(obj, prop) ;
        if(!track) {
            track = new Track(ctx) ;
            track->type = Track::JS_PROP ;
            track->obj = JS_DupValue(ctx, obj) ;    // 目标对象 引用计数+1
            track->prop = prop ;
            track->originValue = track->readCurrentValue() ;  // 创建时捕获原值
            tracks.push_back(track) ;
        }
        if(frame>0) {
            track->ensureOrigin() ;    // 自动捕获 0 帧原始值
        }
        track->addKeyframe(frame, value, ease, easeS, false) ;
    }

    void Timeline::add(uint32_t frame, JSValue obj, const char * prop, double value, tween::TweenEaseType ease, double easeS) {
        if(frame>totalFrames) {
            totalFrames = frame ;
        }
        Track * track = findTrack(obj, prop) ;
        if(!track) {
            track = new Track(ctx) ;
            track->type = Track::JS_PROP ;
            track->obj = JS_DupValue(ctx, obj) ;
            track->prop = prop ;
            track->originValue = track->readCurrentValue() ;  // 创建时捕获原值
            tracks.push_back(track) ;
        }
        if(frame>0) {
            track->ensureOrigin() ;
        }
        track->addKeyframe(frame, value, ease, easeS, true) ;
    }

#define TIMELINE_DEF_PTR_SET_ADD(ctype, track_type)                                                          \
    void Timeline::set(uint32_t frame, ctype * cptr, double value, tween::TweenEaseType ease, double easeS, JSValue guard) { \
        if(frame>totalFrames) { totalFrames = frame ; }                                                      \
        Track * track = findOrCreatePtrTrack(cptr, track_type, guard) ;                                       \
        if(frame>0) { track->ensureOrigin() ; }                                                              \
        track->addKeyframe(frame, value, ease, easeS, false) ;                                               \
    }                                                                                                        \
    void Timeline::add(uint32_t frame, ctype * cptr, double value, tween::TweenEaseType ease, double easeS, JSValue guard) { \
        if(frame>totalFrames) { totalFrames = frame ; }                                                      \
        Track * track = findOrCreatePtrTrack(cptr, track_type, guard) ;                                       \
        if(frame>0) { track->ensureOrigin() ; }                                                              \
        track->addKeyframe(frame, value, ease, easeS, true) ;                                                \
    }

    TIMELINE_DEF_PTR_SET_ADD(int32_t, Track::C_INT32)
    TIMELINE_DEF_PTR_SET_ADD(int64_t, Track::C_INT64)
    TIMELINE_DEF_PTR_SET_ADD(float,   Track::C_FLOAT)
    TIMELINE_DEF_PTR_SET_ADD(double,  Track::C_DOUBLE)

    void Timeline::unset(uint32_t frame) {
        frameActions.erase(frame) ;     // 连同删除该帧的 jumpTo/playTo 动作
        // 倒序遍历，删除空轨道
        for(size_t i=tracks.size(); i>0; i--) {
            Track * track = tracks[i-1] ;
            track->removeKeyframeAt(frame) ;
            if(track->keyframes.empty()) {
                delete track ;
                tracks.erase(tracks.begin()+i-1) ;
            }
        }
    }

    void Timeline::unset(uint32_t frame, JSValue obj, const char * prop) {
        for(size_t i=tracks.size(); i>0; i--) {
            Track * track = tracks[i-1] ;
            if(track->match(obj, prop)) {
                track->removeKeyframeAt(frame) ;
                if(track->keyframes.empty()) {
                    delete track ;
                    tracks.erase(tracks.begin()+i-1) ;
                }
                return ;
            }
        }
    }

    void Timeline::unset(uint32_t frame, void * cptr) {
        for(size_t i=tracks.size(); i>0; i--) {
            Track * track = tracks[i-1] ;
            if(track->match(cptr)) {
                track->removeKeyframeAt(frame) ;
                if(track->keyframes.empty()) {
                    delete track ;
                    tracks.erase(tracks.begin()+i-1) ;
                }
                return ;
            }
        }
    }

    void Timeline::unset(void * cptr) {
        for(size_t i=tracks.size(); i>0; i--) {
            Track * track = tracks[i-1] ;
            if(track->match(cptr)) {
                delete track ;
                tracks.erase(tracks.begin()+i-1) ;
                return ;
            }
        }
    }

    // ---------- C++ 事件订阅 ----------

    void Timeline::on(EventType type, EventCallback func, void * opaque) {
        if(!func) {
            return ;
        }
        subscriptions.push_back({type, func, opaque}) ;
        updateEventFlags() ;
    }

    void Timeline::off(EventType type, EventCallback func, void * opaque) {
        for(size_t i=subscriptions.size(); i>0; i--) {
            EventSub & sub = subscriptions[i-1] ;
            if(sub.type==type && sub.func==func && sub.opaque==opaque) {
                if(emitting) {
                    sub.func = nullptr ;    // 派发中仅标记失效，派发结束后清理
                } else {
                    subscriptions.erase(subscriptions.begin()+i-1) ;
                }
                updateEventFlags() ;
                return ;
            }
        }
    }

    void Timeline::updateEventFlags() {
        hasCppFrame = false ;
        hasCppChange = false ;
        for(auto & sub : subscriptions) {
            if(!sub.func) {
                continue ;
            }
            if(sub.type==EVENT_FRAME) {
                hasCppFrame = true ;
            }
            else if(sub.type==EVENT_CHANGE) {
                hasCppChange = true ;
            }
        }
    }

    // 统一事件派发：先 C++ 订阅回调，再 JS emitSync
    // 回调中可安全 on/off（索引遍历 + 标记失效 + 快照 size，新订阅下轮派发才生效）
    void Timeline::emitEvent(EventType evt, uint32_t frame) {
        emitting++ ;
        size_t n = subscriptions.size() ;
        for(size_t i=0; i<n; i++) {
            EventSub & sub = subscriptions[i] ;
            if(sub.func && sub.type==evt) {
                sub.func(this, evt, frame, sub.opaque) ;
            }
        }
        emitting-- ;
        if(!emitting) {
            // 清理派发期间退订的条目（倒序）
            for(size_t i=subscriptions.size(); i>0; i--) {
                if(!subscriptions[i-1].func) {
                    subscriptions.erase(subscriptions.begin()+i-1) ;
                }
            }
        }
        switch(evt) {
            case EVENT_FRAME:
                emitSyncFree("frame", {JS_NewUint32(ctx, frame)}) ;
                break ;
            case EVENT_CHANGE:
                emitSyncFree("change", {JS_NewUint32(ctx, frame)}) ;
                break ;
            case EVENT_STOP:
                emitSync("stop") ;
                break ;
            case EVENT_PAUSE:
                emitSync("pause") ;
                break ;
            case EVENT_ARRIVE: {
                char name[24] ;
                snprintf(name, sizeof(name), "arrive:%u", (unsigned)frame) ;
                emitSyncFree(name, {JS_NewUint32(ctx, frame)}) ;
                break ;
            }
        }
    }

    void Timeline::watchArrive(uint32_t frame) {
        arriveFrames.insert(frame) ;
    }

    void Timeline::unwatchArrive(uint32_t frame) {
        arriveFrames.erase(frame) ;
    }

    // 设置帧动作：mask 选择更新哪些字段，未提供的字段保持原设置；
    // 次数同时写入当前值与初始值（play() 时以初始值恢复）；全部动作均无效时删除该帧条目
    void Timeline::setFrameAction(uint32_t frame, uint8_t mask, int64_t jumpTo, int jumpCount, int64_t playTo, int playCount, bool stop, bool resume, const char * label) {
        FrameAction act ;
        auto it = frameActions.find(frame) ;
        if(it!=frameActions.end()) {
            act = it->second ;
        }
        if(mask & ACTION_JUMP) {
            act.jumpTo = jumpTo ;
            act.jumpCountInit = act.jumpCount = jumpCount ;
        }
        if(mask & ACTION_PLAY) {
            act.playTo = playTo ;
            act.playCountInit = act.playCount = playCount ;
        }
        if(mask & ACTION_STOP) {
            act.stop = stop ;
        }
        if(mask & ACTION_RESUME) {
            act.resume = resume ;
        }
        if(mask & ACTION_LABEL) {
            act.label = label ? label : "" ;
        }
        if(act.jumpTo<0 && act.playTo<0 && !act.stop && !act.resume && act.label.empty()) {
            frameActions.erase(frame) ;
        } else {
            frameActions[frame] = act ;
        }
    }

    // ---------- 播放控制（JS 同名方法转调） ----------

    void Timeline::play(uint32_t from, int64_t to) {
        if(state!=STOPPED) {
            return ;
        }
        // 恢复全部帧动作的触发次数
        for(auto & kv : frameActions) {
            kv.second.jumpCount = kv.second.jumpCountInit ;
            kv.second.playCount = kv.second.playCountInit ;
        }
        repeatCount = repeatCountInit ;      // 恢复有限重播次数
        playToActive = false ;
        playToResume = false ;
        fromFrame = from ;
        if(to<0) {
            dir = 1 ;
            endFrame = totalFrames>0 ? (uint32_t)totalFrames : UINT32_MAX ;
        } else {
            dir = ((uint64_t)to>=(uint64_t)from) ? 1 : -1 ;
            endFrame = (uint32_t)to ;
        }
        frame = from ;
        state = PLAYING ;
        enterLoop() ;
        // 应用起始帧值（原始值/首关键帧），有实际变更时触发 change（起始帧不触发 arrive/帧动作）
        if(applyTracks(frame) && (changeListening || hasCppChange)) {
            emitEvent(EVENT_CHANGE, frame) ;
        }
    }

    void Timeline::stop() {
        if(state==STOPPED) {
            return ;
        }
        frame = 0 ;
        state = STOPPED ;
        rejectPending("timeline stopped") ;
        // 先发事件（此时 loopRef/loopObjRef 仍持有 this），回调中可能重新 play()
        emitEvent(EVENT_STOP, 0) ;
        // 只有仍处于停止态才退出循环；leaveLoop 后 this 可能已析构，禁止再访问成员
        if(state==STOPPED) {
            leaveLoop() ;
        }
    }

    void Timeline::pause() {
        if(state!=PLAYING) {
            return ;
        }
        state = PAUSED ;
        // 先发事件，回调中可能 resume()
        emitEvent(EVENT_PAUSE, frame) ;
        if(state==PAUSED) {
            leaveLoop() ;
        }
    }

    void Timeline::resume() {
        if(state!=PAUSED) {
            return ;
        }
        state = PLAYING ;
        enterLoop() ;
    }

    // 游标预览：写入各轨道在该帧的插补值，有实际变更时触发 change（不推进播放状态）
    void Timeline::playAt(uint32_t f) {
        cursorFrame = f ;
        if(applyTracks(f) && (changeListening || hasCppChange)) {
            emitEvent(EVENT_CHANGE, f) ;
        }
    }

    // 加入 BeShell 主循环，并增加自身引用计数（C++ 侧 loopRef + JS 侧 loopObjRef 配对）
    void Timeline::enterLoop() {
        next_frame_us = gettime_us() + interval_us ;
        JSEngine::fromJSContext(ctx)->addLoopObject(this, true, 0) ;
        loopRef = shared() ;
        JS_FreeValue(ctx, loopObjRef) ;          // 配对不变式下为 JS_UNDEFINED，防御性释放
        loopObjRef = JS_DupValue(ctx, jsobj) ;   // 播放期间强持有 jsobj，保证事件可投递
    }

    // 从主循环移除，并释放自身引用计数
    // 注意：释放 loopObjRef 可能使 jsobj 计数归 0 触发 finalizer 进而析构本对象，
    // 因此先清 loopRef/loopObjRef 成员（保证析构路径幂等、不递归），且本函数返回后
    // 调用方不得再访问任何成员
    void Timeline::leaveLoop() {
        JSEngine::fromJSContext(ctx)->removeLooping(this) ;
        loopRef = nullptr ;
        JSValue ref = loopObjRef ;
        loopObjRef = JS_UNDEFINED ;
        JS_FreeValue(ctx, ref) ;                 // 此后 this 可能已析构
    }

    // 帧 tick：严格等间隔节拍，每轮 loop 最多推进一帧（不补帧）
    // 推进顺序：按 dir 推进一帧 → 插值应用与事件 → 帧动作（jumpTo 当 tick 链式传送，
    // playTo 改向）→ nextFrame Promise → 终点判定（repeat 回绕 / stop）
    void Timeline::loop(JSContext * ctx) {
        if(state!=PLAYING) {
            return ;
        }
        uint64_t now = gettime_us() ;
        if(now < next_frame_us) {
            return ;
        }

        frame += dir ;
        next_frame_us += interval_us ;
        if(next_frame_us <= now) {
            // 严重滞后（loop 被长时间阻塞），重置节拍，不突发连发
            next_frame_us = now + interval_us ;
        }

        // 应用当前帧并检查帧动作；jumpTo 传送后在同一 tick 内对目标帧链式重复该过程
        int chainDepth = 0 ;
        while(true) {
            // 关键帧插补：计算并应用本帧全部轨道值（值未变化的轨道跳过写入）
            bool changed = applyTracks(frame) ;

            // change 事件（仅在有轨道值实际变更的帧触发；先于 frame 事件；C++ 订阅先于 JS 监听）
            if(changed && (changeListening || hasCppChange)) {
                emitEvent(EVENT_CHANGE, frame) ;
            }

            // frame 事件（在属性值更新之后触发；无监听时跳过，避免每帧空转）
            if(frameListening || hasCppFrame) {
                emitEvent(EVENT_FRAME, frame) ;
            }

            // arrive 事件（播放头到达/途经监听帧时触发，含 jumpTo 落点）
            if(!arriveFrames.empty() && arriveFrames.count(frame)) {
                emitEvent(EVENT_ARRIVE, frame) ;
            }

            // 事件回调中可能 stop()，此后不得再访问成员
            if(state!=PLAYING) {
                return ;
            }

            // 帧动作（find 需在事件之后：回调中可能 set/unset 改动帧动作表）
            auto it = frameActions.find(frame) ;
            if(it==frameActions.end()) {
                break ;
            }
            FrameAction & act = it->second ;
            if(act.stop) {
                stop() ;                // 到达本帧即停止；stop 后 this 可能已析构，禁止再访问成员
                return ;
            }
            if(act.jumpTo>=0 && act.jumpCount!=0) {
                if(act.jumpCount>0) {
                    act.jumpCount-- ;       // -1 不限次数，不递减避免向下溢出
                }
                if(playToActive) {          // 中途遇到跳转：放弃 playTo 目标，恢复原方向/终点
                    playToActive = false ;
                    playToResume = false ;
                    dir = savedDir ;
                    endFrame = savedEnd ;
                }
                frame = (uint32_t)act.jumpTo ;
                if(++chainDepth>64) {
                    printf("Timeline: jumpTo chain too deep, stopped\n") ;
                    stop() ;                // stop 后 this 可能已析构，禁止再访问成员
                    return ;
                }
                continue ;                  // 链式触发目标帧的 arrive/帧动作
            }
            if(act.playTo>=0 && act.playCount!=0 && act.playTo!=(int64_t)frame) {
                if(act.playCount>0) {
                    act.playCount-- ;
                }
                if(!playToActive) {
                    savedDir = dir ;        // 记录进入前方向与终点，到达后恢复
                    savedEnd = endFrame ;
                }
                dir = act.playTo>(int64_t)frame ? 1 : -1 ;
                endFrame = (uint32_t)act.playTo ;
                playToActive = true ;
                playToResume = act.resume ;   // 到达后是否继续（否则停止）
            }
            break ;
        }

        // 解决等待本帧的 Promise（resolve 当前帧序号，同样在属性值更新之后）
        std::vector<JSValue> resolves = std::move(pendingResolves) ;
        std::vector<JSValue> rejects = std::move(pendingRejects) ;
        for(auto & reject : rejects) {
            JS_FreeValue(ctx, reject) ;
        }
        JSValueConst argv[1] = { JS_NewUint32(ctx, frame) } ;
        for(auto & resolve : resolves) {
            JSValue ret = JS_Call(ctx, resolve, JS_UNDEFINED, 1, argv) ;
            if(JS_IsException(ret)) {
                js_std_dump_error(ctx) ;
            }
            JS_FreeValue(ctx, ret) ;
            JS_FreeValue(ctx, resolve) ;
        }
        JS_FreeValue(ctx, (JSValue)argv[0]) ;

        // playTo 到达目标帧：勾选「然后继续」则恢复原方向/终点继续，否则停止
        if(playToActive && frame==endFrame) {
            if(playToResume) {
                playToActive = false ;
                playToResume = false ;
                dir = savedDir ;
                endFrame = savedEnd ;
            } else {
                stop() ;            // stop 后 this 可能已析构，禁止再访问成员
                return ;
            }
        }

        // 达到终点（"frame"/"change" 事件回调中可能已改变 state，需重新确认仍在播放）
        if(state==PLAYING && !playToActive && endFrame!=UINT32_MAX
            && (dir>0 ? frame>=endFrame : frame<=endFrame)) {
            if(repeat && repeatCount!=0) {
                if(repeatCount>0) {
                    repeatCount-- ;         // 额外重播次数 -1（-1=无限，不递减）
                }
                frame = fromFrame ;
            } else {
                // 与手动 stop() 同路径；stop() 后 this 可能已析构，禁止再访问成员
                stop() ;
            }
        }
    }

    void Timeline::rejectPending(const char * msg) {
        std::vector<JSValue> resolves = std::move(pendingResolves) ;
        std::vector<JSValue> rejects = std::move(pendingRejects) ;
        for(auto & resolve : resolves) {
            JS_FreeValue(ctx, resolve) ;
        }
        for(auto & reject : rejects) {
            JSValue err = JS_NewError(ctx) ;
            JS_SetPropertyStr(ctx, err, "message", JS_NewString(ctx, msg)) ;
            JSValueConst argv[1] = { err } ;
            JSValue ret = JS_Call(ctx, reject, JS_UNDEFINED, 1, argv) ;
            if(JS_IsException(ret)) {
                js_std_dump_error(ctx) ;
            }
            JS_FreeValue(ctx, ret) ;
            JS_FreeValue(ctx, err) ;
            JS_FreeValue(ctx, reject) ;
        }
    }
}
