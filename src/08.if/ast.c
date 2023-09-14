#include "function.h"

// ast_node.c
AST_node *mkAST_node(AST_type op, AST_node *left, AST_node *mid, AST_node *right, int intv)
{
    AST_node *node = malloc(sizeof(AST_node));
    if (node == NULL)
        custom_error_int("malloc", 0);
    node->op = op;
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

AST_node *mkAST_left(AST_type op, AST_node *left)
{
    return mkAST_node(op, left, NULL, NULL, 0);
}

static int OpPrec[] = {
    0, 10, 10,     // T_EOF, T_PLUS, T_MINUS
    20, 20,        // T_STAR, T_SLASH
    30, 30,        // T_EQ, T_NE
    40, 40, 40, 40 // T_LT, T_GT, T_LE, T_GE
};

AST_node *primary()
{
    AST_node *n;
    int id;
    switch (t_instance.token)
    {
    case T_INTLIT:
        n = mkAST_leaf(A_INTLIT, t_instance.v);
        break;
    case T_IDENT:
        id = findglob(Text);
        if (id == -1)
            custom_error_chars("Unknown variable", Text);
        n = mkAST_leaf(A_IDENT, id);
        break;
    default:
        custom_error_int("Syntax error, token", t_instance.token);
    }
    scan(&t_instance);
    return n;
}

static int op_precedence(Token_type tokentype)
{
    int prec = OpPrec[tokentype];
    if (prec == 0)
        custom_error_int("Syntax error, token", tokentype);
    return (prec);
}

static AST_type arithop(Token_type tokentype)
{
    if (tokentype > T_EOF && tokentype < T_INTLIT)
        return (tokentype);
    custom_error_int("Syntax error, token", tokentype);
}

AST_node *parse_ast_expr(int prev)
{
    AST_node *left, *right;
    left = primary();
    if (t_instance.token == T_RPAREN || t_instance.token == T_SEMI)
        return left;
    Token_type ty = t_instance.token;
    while (op_precedence(ty) > prev)
    {
        scan(&t_instance);
        right = parse_ast_expr(OpPrec[ty]);
        left = mkAST_node(arithop(ty), left, NULL, right, 0);
        ty = t_instance.token;
        if (ty == T_RPAREN || ty == T_SEMI)
            return left;
    }
    return left;
}