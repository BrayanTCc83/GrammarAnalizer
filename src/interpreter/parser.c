#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "../headers/global.h"
#include "../headers/interpreter.h"

Parser *new_parser(Scanner *scanner) {
    instance(Parser);

    o->scanner = scanner;
    o->grammar = o->scanner->grammar;
    o->errors = new_linked_list(symbol_compare, delete_symbol, symbol_to_string);
    o->rules = new_hash_table(o->grammar->no_terminal->size + 3);
    hash_table_set_key_fn(o->rules, symbol_compare, delete_symbol, symbol_to_string, symbol_to_hash);
    hash_table_set_value_fn(o->rules, production_compare, delete_production, production_to_string);

    return o;
}

static Production *parser_create_production(Parser *parser, Symbol *symbol) {
    Production *production = new_production(symbol);

    grammar_add_production(parser->grammar, production);

    return production;
}

// Set start and generates productions
static bool parser_generate_productions(Parser *parser) {
    char *token = NULL;
    Production *production = NULL;
    Symbol *symbol = NULL;
    bool is_new_production = true;
    FILE *file = parser->scanner->file;

    while(feof(file) != EOF){
        token = scanner_read_token(parser->scanner);
        if(token == (void*)EOF) {
            break;
        }
    
        symbol = new_symbol(ERROR, token);
        if(is_new_production) {
            getc(file);
            scanner_read_token(parser->scanner);

            Symbol *s = grammar_search_no_terminal(*parser->grammar, symbol);
            production = parser_create_production(parser, s);
        } else {
            Symbol *s = grammar_search_no_terminal(*parser->grammar, symbol);
            if(!s) {
                s = grammar_search_terminal(*parser->grammar, symbol);
            }
            production_insert(production, s);
        }

        if(getc(file) == '\n') {
            is_new_production = true;
        } else {
            is_new_production = false;
        }
    }
    return true;
}

static bool parser_generate_rules(Parser *parser) {
    LinkedList *list = set_as_list(*parser->grammar->productions);
    SimpleNode *ref = list->begin;
    while(ref) {
        Production *production = (Production*) ref->data;
        if(!hash_table_insert(parser->rules, production->productor, production)) {
            return false;
        }
        ref = ref->next;
    }

    return true;
}

bool parser_start(Parser *parser) {
    if(scanner_start(parser->scanner)) {
        return parser_generate_productions(parser) && parser_generate_rules(parser);
    }

    SimpleNode *ref = parser->scanner->errors->begin;
    while(ref) {
        fprintf(stderr, "[Scanner Error]: %s\n", symbol_to_string(ref->data));
        ref = ref->next;
    }
    return false;
}

void delete_parser(Parser *parser) {
    delete_scanner(parser->scanner);
    delete_linked_list(parser->errors);
    delete_hash_table(parser->rules);
    free(parser->errors);

    parser->grammar = NULL;
    parser->rules = NULL;
    parser->errors = NULL;

    free(parser);
}