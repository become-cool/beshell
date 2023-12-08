 
#include <iostream>
#include "telnet/Protocal.hpp"
using namespace std ;
using namespace be;

void print_block(uint8_t * data, int columns, int rows) {
    for(int r=0; r<rows; r++ ) {
        for(int c=0; c<columns; c++ ) {
            printf("0x%02x ", *data) ;
            data++ ;
        }
        printf("\n") ;
    }
}

void package_handler(Package & pkg) {
    printf(">>> receive package, pkgid:%d, cmd:%d, length:%d\n",pkg.head.fields.pkgid,pkg.head.fields.cmd,pkg.body_len) ;
}

int main() {
    
    uint8_t data[] = {
        // 'h', 'e', 'l', 'l', 'o', '\n',
        // 5, 18, 0, 2, 5, 'h', 'e', 'l', 'l', 'o', 0,
        // 5, 18, 1, 2, 5, 'h', 'e', 'l', 'l', 'o', 0,
        // 'h', 'e', 'l', 'l', 'o','\n',

        // 5, 18, 1, 2, 0, 0,
    } ;
    
    Parser parser([](Package & pkg){
        printf(">>> receive package, pkgid:%d, cmd:%d, length:%d\n",pkg.head.fields.pkgid,pkg.head.fields.cmd,pkg.body_len) ;
    }) ;

    int remain = sizeof(data) ;
    uint8_t * buff = data ;
    size_t seg = 3 ;

    while(remain>0) {
        seg = remain>3? 3: remain ;

        parser.parse(buff, seg) ;

        buff+= seg ;
        remain-= seg ;
    }

    

    return 0 ;
}