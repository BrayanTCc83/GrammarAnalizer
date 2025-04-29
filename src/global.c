#include "headers/global.h"

char *to_string() {
    char *ptr = (char*) malloc(
        2048
        #ifdef DEV
        * 10
        #endif
    );
    if(!ptr) {
        printf("Error on reserve memory for char pointer.\n");
        exit(EXIT_FAILURE);
    }
    return ptr;
}

#ifdef DEV
char *to_representation() {
    char *ptr = (char*) malloc( 2048 * 10 );
    if(!ptr) {
        printf("Error on reserve memory for char pointer.\n");
        exit(EXIT_FAILURE);
    }
    return ptr;
}
#endif