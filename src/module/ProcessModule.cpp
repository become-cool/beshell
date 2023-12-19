#include "module/ProcessModule.hpp"

using namespace std ;

namespace be {
    ProcessModule::ProcessModule(): NativeModule("process") {
        isGlobal = true ;
        isReplGlobal = true ;
    }

    void ProcessModule::defineExports() {
        exportFunction("reboot",reboot) ;
    }
    
    JSValue ProcessModule::reboot(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        return JS_UNDEFINED ;
    }
}