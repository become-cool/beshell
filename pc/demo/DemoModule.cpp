#include "DemoModule.hpp"
#include "Demo.hpp"

DemoModule::DemoModule(JSContext * ctx, const char * name)
    : NativeModule(ctx, name, 0)
{
    exportClass<Demo>() ;
    exportFunction("func",jsFunc,0) ;
}

void DemoModule::import() {
    dd
}

void DemoModule::use(be::BeShell & beshell) {
    beshell.addModule<DemoModule>("demo") ;
}

JSValue DemoModule::jsFunc(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    return JS_UNDEFINED ;
}
