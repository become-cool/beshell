#include "Demo.hpp"

using namespace std ;
using namespace be ;


JSValue jsMethod1(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    cout << "method1" << endl ;
    return JS_UNDEFINED ;
}
JSValue jsMethod2(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    cout << "method2" << endl ;
    return JS_UNDEFINED ;
}
JSValue jsMethod3(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    cout << "method3" << endl ;
    
    cout << NativeClass::instanceOf<DemoBase>(ctx, argv[0]) << endl ;

    return JS_UNDEFINED ;
}

// DemoBase -------------------
DEFINE_NCLASS_META(DemoBase,NativeClass)
std::vector<JSCFunctionListEntry> DemoBase::methods = {
    JS_CFUNC_DEF("method1", 0, jsMethod1),
    JS_CFUNC_DEF("method2", 0, jsMethod2),
} ;
DemoBase::DemoBase(JSContext * ctx, JSValue jsobj)
    : NativeClass(ctx,build(ctx,jsobj))
{}

JSValue DemoBase::constructor(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    auto obj = new DemoBase(ctx) ;
    obj->self = std::shared_ptr<DemoBase> (obj) ;
    return obj->jsobj ;
}

// Demo -------------------
DEFINE_NCLASS_META(Demo,DemoBase)
std::vector<JSCFunctionListEntry> Demo::methods = {
    JS_CFUNC_DEF("method2", 0, jsMethod2),
    JS_CFUNC_DEF("method3", 0, jsMethod3),
} ;

Demo::Demo(JSContext * ctx, JSValue jsobj)
    : DemoBase(ctx, build(ctx,jsobj))
{}
JSValue Demo::constructor(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    auto obj = new Demo(ctx) ;
    obj->self = std::shared_ptr<Demo> (obj) ;
    return obj->jsobj ;
}

// Demo -------------------
DEFINE_NCLASS_META(DemoChild,Demo)
std::vector<JSCFunctionListEntry> DemoChild::methods = {
    JS_CFUNC_DEF("method2", 0, jsMethod2),
    JS_CFUNC_DEF("method3", 0, jsMethod3),
} ;


DemoChild::DemoChild(JSContext * ctx, JSValue jsobj)
    : Demo(ctx, build(ctx,jsobj))
{}
JSValue DemoChild::constructor(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    auto obj = new DemoChild(ctx) ;
    obj->self = std::shared_ptr<DemoChild> (obj) ;
    return obj->jsobj ;
}
