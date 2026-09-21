#pragma once

#include "../../EventEmitter.hpp"
#include "../../JSEngine.hpp"
#include "Tween.hpp"
#include <string>
#include <map>
#include <set>

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
        double originValue = 0 ;            // 轨道创建时捕获的目标原值（ensureOrigin 用；
                                            // 创建时即捕获而非首个 >0 帧关键帧时读当前值，
                                            // 防止 playAt 预览改写当前值后污染 0 帧原点）

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
        // 以创建时捕获的 originValue 补一个 0 帧关键帧（防止首次突变，使孤立关键帧可插补）
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
    // 支持区间播放与倒放：play(fromFrame, toFrame)，toFrame<fromFrame 时倒放
    // 继承 EventEmitter，事件（均携带帧序号参数）：
    //   "frame"  每帧属性更新后触发（无论值是否变化）
    //   "change" 仅在有轨道值实际变更的帧触发（先于 "frame"）；
    //            轨道值未变化的帧跳过属性写入（不触发 JS setter 副作用）
    //   "stop"/"pause" 停止/暂停时触发
    //   "arrive:<帧号>" 播放头到达/途经指定帧时触发（含 jumpTo 落点；起始帧不触发）
    //
    // 帧动作（set/add 的 options 参数设置）：
    //   jumpTo   到达该帧时立即跳转到目标帧（当 tick 链式触发目标帧动作）
    //   playTo   到达该帧时改向目标帧播放（支持倒放），到达后恢复原方向与终点
    //   动作带触发次数：每次触发 -1，0 不再触发，-1 不限次数（不递减）；play() 时恢复初始值
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

        // 事件类型（与 JS 事件名一一对应：frame/change/stop/pause/arrive:<帧号>）
        typedef enum {
            EVENT_FRAME = 0,    // 每帧属性更新后
            EVENT_CHANGE,       // 有轨道值实际变更的帧
            EVENT_STOP,         // 停止（手动 stop 与播完自动停止）
            EVENT_PAUSE,        // 暂停
            EVENT_ARRIVE,       // 到达/途经监听帧（配合 watchArrive 或 JS "arrive:<帧号>"）
        } EventType ;

        // 帧动作：播放头到达某帧时的跳转/改向（次数每次触发 -1，0 不触发，-1 不限且不递减）
        typedef struct {
            int64_t jumpTo = -1 ;           // 目标帧，<0 = 无
            int jumpCount = 0 ;             // 剩余触发次数
            int jumpCountInit = 0 ;         // 初始次数（play() 时恢复）
            int64_t playTo = -1 ;
            int playCount = 0 ;
            int playCountInit = 0 ;
            bool stop = false ;             // 到达本帧即停止播放（优先于 jump/play）
            bool resume = false ;           // playTo 到达后继续原顺序播放（否则停止）
            std::string label ;             // 帧标记名（空 = 无）
        } FrameAction ;

        // setFrameAction 的字段更新掩码
        typedef enum {
            ACTION_JUMP = 1,                // 更新 jumpTo/jumpCount
            ACTION_PLAY = 2,                // 更新 playTo/playCount
            ACTION_STOP = 4,                // 更新 stop
            ACTION_LABEL = 8,               // 更新 label
            ACTION_RESUME = 16,             // 更新 resume
        } ActionMask ;

        // C++ 事件回调：frame 为当前帧序号（stop 时为 0）
        typedef void (*EventCallback)(Timeline * tl, EventType event, uint32_t frame, void * opaque) ;

    private:
        double fps ;
        uint64_t interval_us ;              // 帧间隔 = 1000000/fps
        uint64_t totalFrames = 0 ;          // 总帧数，0 = 不限（播放到 stop()）
                                            // set/add 的 frame 超过总帧数时自动扩展为该帧号（不收缩）
        bool repeat = false ;               // true: 播完总帧数后重新计数，不自动停止
        int64_t repeatCount = -1 ;          // 额外重播次数（-1=无限；0=只播一次）；每次回绕 -1
        int64_t repeatCountInit = -1 ;      // play() 时恢复的初始重播次数

        uint64_t next_frame_us = 0 ;        // 下一帧截止时刻
        uint32_t frame = 0 ;                // 当前帧号
        State state = STOPPED ;

        uint32_t cursorFrame = 0 ;          // 游标帧号（evalAt() 缺省求值帧，供编辑器等 scrub 场景使用）

        // 播放会话状态（play() 时确定）
        uint32_t fromFrame = 0 ;            // 起始帧
        uint32_t endFrame = 0 ;             // 终点帧（UINT32_MAX = 不限）
        int dir = 1 ;                       // 播放方向：+1 顺放 / -1 倒放
        bool playToActive = false ;         // playTo 改向进行中
        bool playToResume = false ;         // playTo 到达后是否继续原顺序播放（否则停止）
        uint32_t savedEnd = 0 ;             // playTo 进入前的终点（到达后恢复）
        int savedDir = 1 ;                  // playTo 进入前的方向（到达后恢复）

        std::map<uint32_t, FrameAction> frameActions ;  // 帧动作表（帧号 -> jumpTo/playTo）
        std::set<uint32_t> arriveFrames ;               // arrive 事件监听帧集合（JS "arrive:<帧号>" 与 C++ watchArrive 共用）

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
        Timeline(JSContext * ctx, JSValue _jsobj=JS_NULL, double fps=60, uint64_t frames=0, bool repeat=false, int64_t repeatCount=-1) ;
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
        // 移除某 C 变量的整条轨道（全部关键帧）
        void unset(void * cptr) ;

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
        int64_t getRepeatCount() const { return repeatCountInit ; }
        const std::vector<Track*> & allTracks() const { return tracks ; }
        // 帧动作表（帧号 -> 帧动作），供导出/序列化场景枚举
        const std::map<uint32_t, FrameAction> & getFrameActions() const { return frameActions ; }

        // 游标帧号（evalAt 缺省求值帧，纯记录，不影响播放）
        uint32_t getCursor() const { return cursorFrame ; }
        void setCursor(uint32_t frame) { cursorFrame = frame ; }

        // 播放控制（JS 同名方法转调；stop 内最后才 leaveLoop——可能析构本对象）
        // play(from, to)：从 from 播到 to；to<0 时播到 totalFrames（0=不限）；to<from 时倒放
        void play(uint32_t from=0, int64_t to=-1) ;
        void stop() ;
        void pause() ;
        void resume() ;

        // 游标预览：把各轨道在指定帧的插补值写入目标（不推进播放、不改播放状态），
        // 并把游标移动到该帧；有实际变更时触发 change 事件（不触发 frame/arrive/帧动作）。
        // 轨道关键帧与 0 帧原点（创建时捕获）均不受其影响，供编辑器等 scrub 场景使用
        void playAt(uint32_t frame) ;

        // 设置帧动作（mask 选择更新哪些字段；次数每次触发 -1，0 不触发，-1 不限）
        void setFrameAction(uint32_t frame, uint8_t mask, int64_t jumpTo, int jumpCount, int64_t playTo, int playCount, bool stop, bool resume, const char * label) ;
        // C++ 侧 arrive 事件监听帧登记/注销（与 JS "arrive:<帧号>" 共用帧集合，配合 on(EVENT_ARRIVE) 使用）
        void watchArrive(uint32_t frame) ;
        void unwatchArrive(uint32_t frame) ;

        static JSValue constructor(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue play(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue stop(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue pause(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue resume(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue playAt(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue nextFrame(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue set(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue add(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue unset(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue evalAt(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue getCursor(JSContext *ctx, JSValueConst this_val) ;
        static JSValue setCursor(JSContext *ctx, JSValueConst this_val, JSValueConst val) ;
    } ;
}
