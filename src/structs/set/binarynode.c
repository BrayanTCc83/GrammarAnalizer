#include "../../headers/set.h"
#include "../../headers/global.h"

BinaryNode *new_binary_node(GenericValue value) {
    #ifdef DEV
    LOG("Instanciating new binary node.");
    #endif
    instance(BinaryNode)
    #ifdef DEV
    LOG("Instanciate binary node on '%p'.", o);
    #endif

    o->data = value;
    o->height = 1;
    o->left = o->right = NULL;

    return o;
}

static int _binary_node_get_height(BinaryNode node) {
    int left_height = node.left ? node.left->height : 0;
    int right_height = node.right ? node.right->height : 0;
    return 1 + (left_height > right_height ? left_height : right_height);
}

static int _binary_node_get_balance_factor(BinaryNode node) {
    int left_height = node.left ? node.left->height : 0;
    int right_height = node.right ? node.right->height : 0;
    return left_height - right_height;
}

static BinaryNode *_binary_node_rotation_ll(BinaryNode *node) {
    if(!node->left) {
        return node;
    }

    BinaryNode *rotated = node->left;
    
    node->left = rotated->right;
    rotated->right = node;
    rotated->right->height = _binary_node_get_height(*rotated->right);
    #ifdef DEV
    LOG("Reset height of binary node '%p' by left rotation is %i.", rotated->right, rotated->right->height);
    #endif

    return rotated;
}

static BinaryNode *_binary_node_rotation_rr(BinaryNode *node) {
    if(!node->right) {
        return node;
    }
    
    BinaryNode *rotated = node->right;
    
    node->right = rotated->left;
    rotated->left = node;
    rotated->left->height = _binary_node_get_height(*rotated->left);
    #ifdef DEV
    LOG("Reset height of binary node '%p' by right rotation is %i.", rotated->left, rotated->left->height);
    #endif

    return rotated;
}

static BinaryNode *_binary_node_rotation_lr(BinaryNode *node) {
    if(!node || !node->left || !node->left->right) {
        return node;
    }
    
    node->left = _binary_node_rotation_ll(node->left);
    return _binary_node_rotation_rr(node);
}

static BinaryNode *_binary_node_rotation_rl(BinaryNode *node) {
    if (!node || !node->right || !node->right->left) {
        return node;
    }
    
    node->right = _binary_node_rotation_rr(node->right);
    return _binary_node_rotation_ll(node);
}

OperationResult binary_node_insert(BinaryNode *node, ComparationFunction compareFn, GenericValue value) {
    OperationResult result = {
        .success = true,
        .returned = node
    };

    if(node == NULL) {
        result.returned = new_binary_node(value);
        return result;
    }

    ComparationResult cmpResult = compareFn(node->data, value);
    if(cmpResult == SMALLER) {
        #ifdef DEV
        LOG("Inserting value on right of '%p'.", node);
        #endif
        result = binary_node_insert(node->right, compareFn, value);
        node->right = (BinaryNode*) result.returned;
    } else if(cmpResult == BIGGER) {
        #ifdef DEV
        LOG("Inserting value on left of '%p'.", node);
        #endif
        result = binary_node_insert(node->left, compareFn, value);
        node->left = (BinaryNode*) result.returned;
    } else if(cmpResult == EQUALS) {
        #ifdef DEV
        LOG("The value is inserted yet in Set as '%p'.", node);
        #endif
        result.success = false;
        return result;
    }

    result.returned = node;
    if(!result.success) {
        return result;
    }

    node->height = _binary_node_get_height(*node);
    #ifdef DEV
    LOG("Height of binary node '%p' is %i.", node, node->height);
    #endif

    int root_balance = _binary_node_get_balance_factor(*node);
    #ifdef DEV
    LOG("Balance factor of node '%p' is %i.", node, root_balance);
    #endif
    if(root_balance > 1) {
        int left_balance = _binary_node_get_balance_factor(*node->left);
        #ifdef DEV
        LOG("Balance factor of node '%p' in left (%p) is %i.", node, node->left, left_balance);
        #endif
        if(left_balance > 0) {
            node = _binary_node_rotation_ll(node);
        } else if(left_balance < 0) {
            node = _binary_node_rotation_lr(node);
        }
        node->height = _binary_node_get_height(*node);
    } else if(root_balance < -1) {
        int right_balance = _binary_node_get_balance_factor(*node->right);
        #ifdef DEV
        LOG("Balance factor of node '%p' in right (%p) is %i.", node, node->right, right_balance);
        #endif
        if(right_balance < 0) {
            node = _binary_node_rotation_rr(node);
        } else if(right_balance > 0) {
            node = _binary_node_rotation_rl(node);
        }
        node->height = _binary_node_get_height(*node);
    }

    result.returned = node;
    result.success = true;
    return result;
}

static GenericValue _binary_node_pre(BinaryNode *node) {
    while(node->right != NULL) {
        node = node->right;
    }
    return node->data;
}

static GenericValue _binary_node_suc(BinaryNode *node) {
    while(node->left != NULL) {
        node = node->left;
    }
    return node->data;
}

OperationResult binary_node_remove(BinaryNode *node, ComparationFunction compareFn, GenericValue value) {
    OperationResult result = {
        .success = true,
        .returned = node
    };
    
    if(!node) {
        result.success = false;
        return result;
    }

    ComparationResult cmpResult = compareFn(node->data, value);
    if(cmpResult == SMALLER) {
        #ifdef DEV
        LOG("Going to right of '%p'.", node);
        #endif
        result = binary_node_remove(node->right, compareFn, value);
        node->right = result.returned;
    } else if(cmpResult == BIGGER) {
        #ifdef DEV
        LOG("Going to left of '%p'.", node);
        #endif
        result = binary_node_remove(node->left, compareFn, value);
        node->left = result.returned;
    } else {
        #ifdef DEV
        LOG("Removing node '%p' with data.", node);
        #endif
        if (!node->left && !node->right) {
            #ifdef DEV
            LOG("Leaf node. Deleting '%p'", node);
            #endif
            delete_binary_node(node, NULL);
            result.returned = NULL;
            return result;
        } else if (!node->left) {
            #ifdef DEV
            LOG("One child (right). Replacing '%p' with '%p'", node, node->right);
            #endif
            result.returned = node->right;
            node->right = NULL;
            delete_binary_node(node, NULL);
            return result;
        } else if (!node->right) {
            BinaryNode *temp = node;
            #ifdef DEV
            LOG("One child (left). Replacing '%p' with '%p'", node, node->left);
            #endif
            result.returned = node->left;
            node->left = NULL;
            delete_binary_node(node, NULL);
            return result;
        } else {
            #ifdef DEV
            LOG("Two children. Finding successor for '%p'", node);
            #endif
            GenericValue successor = _binary_node_suc(node->right);
            node->data = successor;
            result = binary_node_remove(node->right, compareFn, successor);
            node->right = result.returned;
        }
    }

    result.returned = node;
    if(!result.success) {
        return result;
    }

    node->height = _binary_node_get_height(*node);
    int root_balance = _binary_node_get_balance_factor(*node);
    #ifdef DEV
    LOG("Balance factor of node '%p' is %i.", node, root_balance);
    #endif
    if (root_balance > 1) {
        int left_balance = _binary_node_get_balance_factor(*node->left);
        #ifdef DEV
        LOG("Left-heavy node '%p'. Left balance = %d", node, left_balance);
        #endif
        if (left_balance >= 0) {
            node = _binary_node_rotation_ll(node);
        } else if(left_balance < 0) {
            node = _binary_node_rotation_lr(node->left);
        }
    } else if (root_balance < -1) {
        int right_balance = _binary_node_get_balance_factor(*node->right);
        #ifdef DEV
        LOG("Right-heavy node '%p'. Right balance = %d", node, right_balance);
        #endif
        if (right_balance <= 0) {
            node = _binary_node_rotation_rr(node);
        } else if(right_balance > 0) {
            node = _binary_node_rotation_rl(node);
        }
    }

    result.returned = node;
    return result;
}

GenericValue binary_node_search(BinaryNode *node, ComparationFunction compareFn, GenericValue value) {
    ComparationResult result = compareFn(node->data, value);
    if(result == EQUALS) {
        #ifdef DEV
        LOG("Value found at '%p'.", node);
        #endif
        return node->data;
    }

    if(result == BIGGER && node->left) {
        #ifdef DEV
        LOG("Searching value at left of '%p'.", node);
        #endif
        return binary_node_search(node->left, compareFn, value);
    }
    
    if(result == SMALLER && node->right) {
        #ifdef DEV
        LOG("Searching value at right of '%p'.", node);
        #endif
        return binary_node_search(node->right, compareFn, value);
    }

    #ifdef DEV
    LOG("Value not found %p.", node);
    #endif
    return NULL;
}

#ifdef DEV
char *binary_node_to_representation(BinaryNode node, StringifyFunction strinfigyFn) {
    char *representation = to_representation();
    if(!strinfigyFn) {
        sprintf(representation, "{ .data: %p, .height: %i, .left: %p, .right: %p }", node.data, node.height, node.left, node.right);
    } else {
        sprintf(representation, "{ .data: %s, .height: %i, .left: %p, .right: %p }", strinfigyFn(node.data), node.height, node.left, node.right);
    }
    return representation;
}
#endif

// In order: Left, Root, Right
static char *_binary_node_print_in_order(BinaryNode node, StringifyFunction stringify) {
    #ifdef DEV
    LOG("Parsing binary node to string '%p'.", node);
    #endif
    char *string = to_string();
    char *left = NULL, *root = stringify(node.data), *right = NULL;
    int idx = 0;

    if(node.left) {
        left = _binary_node_print_in_order(*node.left, stringify);
    }

    if(node.right) {
        right = _binary_node_print_in_order(*node.right, stringify);
    }

    // Print over string the content of prev node.
    if(left) {
        idx += sprintf(string, "%s, ", left);
    }
    idx += sprintf(string + idx, "%s", root);
    if(right) {
        sprintf(string + idx, ", %s", right);
    }

    #ifdef DEV
    LOG("String representation of binary node '%p': %s.", node, string);
    #endif

    return string;
}

char *binary_node_to_string(BinaryNode node, StringifyFunction stringify) {
    return _binary_node_print_in_order(node, stringify);
}

void delete_binary_node(BinaryNode *node, DeleteFunction deleteFn) {
    #ifdef DEV
    LOG("Deleting binary node '%p' %s.", node, binary_node_to_representation(*node, NULL));
    #endif

    if(deleteFn) {
        deleteFn(node->data);
    }

    node->height = 0;

    if(node->left) {
        delete_binary_node(node->left, deleteFn);
    }
    node->left = NULL;
    
    if(node->right) {
        delete_binary_node(node->right, deleteFn);
    }
    node->right = NULL;

    free(node);
}