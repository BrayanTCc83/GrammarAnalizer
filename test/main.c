#include <stdio.h>
#include <stdlib.h>
#include "../src/headers/global.h"
#include "../src/headers/linkedlist.h"
#include "../src/headers/set.h"
#include "../src/headers/hashtable.h"

ComparationResult int_compare(GenericValue, GenericValue);
char *int_to_string(GenericValue);
int *new_int(int);
void delete_int(GenericValue);

ComparationResult char_compare(GenericValue, GenericValue);
int char_to_hash(GenericValue);
char *char_to_string(GenericValue);
char *new_char(char);

int main(int argc, char *argv[]) {
    HashTable *table = new_hash_table(30);
    hash_table_set_key_fn(table, char_compare, NULL, char_to_string, char_to_hash);
    hash_table_set_value_fn(table, int_compare, delete_int, int_to_string);

    hash_table_insert(table, new_char('A'), new_int(10));
    hash_table_insert(table, new_char('A'), new_int(15));
    hash_table_insert(table, new_char('B'), new_int(10));
    hash_table_insert(table, new_char('C'), new_int(10));

    return EXIT_SUCCESS;
}

int *new_int(int n) {
    instance(int);
    *o = n;
    return o;
}

void delete_int(GenericValue n) {
    *(int*)n = 0;
    free((int*)n);
}

char *int_to_string(GenericValue n) {
    char *string = to_string();
    sprintf(string, "%i", *((int*)n));
    return string;
}

ComparationResult int_compare(GenericValue n1, GenericValue n2) {
    int N1 = *(int*)n1, N2 = *(int*)n2;
    ComparationResult result = N1 < N2 ? SMALLER : N1 > N2 ? BIGGER : EQUALS;
    #ifdef DEV
    LOG("Comparing values %i and %i, result %i.", N1, N2, result);
    #endif
    return result;
}

ComparationResult char_compare(GenericValue c1, GenericValue c2) {
    char C1 = *(char*)c1, C2 = *(char*)c2;
    ComparationResult result = C1 < C2 ? SMALLER : C1 > C2 ? BIGGER : EQUALS;
    #ifdef DEV
    LOG("Comparing values %c and %c, result %i.", C1, C2, result);
    #endif
    return result;
}

char *char_to_string(GenericValue c) {
    char *string = to_string();
    sprintf(string, "%c", *((char*)c));
    return string;
}

int char_to_hash(GenericValue c) {
    return *((char*)c);
}

char *new_char(char c) {
    instance(char);
    *o = c;
    return o;
}
