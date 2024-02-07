#include "ParallelModule.hpp"
#include "I8080.hpp"

namespace be {

ParallelModule::ParallelModule(JSContext * ctx, const char * name)
    : NativeModule(ctx, name, 0)
{
    exportClass<I8080>() ;
}

void ParallelModule::import() {
}

JSValue ParallelModule::jsFunc(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    return JS_UNDEFINED ;
}

}