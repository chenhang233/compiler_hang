#include "function.h"

AST *mkast_node(AST_TYPE op, AST *l, AST *r, AST_V v)
{
    AST *ast = malloc(sizeof(AST));
    if (ast == NULL)
    {
        custom_error_chars("malloc", "NULL");
    }
    ast->op = op;
    ast->l = l;
    ast->r = r;
    ast->v = v;
    return ast;
}

AST *mkast_leaf(AST_TYPE op, AST_V v)
{
    return mkast_node(op, NULL, NULL, v);
}