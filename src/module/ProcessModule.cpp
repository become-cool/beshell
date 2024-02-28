#include "module/ProcessModule.hpp"

#ifdef ESP_PLATFORM

#include "esp_system.h"

#endif
using namespace std;

namespace be {
    ProcessModule::ProcessModule(JSContext *ctx, const char *name, uint8_t flagGlobal)
            : NativeModule(ctx, name, flagGlobal) {
        exportFunction("reboot", reboot);
    }

    JSValue ProcessModule::reboot(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
#ifdef ESP_PLATFORM
        esp_restart();
#endif
        return JS_NewBool(ctx, true);
    }
}