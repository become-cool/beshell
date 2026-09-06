#ifndef BE_NATIVECLASS_HPP
#define BE_NATIVECLASS_HPP

#include <map>
#include <vector>
#include <cassert>
#include <iostream>
#include <memory>
#include "quickjs.h"
#include "qjs_utils.h"
// #include "./quickjs_private.h"
#include <string.h>

namespace be {
    
	typedef JSValue (*NClassConstructorFunc)(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
	typedef void (*NClassFinalizerFunc)(JSRuntime *rt, JSValue val) ;

    class NativeClass {


    protected:
        JSContext * ctx ;

        /**
         * self 代表本对象所有 C++ 侧的引用（C++ 侧计数器）。
         *
         * 计数模型：C++ 对象在两个计数都归 0 时才销毁——
         *   - JS 侧计数：jsobj 的 QuickJS 引用计数，归 0 时触发 finalizer()，
         *     finalizer() 将 self 成员（锚点）置空，表示 JS 侧不再持有；
         *   - C++ 侧计数：self 的 shared_ptr 控制块计数。
         *     constructor() 调 shared() 建立锚点(=1)；任何需要延长 C++ 对象生命的
         *     C++ 持有方（如注册进主循环、异步任务）应保存 shared() 返回的拷贝(+1)，
         *     持有结束时释放拷贝(-1)。参考 Timeline::loopRef：play/resume 时
         *     loopRef = shared()，pause/stop 时 loopRef = nullptr。
         *
         * 注意：不要直接把 self 成员交给某个持有方使用——各持有方的释放时机相互独立
         * （finalizer 由 GC 触发，其他持有方由各自业务触发），一个成员只能表达一个
         * 释放事件；持有方必须保存自己的 shared_ptr 拷贝。
         *
         * 若 C++ 持有期间仍需经 jsobj 投递事件（fire-and-forget 场景），持有方还必须
         * 同时强持有 jsobj（如 Timeline::loopObjRef = JS_DupValue(jsobj)，与 loopRef
         * 配对进出）——jsobj 成员本身是弱持有（其唯一计数随 constructor 返回给 JS 侧），
         * JS 侧丢引用后 GC 会回收 jsobj 及 _handlers/闭包，事件将静默丢失。
         */
        std::shared_ptr<NativeClass> self ;

        static std::vector<JSCFunctionListEntry> methods ;
        static std::vector<JSCFunctionListEntry> staticMethods ;
        static JSValue constructor(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static void finalizer(JSRuntime *rt, JSValue val) ;

    protected :

        NativeClass(JSContext * ctx, JSValue jsobj) ;

        static std::map<JSContext*, std::map<JSClassID, JSValue>> mapCtxClassID2Constructor ;

    public:
    
        static JSClassID classID ;
        static const char * className ;
        JSValue jsobj ;

        virtual ~NativeClass() ;
        const std::shared_ptr<NativeClass> & shared() ;

        static NativeClass * fromJS(JSValue jsObj) ;
        
        static JSValue build(JSContext * ctx) ;
        static JSValue build(JSContext * ctx, JSValue jsobj) ;

        static JSValue defineClass(
                JSContext * ctx
                , JSClassID & classID
                , const char * className
                , const std::vector<JSCFunctionListEntry>& methods
                , const std::vector<JSCFunctionListEntry>& staticMethods
                , NClassConstructorFunc constructor
                , NClassFinalizerFunc finalizer=NativeClass::finalizer
                , JSClassID parentClassID=0
        ) ;
        static JSValue defineClass(JSContext * ctx);

        static JSValue getClass(JSContext * ctx, const JSClassID & classID) ;

        static JSValue newObject(JSContext * ctx, JSValue constructor) ;

        template <typename N>
        inline static bool instanceOf(JSContext * ctx, JSValue jsobj) {
            JSValue proto = JS_GetClassProto(ctx, N::classID);
            JSValue cotr = JS_GetPropertyStr(ctx, proto, "constructor") ;
            return JS_IsInstanceOf(ctx, jsobj, cotr) ;
        }
        
        static JSValue setDestructor(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;

    private:
            JSValue destructor = JS_NULL ;
    } ;    
}

#define DECLARE_NCLASS_META                                             \
    public:                                                             \
        static const char * className ;                                 \
        static JSValue build(JSContext * ctx) ;                         \
        static JSValue build(JSContext * ctx, JSValue jsobj) ;          \
        static JSValue defineClass(JSContext * ctx) ;                   \
        static JSClassID classID ;                                      \
    friend class NativeClass ;                                          \
    private:

#define DEFINE_NCLASS_META_STATIC(CLASS)                                \
    const char * CLASS::className = #CLASS ;                            \
    JSClassID CLASS::classID = 0 ;

#define DEFINE_NCLASS_META_BUILD(CLASS)                                 \
    JSValue CLASS::build(JSContext * ctx) {                             \
        CLASS::defineClass(ctx) ;                                       \
        return JS_NewObjectClass(ctx, CLASS::classID) ;                 \
    }                                                                   \
    JSValue CLASS::build(JSContext * ctx, JSValue jsobj) {              \
        if( JS_IsNone(jsobj) ){                                         \
            return CLASS::build(ctx) ;                                  \
        }                                                               \
        else if(JS_IsFunction(ctx, jsobj)) {                            \
            if( JS_GetClassIDFromConstructor(ctx, jsobj)==CLASS::classID ) {   \
                return JS_NewObjectClass(ctx, CLASS::classID) ;         \
            }                                                           \
            else {                                                      \
                JSValue proto = JS_GetPropertyStr(ctx, jsobj, "prototype") ;    \
                return JS_NewObjectProto(ctx,proto) ;                   \
            }                                                           \
        }                                                               \
        else {                                                          \
            return jsobj ;                                              \
        }                                                               \
    }

#define DEFINE_NCLASS_META_DEFINECLASS(CLASS,PARENT_CLASS)              \
    JSValue CLASS::defineClass(JSContext * ctx) {                       \
        if(PARENT_CLASS::classID==0) {                                  \
            PARENT_CLASS::defineClass(ctx) ;                            \
        }                                                               \
        return NativeClass::defineClass(                                \
                ctx,CLASS::classID,CLASS::className                     \
                , CLASS::methods, CLASS::staticMethods                  \
                , CLASS::constructor                                    \
                , CLASS::finalizer                                      \
                , PARENT_CLASS::classID                                 \
            ) ;                                                         \
    }

#define DEFINE_NCLASS_META(CLASS,PARENT_CLASS)                          \
    DEFINE_NCLASS_META_STATIC(CLASS)                                    \
    DEFINE_NCLASS_META_BUILD(CLASS)                                     \
    DEFINE_NCLASS_META_DEFINECLASS(CLASS,PARENT_CLASS)

#define DEFINE_NCLASS_META_NAME(CLASS,PARENT_CLASS, name)               \
    const char * CLASS::className = name ;                              \
    JSClassID CLASS::classID = 0 ;                                      \
    DEFINE_NCLASS_META_BUILD(CLASS)                                     \
    DEFINE_NCLASS_META_DEFINECLASS(CLASS,PARENT_CLASS)

#define CHECK_NCLASS_ARGV(CLASS,jsval)                                  \
    if( !be::NativeClass::instanceOf<CLASS>(ctx,jsval) ) {              \
        JSTHROW("value is not a %s object", #CLASS)                     \
    }

#define JSVALUE_TO_NCLASS(CLASS, jsval, var)                            \
    CHECK_NCLASS_ARGV(CLASS,jsval)                                      \
    CLASS * var = (CLASS *) be::NativeClass::fromJS(jsval) ;            \
    if(!var) {                                                          \
        JSTHROW("invalid native pointer")                               \
    }
#define JSVALUE_TO_NCLASS_VAR(CLASS, jsval, var)                        \
    CHECK_NCLASS_ARGV(CLASS,jsval)                                      \
    var = (CLASS *) be::NativeClass::fromJS(jsval) ;                    \
    if(!var) {                                                          \
        JSTHROW("invalid native pointer")                               \
    }

#define THIS_NCLASS(CLASS,var)                                          \
    JSVALUE_TO_NCLASS(CLASS,this_val,var)


#endif
