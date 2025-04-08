#ifndef __GRAMMAR_GLOBAL_MACROS
#define __GRAMMAR_GLOBAL_MACROS

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

/**
 * Used to set the possible result of a comparision between two elements.
 */
typedef enum _comparation_result {
    SMALLER = -1,
    EQUALS = 0,
    BIGGER = 1
} ComparationResult;

/**
 * Used to implement GenericValue data types for nodes, it require a function
 * to compare values.
 */
typedef void* GenericValue;

/**
 * Used to return two values of an operation, first the result of the operation,
 * then if was success or there any error.
 */
typedef struct _operation_result {
    GenericValue returned;
    bool success;
} OperationResult;

/**
 * Used to comparate two GenericValue data types.
 * 
 * @return
 * SMALLER when first is less than second.
 * 
 * EQUALS when two are the same.
 * 
 * BIGGER when first is great than second.
 */
typedef ComparationResult (*ComparationFunction)(GenericValue, GenericValue);

/**
 * Used to give the string representation of a generic value.
 */
typedef char *(*StringifyFunction)(GenericValue);

/**
 * Used to delete a GenericValue data type.
 */
typedef void (*DeleteFunction)(GenericValue);

/**
 * Used to define a global abstract instant macro, every object that we
 * create use this macro to instance it.
 */
#define instance(Class) \
    Class *o = (Class*) malloc(sizeof(Class)); \
    if(!o) { \
        printf("Error on reserve memory for struct '%s'.\n", #Class); \
        exit(EXIT_FAILURE); \
    }

char *to_string();

#ifdef DEV
char *to_representation();
#define LOG(format, ...) {\
    char *message = (char*) malloc( 500 ); \
    sprintf(message, format __VA_OPT__(,) __VA_ARGS__); \
    printf("[DEV]: %s\n", message); \
}
#endif

#endif // __GRAMMAR_GLOBAL_MACROS