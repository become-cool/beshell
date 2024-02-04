#include "DemoModule.hpp"
#include "BeShell.hpp"
#include "Demo.hpp"



void useDemo(be::BeShell & beshell, JSContext * ctx) {

    beshell.engine->mloader.add<DemoModule>("name") ;

    // new DemoClass(ctx) ;
}