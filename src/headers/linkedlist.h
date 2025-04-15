#ifndef __GRAMMAR_LINKED_LIST
#define __GRAMMAR_LINKED_LIST

#include <stdbool.h>
#include "global.h"

SimpleNode *new_simple_node(GenericValue);
void delete_simple_node(SimpleNode*, DeleteFunction);

LinkedList *new_linked_list(ComparationFunction, DeleteFunction, StringifyFunction);
bool linked_list_push(LinkedList*, GenericValue);
bool linked_list_shift(LinkedList*, GenericValue);
GenericValue linked_list_pop(LinkedList*);
GenericValue linked_list_unshift(LinkedList*);
LinkedList *linked_list_remove_all(LinkedList*, GenericValue);
LinkedList *linked_list_get_all(LinkedList*, GenericValue);
ComparationResult linked_list_compare(GenericValue, GenericValue);
Set *linked_list_as_set(LinkedList);
char *linked_list_to_string(GenericValue);
bool linked_list_is_void(LinkedList);
void delete_linked_list(GenericValue);

#ifdef DEV
char *simple_node_to_representation(SimpleNode, StringifyFunction);
char *linked_list_to_representation(LinkedList);
#endif

#endif // __GRAMMAR_LINKED_LIST