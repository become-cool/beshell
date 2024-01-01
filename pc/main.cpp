 
#include <iostream>
#include <memory>
#include <string.h>
#include "path.hpp"
#include "BeShell.hpp"
#include "path.hpp"
#include "BeShell.hpp"
#include "telnet/Protocal.hpp"
using namespace std ;
using namespace be;

int main(int argc, char* argv[]) {

    char p [256] ;
    strcpy(p,"/./..") ;
    path_normalize(p) ;
    cout << p << endl ;
    
    
    BeShell beshell ;

    beshell.useBasic() ;
    beshell.setup() ;

    char * dirname = strdup(argv[0]) ;
    path_dirname(argv[0],dirname) ;

    string scriptpath(dirname) ;
    scriptpath+= "/../demo.js" ;
    cout << scriptpath ;

    // if(argc>1) {
        if( JS_IsException(beshell.engine->evalScript(scriptpath.c_str())) ) {
            beshell.engine->dumpError() ;
        }
    // }

    free(dirname) ;

    beshell.run() ;

    return 0 ;
}