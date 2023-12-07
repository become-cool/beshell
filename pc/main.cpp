 
#include <iostream>
#include "BeShell.hpp"
using namespace std ;
using namespace beshell;


int main() {
    
    BeShell beshell ;
    beshell.setup() ;

    beshell.engine.eval("import 'loader'\n") ;

    while(1) {
        beshell.loop() ;
    }

    return 0 ;
}