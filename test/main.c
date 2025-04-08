#include <stdio.h>
#include <stdlib.h>
#include "../src/headers/global.h"
#include "../src/headers/set.h"

ComparationResult int_compare(GenericValue, GenericValue);
char *int_to_string(GenericValue);
int *new_int(int);
void delete_int(GenericValue);

ComparationResult char_compare(GenericValue, GenericValue);
char *char_to_string(GenericValue);
char *new_char(char);

int main(int argc, char *argv[]) {
    Set *set1 = new_set(int_compare, delete_int, int_to_string);
    Set *set2 = new_set(int_compare, delete_int, int_to_string);

    set_insert(set1, new_int(350));
    set_insert(set1, new_int(200));
    set_insert(set1, new_int(400));
    set_insert(set1, new_int(100));
    set_insert(set1, new_int(300));
    set_insert(set1, new_int(160));

    set_insert(set2, new_int(350));
    set_insert(set2, new_int(200));
    set_insert(set2, new_int(870));
    set_insert(set2, new_int(1500));
    set_insert(set2, new_int(80));
    set_insert(set2, new_int(70));
    set_insert(set2, new_int(45));

    Set *set3 = set_union(*set1, *set2);
    Set *set4 = set_difference(*set1, *set2);
    Set *set5 = set_difference(*set2, *set1);
    printf("Set1: %s with size %i.\n", set_to_string(*set1), set1->size);
    printf("Set2: %s with size %i.\n", set_to_string(*set2), set2->size);
    printf("Set1 U Set2: %s with size %i.\n", set_to_string(*set3), set3->size);
    printf("Set1 - Set2: %s with size %i.\n", set_to_string(*set4), set4->size);
    printf("Set2 - Set1: %s with size %i.\n", set_to_string(*set5), set5->size);

    printf("Looking for 1000, into set '%s'.\n", set_search(*set1, new_int(1000)) == true ? "EXISTS": "NOT EXISTS");
    printf("Set as list of integers: %s\n", linked_list_to_string(*set_as_list(*set1)));
    set_remove(set1, new_int(1000));
    printf("Set of integers: %s\n", set_to_string(*set1));
    printf("Looking for 1000, into set '%s'.\n", set_search(*set1, new_int(1000)) == true ? "EXISTS": "NOT EXISTS");

    delete_set(set1);
    delete_set(set2);
    //delete_set(set3);
    //delete_set(set4);
    //delete_set(set5);

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

char *new_char(char c) {
    instance(char);
    *o = c;
    return o;
}
