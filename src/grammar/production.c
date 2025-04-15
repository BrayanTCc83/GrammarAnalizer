#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "../headers/global.h"
#include "../headers/grammar.h"

Production *new_production(Symbol *productor) {
    instance(Production);

    o->productor = productor;
    o->product = new_linked_list(symbol_compare, delete_symbol, symbol_to_string);
    o->first = NULL;
    o->select = NULL;

    return o;
}

bool production_insert(Production *production, Symbol *symbol) {
    return linked_list_push(production->product, symbol);
}

ComparationResult production_compare(GenericValue p1, GenericValue p2) {
    Production *production1 = (Production*) p1, *production2 = (Production*) p2;
    ComparationResult result = symbol_compare(production1->productor, production2->productor);
    if(result != EQUALS) {
        return result;
    }
    
    return linked_list_compare(production1->product, production2->product);
}

char *production_to_string(GenericValue p) {
    Production *production = (Production*) p;
    char *string = to_string();

    int idx = sprintf(string, "%s -> ", symbol_to_string(production->productor));
    SimpleNode *ref = production->product->begin;
    while (ref) {
        idx += sprintf(string + idx, "%s ", symbol_to_string(ref->data));
        ref = ref->next;
    }

    return string;
}

void delete_production(GenericValue p) {
    Production *production = (Production*) p;

    delete_symbol(production->product);
    delete_linked_list(production->product);

    production->product = NULL;
    production->productor = NULL;
    
    free(p);
}