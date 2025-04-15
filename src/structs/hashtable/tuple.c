#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../../headers/global.h"
#include "../../headers/linkedlist.h"
#include "../../headers/hashtable.h"

HashTable *this = NULL;

Tuple *new_tuple(GenericValue key, GenericValue value) {
    instance(Tuple);

    o->key = key;
    o->value = value;

    return o;
}

ComparationResult tuple_compare(GenericValue t, GenericValue t2) {
    Tuple *tuple = (Tuple*) t, *tuple2 = t2;
    if(!this->keyCmpFn) {
        return EQUALS;
    }
    return this->keyCmpFn(tuple->key, tuple2->key);
}

char *tuple_to_string(GenericValue t) {
    Tuple *tuple = (Tuple*) t;
    char *string = to_string();
    int idx = 0;

    if(this->keyStrFn) {
        idx += sprintf(string, "(%s: ", this->keyStrFn(tuple->key));
    } else {
        idx += sprintf(string, "(%p: ", tuple->key);
    }

    if(this->valueStrFn) {
        idx += sprintf(string + idx, "%s)", this->valueStrFn(tuple->value));
    } else {
        idx += sprintf(string + idx, "%p)", tuple->value);
    }

    return string;
}

void delete_tuple(GenericValue t) {
    Tuple* tuple = (Tuple*) t;
    if(this->keyDelFn) {
        this->keyDelFn(tuple->key);
    }
    if(this->valueDelFn) {
        this->valueDelFn(tuple->value);
    }
    tuple->key = NULL;
    tuple->value = NULL;
    free(tuple);
}