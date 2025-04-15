#ifndef __GRAMMAR_SET
#define __GRAMMAR_SET

#include <stdio.h>
#include <stdbool.h>

#include "global.h"
#include "linkedlist.h"

/**
 * Used to instance a new binary node, with all inner values init.
 * The value is the argument given, height default is 0, and its
 * pointers are NULL.
 */
BinaryNode *new_binary_node(GenericValue);

/**
 * Insert a node as children of a node in the binary tree.
 * If its needed go deep to the leaves.
 */
OperationResult binary_node_insert(BinaryNode*, ComparationFunction, GenericValue);

/**
 * Remove a binary node of its parent, if its needed resort the tree.
 */
OperationResult binary_node_remove(BinaryNode*, ComparationFunction, GenericValue);

/**
 * Find if in the children of a node its contained the value.
 */
GenericValue binary_node_search(BinaryNode*, ComparationFunction, GenericValue);

#ifdef DEV
/**
 * Retrieves the representation of node as a legible string data.
 */
char *binary_node_to_representation(BinaryNode, StringifyFunction);
#endif

/**
 * Retrieves the representation of node as a legible string data.
 */
char *binary_node_to_string(BinaryNode, StringifyFunction);

/**
 * Clear all fields of our binary node and its GenericValue data type if a
 * delete function different to NULL is given, then free used memory.
 */
void delete_binary_node(BinaryNode*, DeleteFunction);

/**
 * Instantiate a new set, with a comparation and delete function for
 * the GenericValue data types used into, if all are NULL, there are an error,
 * 'cause the inner data cannot be compared or deleted correctly (optional
 * but recommended).
 */
Set *new_set(ComparationFunction, DeleteFunction, StringifyFunction);

/**
 * Insert a new element into the set only if it isn´t in the set previusly.
 */
bool set_insert(Set*, GenericValue);

/**
 * Remove the data from the set if its contain it.
 */
bool set_remove(Set*, GenericValue);

/**
 * Find if in the children of a node its contained the value.
 */
GenericValue set_search(Set, GenericValue);

/**
 * Parse the set as a linked list.
 */
LinkedList *set_as_list(Set);

/**
 * Build a new set with all the data of first set and the elements of second
 * that are not contained in the first.
 */
Set *set_union(Set, Set);

/**
 * Gets all the elements of first set that are not contained into second.
 * The operation isn´t conmutable.
 * 
 * When the resultant set is void set so the two sets are the same.
 */
Set *set_difference(Set, Set);

bool set_disjoint(Set, Set);

#ifdef DEV
/**
 * Retrieves the representation of set as its representation.
 */
char *set_to_representation(Set);
#endif

/**
 * Retrieves the representation of set as a legible string data.
 * 
 * Example: { 'A', 'B', 'C', 'D' }
 */
char *set_to_string(Set);

/**
 * Clear all fields of our set and each of inner binary nodes used, then
 * free used memory.
 */
void delete_set(Set*);

#endif // __GRAMMAR_SET