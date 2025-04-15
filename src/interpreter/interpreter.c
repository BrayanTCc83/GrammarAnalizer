#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "../headers/global.h"
#include "../headers/interpreter.h"

static Symbol *end_string = NULL;
extern HashTable *this;

Interpreter *new_interpreter(Parser *parser) {
    instance(Interpreter);

    o->parser = parser;
    o->grammar = parser->grammar;
    o->errors = new_linked_list(symbol_compare, delete_symbol, symbol_to_string);

    return o;
}

static void interpreter_find_anulables(Interpreter *interpreter) {
    LinkedList *no_terminals = set_as_list(*interpreter->grammar->no_terminal);
    LinkedList *missings = new_linked_list(symbol_compare, delete_symbol, symbol_to_string);
    HashTable *rules = interpreter->parser->rules;
    Symbol *anulable = NULL;

    // Get all individual no terminals anulables.
    SimpleNode *ref = no_terminals->begin;
    while(ref) {
        LinkedList *productions = hash_table_get(rules, ref->data);
        SimpleNode *prod = productions->begin;
        Tuple *tuple = (Tuple*) prod->data;
        anulable = NULL;

        while(prod) {
            Production *production = (Production*) tuple->value;
            LinkedList *list = production->product;
            if(list->size == 0) {
                anulable = grammar_add_anulable(interpreter->grammar, (Symbol*) tuple->key);
                break;
            }
            prod = prod->next;
        }
        if(!anulable){
            linked_list_push(missings, (Symbol*) tuple->key);
        }
        ref = ref->next;
    }
    // Get all whose generate a set of no terminals anulables.
    ref = missings->begin;
    while(ref) {
        LinkedList *productions = hash_table_get(rules, ref->data);
        SimpleNode *prod = productions->begin;
        while(prod) {
            Production *production = (Production*) ((Tuple*) prod->data)->value;
            Set *set = linked_list_as_set(*production->product);
            if(set_difference(*set, *interpreter->grammar->anulable)->size == 0) {
                grammar_add_anulable(interpreter->grammar, production->productor);
                break;
            }
            prod = prod->next;
        }
        ref = ref->next;
    }
}

Set *interpreter_get_production_first(Interpreter *interpreter, Production *production, HashTable *table) {
    #ifdef DEV
    LOG("Looking for production: %s %p %p\n", production_to_string(production), production, production->first);
    #endif
    if(production->first) {
        #ifdef DEV
        LOG("First set of production %s %p %s\n", production_to_string(production), production, set_to_string(*production->first));
        #endif
        return production->first;
    }

    Symbol *no_terminal = production->productor;
    production->first = new_set(symbol_compare, delete_symbol, symbol_to_string);

    SimpleNode *ref = production->product->begin;
    if(!ref) {
        #ifdef DEV
        LOG("First set of production %s %p %s\n", production_to_string(production), production, set_to_string(*production->first));
        #endif
        return production->first;
    }

    Symbol *symbol = ((Symbol*)ref->data);
    if(symbol->type & TERMINAL) {
        set_insert(production->first, symbol);
    } else {
        do {
            symbol = ((Symbol*)ref->data);
            if(symbol->type == TERMINAL) {
                set_insert(production->first, symbol);
                break;
            }
            Set *set = interpreter_get_no_terminal_first(interpreter, symbol, table);
            production->first = set_union(*production->first, *set);
            ref = ref->next;
        } while(ref && set_search(*interpreter->grammar->anulable, symbol));
    }

    
    #ifdef DEV
    LOG("First set of production %s %p %s\n", production_to_string(production), production, set_to_string(*production->first));
    #endif
    
    return production->first;
}

Set *interpreter_get_no_terminal_first(Interpreter *interpreter, Symbol *no_terminal, HashTable *table) {
    #ifdef DEV
    LOG("First set of non terminal %s %p %p\n", symbol_to_string(no_terminal), no_terminal, no_terminal->first);
    #endif
    if(no_terminal->first) {
        
        #ifdef DEV
        LOG("First set of non terminal %s %p %s\n", symbol_to_string(no_terminal), no_terminal, set_to_string(*no_terminal->first));
        #endif
        
        return no_terminal->first;
    }

    no_terminal->first = new_set(symbol_compare, delete_symbol, symbol_to_string);
    
    #ifdef DEV
    LOG("First set of non terminal %s %p %s\n", symbol_to_string(no_terminal), no_terminal, set_to_string(*no_terminal->first));
    #endif
    
    
    LinkedList *productions = hash_table_get(table, no_terminal);
    SimpleNode *ref = productions->begin;
    while(ref) {
        Tuple *tuple = (Tuple*) ref->data;
        Production *production = tuple->value;
        Set *set = interpreter_get_production_first(interpreter, production, table);
        no_terminal->first = set_union(*no_terminal->first, *set);
        ref = ref->next;
    }
    
    #ifdef DEV
    LOG("First set of non terminal %s %p %s\n", symbol_to_string(no_terminal), no_terminal, set_to_string(*no_terminal->first));
    #endif
    return no_terminal->first;
}

static void interpreter_get_firsts(Interpreter *interpreter) {
    LinkedList *productions = set_as_list(*interpreter->grammar->productions);

    SimpleNode *ref = productions->begin;
    while(ref) {
        interpreter_get_production_first(interpreter, (Production*)ref->data, interpreter->parser->rules);
        ref = ref->next;
    }
    
    LinkedList *no_terminals = set_as_list(*interpreter->grammar->no_terminal);
    ref = no_terminals->begin;
    while(ref) {
        interpreter_get_no_terminal_first(interpreter, (Symbol*)ref->data, interpreter->parser->rules);
        ref = ref->next;
    }
}

static Set *interpreter_get_no_terminal_follow(Interpreter *interpreter, Symbol *symbol) {
    if(symbol->follow) {
        return symbol->follow;
    }

    symbol->follow = new_set(symbol_compare, delete_symbol, symbol_to_string);
    LinkedList *productions = set_as_list(*interpreter->grammar->productions);
    SimpleNode *ref = productions->begin;
    while(ref) {
        Production *production = (Production*)ref->data;
        Symbol *productor = production->productor;
        LinkedList *product = production->product;
        SimpleNode *p = product->begin;
        while(p) {
            if(symbol_compare(p->data, symbol) == EQUALS) {
                break;
            }
            p = p->next;
        }

        if(p && p->next == NULL) {
            symbol->follow = set_union(
                *symbol->follow,
                *interpreter_get_no_terminal_follow(interpreter, productor)
            );

            if(productor->type | START) {
                set_insert(symbol->follow, end_string);
            }
        } else if(p && p->next) {
            Symbol *next = (Symbol*)(p->next->data);
            if(next->type & TERMINAL) {
                set_insert(symbol->follow, next);
            } else if(next->type & NO_TERMINAL) {
                symbol->follow = set_union(*symbol->follow, *next->first);
                if(set_search(*interpreter->grammar->anulable, next) != NULL) {
                    symbol->follow = set_union(
                        *symbol->follow,
                        *interpreter_get_no_terminal_follow(interpreter, next)
                    );
                }
            }
        }
        ref = ref->next;
    }

    return symbol->follow;
}

static void interpreter_get_follows(Interpreter *interpreter) {
    LinkedList *anulables = set_as_list(*interpreter->grammar->anulable);
    SimpleNode *ref = anulables->begin;
    while(ref) {
        Symbol *no_terminal = (Symbol*) ref->data;
        interpreter_get_no_terminal_follow(interpreter, no_terminal);
        ref = ref->next;
    }
    ref = anulables->begin;
    while(ref) {
        Symbol *no_terminal = (Symbol*) ref->data;
        interpreter_get_no_terminal_follow(interpreter, no_terminal);
        ref = ref->next;
    }
}

static void interpreter_get_selects(Interpreter *interpreter) {
    LinkedList *productions = set_as_list(*interpreter->grammar->productions);
    SimpleNode *ref = productions->begin;
    while(ref) {
        Production *production = (Production*) ref->data;
        if(set_search(*interpreter->grammar->anulable, production->productor)) {
            production->select = set_union(*production->first, *production->productor->follow);
        } else {
            production->select = production->first;
        }
        ref = ref->next;
    }
}

static bool interpreter_grammar_s_validation(Interpreter *interpreter) {
    LinkedList *productions = set_as_list(*interpreter->grammar->productions);
    SimpleNode *ref = productions->begin;
    while(ref) {
        Production *production = (Production*) ref->data;
        if(production->product->size == 0) {
            return false;
        }

        Symbol *symbol = (Symbol*) production->product->begin->data;
        if(symbol->type | NO_TERMINAL) {
            return false;
        }
        ref = ref->next;
    }

    return true;
}

static bool interpreter_grammar_disjoins_selection_sets(Interpreter *interpreter) {
    LinkedList *no_terminals = set_as_list(*interpreter->grammar->no_terminal);
    SimpleNode *ref = no_terminals->begin;
    while(ref) {
        LinkedList *prod = hash_table_get(interpreter->parser->rules, ref->data);
        Set *u = new_set(symbol_compare, delete_symbol, symbol_to_string);
        
        SimpleNode *p = prod->begin;
        while(p && p->next) {
            Tuple *tuple1 = (Tuple*) p->data, *tuple2 = (Tuple*) p->next->data;
            Set *set1 = ((Production*)tuple1->value)->select;
            u = set_union(*u, *set1);
            Set *set2 = ((Production*)tuple2->value)->select;

            if(!set_disjoint(*u, *set2)) {
                return false;
            }
            p = p->next;
        }
        ref = ref->next;
    }

    return true;
}

static bool interpreter_grammar_q_validation(Interpreter *interpreter) {
    LinkedList *productions = set_as_list(*interpreter->grammar->productions);
    SimpleNode *ref = productions->begin;
    while(ref) {
        Production *production = (Production*) ref->data;
        Symbol *symbol = (Symbol*) production->product->begin->data;

        if(symbol->type | NO_TERMINAL) {
            return false;
        }
        ref = ref->next;
    }

    return interpreter_grammar_disjoins_selection_sets(interpreter);
}

static bool interpreter_grammar_ll1_validation(Interpreter *interpreter) {
    return interpreter_grammar_disjoins_selection_sets(interpreter);
}

static void interpreter_grammar_clasificate(Interpreter *interpreter) {
    if(interpreter_grammar_s_validation(interpreter)) {
        interpreter->grammar->type = GRAMMAR_S;
    } else if(interpreter_grammar_q_validation(interpreter)) {
        interpreter->grammar->type = GRAMMAR_Q;
    } else if(interpreter_grammar_ll1_validation(interpreter)) {
        interpreter->grammar->type = GRAMMAR_LL_1;
    } else {
        interpreter->grammar->type = UNSOPORTED_GRAMMAR;
    }
}

bool interpreter_start(Interpreter *interpreter) {
    if(!parser_start(interpreter->parser)) {
        SimpleNode *ref = interpreter->parser->errors->begin;
        while(ref) {
            fprintf(stderr, "[Parser Error]: %s.", symbol_to_string(ref->data));
            ref = ref->next;
        }
        return false;
    }

    if(!end_string) {
        reserve(2, char);
        sprintf(a, "%c", END_STRING);
        end_string = new_symbol(TERMINAL, a);
    }

    interpreter_find_anulables(interpreter);
    interpreter_get_firsts(interpreter);
    interpreter_get_follows(interpreter);
    interpreter_get_selects(interpreter);
    interpreter_grammar_clasificate(interpreter);
}

void delete_interpreter(Interpreter *interpreter) {
    delete_linked_list(interpreter->errors);
    delete_parser(interpreter->parser);
    free(interpreter->errors);

    interpreter->errors = NULL;
    interpreter->grammar = NULL;
    interpreter->parser = NULL;

    free(interpreter);
}