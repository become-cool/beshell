#include "LVModule.hpp"

namespace be {
namespace lv {

    LVModule::LVModule(JSContext * ctx, const char * name)
        : NativeModule(ctx, name, 0)
    {
        // exportClass<ClassName>() ;
        exportFunction("func",jsFunc,0) ;
    }

    // void LVModule::import() {
    // }

    
    void LVModule::use(be::BeShell & beshell) {
        driver::display::use(beshell) ;
        beshell.addModule<LVModule>("lv") ;
    }

    JSValue LVModule::jsFunc(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        return JS_UNDEFINED ;
    }
    

}}