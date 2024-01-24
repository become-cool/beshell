#include "module/ProcessModule.hpp"

using namespace std ;

namespace be {
    NativeModule* ProcessModule::factory(JSContext * ctx, const char * name) {
        return new ProcessModule(ctx, name, 1) ;
    }
    void ProcessModule::defineExports() {
        exportFunction("reboot",reboot) ;
    }
    JSValue ProcessModule::reboot(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        return JS_UNDEFINED ;
    }
}