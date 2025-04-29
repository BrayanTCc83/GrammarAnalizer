#include <stdbool.h>
#include "../../headers/global.h"
#include "../../headers/linkedlist.h"
#include "../../headers/set.h"

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
    #ifdef DEV
    LOG("Insert value %s on LinkedList '%p' at end.", list->stringifyFn(value) , list);
    #endif
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
    #ifdef DEV
    LOG("LinkedList '%p': %s.", list, linked_list_to_string(list));
    #endif

    return true;
}

bool linked_list_shift(LinkedList *list, GenericValue value) {
    #ifdef DEV
    LOG("Insert value %s on LinkedList '%p' at begin.", list->stringifyFn(value) , list);
    #endif
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
    #ifdef DEV
    LOG("LinkedList '%p': %s.", list, linked_list_to_string(list));
    #endif

    return true;
}

GenericValue linked_list_pop(LinkedList *list) {
    #ifdef DEV
    LOG("Remove last element from LinkedList '%p'.", list);
    #endif
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
    #ifdef DEV
    LOG("LinkedList '%p': %s.", list, linked_list_to_string(list));
    #endif

    return data;
}

GenericValue linked_list_unshift(LinkedList *list) {
    #ifdef DEV
    LOG("Remove first element from LinkedList '%p'.", list);
    #endif
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
    #ifdef DEV
    LOG("LinkedList '%p': %s.", list, linked_list_to_string(list));
    #endif

    return data;
}

LinkedList *linked_list_remove_all(LinkedList *list, GenericValue value) {
    #ifdef DEV
    LOG("Remove all coincidences of %s from LinkedList '%p'.", list->stringifyFn(value), list);
    #endif
    if(list->begin == NULL) {
        return NULL;
    }

    LinkedList *result = NULL;
    SimpleNode *ref = list->begin, *prev = NULL;
    while(ref) {
        prev = ref;
        ref = ref->next;

        if(list->compareFn(prev->data, value) != EQUALS) {
            continue;
        }

        if(result == NULL) {
            result = new_linked_list(list->compareFn, list->deleteFn, list->stringifyFn);
        }
        GenericValue r = prev->data;
        linked_list_push(result, r);
        if(prev == list->begin) {
            list->begin = ref;
            if(prev == list->end) {
                list->end = ref;
            }
            if(ref) {
                ref->previus = NULL;
            }
        } else if(prev == list->end) {
            list->end = prev->previus;
            if(list->end) {
                list->end->next = prev->next;
            }
        } else if(ref) {
            ref->previus = prev->previus;
            if(ref->previus) {
                ref->previus->next = ref;
            }
        }
        delete_simple_node(prev, NULL);
        list->size--;
    }
    #ifdef DEV
    LOG("LinkedList '%p': %s.", list, linked_list_to_string(list));
    #endif

    return result;
}

LinkedList *linked_list_get_all(LinkedList *list, GenericValue value) {
    #ifdef DEV
    LOG("Get all coincidences of %p from LinkedList '%p'.", value, list);
    #endif
    if(list->begin == NULL) {
        return NULL;
    }

    LinkedList *result = NULL;
    SimpleNode *ref = list->begin, *prev = NULL;
    while(ref) {
        prev = ref;
        ref = ref->next;

        if(list->compareFn(prev->data, value) != EQUALS) {
            continue;
        }

        if(result == NULL) {
            result = new_linked_list(list->compareFn, list->deleteFn, list->stringifyFn);
        }
        GenericValue r = prev->data;
        linked_list_push(result, r);
    }
    #ifdef DEV
    LOG("LinkedList '%p': %s.", list, linked_list_to_string(list));
    #endif

    return result;
}

Set *linked_list_as_set(LinkedList list) {
    Set *set = new_set(list.compareFn, list.deleteFn, list.stringifyFn);

    SimpleNode *ref = list.begin;
    while(ref) {
        set_insert(set, ref->data);
        ref = ref->next;
    }

    return set;
}

char *linked_list_to_string(GenericValue l) {
    LinkedList list = *(LinkedList*) l;
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

ComparationResult linked_list_compare(GenericValue l1, GenericValue l2) {
    LinkedList *list1 = (LinkedList*) l1, *list2 = (LinkedList*) l2;
    ComparationResult result = list1->size > list2->size ?
        BIGGER : list1->size < list2->size ? SMALLER : EQUALS;
    
    if(result != EQUALS) {
        return result;
    }

    SimpleNode *ref1 = list1->begin;
    SimpleNode *ref2 = list1->begin;
    while (ref1 && ref2) {
        result = list1->compareFn(ref1->data, ref2->data);
        if(result != EQUALS) break;

        ref1 = ref1->next;
        ref2 = ref2->next;
    }
    
    return result;
}

void delete_linked_list(GenericValue l) {
    LinkedList *list = (LinkedList*) l;
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