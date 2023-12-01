#include "utils.h"
#include "module_fs.h"
#include "js_main_loop.h"
#include "anchor9.h"
#include <stdio.h>
#include <string.h>


typedef union {
    uint8_t raw[8] ;
    struct {
        uint8_t h1;
        uint8_t h2;
        uint8_t pkgid;
        uint8_t cmd;
        uint8_t len1;
        uint8_t len2;
        uint8_t len3;
        uint8_t len4;
    } fields ;
} head ;

int main( int argc, char *argv[] ){

    dd

    char * rootpath = "../filesystem/root/" ;
    char * scriptpath = NULL ;

    for(int i=0;i<argc;i++) {
        if(strcmp(argv[i],"--root")==0 && argc>=i+2) {
            rootpath = argv[++i] ;
        }
        else if(strcmp(argv[i],"--script")==0 && argc>=i+2) {
            scriptpath = argv[++i] ;
        }
        else if(strcmp(argv[i],"--help")==0) {
            printf(
                "\nbeshell [--root /fs/root] [--script /path/to/script] [--help]\n\n"
            ) ;
            exit(0) ;
        }
    }
    
    module_fs_set_vfs_path_prefix(rootpath) ;

    js_main_loop(scriptpath) ;

    return 0 ;
}
