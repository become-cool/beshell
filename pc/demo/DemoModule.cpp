#include "DemoModule.hpp"
#include "Demo.hpp"

DemoModule::DemoModule(JSContext * ctx, const char * name)
    : NativeModule(ctx, name, 0)
{
    // exportClass<DemoBase>() ;
    exportClass<Demo>() ;
    exportClass<DemoChild>() ;
    exportFunction("func",jsFunc,0) ;
}

void DemoModule::import() {
}

void DemoModule::use(be::BeShell & beshell) {
    beshell.useModule<DemoModule>("demo") ;
}

JSValue DemoModule::jsFunc(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    return JS_UNDEFINED ;
}
