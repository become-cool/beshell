#include "PathModule.hpp"



namespace be {
    NativeModule* PathModule::factory(JSContext * ctx, const char * name) {
        return new PathModule(ctx, name) ;
    }
    void PathModule::defineExports() {
    }
}