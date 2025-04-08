#ifndef __GRAMMAR_LINKED_LIST
#define __GRAMMAR_LINKED_LIST

#include <stdbool.h>
#include "global.h"

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

SimpleNode *new_simple_node(GenericValue);
void delete_simple_node(SimpleNode*, DeleteFunction);

LinkedList *new_linked_list(ComparationFunction, DeleteFunction, StringifyFunction);
bool linked_list_push(LinkedList*, GenericValue);
bool linked_list_shift(LinkedList*, GenericValue);
GenericValue linked_list_pop(LinkedList*);
GenericValue linked_list_unshift(LinkedList*);
char *linked_list_to_string(LinkedList);
bool linked_list_is_void(LinkedList);
void delete_linked_list(LinkedList*);

#ifdef DEV
char *simple_node_to_representation(SimpleNode, StringifyFunction);
char *linked_list_to_representation(LinkedList);
#endif

#endif // __GRAMMAR_LINKED_LIST