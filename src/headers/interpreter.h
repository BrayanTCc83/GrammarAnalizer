#ifndef __GRAMMAR_INTERPRETER
#define __GRAMMAR_INTERPRETER

#include <stdio.h>
#include <stdbool.h>
#include "global.h"
#include "linkedlist.h"
#include "hashtable.h"
#include "set.h"
#include "grammar.h"

#define END_STRING 170

// Debe reconocer símbolos ASCII incluidos del 33 al 126
/*
    NO Terminal (N): se encuentra a la izquierda de al menos una producción y siempre esta
    conformado de letras iniciando con mayúscula, puede contener minúscula, guión bajo
    o apostrofo. A, AB, START, Start, A', A_B

    Terminal (T): puede ser cualquier símbolo que no sea un no terminal.

    Inicial (S): es el primer no terminal en aparecer en las reglas gramaticales.

    Producciones (P): son todas las producciones en un conjunto que pertenezcan a la gramática.
*/
typedef struct _scanner {
    FILE *file;
    Grammar *grammar;
    LinkedList *errors;
} Scanner;

typedef struct _parser {
    Scanner *scanner;
    Grammar *grammar;
    HashTable *rules;
    LinkedList *errors;
} Parser;

typedef struct _interpreter {
    Parser *parser;
    Grammar *grammar;
    LinkedList *errors;
} Interpreter;

Scanner *new_scanner(FILE*);
char *scanner_read_token(Scanner*);
bool scanner_start(Scanner*);
void delete_scanner(Scanner*);

Parser *new_parser(Scanner*);
bool parser_start(Parser*);
void delete_parser(Parser*);

Interpreter *new_interpreter(Parser*);
bool interpreter_start(Interpreter*);
Set *interpreter_get_production_first(Interpreter*, Production*, HashTable*);
Set *interpreter_get_no_terminal_first(Interpreter*, Symbol*, HashTable*);
void delete_interpreter(Interpreter*);

#endif // __GRAMMAR_INTERPRETER