#include "debug.h"


// void print_block(uint8_t * data, int columns, int rows) {
//     for(int r=0; r<rows; r++ ) {
//         for(int c=0; c<columns; c++ ) {
//             printf("0x%02x ", *data) ;
//             data++ ;
//         }
//         printf("\n") ;
//     }
// }

void print_block(uint8_t * data, int length, int columns) {
    int n=0 ;
    while(n<length) {
        for(int c=0; c<columns && n<length; c++ ) {
            printf("0x%02x,", *data) ;
            data++ ;
            n++ ;
        }
        printf("\n") ;
    }
}