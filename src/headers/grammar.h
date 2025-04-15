#ifndef __GRAMMAR_GRAMMAR_DEFINITIONS
#define __GRAMMAR_GRAMMAR_DEFINITIONS

#include <stdio.h>
#include <stdbool.h>
#include "global.h"
#include "linkedlist.h"
#include "hashtable.h"
#include "set.h"

typedef enum _symbol_type {
    ERROR = 0b000,
    TERMINAL = 0b001,
    NO_TERMINAL = 0b010,
    START = 0b100
} SymbolType;

typedef struct _symbol {
    SymbolType type;
    char *value;
    Set *first, *follow;
} Symbol;

typedef struct _production {
    Symbol *productor;
    LinkedList *product;
    Set *first, *select;
} Production;

typedef enum _grammar_type {
    UNSOPORTED_GRAMMAR,
    GRAMMAR_S,
    GRAMMAR_Q,
    GRAMMAR_LL_1
} GrammarType;

typedef struct _grammar {
    Set *no_terminal, *terminal, *start, *productions;
    Set *anulable;
    GrammarType type;
} Grammar;

Symbol *new_symbol(SymbolType, const char *);
int symbol_to_hash(GenericValue);
ComparationResult symbol_compare(GenericValue, GenericValue);
char *symbol_to_string(GenericValue);
void delete_symbol(GenericValue);

Production *new_production(Symbol*);
bool production_insert(Production*, Symbol*);
ComparationResult production_compare(GenericValue, GenericValue);
char *production_to_string(GenericValue);
void delete_production(GenericValue);

Grammar *new_grammar();
Symbol *grammar_add_no_terminal(Grammar*, Symbol*);
Symbol *grammar_add_terminal(Grammar*, Symbol*);
Symbol *grammar_set_start(Grammar*, Symbol*);
Production *grammar_add_production(Grammar*, Production*);
Symbol *grammar_search_no_terminal(Grammar, Symbol*);
Symbol *grammar_search_terminal(Grammar, Symbol*);
Symbol *grammar_add_anulable(Grammar*, Symbol*);
int grammar_production_count(Grammar);
bool grammar_has_start(Grammar);
void grammar_show_first_sets(Grammar);
void grammar_show_follow_sets(Grammar);
void grammar_show_selection_sets(Grammar);
void delete_grammar(Grammar*);

#endif // __GRAMMAR_GRAMMAR_DEFINITIONS