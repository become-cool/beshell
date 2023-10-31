#include "utils.h"
#include "module_fs.h"
#include "js_main_loop.h"
#include "anchor9.h"
#include <stdio.h>
#include <string.h>

void main( int argc, char *argv[] ){

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
}
