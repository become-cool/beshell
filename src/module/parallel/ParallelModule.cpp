#include "ParallelModule.hpp"

namespace be {

ParallelModule::ParallelModule(JSContext * ctx, const char * name)
    : NativeModule(ctx, name, 0)
{
}

void ParallelModule::import() {
}

JSValue ParallelModule::jsFunc(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    return JS_UNDEFINED ;
}

}