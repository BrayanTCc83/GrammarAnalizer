#ifndef __GRAMMAR_HASH_TABLE
#define __GRAMMAR_HASH_TABLE

#include "global.h"
#include "linkedlist.h"

typedef struct _tuple {
    GenericValue key;
    GenericValue value;
} Tuple;

typedef struct _hash_table {
    ComparationFunction valueCmpFn, keyCmpFn;
    StringifyFunction valueStrFn, keyStrFn;
    DeleteFunction valueDelFn, keyDelFn;
    HashingFunction hasingFn;
    LinkedList **lists;
    size_t size;
} HashTable;

Tuple *new_tuple(GenericValue, GenericValue);
ComparationResult tuple_compare(GenericValue, GenericValue);
char *tuple_to_string(GenericValue);
void delete_tuple(GenericValue);

HashTable *new_hash_table(size_t);
void hash_table_set_key_fn(HashTable*, ComparationFunction, DeleteFunction, StringifyFunction, HashingFunction);
void hash_table_set_value_fn(HashTable*, ComparationFunction, DeleteFunction, StringifyFunction);
bool hash_table_insert(HashTable*, GenericValue, GenericValue);
LinkedList *hash_table_remove(HashTable*, GenericValue);
LinkedList *hash_table_get(HashTable*, GenericValue);
char *hash_table_to_string(HashTable*);
void delete_hash_table(HashTable*);

#endif // __GRAMMAR_HASH_TABLE