#pragma once

#include "../EventEmitter.hpp"

namespace be::driver {

    // DriverClass：驱动类的类型标记基类。
    //
    // 继承 EventEmitter，使所有驱动类默认具备事件发布/订阅能力；同时作为「驱动类」的
    // 类型标记，用于在 beshell.use<T>() 中区分驱动类（注册进 driver 模块）与普通
    // NativeClass 子类（如 Partition 等）。目前不额外定义任何成员或方法。
    class DriverClass: public be::EventEmitter {
        DECLARE_NCLASS_META
    private:
        static std::vector<JSCFunctionListEntry> methods ;
    public:
        DriverClass(JSContext * ctx, JSValue _jsobj=JS_NULL) ;
        static JSValue constructor(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
    } ;

}
