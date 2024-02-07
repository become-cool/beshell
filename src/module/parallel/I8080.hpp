#pragma once

#include <NativeClass.hpp>

namespace be {

class I8080: public be::NativeClass<I8080> {
    DECLARE_NCLASS_META
    static std::vector<JSCFunctionListEntry> methods ;
    // static std::vector<JSCFunctionListEntry> staticMethods ;

public:
    I8080(JSContext * ctx) ;

    static JSValue jsMethod(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;

} ;

} // namespace be