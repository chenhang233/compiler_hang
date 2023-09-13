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

AST_node *mkAST_leaf(AST_type op, int id)
{
    return mkAST_node(op, NULL, NULL, NULL, id);
}

static int OpPrec[] = {
    0, 10, 10,     // T_EOF, T_PLUS, T_MINUS
    20, 20,        // T_STAR, T_SLASH
    30, 30,        // T_EQ, T_NE
    40, 40, 40, 40 // T_LT, T_GT, T_LE, T_GE
};

AST_node *parse_ast_expr(int prev)
{
    AST_node *left, *right;
}