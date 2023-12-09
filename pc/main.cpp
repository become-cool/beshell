 
#include <iostream>
#include "BeShell.hpp"
using namespace std ;
using namespace be;


int main() {
    
    BeShell beshell ;
    beshell.setup() ;

    

    while(1) {
        beshell.loop() ;
    }

    return 0 ;
}