#include "driver/display/DisplayModule.hpp"

using namespace std ;

namespace be {
namespace driver {
namespace display {

    NativeModule* DisplayModule::factory(JSContext * ctx, const char * name) {
        return new DisplayModule(ctx,name) ;
    }
    
    void DisplayModule::use(const be::BeShell & beshell) {
        beshell.engine->mloader.add("driver/display", factory) ;
        // exportFuncs
    }

}}}