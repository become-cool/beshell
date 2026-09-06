#pragma once

#include "../../EventEmitter.hpp"
#include "../../JSEngine.hpp"
#include "Tween.hpp"
#include <string>

namespace be::animator {

    /**
     * 关键帧：某帧上的属性值
     * ease/easeS 表示"如何到达该目标值"，作用于 上一关键帧 -> 本关键帧 的插补段
     * （0 帧关键帧上的 ease 无意义）
     */
    typedef struct {
        uint32_t frame ;
        double value ;
        tween::TweenEaseType ease ;
        double easeS ;                      // 缓动参数（如 Back 的回弹系数，缺省 1.70158）
    } Keyframe ;

    /**
     * 轨道：一个动画目标（"对象.属性" 或 C 变量指针）上的关键帧序列
     *
     * 数据结构：关键帧按帧升序存于有序 vector，
     * 插入/删除为低频操作（二分定位），播放求值为每帧高频操作，
     * 段定位用 cursor 缓存，顺序播放摊还 O(1)，回绕/编辑后自动回退二分查找。
     */
    class Track {
    public:
        typedef enum {
            JS_PROP = 0,    // JS 对象的属性（可能由 getter/setter 实现）
            C_INT32,        // C int32_t 变量指针
            C_INT64,        // C int64_t 变量指针
            C_FLOAT,        // C float 变量指针
            C_DOUBLE,       // C double 变量指针
        } TargetType ;

        TargetType type = JS_PROP ;
        JSContext * ctx = nullptr ;
        JSValue obj = JS_UNDEFINED ;    // JS_PROP: 目标对象；C_*: 守卫对象（均 引用计数+1）
        std::string prop ;              // JS_PROP 的属性名
        void * cptr = nullptr ;         // C_* 的变量指针

        std::vector<Keyframe> keyframes ;   // 按 frame 升序
        size_t cursor = 0 ;                 // 求值段缓存
        double lastValue = 0 ;              // 上次实际写入目标的值
        bool hasValue = false ;             // 是否已写入过（首帧必写入）

        Track(JSContext * ctx) : ctx(ctx) {}
        ~Track() ;

        /**
         * 插入关键帧，二分定位
         * merge=false(set): 同帧替换（ease 未提供=TWEEN_EASE_UNKNOWN 时重置为 linear）
         * merge=true (add): 同帧值累加（ease 未提供时保留原设置）
         */
        void addKeyframe(uint32_t frame, double value, tween::TweenEaseType ease, double easeS, bool merge) ;
        // 移除指定帧的关键帧，返回是否移除成功
        bool removeKeyframeAt(uint32_t frame) ;
        // 首次向 frame>0 插入关键帧前调用：若尚无 0 帧关键帧，
        // 以目标当前值自动捕获一个 0 帧关键帧（防止首次突变，使孤立关键帧可插补）
        void ensureOrigin() ;
        // 读取目标当前值
        double readCurrentValue() ;
        // 计算指定帧的插补值：首帧之前保持首帧值，末帧之后保持末帧值，相邻同值跳过插补
        double eval(uint32_t frame) ;
        // 将值写到目标（JS 属性赋值 / C 变量写值，整型四舍五入）
        void apply(JSContext * ctx, double value) ;
        // 求值并与目标当前值/上次写入值比较，不同才写入（避免触发 JS setter 副作用/无效重绘），
        // 返回是否发生了实际变更（供 "change" 事件判定）。
        // 整型/浮点轨道按四舍五入/精度截断后的实际写入值判定；首帧与目标当前值比较，
        // 目标已是该值时不写入、不算变更
        bool applyFrame(JSContext * ctx, uint32_t frame) ;

        bool match(JSValue _obj, const char * _prop) const ;
        bool match(const void * _cptr) const ;
    } ;

    // 时间线，由 BeShell 主循环驱动，提供严格等间隔的帧节拍
    // 继承 EventEmitter，事件（均携带帧序号参数）：
    //   "frame"  每帧属性更新后触发（无论值是否变化）
    //   "change" 仅在有轨道值实际变更的帧触发（先于 "frame"）；
    //            轨道值未变化的帧跳过属性写入（不触发 JS setter 副作用）
    //   "stop"/"pause" 停止/暂停时触发
    class Timeline: public be::EventEmitter, public be::ILoopable {
        DECLARE_NCLASS_META
        static std::vector<JSCFunctionListEntry> methods ;

    public:
        // 播放状态
        typedef enum {
            STOPPED = 0,
            PLAYING = 1,
            PAUSED  = 2,
        } State ;

        // 事件类型（与 JS 事件名一一对应：frame/change/stop/pause）
        typedef enum {
            EVENT_FRAME = 0,    // 每帧属性更新后
            EVENT_CHANGE,       // 有轨道值实际变更的帧
            EVENT_STOP,         // 停止（手动 stop 与播完自动停止）
            EVENT_PAUSE,        // 暂停
        } EventType ;

        // C++ 事件回调：frame 为当前帧序号（stop 时为 0）
        typedef void (*EventCallback)(Timeline * tl, EventType event, uint32_t frame, void * opaque) ;

    private:
        double fps ;
        uint64_t interval_us ;              // 帧间隔 = 1000000/fps
        uint64_t totalFrames = 0 ;          // 总帧数，0 = 不限（播放到 stop()）
                                            // set/add 的 frame 超过总帧数时自动扩展为该帧号（不收缩）
        bool repeat = false ;               // true: 播完总帧数后重新计数，不自动停止

        uint64_t next_frame_us = 0 ;        // 下一帧截止时刻
        uint32_t frame = 0 ;                // 已播放帧数
        State state = STOPPED ;

        std::shared_ptr<NativeClass> loopRef = nullptr ;    // 在 loop 期间的自引用（C++ 侧引用计数+1）
        JSValue loopObjRef = JS_UNDEFINED ;                 // 在 loop 期间对 jsobj 的强引用（JS_DupValue），
                                                            // 与 loopRef 配对进出；防止 JS 侧丢引用后 jsobj 被 GC
                                                            // 导致 _handlers/闭包回收、事件无法投递（fire-and-forget）
        std::vector<JSValue> pendingResolves ;              // 等待下一帧的 Promise resolve 函数
        std::vector<JSValue> pendingRejects ;               // 对应的 reject 函数

        std::vector<Track*> tracks ;                        // 全部轨道
        bool frameListening = false ;                       // 是否有 "frame" 事件监听（无监听时不发事件，避免空转）
        bool changeListening = false ;                      // 是否有 "change" 事件监听

        // C++ 侧事件订阅（与 JS 的 on/off 并存，同一触发点先 C++ 后 JS）
        typedef struct {
            EventType type ;
            EventCallback func ;    // 遍历中退订时置 nullptr 标记失效，遍历结束后清理
            void * opaque ;
        } EventSub ;
        std::vector<EventSub> subscriptions ;               // C++ 事件订阅表
        uint8_t emitting = 0 ;                              // 事件派发深度（回调中可安全 on/off）
        bool hasCppFrame = false ;                          // 是否有 FRAME 的 C++ 订阅（与 frameListening 并列的触发条件）
        bool hasCppChange = false ;                         // 是否有 CHANGE 的 C++ 订阅

        void enterLoop() ;
        void leaveLoop() ;
        void rejectPending(const char * msg) ;
        // 应用指定帧的全部轨道值（值未变化的轨道跳过写入），返回是否有轨道发生实际变更
        bool applyTracks(uint32_t frame) ;
        // 统一事件派发：先 C++ 订阅回调，再 JS emitSync
        void emitEvent(EventType evt, uint32_t frame) ;
        // on/off 后重算 hasCppFrame/hasCppChange
        void updateEventFlags() ;
        Track * findTrack(JSValue obj, const char * prop) ;
        Track * findOrCreatePtrTrack(void * cptr, Track::TargetType type, JSValue guard) ;

    public:
        Timeline(JSContext * ctx, JSValue _jsobj=JS_NULL, double fps=60, uint64_t frames=0, bool repeat=false) ;
        ~Timeline() ;

        // ILoopable，帧 tick
        void loop(JSContext * ctx) override ;

        // EventEmitter 钩子：维护 "frame"/"change" 监听标记
        void eventAdded(const char * eventName) override ;
        void eventRemoved(const char * eventName) override ;

        /**
         * C++ 侧插入关键帧：直接绑定 C 变量指针（适用于 JS 属性由 getter/setter
         * 映射到 C 变量的混合编程场景，跳过 JS 属性赋值开销）。
         * ease 作用于 上一关键帧 -> 本关键帧 的插补段；easeS 为缓动参数。
         * guard 为该变量的属主 JS 对象，非空时 引用计数+1，
         * 防止属主对象被 GC 导致 C 变量失效；Track 析构时释放。
         */
        void set(uint32_t frame, int32_t * cptr, double value, tween::TweenEaseType ease=tween::TWEEN_LINEAR, double easeS=1.70158, JSValue guard=JS_UNDEFINED) ;
        void set(uint32_t frame, int64_t * cptr, double value, tween::TweenEaseType ease=tween::TWEEN_LINEAR, double easeS=1.70158, JSValue guard=JS_UNDEFINED) ;
        void set(uint32_t frame, float * cptr, double value, tween::TweenEaseType ease=tween::TWEEN_LINEAR, double easeS=1.70158, JSValue guard=JS_UNDEFINED) ;
        void set(uint32_t frame, double * cptr, double value, tween::TweenEaseType ease=tween::TWEEN_LINEAR, double easeS=1.70158, JSValue guard=JS_UNDEFINED) ;
        void add(uint32_t frame, int32_t * cptr, double value, tween::TweenEaseType ease=tween::TWEEN_EASE_UNKNOWN, double easeS=1.70158, JSValue guard=JS_UNDEFINED) ;
        void add(uint32_t frame, int64_t * cptr, double value, tween::TweenEaseType ease=tween::TWEEN_EASE_UNKNOWN, double easeS=1.70158, JSValue guard=JS_UNDEFINED) ;
        void add(uint32_t frame, float * cptr, double value, tween::TweenEaseType ease=tween::TWEEN_EASE_UNKNOWN, double easeS=1.70158, JSValue guard=JS_UNDEFINED) ;
        void add(uint32_t frame, double * cptr, double value, tween::TweenEaseType ease=tween::TWEEN_EASE_UNKNOWN, double easeS=1.70158, JSValue guard=JS_UNDEFINED) ;

        // JS 侧插入关键帧：绑定 "对象.属性"（对象 引用计数+1，Track 析构时释放）
        // ease=TWEEN_EASE_UNKNOWN 表示"未提供"（add 时保留原设置，set 时重置为 linear）
        void set(uint32_t frame, JSValue obj, const char * prop, double value, tween::TweenEaseType ease, double easeS) ;
        void add(uint32_t frame, JSValue obj, const char * prop, double value, tween::TweenEaseType ease, double easeS) ;

        // 移除关键帧：整帧 / 某对象属性 / 某 C 变量；轨道变空时销毁并释放 JS 引用
        void unset(uint32_t frame) ;
        void unset(uint32_t frame, JSValue obj, const char * prop) ;
        void unset(uint32_t frame, void * cptr) ;

        /**
         * C++ 侧事件订阅（与 JS 侧 tl.on("frame", cb) 同名同语义，重载签名区分两侧）
         * 同一触发点先 C++ 回调后 JS 事件；回调在主循环线程执行，回调中可安全 on/off
         *
         * 注意：监听者（opaque 指向的对象）析构前必须调 off() 退订，否则悬垂回调
         *
         * 示例：
         * ```cpp
         * tl->on(Timeline::EVENT_STOP, [](Timeline * tl, Timeline::EventType evt, uint32_t frame, void * opaque){
         *     // 播完清理...
         * }, this) ;
         * ```
         */
        void on(EventType type, EventCallback func, void * opaque=nullptr) ;
        void off(EventType type, EventCallback func, void * opaque=nullptr) ;

        // 只读访问（供导出/序列化等场景）
        double getFps() const { return fps ; }
        uint64_t getTotalFrames() const { return totalFrames ; }
        bool getRepeat() const { return repeat ; }
        const std::vector<Track*> & allTracks() const { return tracks ; }

        // 播放控制（JS 同名方法转调；stop 内最后才 leaveLoop——可能析构本对象）
        void play() ;
        void stop() ;
        void pause() ;
        void resume() ;

        static JSValue constructor(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue play(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue stop(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue pause(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue resume(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue nextFrame(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue set(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue add(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue unset(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
    } ;
}
