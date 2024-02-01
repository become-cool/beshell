#include "Demo.hpp"

using namespace std ;
using namespace be ;


const char * DemoClass::className = "DemoClass" ;
JSClassID DemoClass::classID = 0 ;
std::map<JSContext*, JSValue> DemoClass::mapCtxProtos ;


void useDemo(JSContext * ctx) {
    // new B(ctx) ;
    new DemoClass(ctx) ;
}