#pragma once

#include <NativeClass.hpp>

class Demo: public be::NativeClass<Demo> {
  DECLARE_NCLASS_META
  static std::vector<JSCFunctionListEntry> methods ;
  // static std::vector<JSCFunctionListEntry> staticMethods ;

public:
    Demo(JSContext * ctx) ;

    static JSValue jsMethod(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;

} ;