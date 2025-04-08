#include "headers/global.h"

char *to_string() {
    char *ptr = (char*) malloc( 1024 );
    if(!ptr) {
        printf("Error on reserve memory for char pointer.\n");
        exit(EXIT_FAILURE);
    }
    return ptr;
}

#ifdef DEV
char *to_representation() {
    char *ptr = (char*) malloc( 1024 );
    if(!ptr) {
        printf("Error on reserve memory for char pointer.\n");
        exit(EXIT_FAILURE);
    }
    return ptr;
}
#endif