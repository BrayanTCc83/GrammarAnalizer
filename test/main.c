#include <stdio.h>
#include <stdlib.h>
#include "../src/headers/global.h"
#include "../src/headers/linkedlist.h"
#include "../src/headers/set.h"
#include "../src/headers/hashtable.h"
#include "../src/headers/interpreter.h"

int main(int argc, char *argv[]) {
    if(argc < 1) {
        fprintf(stderr, "You must give almost one grammar file to test.");
        exit(EXIT_FAILURE);
    }

    for(int i = 1; i <argc; i++) {
        FILE *file = fopen(argv[i], "r");
        Scanner *scanner = new_scanner(file);
        Parser *parser = new_parser(scanner);
        Interpreter *interpreter = new_interpreter(parser);
        interpreter_start(interpreter);
    
        Grammar *grammar = interpreter->grammar;
    
        printf("No terminals: %s\n", set_to_string(*grammar->no_terminal));
        printf("Terminals: %s\n", set_to_string(*grammar->terminal));
        printf("Start: %s\n", set_to_string(*grammar->start));
        printf("Productions: %s\n", set_to_string(*grammar->productions));
        printf("Anulables: %s\n", set_to_string(*grammar->anulable));
        printf("First sets: \n");
        grammar_show_first_sets(*grammar);
        printf("Follow sets: \n");
        grammar_show_follow_sets(*grammar);
        printf("Selection sets: \n");
        grammar_show_selection_sets(*grammar);
        printf("The grammar is: %s\n",
            grammar->type == GRAMMAR_S ? "Grammar S" : grammar->type == GRAMMAR_Q ? "Grammar q" : grammar->type == GRAMMAR_LL_1 ? "Grammar LL(1)" : "Grammar not recognized"
        );

        //delete_interpreter(interpreter);
        //delete_grammar(grammar);
        //fclose(file);
    }

    return EXIT_SUCCESS;
}