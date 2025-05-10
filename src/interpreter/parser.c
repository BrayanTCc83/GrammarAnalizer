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

    return o;
}

static Production *parser_create_production(Parser *parser, Symbol *symbol) {
    Production *production = new_production(symbol);

    grammar_add_production(parser->grammar, production);

    return production;
}

// Set start and generates productions
static bool parser_generate_productions(Parser *parser) {
    #ifdef DEV
    LOG("Generating Productions.");
    #endif
    char *token = NULL, *last_token = NULL;
    Production *production = NULL;
    Symbol *symbol = NULL, *s = NULL;
    FILE *file = parser->scanner->file;

    while(feof(file) != EOF){
        last_token = token;
        token = scanner_read_token(parser->scanner);
        if(token == (void*)EOF) {
            break;
        }
        #ifdef DEV
        LOG("Read token '%s'.", token);
        #endif
        #ifdef DEV
        LOG("Current production '%p'.", production);
        #endif

        if(strcmp(token, "->") == 0) {
            #ifdef DEV
            LOG("Create new production for '%s'.", last_token);
            #endif
            symbol = new_symbol(ERROR, last_token);
            s = grammar_search_no_terminal(*parser->grammar, symbol);
            production = parser_create_production(parser, s);
        } else if(production != NULL && strlen(token) > 0) {
            #ifdef DEV
            LOG("Insert '%s' on production '%s' at Terminal.", token, production_to_string(production));
            #endif
            symbol = new_symbol(ERROR, token);
            s = grammar_search_no_terminal(*parser->grammar, symbol);
            if(!s) {
                s = grammar_search_terminal(*parser->grammar, symbol);
            }
            if(s) {
                production_insert(production, s);
            }
        }

        char c = getc(file);
        if(c == '\n') {
            production = NULL;
            continue;
        }

        if(c == EOF) {
            break;
        }
    }
    #ifdef DEV
    LOG("Grammar Productions: %s", set_to_string(*parser->grammar->productions));
    #endif
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
    #ifdef DEV
    LOG("Interpreter Rules: %s", hash_table_to_string(parser->rules));
    #endif
    return true;
}

bool parser_start(Parser *parser) {
    bool res = scanner_start(parser->scanner);

    parser->rules = new_hash_table(parser->grammar->no_terminal->size + 7);
    hash_table_set_key_fn(parser->rules, symbol_compare, delete_symbol, symbol_to_string, symbol_to_hash);
    hash_table_set_value_fn(parser->rules, production_compare, delete_production, production_to_string);
    #ifdef DEV
    LOG("Productions: %s", hash_table_to_string(parser->rules));
    #endif

    if(res) {
        return parser_generate_productions(parser) && parser_generate_rules(parser);
    }

    SimpleNode *ref = parser->scanner->errors->begin;
    while(ref) {
        fprintf(stderr, "[Scanner Error]: %s\n", symbol_to_string(ref->data));
        ref = ref->next;
    }
    return parser_generate_productions(parser);
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