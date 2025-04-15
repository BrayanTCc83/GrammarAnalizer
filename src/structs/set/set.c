#include "../../headers/set.h"
#include "../../headers/global.h"

Set *new_set(ComparationFunction compareFn, DeleteFunction deleteFn, StringifyFunction stringifyFn) {
    if(!compareFn) {
        printf("Compare function is required to evaluate set content.");
        exit(EXIT_FAILURE);
    }

    if(!stringifyFn) {
        printf("Stringify function is required to can view the representation of set elements.");
        exit(EXIT_FAILURE);
    }

    instance(Set);

    o->compareFn = compareFn;
    o->deleteFn = deleteFn;
    o->stringifyFn = stringifyFn;
    o->deep = o->size = 0;
    o->root = NULL;

    return o;
}

bool set_insert(Set *set, GenericValue value) {
    #ifdef DEV
    LOG("Insert value %s on Set '%p'.", set->stringifyFn(value) , set);
    #endif
    OperationResult result = binary_node_insert(set->root, set->compareFn, value);
    set->root = (BinaryNode*) result.returned;
    set->deep = set->root->height;
    if(result.success) {
        set->size++;
    }
    #ifdef DEV
    printf("%s\n", set_to_representation(*set));
    #endif
    return result.success;
}

bool set_remove(Set *set, GenericValue value) {
    #ifdef DEV
    LOG("Remove value %s from Set '%p'.", set->stringifyFn(value) , set);
    #endif
    OperationResult result = binary_node_remove(set->root, set->compareFn, value);
    set->root = (BinaryNode*) result.returned;
    if(set->root) {
        set->deep = set->root->height;
    } else {
        set->deep = 0;
    }
    if(result.success) {
        set->size--;
    }
    #ifdef DEV
    printf("%s\n", set_to_representation(*set));
    #endif

    return result.success;
}

GenericValue set_search(Set set, GenericValue value) {
    if(!set.root) {
        return NULL;
    }

    return binary_node_search(set.root, set.compareFn, value);
}

// In order: Left, Root, Right
static void _set_in_order_as_list(BinaryNode node, LinkedList *list) {
    if(node.left) {
        _set_in_order_as_list(*node.left, list);
    }

    linked_list_push(list, node.data);

    if(node.right) {
        _set_in_order_as_list(*node.right, list);
    }
}

LinkedList *set_as_list(Set set) {
    LinkedList *list = new_linked_list(set.compareFn, set.deleteFn, set.stringifyFn);
    if(set.root) {
        _set_in_order_as_list(*set.root, list);
    }
    return list;
}

Set *set_union(Set set1, Set set2) {
    Set *set = new_set(set1.compareFn, set1.deleteFn, set1.stringifyFn);
    LinkedList *list1 = set_as_list(set1);
    LinkedList *list2 = set_as_list(set2);

    while(!linked_list_is_void(*list1)) {
        set_insert(set, linked_list_unshift(list1));
    }

    while(!linked_list_is_void(*list2)) {
        set_insert(set, linked_list_unshift(list2));
    }
    
    delete_linked_list(list1);
    delete_linked_list(list2);

    return set;
}

Set *set_difference(Set set1, Set set2) {
    Set *set = new_set(set1.compareFn, set1.deleteFn, set1.stringifyFn);
    LinkedList *list1 = set_as_list(set1);
    LinkedList *list2 = set_as_list(set2);

    while(!linked_list_is_void(*list1)) {
        set_insert(set, linked_list_unshift(list1));
    }

    while(!linked_list_is_void(*list2)) {
        set_remove(set, linked_list_unshift(list2));
    }

    delete_linked_list(list1);
    delete_linked_list(list2);

    return set;
}

bool set_disjoint(Set set1, Set set2) {
    return set_difference(set1, set2)->size == set1.size
        && set_difference(set2, set1)->size == set2.size;
}

#ifdef DEV
// In order: Left, Root, Right
static char *_set_print_in_order(BinaryNode *node, StringifyFunction stringifyFn, int deep) {
    char *string = to_string();
    char *left = NULL, *root = binary_node_to_representation(*node, stringifyFn), *right = NULL;
    int idx = 0;

    if(node->left) {
        left = _set_print_in_order(node->left, stringifyFn, deep + 1);
    }

    if(node->right) {
        right = _set_print_in_order(node->right, stringifyFn, deep + 1);
    }

    // Print over string the content of prev node.
    if(right) {
        idx += sprintf(string, "%s\n", right);
    }
    while(deep) {
        idx += sprintf(string + idx, "-");
        deep--;
    }
    idx += sprintf(string + idx, " %p %s", node, root);
    if(left) {
        sprintf(string + idx, "\n%s", left);
    }

    return string;
}

char *set_to_representation(Set set) {
    if(!set.root) {
        return "- NULL";
    }

    return _set_print_in_order(set.root, set.stringifyFn, 1);
}
#endif

char *set_to_string(Set set) {
    if(!set.root) {
        return "{}";
    }

    char *string = to_string();
    sprintf(string, "{ %s }", binary_node_to_string(*set.root, set.stringifyFn));
    return string;
}

void delete_set(Set *set) {
    if(set->root) {
        delete_binary_node(set->root, set->deleteFn);
    }
    set->compareFn = NULL;
    set->deleteFn = NULL;
    set->stringifyFn = NULL;
    free(set);
}