#include <stdbool.h>
#include "../../headers/global.h"
#include "../../headers/linkedlist.h"

LinkedList *new_linked_list(ComparationFunction compareFn, DeleteFunction deleteFn, StringifyFunction stringifyFn) {
    if(!compareFn) {
        printf("Compare function is required to evaluate set content.");
        exit(EXIT_FAILURE);
    }

    if(!stringifyFn) {
        printf("Stringify function is required to can view the representation of set elements.");
        exit(EXIT_FAILURE);
    }

    instance(LinkedList);

    o->begin = o->end = NULL;
    o->compareFn = compareFn;
    o->deleteFn = deleteFn;
    o->stringifyFn = stringifyFn;
    o->size = 0;

    return o;
}

bool linked_list_push(LinkedList *list, GenericValue value) {
    SimpleNode *node = new_simple_node(value);
    if(!list->begin) {
        list->begin = list->end = node;
        list->size++;
        return true;
    }

    list->end->next = node;
    node->previus = list->end;
    list->end = node;
    list->size++;

    return true;
}

bool linked_list_shift(LinkedList *list, GenericValue value) {
    SimpleNode *node = new_simple_node(value);
    if(!list->begin) {
        list->begin = list->end = node;
        list->size++;
        return true;
    }

    list->begin->previus = node;
    node->next = list->begin;
    list->begin = node;
    list->size++;

    return true;
}

GenericValue linked_list_pop(LinkedList *list) {
    if(list->begin == NULL) {
        return NULL;
    }

    SimpleNode *ref = list->end->previus, *to_remove = list->end;
    list->end = ref;
    if(list->end) {
        list->end->next = NULL;
    }

    GenericValue *data = to_remove->data;
    delete_simple_node(to_remove, NULL);
    list->size--;

    return data;
}

GenericValue linked_list_unshift(LinkedList *list) {
    if(list->begin == NULL) {
        return NULL;
    }

    SimpleNode *ref = list->begin->next, *to_remove = list->begin;
    list->begin = ref;
    if(list->begin) {
        list->begin->previus = NULL;   
    }

    GenericValue *data = to_remove->data;
    delete_simple_node(to_remove, NULL);
    list->size--;

    return data;
}

char *linked_list_to_string(LinkedList list) {
    char *string = to_string();
    int idx = 0;
    int i = 0;
    SimpleNode *aux = list.begin;

    idx += sprintf(string, "[");
    while(aux && aux->next != NULL) {
        idx += sprintf(string + idx, " %s,", list.stringifyFn(aux->data));
        aux = aux->next;
    }

    if(aux) {
        idx += sprintf(string + idx, " %s ", list.stringifyFn(aux->data));
    }

    idx += sprintf(string + idx, "]");

    return string;
}

bool linked_list_is_void(LinkedList list) {
    return list.size == 0;
}

void delete_linked_list(LinkedList *list) {
    SimpleNode *aux = list->begin, *prev;
    while(aux != NULL) {
        prev = aux;
        aux = aux->next;
        delete_simple_node(prev, list->deleteFn);
    }
    list->size = 0;
    list->begin = list->end = NULL;
    list->compareFn = NULL;
    list->deleteFn = NULL;
    list->stringifyFn = NULL;
    free(list);
}

#ifdef DEV
char *linked_list_to_representation(LinkedList list) {
    char *representation = to_representation();
    int idx = 0;
    int i = 0;
    SimpleNode *aux = list.begin;
    while(aux != NULL) {
        idx += sprintf(representation + idx, "[%i]: %p %s\n", i++, aux, simple_node_to_representation(*aux, list.stringifyFn));
        aux = aux->next;
    }
    return representation;
}
#endif