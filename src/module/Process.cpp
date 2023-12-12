#include "module/Process.hpp"

using namespace std ;

namespace be {
    Process::Process(): NativeModule("process") {
        isGlobal = true ;
        isReplGlobal = true ;

        exportFunction("reboot",reboot) ;
    }
    
    JSValue Process::reboot(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        return JS_UNDEFINED ;
    }
}