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
 * Used to give the int hash of a generic value.
 */
typedef int (*HashingFunction)(GenericValue);

/**
 * Used to delete a GenericValue data type.
 */
typedef void (*DeleteFunction)(GenericValue);

/**
 * Used to define a global abstract instant macro, every object that we
 * create use this macro to instance it.
 */
#define instance(Class) \
    Class*o = (Class*) malloc(sizeof(Class)); \
    if(!o) { \
        printf("Error on reserve memory for struct '%s'.\n", #Class); \
        exit(EXIT_FAILURE); \
    }

/**
 * Used to define a global abstract instant macro, every object that we
 * create use this macro to instance it.
 */
#define reserve(n, Class) \
    Class*a = (Class*) calloc(n, sizeof(Class)); \
    if(!a) { \
        printf("Error on reserve memory for struct array '%s'.\n", #Class); \
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

typedef struct _simple_node SimpleNode;

struct _simple_node {
    GenericValue data;
    SimpleNode *previus, *next;
};

typedef struct _linked_list {
    SimpleNode *begin, *end;
    size_t size;
    ComparationFunction compareFn;
    DeleteFunction deleteFn;
    StringifyFunction stringifyFn;
} LinkedList;

/**
 * This is an struct that indicates a binary node as BinaryNode.
 * 
 * Contains two pointers to left and right nodes, also its stored data
 * (as literal string and byte set) and height.
 */
typedef struct _binary_node BinaryNode;

struct _binary_node {
    GenericValue data;
    int height;
    BinaryNode *left, *right;
};

/**
 * The representation of the set, used for grammar sets (First, Follow
 * and Selection Sets [SS]), its implemented as a Autobalanced Binary
 * Tree, with specification AVL.
 * 
 * Contains a root and a deep.
 */
typedef struct _set {
    BinaryNode *root;
    size_t deep, size;
    ComparationFunction compareFn;
    StringifyFunction stringifyFn;
    DeleteFunction deleteFn;
} Set;

#endif // __GRAMMAR_GLOBAL_MACROS