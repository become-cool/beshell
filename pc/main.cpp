 
#include <iostream>
#include <memory>
#include <string.h>
#include "path.hpp"
#include "BeShell.hpp"
#include "demo/Demo.hpp"
#include "path.hpp"
#include "BeShell.hpp"
#include "telnet/Protocal.hpp"
#include "driver/display/DisplayModule.hpp"


using namespace std ;
using namespace be;
using namespace be::driver::display;


int main(int argc, char* argv[]) {

    BeShell beshell ;

    beshell.useBasic() ;
    // DisplayModule::use(beshell) ;
    useDemo(beshell, beshell.engine->ctx) ;

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