#include "Demo.hpp"

using namespace std ;
using namespace be ;



DEFINE_NCLASS_META(DemoClass)


void useDemo(BeShell & beshell, JSContext * ctx) {

    beshell.engine->mloader.add<DemoModule>("name") ;

    // new DemoClass(ctx) ;
}