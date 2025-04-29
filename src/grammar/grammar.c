#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../headers/global.h"
#include "../headers/grammar.h"

Grammar *new_grammar() {
    instance(Grammar);

    o->no_terminal = new_set(symbol_compare, delete_symbol, symbol_to_string);
    o->terminal = new_set(symbol_compare, delete_symbol, symbol_to_string);
    o->start = new_set(symbol_compare, delete_symbol, symbol_to_string);
    o->productions = new_set(production_compare, delete_production, production_to_string);
    o->type = UNSOPORTED_GRAMMAR;
    o->anulable = NULL;

    return o;
}

Symbol *grammar_add_no_terminal(Grammar *grammar, Symbol *symbol) {
    if(set_insert(grammar->no_terminal, symbol)) {
        return symbol;
    }

    Symbol *result = set_search(*grammar->no_terminal, symbol);
    delete_symbol(symbol);
    return result;
}

Symbol *grammar_add_terminal(Grammar *grammar, Symbol *symbol) {
    if(set_insert(grammar->terminal, symbol)) {
        return symbol;
    }

    Symbol *result = set_search(*grammar->terminal, symbol);
    delete_symbol(symbol);
    return result;
}

Symbol *grammar_set_start(Grammar *grammar, Symbol *symbol) {
    if(set_insert(grammar->start, symbol)) {
        return symbol;
    }
    return (Symbol*) set_search(*grammar->start, symbol);
}

Production *grammar_add_production(Grammar *grammar, Production *production) {
    if(set_insert(grammar->productions, production)) {
        return production;
    }
    return (Production*) set_search(*grammar->productions, production);
}

Symbol *grammar_search_no_terminal(Grammar grammar, Symbol *symbol) {
    return (Symbol*) set_search(*grammar.no_terminal, symbol);
}

Symbol *grammar_search_terminal(Grammar grammar, Symbol *symbol) {
    return (Symbol*) set_search(*grammar.terminal, symbol);
}

Symbol *grammar_add_anulable(Grammar *grammar, Symbol *symbol) {
    if(!grammar->anulable) {
        grammar->anulable = new_set(symbol_compare, delete_symbol, symbol_to_string);
    }
    if(set_insert(grammar->anulable, symbol)) {
        return symbol;
    }
    return NULL;
}

int grammar_production_count(Grammar grammar) {
    return grammar.productions->size;
}

bool grammar_has_start(Grammar grammar) {
    return grammar.start->size == 1;
}

void grammar_show_first_sets(Grammar grammar) {
    SimpleNode *ref = set_as_list(*grammar.productions)->begin;
    printf(" - Productions First Set:\n");
    while(ref) {
        if(!((Production*)ref->data)->first) {
            ref = ref->next;
            continue;
        }
        printf("%50s: %s\n", production_to_string(ref->data), set_to_string(*((Production*)ref->data)->first));
        ref = ref->next;
    }

    printf(" - Non terminals First Set:\n");
    ref = set_as_list(*grammar.no_terminal)->begin;
    while(ref) {
        if(!((Symbol*)ref->data)->first) {
            ref = ref->next;
            continue;
        }
        printf("%50s: %s\n", symbol_to_string(ref->data), set_to_string(*((Symbol*)ref->data)->first));
        ref = ref->next;
    }
}

void grammar_show_follow_sets(Grammar grammar) {
    SimpleNode *ref = set_as_list(*grammar.no_terminal)->begin;
    while(ref) {
        if(!((Symbol*)ref->data)->follow) {
            ref = ref->next;
            continue;
        }
        printf("%50s: %s\n", symbol_to_string(ref->data), set_to_string(*((Symbol*)ref->data)->follow));
        ref = ref->next;
    }
}

void grammar_show_selection_sets(Grammar grammar) {
    SimpleNode *ref = set_as_list(*grammar.productions)->begin;
    while(ref) {
        if(!((Production*)ref->data)->select) {
            ref = ref->next;
            continue;
        }
        printf("%50s: %s\n", production_to_string(ref->data), set_to_string(*((Production*)ref->data)->select));
        ref = ref->next;
    }
}

void delete_grammar(Grammar *grammar) {
    delete_set(grammar->no_terminal);
    delete_set(grammar->terminal);
    delete_set(grammar->start);
    delete_set(grammar->productions);

    grammar->no_terminal = NULL;
    grammar->terminal = NULL;
    grammar->start = NULL;
    grammar->productions = NULL;
    grammar->type = UNSOPORTED_GRAMMAR;

    free(grammar);
}