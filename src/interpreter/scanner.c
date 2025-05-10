#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "../headers/global.h"
#include "../headers/interpreter.h"

Scanner *new_scanner(FILE *file) {
    instance(Scanner);

    o->grammar = new_grammar();
    o->errors = new_linked_list(symbol_compare, delete_symbol, symbol_to_string);
    o->file = file;

    return o;
}

char *scanner_read_token(Scanner *scanner) {
    fpos_t pos;
    fgetpos(scanner->file, &pos);
    char *string = malloc( 40 );
    int idx = 0;

    char c;
    while((c = fgetc(scanner->file)) != ' ' && c != '\n' && c != '\t' && c != EOF) {
        idx += sprintf(string + idx, "%c", c);
    }

    if(c == '\n' || c == ' ' || c == '\t') {
        fseek(scanner->file, -1, SEEK_CUR);
    }

    if(c == EOF) {
        fseek(scanner->file, 0, SEEK_END);
    }
    return string;
}

static bool is_valid_no_terminal(const char *token) {
    size_t len = strlen(token);
    char c = token[0];

    if(!(c >= 'A' && c <= 'Z')) {
        return false;
    }

    for(int i = 1; i < len; i++) {
        c = token[i];
        if(
            !(c >= 'a' && c <= 'z')
            && !(c >= 'A' && c <= 'Z')
            && !(c >= '0' && c <= '9')
            && !(c == '_' || c == '\'')
        ) {
            return false;
        }
    }

    return true;
}

static char *error_no_terminal(const char *token) {
    char* string = to_string();
    sprintf(string, "The no terminal %s is wrong, should start with upper case and contains only upper case, lower case, numbers, apostrophe or underscore.", token);
    return string;
}

static bool is_valid_terminal(const char *token) {
    size_t len = strlen(token);
    char c = 0;
    
    for(int i = 0; i < len; i++) {
        c = token[i];
        if(!(c >= 33 && c <= 126)) {
            return false;
        }
    }

    return true;
}

static char *error_terminal(const char *token) {
    char* string = to_string();
    sprintf(string, "The terminal %s is wrong, should be a valid standart ASCII character between 33 and 126.", token);
    return string;
}

static void scanner_set_start(Scanner *scanner, Symbol *symbol) {
    if(grammar_has_start(*scanner->grammar)) {
        return;
    }
    #ifdef DEV
    LOG("Setting start at No Terminal '%s'.", symbol_to_string(symbol));
    #endif
    
    symbol->type |= START;
    grammar_set_start(scanner->grammar, symbol);
}

static Symbol *scanner_insert_no_terminal(Scanner *scanner, const char *token) {
    if(!is_valid_no_terminal(token)) {
        linked_list_push(scanner->errors, new_symbol(ERROR, error_no_terminal(token)));
        return NULL;
    }
    Symbol *s = new_symbol(NO_TERMINAL, token);
    scanner_set_start(scanner, s);
    return grammar_add_no_terminal(scanner->grammar, s);
}

static Symbol *scanner_insert_terminal(Scanner *scanner, const char *token) {
    if(!is_valid_terminal(token)) {
        linked_list_push(scanner->errors, new_symbol(ERROR, error_terminal(token)));
        return NULL;
    }
    Symbol *s = new_symbol(TERMINAL, token);
    return grammar_add_terminal(scanner->grammar, s);
}

static Symbol *scanner_search_no_terminal(Scanner scanner, const char *token) {
    Symbol *s = new_symbol(NO_TERMINAL, token);
    #ifdef DEV
    LOG("Looking for symbol '%s' in No Terminal.", symbol_to_string(s));
    #endif
    return (Symbol*) grammar_search_no_terminal(*scanner.grammar, s);
}

static Symbol *scanner_search_terminal(Scanner scanner, const char *token) {
    Symbol *s = new_symbol(TERMINAL, token);
    #ifdef DEV
    LOG("Looking for symbol '%s' in Terminal.", symbol_to_string(s));
    #endif
    return (Symbol*) grammar_search_terminal(*scanner.grammar, s);
}

// First round: Identifies Terminals, non terminals and Start
static bool scanner_identify_tokens(Scanner *scanner) {
    char *token = NULL, *last_token = NULL;
    bool res = true;
    while(feof(scanner->file) != EOF){
        last_token = token;
        token = scanner_read_token(scanner);
        if(token == (void*)EOF) {
            break;
        }
        #ifdef DEV
        LOG("Read token '%s'.", token);
        #endif

        if(strcmp(token, "->") == 0) {
            #ifdef DEV
            LOG("Insert '%s' at No Terminal.", last_token);
            #endif
            res = scanner_insert_no_terminal(scanner, last_token) != NULL && res;
        } else {
            #ifdef DEV
            LOG("Insert '%s' at Terminal.", token);
            #endif
            res = scanner_insert_terminal(scanner, token) != NULL && res;
        }

        if(getc(scanner->file) == EOF) {
            break;
        }
    }
    scanner->grammar->terminal = set_difference(*scanner->grammar->terminal, *scanner->grammar->no_terminal);
    fseek(scanner->file, 0, SEEK_SET);
    #ifdef DEV
    LOG("Grammar Non Terminals: %s", set_to_string(*scanner->grammar->no_terminal));
    #endif
    #ifdef DEV
    LOG("Grammar Terminals: %s", set_to_string(*scanner->grammar->terminal));
    #endif
    return res;
}

bool scanner_start(Scanner *scanner) {
    return scanner_identify_tokens(scanner);
}

void delete_scanner(Scanner *scanner) {
    delete_linked_list(scanner->errors);
    free(scanner->errors);
    
    scanner->file = NULL;
    scanner->grammar = NULL;
    scanner->errors = NULL;

    free(scanner);
}