#include "function.h"

ASTnode *mkAST_node(AST_node_type op, Primitive_type type, ASTnode *left, ASTnode *mid, ASTnode *right, int intv)
{
    ASTnode *node = malloc(sizeof(ASTnode));
    if (node == NULL)
        custom_error_int("malloc", 0);
    node->op = op;
    node->type = type;
    node->left = left;
    node->mid = mid;
    node->right = right;
    node->v.intvalue = intv;
    return node;
}

ASTnode *mkAST_leaf(AST_node_type op, Primitive_type type, int id)
{
    return mkAST_node(op, type, NULL, NULL, NULL, id);
}

ASTnode *mkAST_left(AST_node_type op, Primitive_type type, ASTnode *left, int v)
{
    return mkAST_node(op, type, left, NULL, NULL, v);
}

static int OpPrec[] = {
    0, 10, 10,     // T_EOF, T_PLUS, T_MINUS
    20, 20,        // T_STAR, T_SLASH
    30, 30,        // T_EQ, T_NE
    40, 40, 40, 40 // T_LT, T_GT, T_LE, T_GE
};

static int arithop(int tokentype) {}

static int op_precedence(int tokentype)
{
}

static ASTnode *primary(void){

};

ASTnode *binexpr(int ptp)
{
}