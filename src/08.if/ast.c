#include "function.h"

// ast_node.c
AST_node *mkAST_node(AST_type op, AST_node *left, AST_node *mid, AST_node *right, int intv)
{
    AST_node *node = malloc(sizeof(AST_node));
    if (node == NULL)
        custom_error_int("malloc", 0);
    node->left = left;
    node->mid = mid;
    node->right = right;
    node->v.intv = intv;
    return node;
}
