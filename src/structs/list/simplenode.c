#include <stdio.h>
#include "../../headers/global.h"
#include "../../headers/linkedlist.h"

SimpleNode *new_simple_node(GenericValue value) {
    instance(SimpleNode);

    o->data = value;
    o->next = o->previus = NULL;

    return o;
}

void delete_simple_node(SimpleNode *node, DeleteFunction deleteFn) {
    if(deleteFn) {
        deleteFn(node->data);
    }
    node->data = NULL;
    node->next = node->previus = NULL;
    free(node);
}

#ifdef DEV
char *simple_node_to_representation(SimpleNode node, StringifyFunction strinfigyFn) {
    char *representation = to_representation();
    if(!strinfigyFn) {
        sprintf(representation, "{ .data: %p, .previus: %p, .next: %p }", node.data, node.previus, node.next);
    } else {
        sprintf(representation, "{ .data: %s, .previus: %p, .next: %p }", strinfigyFn(node.data), node.previus, node.next);
    }
    return representation;
}
#endif