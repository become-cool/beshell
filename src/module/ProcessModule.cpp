#include "module/ProcessModule.hpp"

using namespace std ;

namespace be {
    ProcessModule::ProcessModule(JSContext * ctx, const char * name,uint8_t flagGlobal)
        : NativeModule(ctx, name, flagGlobal)
    {
        exportFunction("reboot",reboot) ;
    }
    JSValue ProcessModule::reboot(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        return JS_UNDEFINED ;
    }
}