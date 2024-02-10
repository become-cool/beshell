#pragma once

#include <NativeClass.hpp>

class DemoBase: public be::NativeClass {
  DECLARE_NCLASS_META
  static std::vector<JSCFunctionListEntry> methods ;
  // static std::vector<JSCFunctionListEntry> staticMethods ;

public:
    DemoBase(JSContext * ctx, JSValue jsobj=JS_NULL) ;
    static JSValue constructor(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
} ;

class Demo: public DemoBase {
  DECLARE_NCLASS_META
  static std::vector<JSCFunctionListEntry> methods ;
  // static std::vector<JSCFunctionListEntry> staticMethods ;

public:
    Demo(JSContext * ctx, JSValue jsobj=JS_NULL) ;
    static JSValue constructor(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
} ;