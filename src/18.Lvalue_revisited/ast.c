#include "function.h"

ASTnode *binexpr(int ptp)
{
    ASTnode *left, *right;
    ASTnode *l_temp, *r_temp;
    left = prefix();
    Token_type type = t_instance.token;
}

static ASTnode *primary(void)
{
    ASTnode *n;

    switch (t_instance.token)
    {
    case T_INTLIT:
        int v = t_instance.intvalue;
        if (v >= 0 && v < 256)
            n = mkAST_leaf(A_INTLIT, P_CHAR, v);
        else
            n = mkAST_leaf(A_INTLIT, P_INT, v);
        break;
    case T_IDENT:
        scan(&t_instance);
        if (t_instance.token == T_LPAREN)
            return funccall();
        reject_token(&t_instance);
        int id = findglob(Text);
        if (id == -1)
            custom_error_int("unknown variable", id);
        n = mkAST_leaf(A_IDENT, Gsym[id].type, id);
        break;
    default:
        custom_error_int("syntax error, token: ", t_instance.token);
    }
    scan(&t_instance);
    return n;
}

ASTnode *prefix(void)
{
    ASTnode *tree;
    Primitive_type type;
    switch (t_instance.token)
    {
    case T_AMPER:
        scan(&t_instance);
        tree = prefix();
        if (tree->op != A_IDENT && tree->op != A_ADDR)
            custom_error_int("Get an address that is not an A_IDENT", tree->op);
        tree->op = A_ADDR;
        tree->type = pointer_to(tree->type);
        break;
    case T_STAR:
        scan(&t_instance);
        tree = prefix();
        if (tree->op != A_IDENT && tree->op != A_DEREF)
            custom_error_int("Get an address that is not an A_IDENT", tree->op);
        tree = mkAST_left(A_DEREF, value_at(tree->type), tree, 0);
        break;
    default:
        tree = primary();
    }
    return tree;
}

ASTnode *funccall(void)
{
    ASTnode *tree;
    int id;
    if (findglob(Text) == -1)
        custom_error_cahrs("undefined variable", Text);

    match_lparen();
    tree = binexpr(0);
    tree = mkAST_left(A_FUNCCALL, Gsym[id].type, tree, id);
    match_rparen();

    return tree;
}
