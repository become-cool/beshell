#include "Timeline.hpp"
#include "../../qjs_utils.h"

namespace be::animator {

    std::vector<JSCFunctionListEntry> Timeline::methods = {
        JS_CFUNC_DEF("play", 0, Timeline::play),
        JS_CFUNC_DEF("stop", 0, Timeline::stop),
        JS_CFUNC_DEF("pause", 0, Timeline::pause),
        JS_CFUNC_DEF("resume", 0, Timeline::resume),
        JS_CFUNC_DEF("nextFrame", 0, Timeline::nextFrame),
        JS_CFUNC_DEF("set", 0, Timeline::set),
        JS_CFUNC_DEF("add", 0, Timeline::add),
        JS_CFUNC_DEF("unset", 0, Timeline::unset),
    } ;

    // 解析 easeName/easeParams：ease 省略时返回 TWEEN_EASE_UNKNOWN（表示"未提供"）
    static bool parseEase(JSContext *ctx, JSValueConst jsEase, JSValueConst jsParams, tween::TweenEaseType & ease, double & easeS) {
        ease = tween::TWEEN_EASE_UNKNOWN ;
        easeS = 1.70158 ;
        if(JS_IsString(jsEase)) {
            const char * cstr = JS_ToCString(ctx, jsEase) ;
            std::string name = cstr ;
            JS_FreeCString(ctx, cstr) ;
            ease = tween::map_name_to_id(name.c_str()) ;
            if(ease==tween::TWEEN_EASE_UNKNOWN) {
                JS_ThrowReferenceError(ctx, "unknown ease name: %s", name.c_str()) ;
                return false ;
            }
        }
        else if(JS_IsNumber(jsEase)) {
            int32_t id ;
            if(JS_ToInt32(ctx, &id, jsEase)!=0) {
                JS_ThrowReferenceError(ctx, "invalid easeName") ;
                return false ;
            }
            ease = (tween::TweenEaseType) id ;
        }
        else if(!JS_IsNone(jsEase)) {
            JS_ThrowReferenceError(ctx, "easeName must be a string or number") ;
            return false ;
        }
        if(JS_IsNumber(jsParams)) {
            JS_ToFloat64(ctx, &easeS, jsParams) ;
        }
        return true ;
    }

    // 设置/累加一个 对象.属性 关键帧，出错返回 -1（已抛出异常）
    static int setOne(JSContext *ctx, Timeline * that, uint32_t frame, JSValueConst obj
            , const char * prop, JSValueConst jsValue, JSValueConst jsEase, JSValueConst jsParams, bool merge) {
        double value ;
        if(JS_ToFloat64(ctx, &value, jsValue)!=0) {
            JS_ThrowReferenceError(ctx, "keyframe value of %s must be a number", prop) ;
            return -1 ;
        }
        tween::TweenEaseType ease ;
        double easeS ;
        if(!parseEase(ctx, jsEase, jsParams, ease, easeS)) {
            return -1 ;
        }
        if(merge) {
            that->add(frame, obj, prop, value, ease, easeS) ;
        } else {
            that->set(frame, obj, prop, value, ease, easeS) ;
        }
        return 0 ;
    }

    // 处理属性表 {prop: value | [value, easeName?, easeParams?], ...}，出错返回 -1
    static int handlePropMap(JSContext *ctx, Timeline * that, uint32_t frame, JSValueConst obj, JSValueConst map, bool merge) {
        int ret = 0 ;
        JSPropertyEnum * ptab = nullptr ;
        uint32_t plen = 0 ;
        if(JS_GetOwnPropertyNames(ctx, &ptab, &plen, map, JS_GPN_STRING_MASK|JS_GPN_ENUM_ONLY)!=0) {
            js_std_dump_error(ctx) ;
            return -1 ;
        }
        for(uint32_t i=0; i<plen && ret==0; i++) {
            const char * pname = JS_AtomToCString(ctx, ptab[i].atom) ;
            JSValue v = JS_GetProperty(ctx, map, ptab[i].atom) ;
            if(JS_IsArray(ctx, v)) {
                // [value, easeName?, easeParams?]
                JSValue jv0 = JS_GetPropertyUint32(ctx, v, 0) ;
                JSValue jv1 = JS_GetPropertyUint32(ctx, v, 1) ;
                JSValue jv2 = JS_GetPropertyUint32(ctx, v, 2) ;
                ret = setOne(ctx, that, frame, obj, pname, jv0, jv1, jv2, merge) ;
                JS_FreeValue(ctx, jv0) ;
                JS_FreeValue(ctx, jv1) ;
                JS_FreeValue(ctx, jv2) ;
            } else {
                ret = setOne(ctx, that, frame, obj, pname, v, JS_UNDEFINED, JS_UNDEFINED, merge) ;
            }
            JS_FreeValue(ctx, v) ;
            JS_FreeCString(ctx, pname) ;
        }
        // 该版本 QuickJS 无 JS_FreePropertyEnum，手动释放
        for(uint32_t i=0; i<plen; i++) {
            JS_FreeAtom(ctx, ptab[i].atom) ;
        }
        js_free(ctx, ptab) ;
        return ret ;
    }

    /**
     * set/add 关键帧，支持三种输入形式：
     *   ① 单属性：set(frm, obj, prop, value [, easeName [, easeParams]])
     *   ② 属性表：set(frm, obj, {prop1: value, prop2: [value, easeName, easeParams], ...})
     *   ③ 多对象：set(frm, [obj1, {prop表}, obj2, {prop表}, ...])
     *
     * set 为替换语义（同帧同属性覆盖，ease 省略重置为 linear），
     * add 为合并语义（同帧同属性值累加，ease 省略保留原设置）。
     * ease 设在目标关键帧上，表示如何到达该值。
     */
    static JSValue setAddCommon(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv, bool merge) {
        THIS_NCLASS(Timeline, that)
        CHECK_ARGC(2) ;
        ARGV_TO_INT32(0, frame)
        if(frame<0) {
            JSTHROW("frame must be >= 0")
        }

        int ret = 0 ;
        if(JS_IsArray(ctx, argv[1])) {
            // 形式③ 多对象：[obj, propMap, obj, propMap, ...]
            uint32_t len = 0 ;
            JSValue jslen = JS_GetPropertyStr(ctx, argv[1], "length") ;
            JS_ToUint32(ctx, &len, jslen) ;
            JS_FreeValue(ctx, jslen) ;
            if(len%2!=0) {
                JSTHROW("array args must be [obj, propMap, ...] pairs")
            }
            for(uint32_t i=0; i<len && ret==0; i+=2) {
                JSValue obj = JS_GetPropertyUint32(ctx, argv[1], i) ;
                JSValue map = JS_GetPropertyUint32(ctx, argv[1], i+1) ;
                if(!JS_IsObject(obj) || JS_IsArray(ctx,obj) || !JS_IsObject(map) || JS_IsArray(ctx,map)) {
                    JS_FreeValue(ctx, obj) ;
                    JS_FreeValue(ctx, map) ;
                    JSTHROW("array args must be [obj, propMap, ...] pairs")
                }
                ret = handlePropMap(ctx, that, (uint32_t)frame, obj, map, merge) ;
                JS_FreeValue(ctx, obj) ;
                JS_FreeValue(ctx, map) ;
            }
        }
        else if(JS_IsObject(argv[1])) {
            if(argc>2 && JS_IsString(argv[2])) {
                // 形式① 单属性
                CHECK_ARGC(4) ;
                const char * prop = JS_ToCString(ctx, argv[2]) ;
                ret = setOne(ctx, that, (uint32_t)frame, argv[1], prop, argv[3]
                    , argc>4 ? argv[4] : JS_UNDEFINED
                    , argc>5 ? argv[5] : JS_UNDEFINED
                    , merge) ;
                JS_FreeCString(ctx, prop) ;
            }
            else if(argc>2 && JS_IsObject(argv[2]) && !JS_IsArray(ctx, argv[2])) {
                // 形式② 属性表
                ret = handlePropMap(ctx, that, (uint32_t)frame, argv[1], argv[2], merge) ;
            }
            else {
                JSTHROW("invalid arguments")
            }
        }
        else {
            JSTHROW("arg2 must be an object or array")
        }
        if(ret<0) {
            return JS_EXCEPTION ;
        }
        return JS_UNDEFINED ;
    }

    /**
     * 构造函数：创建时间线
     *
     * @module animator
     * @class Timeline
     * @method constructor
     * @param fps:number=60 帧率，必须 > 0
     * @param frames:number=0 总帧数，0 = 不限（播放到 stop()）；
     *                        set/add 的帧号超过总帧数时自动扩展
     * @param repeat:boolean=false true 时播完总帧数后重新计数循环播放
     * @return Timeline 时间线实例
     */
    JSValue Timeline::constructor(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        ARGV_TO_DOUBLE_OPT(0, fps, 60)
        if(fps<=0) {
            JSTHROW("fps must be > 0")
        }
        ARGV_TO_INT64_OPT(1, frames, 0)
        bool repeat = argc>2 && JS_ToBool(ctx, argv[2]) ;

        auto obj = new Timeline(ctx, this_val, fps, frames>0 ? (uint64_t)frames : 0, repeat) ;
        obj->shared() ;
        return obj->jsobj ;
    }

    /**
     * 从 stopped 态开始播放：帧计数归零，加入主循环，帧数达到总帧数时自动停止
     * （repeat=true 时重新计数继续播放）。播放期间对象被主循环持有，
     * JS 侧丢失引用也会播完才销毁（fire-and-forget）
     *
     * @module animator
     * @class Timeline
     * @method play
     */
    JSValue Timeline::play(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        THIS_NCLASS(Timeline, that)
        that->play() ;
        return JS_UNDEFINED ;
    }

    /**
     * 停止播放：帧计数归零，挂起的 nextFrame() Promise 以 Error("timeline stopped") reject
     *
     * @module animator
     * @class Timeline
     * @method stop
     * @fires stop 发出 "stop" 事件
     */
    JSValue Timeline::stop(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        THIS_NCLASS(Timeline, that)
        that->stop() ;
        return JS_UNDEFINED ;
    }

    /**
     * 暂停播放：挂起的 nextFrame() Promise 保留，resume() 后继续
     *
     * @module animator
     * @class Timeline
     * @method pause
     * @fires pause 发出 "pause" 事件
     */
    JSValue Timeline::pause(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        THIS_NCLASS(Timeline, that)
        that->pause() ;
        return JS_UNDEFINED ;
    }

    /**
     * 从暂停处继续播放，重新起节拍
     *
     * @module animator
     * @class Timeline
     * @method resume
     */
    JSValue Timeline::resume(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        THIS_NCLASS(Timeline, that)
        that->resume() ;
        return JS_UNDEFINED ;
    }

    /**
     * 等待下一帧：返回 Promise，在下一个帧 tick 时以当前帧序号解决
     *
     * 配合 await 实现帧率受控、严格等间隔的动画循环：
     * ```javascript
     * const tl = new Timeline(60)
     * tl.play()
     * while(running) {
     *     const frame = await tl.nextFrame()
     *     // 更新动画...
     * }
     * ```
     *
     * @module animator
     * @class Timeline
     * @method nextFrame
     * @return Promise\<number\> 当前帧序号
     */
    JSValue Timeline::nextFrame(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        THIS_NCLASS(Timeline, that)
        JSValue resolving_funcs[2] ;
        JSValue promise = JS_NewPromiseCapability(ctx, resolving_funcs) ;
        if(JS_IsException(promise)) {
            return promise ;
        }
        that->pendingResolves.push_back(resolving_funcs[0]) ;
        that->pendingRejects.push_back(resolving_funcs[1]) ;
        return promise ;
    }

    /**
     * 设置关键帧（替换语义：同帧同属性覆盖，ease 省略时重置为 linear）
     *
     * 支持三种输入形式：
     * ```javascript
     * // 单属性
     * tl.set(30, obj, "x", 100, "outQuad", 1.70158)
     * // 属性表
     * tl.set(30, obj, { x: 100, y: [200, "inOutBounce"] })
     * // 多对象
     * tl.set(30, [obj1, {x: 100}, obj2, {y: 50}])
     * ```
     * 缓动设在目标关键帧上，表示"如何到达该值"。
     *
     * @module animator
     * @class Timeline
     * @method set
     * @param frame:number 帧号（超过总帧数时自动扩展总帧数）
     * @param obj:object|array 目标对象，或 [obj, prop表, ...] 多对象数组
     * @param prop:string|object 属性名，或属性表 {prop: value|[value, ease, easeS]}
     * @param value:number 目标值（单属性形式）
     * @param ease:string="linear" 缓动类型名称（同 tween 函数名）
     * @param easeS:number=1.70158 缓动参数（仅 Back/Elastic 有效）
     */
    JSValue Timeline::set(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        return setAddCommon(ctx, this_val, argc, argv, false) ;
    }

    /**
     * 累加关键帧（合并语义：同帧同属性值累加，ease 省略时保留原设置）
     *
     * 输入形式同 set()
     *
     * @module animator
     * @class Timeline
     * @method add
     * @param frame:number 帧号
     * @param obj:object|array 目标对象，或 [obj, prop表, ...] 多对象数组
     * @param prop:string|object 属性名，或属性表 {prop: value|[value, ease, easeS]}
     * @param value:number 累加值（单属性形式）
     * @param ease:string 缓动类型名称（省略时保留原设置）
     * @param easeS:number 缓动参数
     */
    JSValue Timeline::add(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        return setAddCommon(ctx, this_val, argc, argv, true) ;
    }

    /**
     * 移除关键帧
     *
     * ```javascript
     * tl.unset(frm)              // 移除该帧上的全部对象属性（该帧失去关键帧地位）
     * tl.unset(frm, obj, "x")    // 仅移除该帧上的某个对象属性
     * ```
     * 轨道变空时销毁并释放 JS 对象引用。
     *
     * @module animator
     * @class Timeline
     * @method unset
     * @param frame:number 帧号
     * @param obj:object 目标对象（可选，与 prop 一起提供时仅移除该属性）
     * @param prop:string 属性名（可选）
     */
    JSValue Timeline::unset(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        THIS_NCLASS(Timeline, that)
        CHECK_ARGC(1) ;
        ARGV_TO_INT32(0, frame)
        if(frame<0) {
            JSTHROW("frame must be >= 0")
        }
        if(argc>=3) {
            if(!JS_IsObject(argv[1])) {
                JSTHROW("arg2 must be an object")
            }
            const char * prop = JS_ToCString(ctx, argv[2]) ;
            that->unset((uint32_t)frame, argv[1], prop) ;
            JS_FreeCString(ctx, prop) ;
        } else {
            that->unset((uint32_t)frame) ;
        }
        return JS_UNDEFINED ;
    }
}
