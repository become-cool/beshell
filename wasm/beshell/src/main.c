#include <string.h>
#include "utils.h"
#include "module_fs.h"
#include "js_main_loop.h"
#include <emscripten.h>



EMSCRIPTEN_KEEPALIVE void be_exit() {
    emscripten_force_exit(0) ;
}

int main( int argc, char *argv[] ){
    js_main_loop(NULL) ;
    return 0 ;
}
