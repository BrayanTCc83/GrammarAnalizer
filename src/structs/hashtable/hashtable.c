#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../../headers/global.h"
#include "../../headers/linkedlist.h"
#include "../../headers/hashtable.h"

extern HashTable *this;

HashTable *new_hash_table(size_t size) {
    instance(HashTable);
    reserve(size, LinkedList *);

    o->lists = a;
    o->hasingFn = NULL;
    o->keyCmpFn = o->valueCmpFn = NULL;
    o->keyDelFn = o->valueDelFn = NULL;
    o->keyStrFn = o->valueStrFn = NULL;
    o->size = size;

    return o;
}

void hash_table_set_key_fn(HashTable *table, ComparationFunction keyCmpFn, DeleteFunction keyDelFn, StringifyFunction keyStrFn, HashingFunction hashingFn) {
    table->hasingFn = hashingFn;
    table->keyCmpFn = keyCmpFn;
    table->keyDelFn = keyDelFn;
    table->keyStrFn = keyStrFn;
}

void hash_table_set_value_fn(HashTable *table, ComparationFunction valueCmpFn, DeleteFunction valueDelFn, StringifyFunction valueStrFn) {
    table->valueCmpFn = valueCmpFn;
    table->valueDelFn = valueDelFn;
    table->valueStrFn = valueStrFn;
}

bool hash_table_insert(HashTable *table, GenericValue key, GenericValue value) {
    #ifdef DEV
    LOG("Insert value ('%s': %s) on HashTable '%p'.", table->keyStrFn(key), table->valueStrFn(value), table);
    #endif
    if(!table->hasingFn) {
        printf("Hash table has not a hashing function.");
        return false;
    }

    this = table;

    int hash = table->hasingFn(key) % table->size;
    LinkedList *list = table->lists[hash];
    this = table;
    if(!list) {
        list = new_linked_list(tuple_compare, delete_tuple, tuple_to_string);
        table->lists[hash] = list;
    }

    bool res = linked_list_push(list, new_tuple(key, value));
    #ifdef DEV
    LOG("HashTable '%p': %s.", table, hash_table_to_string(table));
    this = table;
    #endif
    this = NULL;
    
    return res;
}

LinkedList *hash_table_remove(HashTable *table, GenericValue key) {
    if(!table->hasingFn) {
        printf("Hash table has not a hashing function.");
        return false;
    }
    
    this = table;
    int hash = table->hasingFn(key) % table->size;
    LinkedList *list = table->lists[hash];
    LinkedList *res = list ? linked_list_remove_all(list, new_tuple(key, NULL)) : NULL;
    this = NULL;

    return res;
}

LinkedList *hash_table_get(HashTable *table, GenericValue key) {
    if(!table->hasingFn) {
        printf("Hash table has not a hashing function.");
        return false;
    }
    
    this = table;
    int hash = table->hasingFn(key) % table->size;
    LinkedList *list = table->lists[hash];
    LinkedList *res = list ? linked_list_get_all(list, new_tuple(key, NULL)) : NULL;
    this = NULL;

    return res;
}

char *hash_table_to_string(HashTable *table) {
    char *string = to_string();
    int idx = 0;
    this = table;

    idx += sprintf(string, "{");
    for(int i = 0; i < table->size; i++) {
        LinkedList *list = table->lists[i];
        if(!list) {
            continue;
        }

        SimpleNode *ref = list->begin;
        while(ref && ref->next != NULL) {
            idx += sprintf(string + idx, " %s,", tuple_to_string(ref->data));
            ref = ref->next;
        }
        if(ref) {
            idx += sprintf(string + idx, " %s ", tuple_to_string(ref->data));
        }
    }
    idx += sprintf(string + idx, "}");

    this = NULL;
    return string;
}

void delete_hash_table(HashTable *table) {
    this = table;

    for(int i = 0; i < table->size; i++) {
        LinkedList *list = table->lists[i];
        if(!list) {
            continue;
        }
        delete_linked_list(list);
        table->lists[i] = NULL;
    }

    this = NULL;
    table->size = 0;
    table->hasingFn = NULL;
    table->keyCmpFn = table->valueCmpFn = NULL;
    table->keyDelFn = table->valueDelFn = NULL;
    table->keyStrFn = table->valueStrFn = NULL;

    free(table->lists);
    table->lists = NULL;
    free(table);
}