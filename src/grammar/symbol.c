#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../headers/global.h"
#include "../headers/grammar.h"

static const size_t p = 31;
static const size_t m = 1e9 + 9;

Symbol *new_symbol(SymbolType type, const char *value) {
    instance(Symbol);
    reserve(strlen(value) + 1, char);

    o->type = type;
    o->value = a;
    o->first = NULL;
    o->follow = NULL;
    strcpy(o->value, value);

    return o;
}

int symbol_to_hash(GenericValue s) {
    #ifdef DEV
    LOG("Hashing Symbol '%s'.", symbol_to_string(s));
    #endif
    Symbol *symbol = (Symbol*) s;
    size_t len = strlen(symbol->value);
    int hash = 0, p_pow = 1;

    for(int i = 0; i < len; i++) {
        hash = ( hash + symbol->value[i] - 32 ) % m;
        p_pow = (p_pow * p) % m;
    }

    #ifdef DEV
    LOG("Hash generated %d.", hash);
    #endif
    return hash;
}

ComparationResult symbol_compare(GenericValue s1, GenericValue s2) {
    Symbol *symbol1 = (Symbol*) s1, *symbol2 = (Symbol*) s2;
    return strcmp(symbol1->value, symbol2->value);
}

char *symbol_to_string(GenericValue s) {
    Symbol *symbol = (Symbol*) s;
    char *string = to_string();
    sprintf(string, "%s", symbol->value);
    return string;
}

void delete_symbol(GenericValue s) {
    Symbol *symbol = (Symbol*) s;

    memset(symbol->value, '\0', strlen(symbol->value));
    free(symbol->value);

    symbol->type = 0;
    symbol->value = NULL;

    free(s);
}